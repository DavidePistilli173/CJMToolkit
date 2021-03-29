#ifndef BUTTONSELECTOR_HPP
#define BUTTONSELECTOR_HPP

#include "StateButton.hpp"

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
       * @tparam DataList Type of the buttonData list.
       * @param buttonData Data about all the buttons required by the selector.
       * @param layoutType Type of layout to use for the buttons.
       * @param parent Parent widget of the selector.
       */
      template<typename DataList>
      ButtonSelector(
         const DataList& buttonData, LayoutType layoutType = LayoutType::horizontal, QWidget* parent = nullptr) :
         QWidget(parent)
      {
         mainPanel_.layoutType_ = layoutType;
         initUI_(buttonData);
      }

   private:
      /********** METHODS **********/
      /**
       * @brief Initialise the UI of the widget.
       * @tparam DataList Type of the buttonData list.
       * @param buttonData Data for the buttons to create.
       */
      template<typename DataList>
      void initUI_(const DataList& buttonData)
      {
         using Panel = MainPanel;

         switch (mainPanel_.layoutType_)
         {
         case LayoutType::horizontal:
            mainPanel_.mainLayout = new QHBoxLayout();
            break;
         case LayoutType::vertical:
            mainPanel_.mainLayout = new QVBoxLayout();
            break;
         }
         mainPanel_.mainLayout->setContentsMargins(Panel::margin, Panel::margin, Panel::margin, Panel::margin);
         mainPanel_.mainLayout->setSpacing(Panel::spacing);
         setLayout(mainPanel_.mainLayout);

         for (const auto& data : buttonData)
         {
            auto newButton = mainPanel_.buttons_.emplace_back(new StateButton(data, this));
            newButton->setSizePolicy(Panel::btn_size_policy);
            mainPanel_.mainLayout->addWidget(newButton);
         }
      }

      /********** VARIABLES **********/
      MainPanel mainPanel_; /**< Main panel of the widget. */
   };
} // namespace cjm::qt

#endif // BUTTONSELECTOR_HPP
