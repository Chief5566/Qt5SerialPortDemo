#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_ComScan_clicked()
{
    comPortScan();
}

void Widget::on_pushButton_ComConnect_clicked()
{
    configCom();
}

void Widget::on_pushButton_Send_clicked()
{
    sendData();
}

void Widget::on_pushButton_Close_clicked()
{
    this->close();
}

void Widget::on_pushButton_Min_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}

void Widget::on_checkBox_AutoSend_clicked(bool checked)
{
    if(checked == true){
        ui->lineEdit_AutoSendTime->setEnabled(false);
        ui->groupBox_send->setEnabled(false);
        bool ok;
        int time = ui->lineEdit_AutoSendTime->text().toInt(&ok,10);
        timer->start(time);
    }
    else{
        ui->lineEdit_AutoSendTime->setEnabled(true);
        ui->groupBox_send->setEnabled(true);
        timer->stop();
    }
}


