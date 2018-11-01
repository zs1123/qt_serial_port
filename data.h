#ifndef DATA_H
#define DATA_H
#include<QString>
#include<QDateTime>
struct Data{
    QDateTime date;
    int place;
    int temper;
    int humid;
    Data(){}
    Data(QDateTime _date,int _place,int _temper,int _humid):date(_date),place(_place),temper(_temper),humid(_humid){
    }
};
#endif // DATA_H
