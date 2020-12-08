#pragma once

#include <QtWidgets/QWidget>
#include "ui_MyRegi.h"
#include <QTcpSocket>

class MyRegi : public QWidget
{
	Q_OBJECT

public:
	MyRegi(QWidget *parent = Q_NULLPTR);
	~MyRegi();
	QTcpSocket* recieved_socket;

public slots:
	void recieveData(QTcpSocket* recieved_socket); // Этот слот будет принимать сокет из другой формы. В теории.
	void ServMessage(QString String);


signals:
	void Return();

private:
	Ui::MyRegi ui;
	QTcpSocket* regis_socket;


private slots:
	void my_Registration(); //Слот обработки нажатия на кнопку "Зарегистрироваться"
	void my_Back(); //Слот обработки нажатия на кнопку "Назад"
	void my_readyRead();
};
