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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"

#include <QMessageBox>
#include <QtSerialPort/QSerialPort>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ad = new AtlasDialog(this);
    //aboutAtlas = new About(this);

    pf = new PlotFrame(ui->centralWidget);
    pf->move(560,20);

    serial = new QSerialPort(this);
    sd = new SerialDialog(this);

    mainTimer = new QTimer(this);
    //mainTimer->start(1000);
    //delayTimer->setSingleShot(true);

    ui->actionQuit->setEnabled(true);

// make all connections //initActionsConnections in Terminal example
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort2()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), sd, SLOT(show()));

// make other connections (see Terminal example)
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)),
            this, SLOT(handleError(QSerialPort::SerialPortError)));

    //connect(serial, SIGNAL(readyRead()),
    //        this, SLOT(readTentacleI2CData()));

    connect(serial, SIGNAL(readyRead()),
            this, SLOT(readAtlasUSBData2()));

    connect(mainTimer, SIGNAL(timeout()),
            this, SLOT(on_mainTimer()));

    pH1Frame = new EZOFrame(ui->EZOTab);

    setupEZOFrames();

    sd->setModal(true);
    sd->show();
    //connect( sd, SIGNAL(accepted()),
             //this, SLOT(openSerialPort2()) );

}

void MainWindow::setupEZOFrames()
{
    connect( pH1Frame, SIGNAL(cmdAvailable(QByteArray)),
             this, SLOT(writeData(QByteArray)) );
    connect( pH1Frame->stamp, SIGNAL(measRead()),
             this, SLOT(displayAllMeas()) );
}

MainWindow::~MainWindow()
{
    //delete sd;
    delete ui;
}

void MainWindow::on_mainTimer()
{
   // pH1Frame->on_btnReadMeas_clicked();
}

void MainWindow::openSerialPort2()
{
    qDebug() << "entry";
    SerialDialog::PortParameters p = sd->getCp();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->actionConfigure->setEnabled(false);
            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                       .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Open error"));
    }
}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    ui->statusBar->showMessage(tr("Disconnected"));
}

void MainWindow::writeData(const QByteArray &data)
{
    qDebug() << data;
    serial->write(data);
}

void MainWindow::displayAllMeas()
{ 
    double dval;
    if (pH1Frame->stamp->getUsbProps().probeType == "pH") {
        dval = pH1Frame->stamp->getUsbProps().currentpH;
        if (dval > 0 && dval < 14) ui->pH1Label->setText(QString::number(dval, 'f', 2 ));
    } else if (pH1Frame->stamp->getUsbProps().probeType == "ORP") {
        dval = pH1Frame->stamp->getUsbProps().currentORP;
        if (dval > -1021 && dval < 1021) ui->pH1Label->setText(QString::number(dval, 'f', 1 ) + " mV");
    }
    pf->realtimeTentacleSlot(dval);
}

void MainWindow::readAtlasUSBData()
{
    while(serial->canReadLine()) {
        QByteArray serialdata = serial->readLine();
//reads in data line by line, separated by \n or \r characters
        qDebug() << serialdata;
        QByteArray response = serialdata.trimmed();

        if ( response.contains("OK") ) ui->statusBar->showMessage("Success");
        else if ( response.contains("*ER") ) ui->statusBar->showMessage("Unknown Command");
        else if ( response.contains("*OV") ) ui->statusBar->showMessage("Over Voltage");
        else if ( response.contains("*UV") ) ui->statusBar->showMessage("Under Voltage");
        else if ( response.contains("*RS") ) ui->statusBar->showMessage("Device Reset");
        else if ( response.contains("*RE") ) ui->statusBar->showMessage("Boot up Completed");
        else if ( response.contains("*SL") ) ui->statusBar->showMessage("Device Asleep");
        else if ( response.contains("*WA") ) ui->statusBar->showMessage("Device Woken Up");
        else pH1Frame->stamp->parseAtlasUSB(response);
    }
}

void MainWindow::readAtlasUSBData2()
{
    QByteArray chunk = serial->readAll();
    serialbuffer.append(chunk);

    while ( serialbuffer.contains("\r") ) {
        //QString s;
        //s = QString::fromLatin1(byte_array.mid(10, 5));
        int crpos = serialbuffer.indexOf("\r");
        QByteArray response = serialbuffer.left(crpos);
        //response = response.trimmed();

        serialbuffer.remove(0, crpos+1);
        qDebug() << crpos << response << serialbuffer;


        if ( response.contains("OK") ) ui->statusBar->showMessage("Success");
        else if ( response.contains("*ER") ) ui->statusBar->showMessage("Unknown Command");
        else if ( response.contains("*OV") ) ui->statusBar->showMessage("Over Voltage");
        else if ( response.contains("*UV") ) ui->statusBar->showMessage("Under Voltage");
        else if ( response.contains("*RS") ) ui->statusBar->showMessage("Device Reset");
        else if ( response.contains("*RE") ) ui->statusBar->showMessage("Boot up Completed");
        else if ( response.contains("*SL") ) ui->statusBar->showMessage("Device Asleep");
        else if ( response.contains("*WA") ) ui->statusBar->showMessage("Device Woken Up");
        else pH1Frame->stamp->parseAtlasUSB(response);
    }
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError)
    {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::on_action_Help_Tentacle_triggered()
{
    ad->show();
}

void MainWindow::on_contCB_clicked(bool checked)
{
    pH1Frame->on_contCB_clicked(checked);
}

void MainWindow::on_actionScreenshot_triggered()
{
    ui->centralWidget->grab().save("image.png");
}

void MainWindow::on_actionAbout_AtlasTerminal_triggered()
{
    About aboutAtlas;
    aboutAtlas.exec();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    qApp->aboutQt();
}
