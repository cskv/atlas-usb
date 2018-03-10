/***************************************************************************
**
**  This file is part of AtlasTerminal, a host computer GUI for
**  Atlas Scientific(TM) stamps
**  connected to an Arduino(TM)
**  with Tentacle (TM) interface shield.
**  Copyright (C) 2016 Paul JM van Kan
**
**  AtlasTerminal is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.

**  AtlasTerminal is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.

**  You should have received a copy of the GNU General Public License
**  along with AtlasTerminal.  If not, see <http://www.gnu.org/licenses/>.

***************************************************************************
**           Author: Paul JM van Kan                                     **
**  Website/Contact:                                                     **
**             Date:                                                     **
**          Version:                                                     **
***************************************************************************/

#include "qatlasusb.h"
#include <QtDebug>

QATLASUSB::QATLASUSB(QObject *parent) : QObject(parent)
{

}

QATLASUSB::~QATLASUSB()
{

}

/*
Command         Function
L,x L,?         Enable / Disable or Query the LEDs (pHEZO pg.23)
C,x C,?         Continuous mode (pg.24)
R               Returns a single reading (pg.25)
T           Set or Query the temperature compensation (pg.26)
Cal         Performs calibration (pg.27-31)
SLOPE       Queries slope (p.32)
NAME        Device name (p.33)
I           Device information (pg.33)
RESPONSE    Response code (p.34)
STATUS      Retrieve status information (pg.35)
SLEEP       Enter low power sleep mode (pg.36)
SERIAL      Change baud rate in UART mode (pg.37)
Factory     Reset to factory settings (p.38)
PLOCK,x PLOCK,? Locks UART and baud rate/I2C and I2C Address (p.32 ORP-EZO)

*/

// ATLAS commands
//---------------------------------------------------
/*!
 * \brief Get the current state of the LED on the Atlas Scientific EZO stamp.
 *
 * Atlas function: L,?
 * \return cmd for EZO function L,?
 * \code command = readLED(); \endcode
 * EZO response: ?L,x<CR> with x is 0 (LED off) or 1 (LED on)
 */
