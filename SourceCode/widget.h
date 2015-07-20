#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QIODevice>
#include <QString>
#include <QByteArray>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTimer>
#include <QBitmap>
#include <QPainter>
#include <QBrush>
#include <QIODevice>
#include <QDataStream>
#include <QFile>
#include <QFileDevice>
#include <QFileDialog>
#include <QLabel>
#include "custom_label.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public slots:
    void comPortScan();
    void comDataRec();
    void init();
    void configCom();
    void sendData();

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void uiCircular();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void labelCleanSendSlot();
    void labelCleanReceiveSlot();
    void labelOpenFileSlot();
    void labelSaveFileSlot();

    void on_pushButton_ComScan_clicked();

    void on_pushButton_ComConnect_clicked();

    void on_pushButton_Send_clicked();

    void on_pushButton_Close_clicked();

    void on_pushButton_Min_clicked();

    void on_checkBox_AutoSend_clicked(bool checked);


private:
    Ui::Widget *ui;
    QSerialPort serial;
    bool comConnect;
    QPoint movePoint; //移动的距离
    bool mousePress; //鼠标按下
    QTimer *timer;
};

#endif // WIDGET_H
