#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>     // add
#include <qserialportinfo.h>                // add
#include <stdio.h>
#include <qmessagebox.h>
#include <qdebug.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSerialPort *port;      // add




private slots:  // 콜벡함수
    void text_reading();

    void on_pushButton_scan_again_clicked();

    void on_pushButton_open_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_send_clicked();

    void on_pushButton_send_2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