QByteArray QATLASUSB::readLED()
{
    QByteArray cmd = "L,?\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}
/*!
 * \brief Set the state of the LED on the Atlas Scientific stamp.
 *
 * \param state
 * \retval cmd for EZO function L,x
 *
 * Example:
 * @code writeLED(bool state); @endcode
 * Atlas function: L,state
 * Response: OK\r (Success)
 */
QByteArray QATLASUSB::writeLED(bool state)
{
    QByteArray cmd;
    state ? cmd = "L,1\r" : cmd = "L,0\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//---------------------------------------------------
/*!
 * \brief Get the mode on the Atlas Scientific EZO stamp.
 *
 * Atlas function: C,?
 * \return cmd for EZO function C,?
 * \code command = readCont(); \endcode
 * EZO response: ?C,x<CR> with x is 0 (data on request) or 1 (cont. data)
 */
QByteArray QATLASUSB::readCont()
{
    QByteArray cmd = "C,?\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}
/*!
 * \brief Set the mode on the Atlas Scientific stamp.
 *
 * \param state
 * \retval cmd for EZO function C,x
 *
 * Example:
 * @code writeLED(bool state); @endcode
 * Atlas function: C,state
 * Response: OK\r (Success)
 */
QByteArray QATLASUSB::writeCont(bool state)
{
    QByteArray cmd;
    state ? cmd = "C,1\r" : cmd = "C,0\r";
    //qDebug() << cmd;
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//--------------------------------------------------------
/**
 * @brief Get the current pH, ORP, EC or D.O. value from the Atlas Scientific stamp
 *
 * @code readpHORP(); @endcode
 * Example:
 * Atlas function: R
 * Response: Response: OK\rxx.xxx\r with xx.xxx is the measured value e.g. 7.012
 */
QByteArray QATLASUSB::readpHORP()
{
    QByteArray cmd = "R\r";
    qDebug() << cmd;
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//---------------------------------------------------------
/**
 * @brief Get the current temperature from the Atlas Scientific stamp
 *
 * Example:
 * @code readTemp(); @endcode
 * Atlas function: T,?
 * Response: ?T,xx.xx with xx.xx is the temperature e.g. 25.00
 */
QByteArray QATLASUSB::readTemp()
{
    QByteArray cmd = "T,?\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}
/**
 * @brief Set the temperature on the Atlas Scientific stamp
 *
 * Example:
 * @code writeTemp(); @endcode
 * Atlas function: T,xx.xx
 * Response: OK\r (Success)
 */
QByteArray QATLASUSB::writeTemp(double temperature)
{
    QByteArray cmd = "T,";
    cmd.append(QByteArray::number(temperature, 'f', 2));
    cmd.append("\r");
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//----------------------------------------------
/**
 * @brief Get the Calibration state from the Atlas Scientific stamp
 *
 * Example:
 * @code readCal(); @endcode
 * Atlas function: Cal?
 * Response: ?Cal,x\r with x is 0, 1, 2, 3
 */
QByteArray QATLASUSB::readCal()
{
    QByteArray cmd = "Cal,?\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}
/**
 * @brief Perform a pH calibration of the Atlas Scientific stamp
 *
 * Example:
 * @code dopHCal(int taskid); @endcode
 * Atlas function: Cal,0 (clear), 1 (pH7), 2 (pH4) or 3 (pH10)
 * Response: OK\r (Success)
 */
QByteArray QATLASUSB::dopHCal(int taskid)
{
    QByteArray cmd;
    cmd = "Cal,";
    switch (taskid) {
            case 0 : cmd += "clear\r";
            case 1 : cmd += "mid,7.00\r";
            case 2 : cmd += "low,4.00\r";
            case 3 : cmd += "high,10.00\r";
    }
    lastAtlasUSBCmd = cmd;
    return cmd;
}
/**
 * @brief Perform a ORP (mV) calibration of the Atlas Scientific stamp
 *
 * Example:
 * @code doORPCal(double orpRef); @endcode
 * Atlas function: Cal,xxx.x with xxx.x in mV
 * Response: 1
 */
QByteArray QATLASUSB::doORPCal(double orpRef)
{
    QByteArray cmd = "Cal,";
    cmd.append(QByteArray::number(orpRef,'f',1));
    cmd.append("\r");
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//---------------------------------------------------
/**
 * @brief Get the pH calibration slopes after executing Cal,123 commands
 *
 * Example:
 * @code readSlope(); @endcode
 * Atlas function: SLOPE,?
 * Response: OK\r?SLOPE,xx.x,yyy.y
 * with xx.x is acid slope e.g 99.7, yyy.y is basic slope e.g. 100.3
 */
QByteArray QATLASUSB::readSlope()
{
    QByteArray cmd = "SLOPE,?\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//----------------------------------------------------
/**
 * @brief Get the device name of the Atlas Scientific EZO stamp.
 *
 * Example:
 * @code readName(); @endcode
 * Atlas function: NAME,?
 * Response: ?NAME, ssssss
 * with ssssss is the name of the device (ascii)
 */
QByteArray QATLASUSB::readName()
{
    QByteArray cmd = "NAME,?\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}
/**
 * @brief Set the device name of the Atlas Scientific EZO stamp.
 *
 * Example:
 * @code writeName(); @endcode
 * Atlas function: NAME,ssssss
 * Response: OK\r (Success)
 */
QByteArray QATLASUSB::writeName(QString name)
{
    QByteArray cmd= "NAME,";
    cmd.append(name);       //implicit conversion to UTF8 by overloaded function
    cmd.append("\r");
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//--------------------------------------------------
/**
 * @brief Query the type and firmware version of the EZO stamp.
 *
 * Atlas function: ?I
 * Response: OK\r?I,pH,x.x\r
 * with x.x is firmware version number e.g 1.0
 */
QByteArray QATLASUSB::readInfo()
{
    QByteArray cmd = "I\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//---------------------------------------------------
/**
 * @brief Query if the "OK" RESPONSE of the EZO stamp is on.
 *
 * Atlas function: ?RESPONSE
 * Response: OK\r?RESPONSE,x\r with x is 0 or 1
 */
QByteArray QATLASUSB::readResponse()
{
    QByteArray cmd = "RESPONSE,?\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}
/**
 * @brief Set the "OK" RESPONSE of the EZO stamp on or off.
 *
 * Atlas function: RESPONSE,x with x is 0 or 1
 * Response: OK\r
 */
QByteArray QATLASUSB::writeResponse(bool state)
{
    QByteArray cmd;
    state ? cmd = "RESPONSE,1\r" : cmd = "RESPONSE,0\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//--------------------------------------------------
/**
 * @brief Query the status of the ATLAS Scientific stamp.
 *
 * Atlas function: STATUS
 * Response: ?STATUS,x,y.yyy
 * with x is PSBWU, y.yyy supply voltage Vcc
 */
QByteArray QATLASUSB::readStatus()
{
    QByteArray cmd = "STATUS\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//-------------------------------------------------
/**
 * @brief Put the EZO stamp to sleep (low power mode).
 *
 * Atlas function: SLEEP
 * Response: none
 */
QByteArray QATLASUSB::sleep()
{
    QByteArray cmd = "SLEEP\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}

//---------------------------------------
/**
 * @brief Change baudrate in UART mode.
 *
 * Atlas function: SERIAL, baudrate
 * Response: none
 * 1. 300 bps 2. 1200 bps 3. 2400 bps 4. 9600 bps 5. 19200 bps
 * 6. 38400 bps 7. 57600 bps 8. 115200 bps
 */
QByteArray QATLASUSB::changeSerial(int baudrate)
{
    QByteArray cmd = "SERIAL,";
    cmd.append(QByteArray::number(baudrate));
    cmd.append("\r");
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//---------------------------------------
/**
 * @brief Change baudrate in UART mode.
 *
 * Atlas function: Factory
 * Response: issue STATUS query after this command
 * and see if "S" is in the reply
 */
QByteArray QATLASUSB::factoryReset()
{
    QByteArray cmd = "Factory\r";
    lastAtlasUSBCmd = cmd;
    return cmd;
}
//----------------------------------------------------------------
/**
 * @brief Parse the response of the EZO stamp connected via USB.
 *
 * @param atlasdata
 */
void QATLASUSB::parseAtlasUSB(QByteArray atlasdata)
{
    QByteArray t;
    qDebug() << atlasdata;

    if ( atlasdata.startsWith("?L,") ) {
        t = atlasdata.mid(3,1);
        usbProps.ledState = (t.toInt() == 1);
        usbProps.ledState = (t.toInt() != 0);
        emit ledRead(usbProps.ledState);        //usbProps.ledState = t.toInt;
    } else if ( atlasdata.startsWith("?T,") ) {
        t = atlasdata.mid(3,5);
        usbProps.currentTemp = t.toDouble();
        emit infoRead();
    } else if ( atlasdata.startsWith("?CAL,") ) {
        t = atlasdata.mid(5,1);
        usbProps.calState = t.toInt();
        emit infoRead();
    } else if ( atlasdata.startsWith("?SLOPE,") ) {
        t = atlasdata.mid(7,4);
        usbProps.acidSlope = t.toDouble();
        t = atlasdata.mid(12,6);
        usbProps.basicSlope = t.toDouble();
        emit infoRead();
    } else if ( atlasdata.startsWith("?I,") ) {
        t = atlasdata.mid(3,2);
        if (t.contains("pH")) {
            usbProps.probeType = QString(t);     // EZO "pH" stamp
            t = atlasdata.mid(6,4);
            usbProps.version = QString(t);
        } else {
            t = atlasdata.mid(3,3);              // EZO "ORP" stamp
            usbProps.probeType = QString(t);
            t = atlasdata.mid(7,4);
            usbProps.version = QString(t);
        }
        emit infoRead();
    } else  if ( atlasdata.startsWith("?STATUS,") ) {
        t = atlasdata.mid(8,1);
        usbProps.rstCode = t;
        t = atlasdata.mid(10,5);
        usbProps.voltage = t.toDouble();
        emit infoRead();
    } else  if ( atlasdata.startsWith("?NAME,") ) {
        t = atlasdata.mid(6,8);
        usbProps.name = QString(t);
        emit infoRead();
    } else {
        t = atlasdata.mid(0,7);     // pH: 6 bytes ORP: 7 bytes max
        if (usbProps.probeType == "pH") {
            usbProps.currentpH = t.toDouble();
            if ( usbProps.currentpH > 0 && usbProps.currentpH < 14 ) emit measRead();
        } else if (usbProps.probeType == "ORP") {
            usbProps.currentORP = t.toDouble();
            if ( usbProps.currentORP > -1021 && usbProps.currentORP < 1021 ) emit measRead();
        }
    }
}

void QATLASUSB::setBaud(const int &value)
{
    usbProps.baud = value;
}

void QATLASUSB::setAsSerial(const bool &value)
{
    usbProps.isConnectedAsSerial = value;
}

QATLASUSB::AtlasUSBProperties QATLASUSB::getUsbProps() const
{
    return usbProps;
}
