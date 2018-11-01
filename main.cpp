#include "mainwindow.h"
#include <QApplication>
#include<iostream>
#include <QSerialPort>
#include<qserialportinfo>
#define VID 0x1a86
#define PID 0x7523
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

//    QCoreApplication a(argc, argv);
//    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
//    {
//        qDebug() << "Name : " << info.portName();
//        qDebug() << "Description : " << info.description();
//        qDebug() << "Manufacturer: " << info.manufacturer();
//        qDebug() << "Serial Number: " << info.serialNumber();
//        qDebug() << "System Location: " << info.systemLocation();
//    }



}

