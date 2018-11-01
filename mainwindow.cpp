#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QPainter>
#include<QTableWidgetItem>
#include<QPen>
#include"file.h"
#include<QTimer>
#include<QChar>
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),curr_time(new QTimer()),m_serial(new QSerialPort(this)),hisds(),m_settings(new SettingsDialog),m_status(new QLabel)
{
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);

    m_Timer = new QTimer();
    curr_time->start(1000);
    connect(m_Timer,SIGNAL(timeout()),this,SLOT(timeOut()));
    //connect(ui->actionConfigure, &QAction::triggered, m_settings, &SettingsDialog::show);
    ui->setupUi(this);
    ui->dt_s->setMinimumDateTime(QDateTime::fromString("2018-01-01 00:00:00","yyyy-MM-dd HH:mm:ss"));
    ui->dt_e->setDateTime(QDateTime::currentDateTime());
    ui->log->setReadOnly(true);

    ui->tb->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tb->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tb->setColumnWidth(0,130);
    ui->tb->setColumnWidth(1,40);
    ui->tb->setColumnWidth(2,80);
    ui->tb->setColumnWidth(3,80);
    ui->tb->resizeRowsToContents();
    set_time();
    //ui->lb_A->setPalette(QPalette())
    setFixedSize(this->width(),this->height());
    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);

    connect(curr_time,SIGNAL(timeout()),this,SLOT(set_time()));
    f = new File("e:/data.txt");
}


void MainWindow::set_time(){
    ui->curr_time->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
}

void MainWindow::timeOut(){
    int len = INFO::cus_A.size();
    if(len>0){
        INFO::cus_A.push_back(Data(QDateTime::currentDateTime(),INFO::cus_A[len-1].place,INFO::cus_A[len-1].temper,INFO::cus_A[len-1].humid));
        if(INFO::cus_A.size()>30){
            INFO::cus_A.erase(INFO::cus_A.begin(),INFO::cus_A.end() -30);
        }
    }
    len = INFO::cus_B.size();
    if(len >0){
        INFO::cus_B.push_back(Data(QDateTime::currentDateTime(),INFO::cus_B[len-1].place,INFO::cus_B[len-1].temper,INFO::cus_B[len-1].humid));
        if(INFO::cus_B.size()>30){
            INFO::cus_B.erase(INFO::cus_B.begin(),INFO::cus_B.end() -30);
        }
    }
    flush();
}
bool MainWindow::openSerialPort()
{
    const SettingsDialog::Settings p = m_settings->settings();
    m_serial->setPortName(p.name);
    m_serial->setBaudRate(p.baudRate);
    m_serial->setDataBits(p.dataBits);
    m_serial->setParity(p.parity);
    m_serial->setStopBits(p.stopBits);
    m_serial->setFlowControl(p.flowControl);
    if (m_serial->open(QIODevice::ReadWrite)) {
      showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
      return true;
    } else {
        QMessageBox::critical(this, tr("Error"), m_serial->errorString());
        qDebug() <<tr("Open error");
        return false;
    }
}

void MainWindow::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
    showStatusMessage(tr("Disconnected"));
}

void MainWindow::writeData(const QByteArray &data)
{
    m_serial->write(data);
}

void MainWindow::readData()
{
    const QByteArray data = m_serial->readAll();
    QString s = QString(data);

    QChar place = s[0];
    QString t;
    t += s.at(6);
    t +=s.at(7);
    t.trimmed();
    QString h;
    h += s.at(13);
    h+=s.at(14);
    h.trimmed();
    int tt =t.toInt();
    int hh = h.toInt();
    int pp = s.at(0) == 'A'?0:1;

    Data d(Data(QDateTime::currentDateTime(),pp,tt,hh));

    if(place == 'A')
        INFO::cus_A.push_back(d);
    else
        INFO::cus_B.push_back(d);
    flush();
    s[16] = 0;
    s[17] = 0;
    f->append(d);
    m_Timer->stop();
    m_Timer->start(TIMEOUT);

    ui->log->append(s);
}

void MainWindow::showStatusMessage(const QString &message)
{
    ui->log->append(message);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete f;
}

void MainWindow::flush(){

    //INFO::cus.push_back(Data(QDateTime::currentDateTime(),1,rand()%3+20,rand()%10+50));
    ui->lb_tem->update();
    ui->lb_hum->update();

}

void MainWindow::mousePressEvent(QMouseEvent *event){
    qDebug() << event->x() << ","<< event->y();
}

void MainWindow::paintEvent(QPaintEvent* event){

    //paint.drawLine(200,100,50,);

}

void MainWindow::on_change_clicked()
{
    index +=5;
}

void MainWindow::initHisDs(){

    hisds.clear();
    f->read(hisds);
}

void MainWindow::on_pushButton_clicked()
{

    ui->tb->setRowCount(0);
    QDateTime s = ui->dt_s->dateTime();
    QDateTime e = ui->dt_e->dateTime();
    qDebug() << "s : "<< s << "e :"<< e;

    initHisDs();

    int row = ui->tb->rowCount();

    for(int i = 0;i < hisds.size();i++){
        if(hisds[i].date >= s && hisds[i].date <= e){
            ui->tb->insertRow(row);
            ui->tb->setItem(row,0,new QTableWidgetItem(hisds[i].date.toString("yyyy-MM-dd HH:mm:ss")));
            ui->tb->setItem(row,1,new QTableWidgetItem(QString::number(hisds[i].place)));
            ui->tb->setItem(row,2,new QTableWidgetItem(QString::number(hisds[i].temper)));
            ui->tb->setItem(row,3,new QTableWidgetItem(QString::number(hisds[i].humid)));
            row++;
        }
    }

}

void MainWindow::on_clear_clicked()
{
    ui->log->clear();
}

void MainWindow::on_btn_setting_clicked()
{
    m_settings->show();
}

void MainWindow::on_connect_clicked()
{
    if(openSerialPort()){
        m_Timer->start(1000);
    }
}

void MainWindow::on_disconnect_clicked()
{
    closeSerialPort();
    if(!m_serial->isOpen()){
        m_Timer->stop();
    }
}

