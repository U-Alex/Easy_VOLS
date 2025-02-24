#include "logger.h"

Logger::Logger()
{ }

Logger::~Logger()
{ }

void Logger::setTextEdit(QTextEdit *ptr)
{
    Logger *logger = Logger::Instance();
    logger->text_edit = static_cast<QTextEdit*>(ptr);
}

void Logger::sendLog(const QStringList& text, const QColor& color)
{
    Logger *logger = Logger::Instance();

    if (logger->text_edit != nullptr) {
        logger->text_edit->moveCursor(QTextCursor::End);
        logger->text_edit->setTextColor(color);
        logger->text_edit->insertPlainText(text.join("  "));
        logger->text_edit->insertPlainText("\n");
    }
}
