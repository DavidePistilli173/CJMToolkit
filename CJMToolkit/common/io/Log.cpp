#include "Log.hpp"

namespace cjm::io
{
   /********** STATIC VARIABLES DEFINITIONS ***************************************************************************/
   bool                 Log::initialised_{ false };
   std::unique_ptr<Log> Log::logger_;

   /********** METHOD DEFINITIONS *************************************************************************************/

   bool Log::init(std::string_view logFile)
   {
      if (!initialised_)
      {
         // Initialise the logger's instance.
         logger_ = std::unique_ptr<Log>(new Log());
         if (logger_ == nullptr)
         {
            return false;
         }

         // Stop sync with stdio.
         std::ios_base::sync_with_stdio(false);

         // Initialise the logger.
         logger_->outputFile_ = std::ofstream(logFile.data());
         if (logger_->outputFile_.fail())
         {
            return false;
         }
         logger_->startTime_ = std::chrono::steady_clock::now();

         initialised_ = true;
      }

      return true;
   }

   Log::Level Log::level() const
   {
      return logLevel_;
   }

   Log* Log::logger()
   {
      return logger_.get();
   }

   void Log::setLevel(Level level)
   {
      logLevel_ = level;
      trace("Current log level set to:", logLevel_);
   }
} // namespace cjm::io
