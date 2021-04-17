#ifndef COMMON_QT_INFODISPLAY_HPP
#define COMMON_QT_INFODISPLAY_HPP

#include "common/io/Log.hpp"

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <string_view>

namespace cjm::qt
{
   /**
    * @brief Display information labels.
    */
   class InfoDisplay : public QWidget
   {
      Q_OBJECT

   public:
      /**
       * @brief Main panel of the widget.
       */
      struct MainPanel
      {
         /********** CONSTANTS **********/
         static constexpr int margin{ 5 };  /**< Content margin. */
         static constexpr int spacing{ 5 }; /**< Spacing between elements. */

         /********** UI OBJECTS **********/
         QGridLayout*                             layout{ nullptr }; /**< Layout of the panel. */
         std::vector<std::pair<QLabel*, QLabel*>> infoLabels;        /**< List of all labels. */
      };

      /**
       * @brief Constructor.
       * @param parent Parent of the current widget.
       */
      explicit InfoDisplay(QWidget* parent = nullptr);

      /**
       * @brief Initialise the widget.
       * @return true on success, false otherwise.
       */
      bool init();

      /**
       * @brief Insert an info label into the widget.
       * @param title Title of the label.
       * @param value Current value associated with the label.
       * @param row Row where to insert the info label.
       */
      void insertLabel(std::string_view title, std::string_view value, int row);

      /**
       * @brief Set the value of a label.
       * @param value New value.
       * @param row Row of the label to change.
       */
      void setValue(std::string_view value, int row);

   private:
      bool initialised_{ false }; /**< true if the widget has been initialised. */

      cjm::io::Log* logger_; /**< Message logger. */

      MainPanel mainPanel_; /**< Main panel. */
   };
} // namespace cjm::qt

#endif // COMMON_QT_INFODISPLAY_HPP
