#include "Settings.hpp"

namespace cjm::qt
{
   Settings::Settings(std::string_view fileName, Format fileFormat) : format_{ fileFormat }, fileName_{ fileName }
   {
      file_.setFileName(fileName_.data());
      if (!file_.open(open_mode))
      {
         logger_->error("Failed to open the settings file.", fileName_);
         status_ = Status::file_error;
      }
      else
      {
         loadSettings_();
      }
   }

   Settings::Status Settings::status() const
   {
      return status_;
   }

   void Settings::loadSettings_()
   {
      switch (format_)
      {
      case Format::xml:
         xmlLoadSettings_();
         break;
      }
   }

   void Settings::xmlLoadSettings_()
   {
      QXmlStreamReader reader;
      reader.setDevice(&file_);
      if (reader.hasError())
      {
         logger_->error(
            "Error while reading the xml settings file.", fileName_, reader.errorString().toUtf8().constData());
         status_ = Status::file_error;
         return;
      }

      // Parse the entire xml document.
      while (!reader.atEnd() && !reader.hasError())
      {
         // Read the next token and report its type.
         QXmlStreamReader::TokenType token{ reader.readNext() };
         std::string                 nodeName{ reader.name().toUtf8().constData() };

         switch (token)
         {
         case QXmlStreamReader::TokenType::Invalid:
            logger_->error(
               "Invalid token in the xml settings file.", fileName_, reader.errorString().toUtf8().constData());
            status_ = Status::format_error;
            break;
         case QXmlStreamReader::TokenType::NoToken:
         case QXmlStreamReader::TokenType::StartDocument:
         case QXmlStreamReader::TokenType::Comment:
         case QXmlStreamReader::TokenType::DTD:
         case QXmlStreamReader::TokenType::EntityReference:
         case QXmlStreamReader::TokenType::ProcessingInstruction:
            break;
         case QXmlStreamReader::EndDocument:
            internalReturnToRoot_();
            break;
         case QXmlStreamReader::TokenType::StartElement:
            addNode(nodeName);
            internalEnterNode_(nodeName, last_node_idx);
            xmlProcessAttributes_(reader);
            break;
         case QXmlStreamReader::TokenType::EndElement:
            internalExitNode_();
            break;
         case QXmlStreamReader::TokenType::Characters:
            setValue(reader.text().toLocal8Bit().constData());
            break;
         }
      }

      if (reader.hasError())
      {
         logger_->error(
            "Error while reading the xml settings file.", fileName_, reader.errorString().toUtf8().constData());
         status_ = Status::format_error;
      }
   }

   void Settings::xmlProcessAttributes_(QXmlStreamReader& reader)
   {
      QXmlStreamAttributes attributes{ reader.attributes() };

      for (const auto& attribute : attributes)
      {
         setAttribute(attribute.name().toUtf8().constData(), attribute.value().toUtf8().constData());
      }
   }
} // namespace cjm::qt
