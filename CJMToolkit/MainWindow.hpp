#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "common/data/BaseSettings.hpp"
#include "common/io/Log.hpp"
#include "common/qt/ButtonSelector.hpp"
#include "common/qt/StateButton.hpp"

#include <QLayout>
#include <QMainWindow>
#include <array>
#include <string_view>

/**
 * @brief Main window of the application.
 */
class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   /********** DATA ***************************************************************************************************/

   static constexpr std::string_view application_name{ "CJMTooklit" }; /**< Name of the application. */

   /**
    * @brief Size data.
    */
   struct Size
   {
      static constexpr std::string_view node{ "Size" };       /**< Name of the settings node. */
      static constexpr std::string_view minimum{ "Minimum" }; /**< Minimum size node. */
      static constexpr std::string_view width{ "Width" };     /**< Width group. */
      static constexpr std::string_view height{ "Height" };   /**< Height group. */
   };

   /**
    * @brief Stylesheet data.
    */
   struct StyleSheet
   {
      static constexpr std::string_view name{ "StyleSheet" }; /**< Stylesheet node. */
      static constexpr std::string_view file{ "file" };       /**< Name of the stylesheet file. */
   };

   /**
    * @brief Selector for choosing the page of the application.
    */
   struct PageSelectorPanel
   {
      /********** DATA ************************************************************************************************/

      using ButtonData = cjm::qt::StateButton::Data;

      /**
       * @brief Order of the panel's elements.
       */
      enum class Order
      {
         debug
      };

      /**
       * @brief Data for all buttons in the page selectors.
       */
      static constexpr std::array<ButtonData, static_cast<int>(Order::debug) + 1> buttons{ ButtonData{
         .label = "DEBUG", .value = static_cast<long>(Order::debug) } };

      /********** UI OBJECTS ******************************************************************************************/
      cjm::qt::ButtonSelector* pageSelector_{ nullptr }; /**< Selector for choosing the page of the application. */
   };

   /**
    * @brief Main panel of the window.
    */
   struct MainPanel
   {
      /********** DATA ************************************************************************************************/

      /**
       * @brief Order of the elements of the panel.
       */
      enum class Order
      {
         page_selector,
         page
      };

      static constexpr int margin{ 5 };  /**< Content margin. */
      static constexpr int spacing{ 5 }; /**< Spacing between elements. */

      /********** UI OBJECTS ******************************************************************************************/

      QWidget*          mainWidget{ nullptr }; /**< Central widget of the application. */
      QVBoxLayout*      mainLayout{ nullptr }; /**< Main layout of the panel. */
      PageSelectorPanel pageSelectorPanel;     /**< Panel that contains the page selector. */
   };

   /********** CONSTRUCTORS *******************************************************************************************/

   /**
    * @brief Create the main window.
    * @param settings Settings to use in the application.
    * @param parent Parent widget of the application.
    */
   MainWindow(cjm::data::BaseSettings& settings, QWidget* parent = nullptr);

   /**
    * @brief Destructor.
    */
   ~MainWindow();

private:
   /********** METHODS ************************************************************************************************/

   /**
    * @brief Initialise the page selector.
    */
   void initPageSelector_(PageSelectorPanel& pageSelectorPanel);

   /**
    * @brief Initialise the UI of the window.
    */
   void initUI_();

   /**
    * @brief Load sizes of the main window.
    */
   void loadSizes_();

   /**
    * @brief Load the stylesheets to apply to all children of the main window.
    */
   void loadStyleSheets_();

   /********** VARIABLES **********************************************************************************************/
   cjm::io::Log*            logger_{ nullptr }; /**< Message logger. */
   cjm::data::BaseSettings& settings_;          /**< Application settings. */
   MainPanel                mainPanel_;         /**< Main panel of the window. */
};
#endif // MAINWINDOW_HPP
