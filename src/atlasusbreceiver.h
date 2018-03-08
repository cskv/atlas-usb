#ifndef ATLASUSBRECEIVER_H
#define ATLASUSBRECEIVER_H

#include <QObject>
#include <QSerialPort>

class AtlasUSBReceiver : public QObject
{
    Q_OBJECT
public:
    explicit AtlasUSBReceiver(QObject *parent = 0);
    ~AtlasUSBReceiver();

signals:

public slots:
    void processLine(const QByteArray & line) {
      //...
    }
    void readData() {
      // IMPORTANT: That's a *while*, not an *if*!
      while (m_port.canReadLine()) processLine(m_port.readLine());
    }

private:
    QSerialPort m_port;
    QByteArray m_buffer;
};

#endif // ATLASUSBRECEIVER_H
