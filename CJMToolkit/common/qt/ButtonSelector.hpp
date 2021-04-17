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

#ifndef COMMON_QT_BUTTONSELECTOR_HPP
#define COMMON_QT_BUTTONSELECTOR_HPP

#include "StateButton.hpp"
#include "common/algorithm/utility.hpp"
#include "common/io/Log.hpp"

#include <QLayout>
#include <QWidget>
#include <array>
#include <vector>

namespace cjm::qt
{
   /**
    * @brief Set of buttons that allow to selecct different states.
    */
   class ButtonSelector : public QWidget
   {
      Q_OBJECT

   public:
      /**
       * @brief Supported layouts.
       */
      enum class LayoutType
      {
         horizontal,
         vertical
      };

      /**
       * @brief Data of the main panel of the widget.
       */
      struct MainPanel
      {
         /********** CONSTANTS **********/
         static constexpr int margin{ 5 };  /**< Content margin. */
         static constexpr int spacing{ 5 }; /**< Spacing between elements. */

         /**
          * @brief Size policy of the buttons.
          */
         static constexpr QSizePolicy btn_size_policy{ QSizePolicy::Expanding, QSizePolicy::Expanding };

         /********** VARIABLES **********/
         LayoutType layoutType_; /**< Layout choice of the panel. */

         /********** UI OBJECTS *********/
         QLayout*                  mainLayout{ nullptr }; /**< Main layout of the panel. */
         std::vector<StateButton*> buttons_;              /**< List of all buttons. */
      };

      /********** CONSTRUCTORS **********/
      /**
       * @brief Constructor.
       * @param layoutType Type of layout to use for the buttons.
       * @param parent Parent widget of the selector.
       */
      ButtonSelector(LayoutType layoutType = LayoutType::horizontal, QWidget* parent = nullptr);

      /**
       * @brief Initialise the selector.
       * @tparam DataList Type of the buttonData list.
       * @param buttonData Data about all the buttons required by the selector.
       */
      template<typename DataList>
      bool init(const DataList& buttonData)
      {
         logger_ = cjm::io::Log::logger();
         if (logger_ == nullptr) return false;

         if (!initUI_(buttonData))
         {
            logger_->error("Failed to initialise the UI.");
            return false;
         }

         return true;
      }

   private:
      /********** METHODS **********/
      /**
       * @brief Initialise the UI of the widget.
       * @tparam DataList Type of the buttonData list.
       * @param buttonData Data for the buttons to create.
       */
      template<typename DataList>
      bool initUI_(const DataList& buttonData)
      {
         using cjm::alg::constructObj;
         using Panel = MainPanel;

         switch (mainPanel_.layoutType_)
         {
         case LayoutType::horizontal:
            if (!constructObj<QLayout, QHBoxLayout>(logger_, mainPanel_.mainLayout))
            {
               logger_->error("Failed to create the layout.");
               return false;
            }
            break;
         case LayoutType::vertical:
            if (!constructObj<QLayout, QVBoxLayout>(logger_, mainPanel_.mainLayout))
            {
               logger_->error("Failed to create the layout.");
               return false;
            }
            break;
         }
         mainPanel_.mainLayout->setContentsMargins(Panel::margin, Panel::margin, Panel::margin, Panel::margin);
         mainPanel_.mainLayout->setSpacing(Panel::spacing);
         setLayout(mainPanel_.mainLayout);

         for (const auto& data : buttonData)
         {
            auto newButton = mainPanel_.buttons_.emplace_back(nullptr);
            if (!constructObj(logger_, newButton, std::tie(data)))
            {
               logger_->error("Failed to create button.");
               return false;
            }
            newButton->setSizePolicy(Panel::btn_size_policy);
            mainPanel_.mainLayout->addWidget(newButton);
         }

         return true;
      }

      /********** VARIABLES **********/
      cjm::io::Log* logger_; /**< Message logger. */

      MainPanel mainPanel_; /**< Main panel of the widget. */
   };
} // namespace cjm::qt

#endif // COMMON_QT_BUTTONSELECTOR_HPP
