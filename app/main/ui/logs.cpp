#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include "../logger.h"

ftxui::Component Logs() {
    using namespace ftxui;

    auto logs_renderer = Renderer([] {
        // Aktualisierte Log-Informationen aus dem Logger abrufen
        std::string logText = Logger::getLastLogMessage();
        LogType logType = Logger::getLastLogType();

        // Die letzten Logs für die Anzeige abrufen (maximal 5)
        auto recentLogs = Logger::getRecentLogs(5);
        
        Elements logElements;
        
        if (recentLogs.empty()) {
            // Wenn keine Logs vorhanden sind, nur die Standardnachricht anzeigen
            logElements.push_back(
                text(logText)
                | (logType == LogType::INFO ? bgcolor(Color::NavyBlue) :
                   logType == LogType::WARNING ? bgcolor(Color::Orange1) :
                   logType == LogType::ERROR ? bgcolor(Color::IndianRed1) : bgcolor(Color::Black))
                | (logType == LogType::WARNING ? color(Color::Black) :
                   logType == LogType::ERROR ? color(Color::Black) : color(Color::White))
            );
        } else {
            // Neueste Logs in umgekehrter Reihenfolge anzeigen (neueste zuerst)
            for (auto it = recentLogs.rbegin(); it != recentLogs.rend(); ++it) {
                const auto& [type, message] = *it;
                
                logElements.push_back(
                    text(message)
                    | (type == LogType::INFO ? bgcolor(Color::NavyBlue) :
                       type == LogType::WARNING ? bgcolor(Color::Orange1) :
                       type == LogType::ERROR ? bgcolor(Color::IndianRed1) : bgcolor(Color::Black))
                    | (type == LogType::WARNING ? color(Color::Black) :
                       type == LogType::ERROR ? color(Color::Black) : color(Color::White))
                );
            }
        }
        
        return window(
            text(" Logs "),
            vbox(logElements)
        );
    });

    return logs_renderer;
}