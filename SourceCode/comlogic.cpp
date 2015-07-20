#include "widget.h"
#include "ui_widget.h"

void Widget::comPortScan()
{
    ui->comboBox_ComPort->clear();
    foreach(const QSerialPortInfo &info,
            QSerialPortInfo::availablePorts()){
        ui->comboBox_ComPort->addItem(info.portName());
    }
}

void Widget::uiCircular()
{
        this->setWindowFlags(Qt::FramelessWindowHint);
        QBitmap bmp(this->size());
        bmp.fill();
        QPainter p(&bmp);
        p.setPen(Qt::NoPen);//绘制矩形用NoPen,帮助文档有说明。
        p.setBrush(Qt::black);
        p.drawRoundedRect(bmp.rect(),15,15);
        this->setMask(bmp);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        mousePress = true;
        movePoint = event->pos();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePress){
        QPoint movePos = event->globalPos();
        this->move(movePos - movePoint);
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    mousePress = false;
}

void Widget::comDataRec()
{
    QByteArray rec = serial.readAll();
    ui->textEdit_Receive->insertPlainText(rec);
}

void Widget::labelCleanReceiveSlot()
{
    ui->textEdit_Receive->clear();
}

void Widget::labelCleanSendSlot()
{
    ui->textEdit_Send->clear();
}

void Widget::labelOpenFileSlot()
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("打开一个txt文件"),
                QDir::currentPath(),
                tr("txt文件(*.txt)"));
    if(filePath != NULL){
    QFile openFile(filePath);
    if(!openFile.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this,"错误",
                              "文件打开失败！！",
                              QMessageBox::Yes);
        return;
    }
    while (!openFile.atEnd()) {
        QByteArray data = openFile.readLine();
        ui->textEdit_Send->clear();
        ui->textEdit_Send->setText(QString(data));
    }
    }
}

void Widget::labelSaveFileSlot()
{
    QString fileName= QFileDialog::getSaveFileName(
                this,
                tr("保存文件"),
                QDir::currentPath(),
                tr("文本文件(*txt)"));
    if(fileName != NULL){
        QFile saveFile(fileName);
        if(!saveFile.open(QIODevice::WriteOnly | QIODevice::Text)){
            QMessageBox::critical(this,"错误",
                                  "文件打开失败！！",
                                  QMessageBox::Yes);
            return;
        }
        QTextStream writeData(&saveFile);
        writeData << ui->textEdit_Receive->toPlainText();
    }
}

void Widget::init()
{
    uiCircular();
    comConnect = false;
    mousePress = false;
    this->setMouseTracking(true);
    ui->textEdit_Receive->setReadOnly(true);
    ui->groupBox_AutoSend->setEnabled(false);
    ui->groupBox_send->setEnabled(false);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(sendData()));
    connect(&serial,SIGNAL(readyRead()),this,SLOT(comDataRec()));
    connect(ui->label_CleanRecShow,SIGNAL(clicked()),this,SLOT(labelCleanReceiveSlot()));
    connect(ui->label_CleanSendShow,SIGNAL(clicked()),this,SLOT(labelCleanSendSlot()));
    connect(ui->label_SendData,SIGNAL(clicked()),this,SLOT(labelOpenFileSlot()));
    connect(ui->label_SaveShow,SIGNAL(clicked()),this,SLOT(labelSaveFileSlot()));
    comPortScan();  
    comPortScan();
}

void Widget::configCom()
{
    QString comPort = ui->comboBox_ComPort->currentText();
    int baudRate = ui->comboBox_BaudRate->currentIndex();
    int dataBit = ui->comboBox_DataBit->currentIndex();
    int checkBit = ui->comboBox_CheckBit->currentIndex();
    int stopBit = ui->comboBox_StopBit->currentIndex();

    if(comConnect == false)
    {
        ui->groupBox_AutoSend->setEnabled(true);
        ui->groupBox_send->setEnabled(true);
        serial.setPortName(comPort);

        switch(baudRate){
        case 0: serial.setBaudRate(QSerialPort::Baud1200);break;
        case 1: serial.setBaudRate(QSerialPort::Baud2400);break;
        case 2: serial.setBaudRate(QSerialPort::Baud4800);break;
        case 3: serial.setBaudRate(QSerialPort::Baud9600);break;
        case 4: serial.setBaudRate(QSerialPort::Baud19200);break;
        case 5: serial.setBaudRate(QSerialPort::Baud38400);break;
        case 6: serial.setBaudRate(QSerialPort::Baud57600);break;
        case 7: serial.setBaudRate(QSerialPort::Baud115200);break;
        default:break;
        }

        switch(dataBit){
        case 0: serial.setDataBits(QSerialPort::Data5);break;
        case 1: serial.setDataBits(QSerialPort::Data6);break;
        case 2: serial.setDataBits(QSerialPort::Data7);break;
        case 3: serial.setDataBits(QSerialPort::Data8);break;
        default:break;
        }

        switch(checkBit){
        case 0: serial.setParity(QSerialPort::NoParity);break;
        case 1: serial.setParity(QSerialPort::EvenParity);break;
        case 2: serial.setParity(QSerialPort::OddParity);break;
        case 3: serial.setParity(QSerialPort::SpaceParity);break;
        case 4: serial.setParity(QSerialPort::MarkParity);break;
        default:break;
        }

        switch(stopBit){
        case 0: serial.setStopBits(QSerialPort::OneStop);break;
        case 1: serial.setStopBits(QSerialPort::OneAndHalfStop);break;
        case 2: serial.setStopBits(QSerialPort::TwoStop);break;
        default:break;
        }

        if(!serial.open(QIODevice::ReadWrite)){
            comConnect = false;
            QMessageBox::warning(this,"错误",
                                  serial.errorString(),
                                  QMessageBox::Yes);
        }
        else{
            comConnect = true;
            ui->groupBox_ComControl->setEnabled(false);
            ui->pushButton_ComConnect->setText("断开串口");
        }
    }
    else{
        comConnect = false;
        if(serial.isOpen()) serial.close();
        ui->groupBox_ComControl->setEnabled(true);
        ui->pushButton_ComConnect->setText("连接串口");
        ui->groupBox_AutoSend->setEnabled(false);
        ui->groupBox_send->setEnabled(false);
    }
}

void Widget::sendData()
{
    if(serial.isOpen()){
        QByteArray send =ui->textEdit_Send->toPlainText().toLatin1();
        serial.write(send,send.length());
    }
}
