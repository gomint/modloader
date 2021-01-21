//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#include <ModLoader/Logger.h>

namespace GoMint {

    Logger::Logger(std::shared_ptr<spdlog::logger> logger)
        : m_logger(std::move(logger)) {

    }

    void Logger::log(LogLevel level, const std::string& msg) {
        m_logger->log(static_cast<spdlog::level::level_enum>(level), msg);
    }

    void Logger::trace(const std::string& msg) {
        m_logger->trace(msg);
    }

    void Logger::debug(const std::string& msg) {
        m_logger->debug(msg);
    }

    void Logger::info(const std::string& msg) {
        m_logger->info(msg);
    }

    void Logger::warning(const std::string& msg) {
        m_logger->warn(msg);
    }

    void Logger::error(const std::string& msg) {
        m_logger->error(msg);
    }

    void Logger::critical(const std::string& msg) {
        m_logger->critical(msg);
    }

}