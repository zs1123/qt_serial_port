#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"data.h"
#include"file.h"
#include <QMainWindow>
#include<QVector>
#include<QMouseEvent>
#include"libusb.h"
#include<QImage>
#include <QSerialPort>
#include<qserialportinfo>
#include<QLabel>
#define TIMEOUT 1000
#include"settingdialog.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void on_change_clicked();

    void timeOut();

    void flush();

    void on_pushButton_clicked();

    void on_clear_clicked();

    void on_btn_setting_clicked();

    bool openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();
    void set_time();
    void on_connect_clicked();

    void on_disconnect_clicked();

private:
    Ui::MainWindow *ui;
    QTimer* m_Timer;
    QTimer* curr_time;
    QSerialPort* m_serial;
    SettingsDialog* m_settings;
    QLabel *m_status = nullptr;
    void showStatusMessage(const QString &message);
    File *f;
    int index;
    QVector<Data> hisds;
    void initHisDs();


};

#endif // MAINWINDOW_H
