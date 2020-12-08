#pragma once

#include <QtWidgets/QWidget>
#include "ui_MyAutho.h"
#include <QTcpSocket>

class MyAutho : public QWidget
{
	Q_OBJECT

public:
	MyAutho(QWidget *parent = Q_NULLPTR);
	~MyAutho();
	QTcpSocket* recieved_socket_2;

public slots:
	void recieveData_2(QTcpSocket* recieved_socket_2); // Этот сигнал будет принимать сокет из другой формы. В теории.
	void ServMessage_2(QString String);

signals:
	void Return_2();
	void Success(QString String);// Оповещаем главную форму о том, что пароль введен верно

private:
	Ui::MyAutho ui;
	QTcpSocket* autho_socket;

private slots:
	void my_Authorization(); //Слот обработки нажатия на кнопку "Войти"
	void my_Back();

};
