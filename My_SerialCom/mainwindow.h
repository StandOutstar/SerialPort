#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDateTime>
#include <QTime>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QByteArray QString2Hex(QString str);
    char ConvertHexChar(char ch);
private:
    Ui::MainWindow *ui;
    QSerialPort *my_serialport;
    QTimer *timer;

private slots:
    void my_readuart();//串口接收数据槽函数
    void on_pushButton_clicked();
    void on_pushButton_18_clicked();
    void qtimeSlot();
};

#endif // MAINWINDOW_H
