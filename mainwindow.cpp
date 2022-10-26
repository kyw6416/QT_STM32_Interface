#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 콤보박스 내용 추가하
    // --------------- setting speed--------------
    ui -> comboBox_speed -> addItem("9600", QSerialPort::Baud9600);     // QSerialPort 안에 있는 enum값을 불러옴
    ui -> comboBox_speed -> addItem("115200", QSerialPort::Baud115200); // QSerialPort 안에 있는 enum값을 불러옴

    // --------------- setting databit--------------
    ui -> comboBox_databit -> addItem("8", QSerialPort::Data8);         // 8bit
    ui -> comboBox_databit -> addItem("7", QSerialPort::Data7);         // 7bit

    // --------------- setting Parity--------------
    ui -> comboBox_parity -> addItem("None", QSerialPort::NoParity);
    ui -> comboBox_parity -> addItem("Even", QSerialPort::EvenParity);
    ui -> comboBox_parity -> addItem("Odd", QSerialPort::OddParity);

    port = new QSerialPort(this); // 오브젝트 후 오브젝트에 할당
    QObject::connect(port,SIGNAL(readyRead()),this,SLOT(text_reading()));  // port부터 데이터가 있으면 callback
    on_pushButton_scan_again_clicked();

}

MainWindow::~MainWindow()
{
    port->close();
    delete ui;
}

void MainWindow::text_reading()
{
#if 1
    QList <QByteArray> receivedList;            // QByteArray를 List 자료구조로 만들기
    char tmp_char[6];                                // class
    char wet_char[6];
    char msg[40];

    while (port -> canReadLine()){                        // \n 존재확인
        receivedList.append(port -> readLine());          // \n 읽어서 received
    }

    for (QByteArray received : receivedList) {
        sprintf(msg, "%s", received.data());
        if (!strncmp(msg,"(Tmp)", strlen("(Tmp)"))){
            strcpy(tmp_char, msg+5);
            int tmp = atoi(tmp_char);
            ui -> lcdNumber_Tmp -> display(tmp);
        }
        else if (!strncmp(msg,"(wet)", strlen("(wet)"))){
            strcpy(wet_char, msg+5);
            int wet = atoi(wet_char);
            ui -> lcdNumber_Wet -> display(wet);
        }
        ui -> textEdit_recive -> append(received);  // 1개씩 추가
        ui -> textEdit_recive -> show();            // display
    }

#endif

#if 0
    QByteArray  received; // 1byte를 Array형태

    while (port -> canReadLine()){               // \n 존재확인
        received = port -> readLine();          // \n 읽어서 received
    }
    ui -> textEdit_recive -> append(received);  // 1개씩 추가
    ui -> textEdit_recive -> show();            // display

    qDebug() << received;
#endif
}

void MainWindow::on_pushButton_scan_again_clicked()
{
    ui -> comboBox_port -> clear();     // comboBox clear

    // stm32와 연결
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        ui -> comboBox_port -> addItem(serialPortInfo.portName());
    }
}

void MainWindow::on_pushButton_open_clicked()
{
    port -> setPortName(ui -> comboBox_port -> currentText());  // 포트 연결
    port -> clear();

    if (ui -> comboBox_speed -> currentIndex() == 0)
        port -> setBaudRate(QSerialPort::Baud9600);
    else if (ui -> comboBox_speed -> currentIndex() == 1)
        port -> setBaudRate(QSerialPort::Baud115200);

    if (ui -> comboBox_databit -> currentIndex() == 0)
        port -> setDataBits(QSerialPort::Data8);
    else if (ui -> comboBox_databit -> currentIndex() == 1)
        port -> setDataBits(QSerialPort::Data7);

    if (ui -> comboBox_parity -> currentIndex() == 0)
        port -> setParity(QSerialPort::NoParity);
    else if (ui -> comboBox_parity -> currentIndex() == 1)
        port -> setParity(QSerialPort::EvenParity);
    else if (ui -> comboBox_parity -> currentIndex() == 2)
        port -> setParity(QSerialPort::OddParity);

    if(!port->open(QIODevice::ReadWrite))
    {
        qDebug() << " \n Serial Port Open Error !!!\n";
    }
    else
    {
        ui -> progressBar_port_Status -> setValue(100);
        qDebug() << " \n Serial Port Open Success  !!!\n";
    }

}

void MainWindow::on_pushButton_close_clicked()
{
    port -> clear();
    port->close();
    ui -> progressBar_port_Status -> setValue(0);
}

void MainWindow::on_pushButton_send_clicked()
{
    QByteArray sendData;

    sendData = QString(ui -> lineEdit_send -> text() + "\n").toUtf8();
    port -> write(sendData.data());

    qDebug() << sendData;
}

void MainWindow::on_pushButton_send_2_clicked()
{
    if (ui -> comboBox_dht11 -> currentIndex() == 1)
    ui -> lineEdit_send -> setText(" ");
    ui -> textEdit_recive -> setText(" ");
}
