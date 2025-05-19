#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <vector>
#include <utility> // für std::pair

enum class LogType {
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);
    
    // Neue Methoden für die Konfiguration des Loggers
    static void setOutputToFile(const std::string& filepath = "picsim.log");
    static void setOutputToConsole();
    static void setEnabled(bool enabled);
    static bool isEnabled();
    
    // Methoden für das UI
    static std::string getLastLogMessage();
    static LogType getLastLogType();
    static std::vector<std::pair<LogType, std::string>> getRecentLogs(size_t count = 10);

private:
    static std::ofstream logFile;
    static bool useFile;
    static bool enabled;
    static std::mutex logMutex;
    static std::vector<std::pair<LogType, std::string>> logHistory;
    static std::string lastMessage;
    static LogType lastType;
    
    static void log(const std::string& level, const std::string& message, LogType type);
};

#endif // LOGGER_H
