/*
   MIT License

   Copyright (c) [2021] [Davide Dravindran Pistilli] [https://github.com/DavidePistilli173/CJMToolkit]

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "common/data/BaseSettings.hpp"
#include "common/data/Version.hpp"
#include "common/io/Log.hpp"
#include "common/qt/ButtonSelector.hpp"
#include "common/qt/StateButton.hpp"
#include "panels/DebugPanel.hpp"
#include "version_info.hpp"

#include <QLayout>
#include <QMainWindow>
#include <QStackedLayout>
#include <array>
#include <string_view>

/**
 * @brief Main window of the application.
 */
class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   /********** DATA **********/
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
      /********** DATA **********/
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

      /********** UI OBJECTS *********/
      cjm::qt::ButtonSelector* pageSelector_{ nullptr }; /**< Selector for choosing the page of the application. */
   };

   /**
    * @brief Panel containing the pages of the application.
    */
   struct PagePanel
   {
      /********** CONSTANTS **********/
      static constexpr int margin{ 5 };  /**< Content margin. */
      static constexpr int spacing{ 5 }; /**< Spacing. */

      /********** UI OBJECTS **********/
      QStackedLayout* layout;               /**< Layout of the panel. */
      DebugPanel*     debugPage{ nullptr }; /**< Debug page. */
   };

   /**
    * @brief Main panel of the window.
    */
   struct MainPanel
   {
      /********** DATA **********/
      /**
       * @brief Order of the elements of the panel.
       */
      enum class Order
      {
         page_selector,
         page
      };

      /**
       * @brief Stretches of the elements of the panel.
       */
      static constexpr std::array<int, static_cast<size_t>(Order::page) + 1> stretches{ 1, 10 };

      static constexpr int margin{ 5 };  /**< Content margin. */
      static constexpr int spacing{ 5 }; /**< Spacing between elements. */

      /********** UI OBJECTS **********/
      QWidget*          mainWidget{ nullptr }; /**< Central widget of the application. */
      QVBoxLayout*      mainLayout{ nullptr }; /**< Main layout of the panel. */
      PageSelectorPanel pageSelectorPanel;     /**< Panel that contains the page selector. */
      PagePanel         pagePanel;             /**< Panel containing all application pages. */
   };

   /********** CONSTRUCTORS **********/
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

   /**
    * @brief Initialise the main window.
    * @return true on success, false otherwise.
    */
   bool init();

private:
   /********** METHODS **********/
   /**
    * @brief Initialise the pages of the application.
    * @param panel Panel to initialise.
    * @return true on success, false otherwise.
    */
   bool initPages_(PagePanel& panel);

   /**
    * @brief Initialise the page selector.
    * @return true on success, false otherwise.
    */
   bool initPageSelector_(PageSelectorPanel& pageSelectorPanel);

   /**
    * @brief Initialise the UI of the window.
    * @return true on success, false otherwise.
    */
   bool initUI_();

   /**
    * @brief Initialise the window.
    * @return true on success, false otherwise.
    */
   bool initWindow_();

   /**
    * @brief Load settings from file.
    * @return true on success, false otherwise.
    */
   bool loadSettings_();

   /**
    * @brief Load sizes of the main window.
    * @return true on success, false otherwise.
    */
   bool loadSizes_();

   /**
    * @brief Load the stylesheets to apply to all children of the main window.
    * @return true on success, false otherwise.
    */
   bool loadStyleSheets_();

   /********** VARIABLES **********/
   cjm::io::Log*            logger_{ nullptr }; /**< Message logger. */
   cjm::data::BaseSettings& settings_;          /**< Application settings. */
   MainPanel                mainPanel_;         /**< Main panel of the window. */
};
#endif // MAINWINDOW_HPP
