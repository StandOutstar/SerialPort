#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //时间
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(qtimeSlot()));
    timer->start(1000);

    ui->textEdit->append(tr("检测到端口列表:"));
    //获取串口信息
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox_2->addItem(info.portName());//记录端口名
            ui->textEdit->append(info.portName());//端口名
            ui->textEdit->append(info.description());//端口描述
            ui->textEdit->append(info.manufacturer());//制造商
            serial.close();//关闭
        }
    }
    ui->comboBox->setCurrentIndex(3);//默认9600
    ui->pushButton->setText(tr("打开串口"));
    ui->pushButton_18->setEnabled(false);//发送按钮失效

}

MainWindow::~MainWindow()
{
    delete ui;
}
//接收数据槽函数
void MainWindow::my_readuart()
{
    QByteArray requestData;
    requestData = my_serialport->readAll();
    if(requestData!= NULL)
    {
        ui->textEdit->append(tr(requestData.toHex()));
    }
    requestData.clear();
}
//打开串口槽函数
void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->text()==tr("打开串口")){
        my_serialport= new QSerialPort();
        my_serialport->setPortName(ui->comboBox_2->currentText());//获取端口名
        my_serialport->open(QIODevice::ReadWrite);//打开串口
        my_serialport->setBaudRate(ui->comboBox->currentText().toInt());//设置波特率
        //校验设置
        switch(ui->comboBox_3->currentIndex()){
        case 0: my_serialport->setParity(QSerialPort::NoParity);break;
        case 1: my_serialport->setParity(QSerialPort::OddParity);break;
        case 2: my_serialport->setParity(QSerialPort::EvenParity);break;
        default: break;
        }
        //数据位数
        switch(ui->comboBox_4->currentIndex()){
        case 0: my_serialport->setDataBits(QSerialPort::Data8);break;
        case 1: my_serialport->setDataBits(QSerialPort::Data7);break;
        case 2: my_serialport->setDataBits(QSerialPort::Data6);break;
        default: break;
        }
        //停止位
        switch(ui->comboBox_5->currentIndex()){
        case 0: my_serialport->setStopBits(QSerialPort::OneStop);break;
        case 1: my_serialport->setStopBits(QSerialPort::TwoStop);break;
        default: break;
        }
        my_serialport->setFlowControl(QSerialPort::NoFlowControl);
        connect(my_serialport,SIGNAL(readyRead()),this,SLOT(my_readuart()));

        ui->comboBox->setEnabled(false);//波特率设置失效
        ui->comboBox_2->setEnabled(false);//端口名设置失效
        ui->comboBox_3->setEnabled(false);//校验设置失效
        ui->comboBox_4->setEnabled(false);//数据位数设置失效
        ui->comboBox_5->setEnabled(false);//停止位设置失效

        ui->label_6->setStyleSheet("background-color:red");
        ui->pushButton->setText(tr("关闭串口"));
        ui->pushButton_18->setEnabled(true);
    }
    else {
        my_serialport->clear();
        my_serialport->deleteLater();
        ui->comboBox->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        ui->comboBox_4->setEnabled(true);
        ui->comboBox_5->setEnabled(true);

        ui->label_6->setStyleSheet("background-color:rgb(130,130,130)");
        ui->pushButton->setText(tr("打开串口"));
        ui->pushButton_18->setEnabled(false);//发送按钮有效
    }
}
//发送槽函数
void MainWindow::on_pushButton_18_clicked()
{
    QByteArray writeData;
    writeData = QString2Hex(ui->lineEdit->text());
    my_serialport->write(writeData);
}

char MainWindow::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}
//16进制转换函数
QByteArray MainWindow::QString2Hex(QString str)
{
    QByteArray senddata;
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
    return senddata;
}
void MainWindow::qtimeSlot()
{
    QTime qtimeObj = QTime::currentTime();
    QString strTime = qtimeObj.toString("hh:mm:ss ap");
//    strTime.prepend("  Time is: ");
    ui->label_9->setText(strTime);
//    ui->label_9->show();
//    QDate qdateObj = QDate::currentDate();
//    QString strDate = qdateObj.toString("dddd, MMMM d, yyyy"); //星期、月份、天、年
//    strDate.prepend("Today is: ");

//    strDate.append(strTime);
//    ui.timeLabel->setText (strDate);

    //qDebug() << strTime;
    //qDebug() << strDate;
}


//2016/7/11 23:33实现了能够发送十六进制数，和接收显示十六进制数。
