//http://stackoverflow.com/questions/24768916/qserialport-readline-extremely-slow-compared-to-readall

#include "atlasusbreceiver.h"

AtlasUSBReceiver::AtlasUSBReceiver(QObject *parent) : QObject(parent)
{
    connect(&m_port, &QIODevice::readyRead, this, &AtlasUSBReceiver::readData);
     // ...
}

AtlasUSBReceiver::~AtlasUSBReceiver()
{

}
