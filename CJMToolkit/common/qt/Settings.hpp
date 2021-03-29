#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "common/data/BaseSettings.hpp"

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <string_view>

namespace cjm::qt
{
   /**
    * @brief Storage for application settings. Can load and save settings to different file formats.
    */
   class Settings : public cjm::data::BaseSettings
   {
   public:
      /**
       * @brief Supported file formats.
       */
      enum class Format
      {
         xml
      };

      /**
       * @brief States of the settings object.
       */
      enum class Status
      {
         no_error,
         file_error,
         format_error
      };

      static constexpr std::string_view default_file{ "settings.cfg" }; /**< Default file name. */

      /**
       * @brief Open mode of the file.
       */
      static constexpr QIODevice::OpenModeFlag open_mode{ QIODevice::OpenModeFlag::ReadWrite };

      /**
       * @brief Create a settings object from a file in a given format.
       * @param fileName Path of the settings file. If it does not exist, it is created.
       * @param fileFormat Format of the file.
       */
      Settings(std::string_view fileName = default_file, Format fileFormat = Format::xml);

      /**
       * @brief Get the current status of the settings.
       * @return Current status of the settings.
       */
      Status status() const;

   private:
      /**
       * @brief Load existing settings from the file.
       */
      void loadSettings_();

      /**
       * @brief Load existing settings from and XML file.
       */
      void xmlLoadSettings_();

      /**
       * @brief Process attributes of the currently selected node.
       * @param reader XML stream reader already placed in the correct node.
       */
      void xmlProcessAttributes_(QXmlStreamReader& reader);

      QFile       file_;                  /**< File handler. */
      Format      format_{ Format::xml }; /**< Format of the settings file. */
      std::string fileName_;              /**< Name of the settings file. */

      Status status_{ Status::no_error }; /**< Current status of the settings. */
   };
} // namespace cjm::qt

#endif // SETTINGS_HPP
