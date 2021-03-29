#ifndef STATEBUTTON_HPP
#define STATEBUTTON_HPP

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

#endif // STATEBUTTON_HPP
