/***************************************************************************
**
**  This file is part of AtlasTerminal, a host computer GUI for
**  Atlas Scientific(stamp) stamps
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

#include "ezoframe.h"
#include "ui_ezoframe.h"

EZOFrame::EZOFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::EZOFrame)
{
    ui->setupUi(this);

    ui->stateLed->setOnColor(Qt::blue);
    ui->stateLed->setOffColor(Qt::gray);
    ui->stateLed->setState(true);

    stampTimer = new QTimer;

    connect(stampTimer, SIGNAL(timeout()), this, SLOT(on_btnReadMeas_clicked()));

    connect( stamp, SIGNAL(ledRead(bool)),
             this, SLOT(displayLedState()) );
    connect( stamp, SIGNAL(infoRead()),
             this, SLOT(displayInfo()) );
    connect( stamp, SIGNAL(measRead()),
             this, SLOT(displayMeas()) );
}

EZOFrame::~EZOFrame()
{
    delete ui;
}

void EZOFrame::updateInfo()
{
    ui->btnSlope->click();
    ui->btnInfo->click();
    ui->btnStatus->click();
    ui->btnGetTemp->click();
    ui->btnCal->click();
}

void EZOFrame::displayLedState()
{
    ui->stateLed->setState(stamp->getUsbProps().ledState);
}

void EZOFrame::displayInfo()
{
    double dval = stamp->getUsbProps().acidSlope;
    if (dval > 0) ui->acidSlopeLabel->setText(QString::number(dval));
    dval = stamp->getUsbProps().basicSlope;
    if (dval > 0) ui->basicSlopeLabel->setText(QString::number(dval));

    ui->probeLabel->setText(stamp->getUsbProps().probeType);
    ui->versionLabel->setText(stamp->getUsbProps().version);

    ui->rstLabel->setText(stamp->getUsbProps().rstCode);
    dval = stamp->getUsbProps().voltage;
    if (dval > 0) ui->voltLabel->setText(QString::number(dval));

    dval = stamp->getUsbProps().currentTemp;
    if (dval > 0) ui->leTemp->setText(QString::number(dval , 'f', 1 ));
    int ival = stamp->getUsbProps().calState;
    if (ival > -1) ui->calLabel->setText(QString::number(ival));

    //ui->leI2CAddress->setText(QString::number(stamp->getI2cAddress()));
}

void EZOFrame::displayMeas()
{
    double dval;
    if (stamp->getUsbProps().probeType == "pH") {
        dval = stamp->getUsbProps().currentpH;
        if (dval > 0 && dval < 14) ui->pHLabel->setText(QString::number(dval, 'f', 2 ));
    } else if (stamp->getUsbProps().probeType == "ORP") {
        dval = stamp->getUsbProps().currentORP;
        if (dval > -1021 && dval < 1021) ui->pHLabel->setText(QString::number(dval, 'f', 1 ) + " mV");
    }
}

void EZOFrame::on_btnGetTemp_clicked()
{
    lastCmd = stamp->readTemp();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnReadMeas_clicked()
{
    lastCmd = stamp->readpHORP();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnLED_clicked()
{
    lastCmd = stamp->readLED();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnSetTemp_clicked()
{
    lastCmd = stamp->writeTemp(ui->leTemp->text().toDouble());
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
    QTimer::singleShot(400, this, SLOT(on_btnGetTemp_clicked()));
}

void EZOFrame::on_btnCal_clicked()
{
    lastCmd = stamp->readCal();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnCalClear_clicked()
{
    if (stamp->getUsbProps().probeType == "pH") lastCmd = stamp->dopHCal(0);
    else if (stamp->getUsbProps().probeType == "ORP") lastCmd = stamp->doORPCal(200.0);
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
    QTimer::singleShot(2000, this, SLOT(on_btnCal_clicked()));
    ui->btnCalHigh->setEnabled(false);
    ui->btnCalLow->setEnabled(false);
}

void EZOFrame::on_btnCalMid_clicked()
{
    lastCmd = stamp->dopHCal(1);
    //serial->write(lastCmd);
    emit cmdAvailable(lastCmd);
    QTimer::singleShot(2000, this, SLOT(on_btnCal_clicked()));
    ui->btnCalHigh->setEnabled(true);
    ui->btnCalLow->setEnabled(true);
}

void EZOFrame::on_btnCalLow_clicked()
{
    lastCmd = stamp->dopHCal(2);
    //serial->write(lastCmd);
    emit cmdAvailable(lastCmd);
    QTimer::singleShot(2000, this, SLOT(on_btnCal_clicked()));
}

void EZOFrame::on_btnCalHigh_clicked()
{
    lastCmd = stamp->dopHCal(3);
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
    QTimer::singleShot(2000, this, SLOT(on_btnCal_clicked()));
}

void EZOFrame::on_btnSlope_clicked()
{
    lastCmd = stamp->readSlope();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnInfo_clicked()
{
    lastCmd = stamp->readInfo();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_btnStatus_clicked()
{
    lastCmd = stamp->readStatus();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_ledCheckBox_clicked(bool checked)
{
    lastCmd = stamp->writeLED(checked);
    //serial->write(lastCmd);
    emit cmdAvailable(lastCmd);
    QTimer::singleShot(300, this, SLOT(on_btnLED_clicked()));
}

void EZOFrame::on_contCB_clicked(bool checked)
{
    lastCmd = stamp->writeCont(checked);
    emit cmdAvailable(lastCmd);
}

void EZOFrame::on_btnSleep_clicked()
{
    lastCmd = stamp->sleep();
    emit cmdAvailable(lastCmd);
    //serial->write(lastCmd);
}

void EZOFrame::on_cbAuto_clicked(bool checked)
{
    if (checked) stampTimer->start(1000);
    else stampTimer->stop();
}

void EZOFrame::on_btnI2CAddr_clicked()
{
    //lastCmd = stamp->changeI2C(ui->leI2CAddress->text().toInt());
    //emit cmdAvailable(lastCmd);
}

void EZOFrame::on_respCB_clicked(bool checked)
{
    lastCmd = stamp->writeResponse(checked);
    emit cmdAvailable(lastCmd);
}
