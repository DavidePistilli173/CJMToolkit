#ifndef LOG_HPP
#define LOG_HPP

#include <array>
#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <string_view>

namespace cjm::io
{
   /**
    * @brief Class for logging information.
    */
   class Log
   {
   public:
      /**
       * @brief Special codes used in the logging messages.
       */
      struct Codes
      {
         static constexpr std::string_view ansi_reset{ "\033[0m" };    /**< Reset the console font. */
         static constexpr std::string_view ansi_bold{ "\033[1m" };     /**< Set the ANSI console font to bold. */
         static constexpr std::string_view ansi_black{ "\033[30m" };   /**< Set the ANSI console font to black. */
         static constexpr std::string_view ansi_red{ "\033[31m" };     /**< Set the ANSI console font to red. */
         static constexpr std::string_view ansi_green{ "\033[32m" };   /**< Set the ANSI console font to green. */
         static constexpr std::string_view ansi_yellow{ "\033[33m" };  /**< Set the ANSI console font to yellow. */
         static constexpr std::string_view ansi_blue{ "\033[34m" };    /**< Set the ANSI console font to blue. */
         static constexpr std::string_view ansi_magenta{ "\033[35m" }; /**< Set the ANSI console font to magenta. */
         static constexpr std::string_view ansi_cyan{ "\033[36m" };    /**< Set the ANSI console font to cyan. */
         static constexpr std::string_view ansi_white{ "\033[37m" };   /**< Set the ANSI console font to white. */
      };

      /**
       * @brief Logging levels.
       */
      enum class Level
      {
         trace,
         info,
         warn,
         error,
         fatal
      };

      /**
       * @brief Keywords for the logging levels.
       */
      static constexpr std::array<std::string_view, static_cast<size_t>(Level::fatal) + 1> level_keys{
         "[TRC]", "[INF]", "[WRN]", "[ERR]", "[FTL]"
      };

      /**
       * @brief ANSI colour codes for the logging levels.
       */
      static constexpr std::array<std::string_view, static_cast<size_t>(Level::fatal) + 1> level_ansi_colours{
         Codes::ansi_reset, Codes::ansi_white, Codes::ansi_yellow, Codes::ansi_red, Codes::ansi_magenta
      };

      /**
       * @brief Separator between the level keyword and the message.
       */
      static constexpr std::string_view level_separator{ " - " };

      static constexpr std::string_view header_left_bracket{ "[" };  /**< Left bracket for the header. */
      static constexpr std::string_view header_right_bracket{ "]" }; /**< Right bracket for the header. */
      static constexpr std::string_view time_ms{ "ms" };             /**< Millseconds in text. */
      static constexpr std::string_view tab{ "   " };                /**< Tab size for log contents. */

      /********** OPERATORS *******************************************************************************************/

      friend std::ostream& operator<<(std::ostream& stream, const Level& level)
      {
         stream << level_keys[static_cast<int>(level)];
         return stream;
      }

      /********** METHODS *********************************************************************************************/

      /**
       * @brief Log an error message.
       */
      template<typename... Args>
      void error(std::string_view msg, const Args&... args)
      {
         log(Level::error, msg, args...);
      }

      /**
       * @brief Log a fatal error message.
       */
      template<typename... Args>
      void fatal(std::string_view msg, const Args&... args)
      {
         log(Level::fatal, msg, args...);
      }

      /**
       * @brief Initialise the logger.
       * @param logFile Path of the output file to use for logging.
       * @return true on success, false otherwise.
       */
      static bool init(std::string_view logFile);

      /**
       * @brief Log an information message.
       */
      template<typename... Args>
      void info(std::string_view msg, const Args&... args)
      {
         log(Level::info, msg, args...);
      }

      /**
       * @brief Get the current logging level.
       * @return Current logging level.
       */
      Level level() const;

      /**
       * @brief Log a message and optional additional information.
       * @param level Desired logging level.
       * @param msg Message to print on the first line.
       * @param args Optional variables to print.
       */
      template<typename... Args>
      void log(Level level, std::string_view msg, const Args&... args)
      {
         if (!initialised_) return;

         if (level >= logLevel_)
         {
            auto timestamp{ std::chrono::duration_cast<std::chrono::milliseconds>(
                               std::chrono::steady_clock::now() - startTime_)
                               .count() };

            std::scoped_lock lck{ ioMtx_ };

            // Log on stdio.
            std::cout << level_ansi_colours[static_cast<int>(level)] << level_keys[static_cast<int>(level)]
                      << level_separator << header_left_bracket << timestamp << time_ms << header_right_bracket << msg
                      << '\n';
            ((std::cout << tab << args), ...);
            std::cout << '\n';

            // Log on file.
            outputFile_ << level_keys[static_cast<int>(level)] << level_separator << header_left_bracket << timestamp
                        << time_ms << header_right_bracket << msg << '\n';
            ((outputFile_ << tab << args), ...);
            outputFile_ << '\n';
         }
      }

      /**
       * @brief Get the current instance of the logger.
       * @return Pointer to the logger. Can be nullptr.
       */
      static Log* logger();

      /**
       * @brief Log a trace message.
       */
      template<typename... Args>
      void trace(std::string_view msg, const Args&... args)
      {
         log(Level::trace, msg, args...);
      }

      /**
       * @brief Set the logging level.
       * @param level New logging level.
       */
      void setLevel(Level level);

      /**
       * @brief Log a warning message.
       */
      template<typename... Args>
      void warn(std::string_view msg, const Args&... args)
      {
         log(Level::warn, msg, args...);
      }

   private:
      /**
       * @brief Constructor.
       */
      Log() = default;

      static bool                 initialised_; /**< true if the logger was initialised. */
      static std::unique_ptr<Log> logger_;      /**< Single instance of the logger. */

      std::mutex    ioMtx_;      /**< Mutex for protecting the standard IO stream. */
      std::ofstream outputFile_; /**< Logging file. */

      std::atomic<Level>                                 logLevel_{ Level::trace }; /**< Current logging level. */
      std::chrono::time_point<std::chrono::steady_clock> startTime_; /**< Starting time of the program. */
   };
} // namespace cjm::io

#endif // LOG_HPP
