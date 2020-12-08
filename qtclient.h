#pragma once

#include <QtWidgets/QWidget>
#include "ui_qtclient.h"
#include <QTcpSocket>
#include "ui_MyRegi.h"
#include "ui_MyAutho.h"

class MyRegi;
class QtCLIENT : public QWidget
{
    Q_OBJECT

public:
    QtCLIENT(QWidget *parent = Q_NULLPTR);
    QString UserName;
    int Chat_number;

public slots:
    void res_Authorization(QString string);


signals:
    void sendData(QTcpSocket* send_socket); // Этот сигна будет передавать сокет в другую форму. В теории.
    void sendData_2(QTcpSocket* send_socket_2); // Этот сигна будет передавать сокет в другую форму. В теории.
    void send_to_Registration(QString String); // Передача сообщения сервера на форму.
    void send_to_Authorization(QString String);


private:
    Ui::QtCLIENTClass ui;
    QTcpSocket* my_socket;
    QTcpSocket* my_socket_2;
    MyRegi* my_reg;


private slots:
    void my_Connect(); //Слот обработки нажатия на кнопку "Подключиться"
    void my_SendMessage(); //Слот обработки нажатия на кнопку "Отправить"
    void my_Connected();// Слот обработки сигнала что подключение состоялось
    void my_DConnected();// Слот обработки сигнала что подключение закрылось
    void my_stateChanged(QAbstractSocket::SocketState socketState); //Обрабатываем сигнал об изменении состояния подключения к серверу
    void my_readyRead();
    void my_Registration(); //Слот обработки нажатия на кнопку "Зарегистрироваться"
    void my_Authorization();
    void Refresh();
    void Exit();
    void MainMessageManufacturing(QString String);
    void ChatChange();
    void TranslateMessage(QString String2);
    void TranslateMessageA(QString String2);
};
