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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QtCore/QtGlobal>

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QTimer>

#include "atlasdialog.h"
#include "qatlasusb.h"

#include "ledindicator.h"
#include "ezoframe.h"
#include "plotframe.h"
#include "qcustomplot.h"
#include "about.h"
#include "serialdialog.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openSerialPort2();
    void closeSerialPort();
    void handleError(QSerialPort::SerialPortError error);

    void writeData(const QByteArray &data);
    void readAtlasUSBData2();

    void on_action_Help_Tentacle_triggered();
    void displayAllMeas();
    void on_contCB_clicked(bool checked);
    void on_actionScreenshot_triggered();
    void setupEZOFrames();
    void on_actionAbout_AtlasTerminal_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionConnect_triggered();

// functions for QSettings  and use of inifiles
    void loadSettings();
    void saveSettings();

private:
    Ui::MainWindow *ui;

    QString m_sSettingsFile;

    LedIndicator* ledStateLed;

    SerialDialog* sd;
    QSerialPort *serial;
    QByteArray lastCmd;
    QByteArray serialbuffer;

    EZOFrame* pH1Frame;
    PlotFrame* pf;

    //QTimer* delayTimer;

    AtlasDialog* ad;

    QCPPlotTitle* plotTitle;
};

#endif // MAINWINDOW_H
