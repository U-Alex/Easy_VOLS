#ifndef LOGGER_H
#define LOGGER_H

//#include <QObject>
#include <QTextEdit>
#include <QString>
#include <QColor>

class Logger : QObject
{
public:
    static Logger *Instance() {
        static Logger logger;
        return &logger;
    }
    static void setTextEdit(QTextEdit *);
    static void sendLog(const QStringList& text, const QColor& color = QColor(Qt::black));

private:
    Logger();
    ~Logger();

    QTextEdit *text_edit = nullptr;
};

#endif // LOGGER_H
