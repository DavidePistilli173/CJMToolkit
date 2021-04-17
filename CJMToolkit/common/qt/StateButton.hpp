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

#ifndef COMMON_QT_STATEBUTTON_HPP
#define COMMON_QT_STATEBUTTON_HPP

#include <QPushButton>
#include <QWidget>

namespace cjm::qt
{
   /**
    * @brief Custom button with additional states.
    */
   class StateButton : public QPushButton
   {
      Q_OBJECT

   public:
      /********** DATA ************************************************************************************************/

      /**
       * @brief Data used to initialise a button.
       */
      struct Data
      {
         std::string_view label; /**< Label of the button. */
         long             value; /**< Value of the button. */
      };

      /**
       * @brief States of the button. Will be represented with its background.
       */
      enum class State
      {
         none,
         green,
         yellow,
         red
      };

      /********** CONSTRUCTORS ****************************************************************************************/

      /**
       * @brief Constructor.
       * @param data Data for the initialisation of the button.
       * @param parent Parent widget of the button.
       */
      StateButton(const Data& data, QWidget* parent = nullptr);

   private:
      /********** VARIABLES *******************************************************************************************/

      State state_{ State::none }; /**< Current state of the button. */
      long  value_{ 0U };          /**< Value associated with the button. */

   signals:
      /**
       * @brief Signal that the button has been clicked.
       * @param value Value associated with the clicked button.
       */
      void sigButtonClicked(long value);
   };
} // namespace cjm::qt

#endif // COMMON_QT_STATEBUTTON_HPP
