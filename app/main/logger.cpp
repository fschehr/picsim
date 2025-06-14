#include "logger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

// Statische Variablen initialisieren
std::ofstream Logger::logFile;
bool Logger::useFile = true;
bool Logger::enabled = true;
std::mutex Logger::logMutex;
std::vector<std::pair<LogType, std::string>> Logger::logHistory;
std::string Logger::lastMessage = "Nothing to log.";
LogType Logger::lastType = LogType::INFO;



void Logger::log(const std::string& level, const std::string& message, LogType type) {
    if (!enabled) {
        return;
    }

    std::lock_guard<std::mutex> lock(logMutex);
    
    // Zeitstempel erstellen
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    std::stringstream timestamp;
    timestamp << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");
    
    std::string formattedMessage = timestamp.str() + " [" + level + "] " + message;
    
    if (useFile) {
        if (logFile.is_open()) {
            logFile << formattedMessage << std::endl;
            logFile.flush(); // Um sicherzustellen, dass Logs sofort geschrieben werden
        }
    } else {
        std::cout << formattedMessage << std::endl;
    }
    
    // Speichern für das UI
    lastMessage = message;
    lastType = type;
    
    // Zum Log-Verlauf hinzufügen (maximal 100 Einträge)
    logHistory.push_back(std::make_pair(type, message));
    if (logHistory.size() > 100) {
        logHistory.erase(logHistory.begin());
    }
}

void Logger::info(const std::string& message) {
    log("INFO", message, LogType::INFO);
}

void Logger::warning(const std::string& message) {
    log("WARNING", message, LogType::WARNING);
}

void Logger::error(const std::string& message) {
    log("ERROR", message, LogType::ERROR);
}

std::string Logger::getLastLogMessage() {
    std::lock_guard<std::mutex> lock(logMutex);
    return lastMessage;
}

LogType Logger::getLastLogType() {
    std::lock_guard<std::mutex> lock(logMutex);
    return lastType;
}

std::vector<std::pair<LogType, std::string>> Logger::getRecentLogs(size_t count) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (logHistory.empty()) {
        return {};
    }
    
    size_t startIndex = (logHistory.size() <= count) ? 0 : logHistory.size() - count;
    return std::vector<std::pair<LogType, std::string>>(
        logHistory.begin() + startIndex,
        logHistory.end()
    );
}

void Logger::setOutputToFile(const std::string& filepath) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (logFile.is_open()) {
        logFile.close();
    }
    
    logFile.open(filepath, std::ios::out | std::ios::trunc);
    useFile = true;
    
    // Initiale Logzeile beim Öffnen der Datei
    if (logFile.is_open() && enabled) {
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        std::stringstream timestamp;
        timestamp << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");
        
        logFile << "------------------------------------------------" << std::endl;
        logFile << timestamp.str() << " [SYSTEM] PicSim Logger gestartet" << std::endl;
        logFile.flush();
    }
    
    // Für das UI updaten
    lastMessage = "PicSim Logger gestartet";
    lastType = LogType::INFO;
    logHistory.push_back(std::make_pair(LogType::INFO, "PicSim Logger gestartet"));
}

void Logger::setOutputToConsole() {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (logFile.is_open()) {
        logFile.close();
    }
    
    useFile = false;
}

void Logger::setEnabled(bool enabled) {
    Logger::enabled = enabled;
}

bool Logger::isEnabled() {
    return enabled;
}
