/***************************************************************************
**
**  This file is part of AtlasTerminal, a host computer GUI for
**  Atlas Scientific(TM) stamps
**  connected via an Atlas Scientific USB EZO(TM) Carrier Board
**  Copyright (C) 2016-2018 Paul JM van Kan
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

#ifndef EZOFRAME_H
#define EZOFRAME_H

#include <QFrame>
#include <QTimer>

#include "atlasdialog.h"
#include "qatlasusb.h"

namespace Ui {
class EZOFrame;
}

class EZOFrame : public QFrame
{
    Q_OBJECT

public:
    explicit EZOFrame(QWidget *parent = 0);
    ~EZOFrame();

    QByteArray lastCmd;
    QAtlasUSB* stamp = new QAtlasUSB();  // wel even aanmaken !

public slots:
        void on_btnReadMeas_clicked();
        void on_contCB_clicked(bool checked);
        void on_btnInfo_clicked();
        void displayBaudrate();

signals:
    void cmdAvailable(QByteArray newCommand);

private slots:
    void on_btnGetTemp_clicked();

    void on_btnSetTemp_clicked();

    void on_btnCal_clicked();
    void on_btnCalClear_clicked();
    void on_btnCalMid_clicked();
    void on_btnCalLow_clicked();
    void on_btnCalHigh_clicked();

    void on_btnSlope_clicked();
    void on_btnStatus_clicked();
    void on_ledCheckBox_clicked(bool checked);
    void on_btnLED_clicked();
    void on_btnSleep_clicked();

    void updateInfo();

    void displayInfo();
    void displayMeas();

    void on_cbAuto_clicked(bool checked);

    void on_btnI2CAddr_clicked();

    void on_respCB_clicked(bool checked);

    void on_btnBaud_clicked();

    void on_btnSetName_clicked();

    void on_btnGetName_clicked();

private:
    Ui::EZOFrame *ui;
    QTimer* stampTimer;
};

#endif // EZOFRAME_H
