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

#include "Settings.hpp"

namespace cjm::qt
{
   using cjm::io::Log;

   Settings::Settings(std::string_view fileName, Format fileFormat) : format_{ fileFormat }, fileName_{ fileName }
   {
      file_.setFileName(fileName_.data());
      if (!file_.open(open_mode))
      {
         logger_->error("Failed to open the settings file.", Log::pack("file name", fileName_));
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
            "Error while reading the xml settings file.",
            Log::pack("file name", fileName_),
            Log::pack("error message", reader.errorString().toUtf8().constData()));
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
               "Invalid token in the xml settings file.",
               Log::pack("file name", fileName_),
               Log::pack("error message", reader.errorString().toUtf8().constData()));
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
            "Error while reading the xml settings file.",
            Log::pack("file name", fileName_),
            Log::pack("error message", reader.errorString().toUtf8().constData()));
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
