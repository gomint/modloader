//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_LOGGER_H
#define MODLOADER_LOGGER_H

#include <ModLoader/ModLoader.h>

#include <spdlog/spdlog.h>

namespace GoMint {

    /**
     * Helper class which provides logging capabilities to Mods.
     *
     * @author Ciel DeVille
     * @version 1.0.0
     * @stability unstable
     */
    class Logger : public ILogger {

    public:

        /**
         * Constructs a new logger given the underlying spdlog logger it should redirect its
         * output into.
         *
         * @param logger The logger to redirect output to
         */
        explicit Logger(std::shared_ptr<spdlog::logger> logger);

        void log(LogLevel level, const std::string& msg) override;
        void trace(const std::string& msg) override;
        void debug(const std::string& msg) override;
        void info(const std::string& msg) override;
        void warning(const std::string& msg) override;
        void error(const std::string& msg) override;
        void critical(const std::string& msg) override;

    private:

        std::shared_ptr<spdlog::logger> m_logger;

    };

    using LoggerPtr = std::unique_ptr<Logger>;

}

#endif //MODLOADER_LOGGER_H
