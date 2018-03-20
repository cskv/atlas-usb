#ifndef LOGGINGFRAME_H
#define LOGGINGFRAME_H

#include <QFrame>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QFile>

namespace Ui {
class LoggingFrame;
}

class LoggingFrame : public QFrame
{
    Q_OBJECT

public:
    explicit LoggingFrame(QWidget *parent = 0);
    ~LoggingFrame();

// getters and setters
    QDir getLogDir() const;
    void setLogDir(const QDir &value);
    void setLogFile(const QString &value);

    void write(const QString &line);
    void read();

    void on_btnStart_clicked();
    void on_btnStop_clicked();

private slots:
    void on_btnWrite_clicked();
    void on_btnRead_clicked();



private:
    Ui::LoggingFrame *ui;
    QDir logDir;
    QFile logFile;
    QTextStream logStream;
};

#endif // LOGGINGFRAME_H
