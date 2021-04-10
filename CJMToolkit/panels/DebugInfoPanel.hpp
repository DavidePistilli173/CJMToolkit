#ifndef DEBUGINFOPANEL_HPP
#define DEBUGINFOPANEL_HPP

#include "common/io/Log.hpp"
#include "common/qt/InfoDisplay.hpp"
#include "common/version_info.hpp"
#include "version_info.hpp"

#include <QGroupBox>
#include <QLayout>
#include <QWidget>
#include <string_view>

class DebugInfoPanel : public QWidget
{
   Q_OBJECT

public:
   /**
    * @brief Left panel of the widget.
    */
   struct LeftPanel
   {
      /**
       * @brief Order of the panel's elements.
       */
      enum class Order
      {
         info
      };

      /**
       * @brief Information labels.
       */
      enum class Info
      {
         app_version,
         lib_version
      };

      /********** CONSTANTS **********/
      static constexpr int margin{ 5 };       /**< Content margin. */
      static constexpr int spacing{ 5 };      /**< Spacing between elements. */
      static constexpr int info_stretch{ 1 }; /**< Stretch of the info display. */

      static constexpr std::string_view info_display_title{ "GENERAL INFO" };  /**< Title of the information box. */
      static constexpr std::string_view app_version{ "Application Version:" }; /**< Application version title. */
      static constexpr std::string_view library_version{ "Library Version:" }; /**< Library version title.*/

      /********** UI OBJECTS **********/
      QVBoxLayout*          layout{ nullptr };      /**< Layout of the panel. */
      QGroupBox*            infoBox{ nullptr };     /**< General information group box. */
      QVBoxLayout*          infoLayout{ nullptr };  /**< Layout for the general info group box. */
      cjm::qt::InfoDisplay* infoDisplay{ nullptr }; /**< Display for general program information. */
   };

   /**
    * @brief Main panel of the widget.
    */
   struct MainPanel
   {
      /**
       * @brief Order of the panel's elements.
       */
      enum class Order
      {
         left,
         right
      };

      /********** CONSTANTS **********/
      static constexpr int margin{ 5 };        /**< Content margin. */
      static constexpr int spacing{ 5 };       /**< Spacing between elements. */
      static constexpr int left_stretch{ 1 };  /**< Stretch of the left panel. */
      static constexpr int right_stretch{ 2 }; /**< Stretch of the right panel. */

      /********** UI OBJECTS **********/
      QHBoxLayout* layout{ nullptr }; /**< Main layout of the panel. */
      LeftPanel    leftPanel;         /**< Left panel of the widget. */
   };

   /**
    * @brief Constructor.
    * @param parent Parent of the current widget.
    */
   DebugInfoPanel(QWidget* parent = nullptr);

   /**
    * @brief Initialise the widget.
    * @return true on success, false otherwise.
    */
   bool init();

private:
   /**
    * @brief Initialise the left panel of the widget.
    * @return true on success, false otherwise.
    */
   bool initLeftPanel_();

   /**
    * @brief Initialise the right panel of the widget.
    * @return true on success, false otherwise.
    */
   bool initRightPanel_();

   /**
    * @brief Initialise the UI of the widget.
    * @return true on success, false otherwise.
    */
   bool initUI_();

   bool          initialised_{ false }; /**< true if the widget has been initialised, false otherwise. */
   cjm::io::Log* logger_{ nullptr };    /**< Message logger. */

   /**
    * @brief Version of the application.
    */
   cjm::data::Version appVersion{ app_version_majour, app_version_minor, app_version_build };

   /**
    * @brief Version of the library.
    */
   cjm::data::Version libVersion{ cjm::version_majour, cjm::version_minor, cjm::version_build };

   MainPanel mainPanel_; /**< Main panel of the widget. */
};

#endif // DEBUGINFOPANEL_HPP
