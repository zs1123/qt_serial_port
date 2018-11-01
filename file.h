#ifndef FILE_H
#define FILE_H
#include<QFile>
#include<QDebug>
#include"data.h"
class File{
private:
    QFile* file;
    QTextStream* stream;
public:
    File(QString filename){
        file = new QFile(filename);
        if(!file->exists()){
            file->setFileName(filename);
        }
        if(!file->open(QFileDevice::ReadWrite| QFileDevice::Text)){
            qDebug() << "file open failed";
            exit(0);
        }
        qDebug() << "open success";
        stream = new QTextStream(file);
    }
    void append(Data d){
        stream->seek(file->size());
        QString str;
        str += d.date.toString("yyyy-MM-dd HH:mm:ss");
        str.append(";");
        str+=QString::number(d.place);
        str.append(";");
        str += QString::number(d.temper);
        str.append(";");
        str+= QString::number(d.humid);
        str.append("\r\n");
        (*stream) << str;
        stream->flush();
        file->flush();
    }
    void append(QVector<Data>& vec){
        stream->seek(file->size());
        for(int i =0;i < vec.size();i++){
            QString str;
            str += vec[i].date.toString("yyyy-MM-dd HH:mm:ss");
            str.append(";");
            str+=QString::number(vec[i].place);
            str.append(";");
            str += QString::number(vec[i].temper);
            str.append(";");
            str+= QString::number(vec[i].humid);
            str.append("\r\n");
            (*stream) << str;
        }
        stream->flush();
        file->flush();
    }

    void read(QVector<Data>& vec){

        stream->seek(0);
        while(!stream->atEnd()){
            QString str = stream->readLine();
            QStringList args = str.split(";");
            vec.push_back(Data(QDateTime::fromString(args[0],"yyyy-MM-dd HH:mm:ss"),args[1].toInt(),args[2].toInt(),args[3].toInt()));
        }
    }


    ~File(){
        file->close();
        delete file;
        delete stream;
    }

};
#endif // FILE_H
