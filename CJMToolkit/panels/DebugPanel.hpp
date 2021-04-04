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

#ifndef DEBUGPANEL_HPP
#define DEBUGPANEL_HPP

#include "common/io/Log.hpp"
#include "common/qt/ButtonSelector.hpp"
#include "common/qt/StateButton.hpp"

#include <QWidget>

/**
 * @brief DEBUG page of the application.
 */
class DebugPanel : public QWidget
{
   Q_OBJECT

public:
   /**
    * @brief Panel containing the page selector.
    */
   struct PageSelectorPanel
   {
      using ButtonData = cjm::qt::StateButton::Data;
      /**
       * @brief Order of panel's elements.
       */
      enum class Order
      {
         info
      };

      /********** CONSTANTS **********/
      /**
       * @brief Data for all buttons in the page selector.
       */
      static constexpr std::array<ButtonData, static_cast<int>(Order::info) + 1> buttons{ ButtonData{
         .label = "INFO", .value = static_cast<long>(Order::info) } };

      /********** UI OBJECTS **********/
      cjm::qt::ButtonSelector* pageSelector{ nullptr }; /**< Selector for choosing the page to display. */
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
         page_selector
      };

      /********** CONSTANTS ********/
      static constexpr int margin{ 5 };  /**< Content margins. */
      static constexpr int spacing{ 5 }; /**< Spacing between the elements. */

      /********** UI OBJECTS ********/
      QVBoxLayout*      layout{ nullptr }; /**< Layout of the panel. */
      PageSelectorPanel pageSelectorPanel; /**< Page selector panel. */
   };

   /**
    * @brief Constructor.
    * @param parent Parent widget of the panel.
    */
   DebugPanel(QWidget* parent = nullptr);

   /**
    * @brief Initialise the panel.
    * @return true on success, false otherwise.
    */
   bool init();

private:
   /********** METHODS **********/
   /**
    * @brief Initialise the page selector panel.
    * @param panel Panel to initialise.
    * @return true on success, false otherwise.
    */
   bool initPageSelector_(PageSelectorPanel& panel);

   /**
    * @brief Initialise the user interface.
    * @return true on success, false otherwise.
    */
   bool initUI_();

   /********** VARIABLES **********/
   bool initialised_{ false }; /**< true if the object has been initilised, false otherwise. */

   cjm::io::Log* logger_; /**< Logger. */

   MainPanel mainPanel_; /**< Main panel of the widget. */
};

#endif // DEBUGPANEL_HPP
