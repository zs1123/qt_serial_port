#ifndef PAINTLABEL_H
#define PAINTLABEL_H

#include<QPainter>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include"data.h"
#include <QtWidgets/qframe.h>
#include <QWidget>
#include <sys/time.h>
#include<QPen>
#include<time.h>
#include<QVector>
#include"info.h"
#define MAXTEM 50
#define MAXX 30
#define min(x,y) ((x) >(y)?(y):(x))
#define inf 0x7fffffff
class PaintLabel:public QLabel
{
    Q_OBJECT

public:
    explicit PaintLabel(QWidget *parent = 0):QLabel(parent){}

    void paintR(QPainter& painter){
        int pointx=20,pointy=this->height() - 20;//确定坐标轴起点坐标

        int width = this->width() - pointx -10;
        int height = this->height() - 30;
        painter.fillRect(pointx,10,width,height,Qt::white);
        painter.drawLine(pointx,10,pointx,pointy);
        painter.drawLine(pointx,pointy,this->width() - 10,pointy);



        double ky = (double)height/MAXTEM;
        double kx=(double)width/30;//y方向的比例系数
        //y轴刻度线
        double _maStep=2;//y轴刻度间隔需根据最大值来表示
        for(int i=0;i<25;i++)
        {
                //主要就是确定一个位置，然后画一条短短的直线表示刻度。
            painter.drawLine(pointx,pointy-(i+1)*height/25,
                                 pointx-4,pointy-(i+1)*height/25);
            if(i% 2 == 0){
                painter.drawText(pointx-20,pointy-(i+0.85)*height/25,
                                 QString::number((int)(_maStep*(i+1))));
            }
        }
    }
    void paintA(QPainter& painter){
        QVector<Data> ds = INFO::cus_A;
        QPen p;
        p.setColor(Qt::black);
        p.setWidth(2);

        painter.setPen(p);


        int pointx=20,pointy=this->height() - 20;//确定坐标轴起点坐标

        int width = this->width() - pointx -10;
        int height = this->height() - 30;

        int prx = pointx + width;

        double ky = (double)height/MAXTEM;
        double kx=(double)width/30;//y方向的比例系数
        //y轴刻度线
        double _maStep=2;//y轴刻度间隔需根据最大值来表示

        int n =ds.size();
        double sum=0;
        double ave=0;
        int _ma=0;//数组里的最大值
        int _mi=inf;

        int maxpos=0,minpos=0;

        QPen pen,penPoint;
        pen.setColor(Qt::green);
        pen.setWidth(1);
        penPoint.setColor(Qt::yellow);
        penPoint.setWidth(2);
        if(n <= 30){
            for(int i =0;i < n;i++){
                if(ds[i].temper >_ma){
                        maxpos = i;
                        _ma = ds[i].temper;
                }

                if(ds[i].temper< _mi){
                    minpos = i;
                    _mi = ds[i].temper;
                }
                sum += ds[i].temper;
            }
            for(int i = n-1;i>0;i--){
                painter.setPen(pen);//黑色笔用于连线

                   painter.drawLine(prx -(n-1-i)*kx,pointy-ds[i].temper*ky,prx- kx*(n-1-i+1),pointy-ds[i-1].temper*ky);
             painter.setPen(penPoint);//蓝色的笔，用于标记各个点
                painter.drawPoint(prx- kx*(n-1-i),pointy-ds[i].temper*ky);
            }

            if(n>0)
                painter.drawPoint(prx- kx*(n-1),pointy-ds[0].temper*ky);//绘制最后一个点
        }else{
            int start = n - 30;
            for(int i =start;i < n;i++){
                if(ds[i].temper >_ma){
                        maxpos = i;
                        _ma = ds[i].temper;
                }

                if(ds[i].temper< _mi){
                    minpos = i;
                    _mi = ds[i].temper;
                }
                sum += ds[i].temper;
            }
            for(int i = n-1;i>start;i--){
                painter.setPen(pen);//黑色笔用于连线

                painter.drawLine(prx -(n-1-i)*kx,pointy-ds[i].temper*ky,prx- kx*(n-1-i+1),pointy-ds[i-1].temper*ky);

                painter.setPen(penPoint);//蓝色的笔，用于标记各个点
                painter.drawPoint(prx- kx*(n-1-i),pointy-ds[i].temper*ky);
            }
            painter.drawPoint(prx- kx*(n-1),pointy-ds[start].temper*ky);//绘制最后一个点

        }
        ave=sum/min(MAXX,ds.size());//平均数

        //绘制平均线
        QPen penAve;
        penAve.setColor(QColor(140,50,220));//选择红色
        penAve.setWidth(1);
        penAve.setStyle(Qt::DotLine);//线条类型为虚线
        painter.setPen(penAve);
        painter.drawLine(pointx,pointy-ave*ky,pointx+width,pointy-ave*ky);

        //绘制最大值和最小值
        if(n >0){
            QPen penMaxMin;
            penMaxMin.setColor(Qt::darkGreen);//暗绿色
            painter.setPen(penMaxMin);
            painter.drawText(prx - kx*(n - maxpos -1),pointy-ds[maxpos].temper*ky-5,
                             "最大值"+QString::number(_ma));
            painter.drawText(prx-kx*(n -minpos -1),pointy-ds[minpos].temper*ky+15,
                             "最小值"+QString::number(_mi));
            penMaxMin.setColor(Qt::red);
            penMaxMin.setWidth(4);
            painter.setPen(penMaxMin);

            painter.drawPoint(prx -kx*(n -maxpos -1),pointy-ds[maxpos].temper*ky);//标记最大值点
            painter.drawPoint(prx - kx*(n - minpos-1),pointy-ds[minpos].temper*ky);//标记最小值点
        }
    }
    void paintB(QPainter& painter){

        QVector<Data> ds = INFO::cus_B;
        QPen p;
        p.setColor(Qt::black);
        p.setWidth(2);

        painter.setPen(p);


        int pointx=20,pointy=this->height() - 20;//确定坐标轴起点坐标

        int width = this->width() - pointx -10;
        int height = this->height() - 30;

        int prx = pointx + width;
        double ky = (double)height/MAXTEM;
        double kx=(double)width/30;//y方向的比例系数
        //y轴刻度线
        double _maStep=2;//y轴刻度间隔需根据最大值来表示

        int n =ds.size();
        double sum=0;
        double ave=0;
        int _ma=0;//数组里的最大值
        int _mi=inf;//inf为 #define inf 0x3f3f3f3f
        int maxpos=0,minpos=0;



        QPen pen,penPoint;
        pen.setColor(Qt::black);
        pen.setWidth(1);
        penPoint.setColor(Qt::blue);
        penPoint.setWidth(2);
        if(n <= 30){
            for(int i =0;i < n;i++){
                if(ds[i].temper >_ma){
                        maxpos = i;
                        _ma = ds[i].temper;
                }

                if(ds[i].temper< _mi){
                    minpos = i;
                    _mi = ds[i].temper;
                }
                sum += ds[i].temper;
            }
            for(int i = n-1;i>0;i--){
                painter.setPen(pen);//黑色笔用于连线

                   painter.drawLine(prx -(n-1-i)*kx,pointy-ds[i].temper*ky,prx- kx*(n-1-i+1),pointy-ds[i-1].temper*ky);
             painter.setPen(penPoint);//蓝色的笔，用于标记各个点
                painter.drawPoint(prx- kx*(n-1-i),pointy-ds[i].temper*ky);
            }

            if(n>0)
                painter.drawPoint(prx- kx*(n-1),pointy-ds[0].temper*ky);//绘制最后一个点
        }else{
            int start = n - 30;
            for(int i =start;i < n;i++){
                if(ds[i].temper >_ma){
                        maxpos = i;
                        _ma = ds[i].temper;
                }

                if(ds[i].temper< _mi){
                    minpos = i;
                    _mi = ds[i].temper;
                }
                sum += ds[i].temper;
            }
            for(int i = n-1;i>start;i--){
                painter.setPen(pen);//黑色笔用于连线

                painter.drawLine(prx -(n-1-i)*kx,pointy-ds[i].temper*ky,prx- kx*(n-1-i+1),pointy-ds[i-1].temper*ky);

                painter.setPen(penPoint);//蓝色的笔，用于标记各个点
                painter.drawPoint(prx- kx*(n-1-i),pointy-ds[i].temper*ky);
            }
            painter.drawPoint(prx- kx*(n-1),pointy-ds[start].temper*ky);//绘制最后一个点

        }
        ave=sum/min(MAXX,ds.size());//平均数

        //绘制平均线
        QPen penAve;
        penAve.setColor(Qt::red);//选择红色
        penAve.setWidth(1);
        penAve.setStyle(Qt::DotLine);//线条类型为虚线
        painter.setPen(penAve);
        painter.drawLine(pointx,pointy-ave*ky,pointx+width,pointy-ave*ky);


        //绘制最大值和最小值
        if(n >0){
            QPen penMaxMin;
            penMaxMin.setColor(Qt::darkGreen);//暗绿色
            painter.setPen(penMaxMin);
            painter.drawText(prx - kx*(n - maxpos -1),pointy-ds[maxpos].temper*ky-5,
                             "最大值"+QString::number(_ma));
            painter.drawText(prx-kx*(n -minpos -1),pointy-ds[minpos].temper*ky+15,
                             "最小值"+QString::number(_mi));
            penMaxMin.setColor(Qt::red);
            penMaxMin.setWidth(4);
            painter.setPen(penMaxMin);

            painter.drawPoint(prx -kx*(n -maxpos -1),pointy-ds[maxpos].temper*ky);//标记最大值点
            painter.drawPoint(prx - kx*(n - minpos-1),pointy-ds[minpos].temper*ky);//标记最小值点
        }
    }
   void paintEvent(QPaintEvent *event)
    {

       QVector<Data> ds = INFO::cus_B;
       QPainter painter(this);

       painter.setRenderHint(QPainter::Antialiasing, true);
        paintR(painter);
        paintA(painter);
        paintB(painter);

        QLabel::paintEvent(event);



    }


};
#endif // PAINTLABEL_H
