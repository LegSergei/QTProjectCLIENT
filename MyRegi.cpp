#include "MyRegi.h"
#include <QTcpSocket>
#include <qdatetime.h>
#include <QCryptographicHash>
#include <QString>

MyRegi::MyRegi(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.te_Main->append(u8"Добро пожаловать!");
	ui.te_Main->append(u8"Здесь вы можете создать нового пользователя.");
	ui.te_Main->append(u8"Пожалуйста помните, что длина логина и пароля не может превышать 15 символов.");
	ui.te_Main->append(u8"Пробелы или комбинация %&? в логине и пароле не допускаются.");
	connect(ui.pb_back, &QPushButton::clicked, this, &MyRegi::my_Back);
	connect(ui.pb_reg, &QPushButton::clicked, this, &MyRegi::my_Registration);
}

MyRegi::~MyRegi()
{
}

void MyRegi::ServMessage(QString String)
{
	ui.te_Main->append(String);

}

void MyRegi::my_readyRead()
{
}

void MyRegi::recieveData(QTcpSocket* recieved_socket)
{
	if (recieved_socket == nullptr)
	{
		QDateTime date = QDateTime::currentDateTime();
		ui.te_Main->append(date.toString("dd.MM.yyyy hh:mm:ss") + u8": ОШИБКА! Переданный указатель не работает");
		return;
	}
	if (!recieved_socket->isValid())
	{
		QDateTime date = QDateTime::currentDateTime();
		ui.te_Main->append(date.toString("dd.MM.yyyy hh:mm:ss") + u8": ОШИБКА! Переданный сокет не валиден");
		return;
	}
	if (!recieved_socket->isOpen())
	{
		QDateTime date = QDateTime::currentDateTime();
		ui.te_Main->append(date.toString("dd.MM.yyyy hh:mm:ss") + u8": ОШИБКА! Переданный сокет закрыт");
		return;
	}
	regis_socket = recieved_socket;
	connect(regis_socket, &QTcpSocket::readyRead, this, &MyRegi::my_readyRead);
}


void MyRegi::my_Registration()
{
	QString Login = ui.le_Login->text();
	if (Login == "")
	{
		ui.te_Main->append(u8"Ошибка! Поле 'Введите Логин' не должно быть пустым!");
		return;
	}
	if (Login.indexOf(" ") != -1)
	{
		ui.te_Main->append(u8"Ошибка! Логин не должен содержать пробелы!");
		return;
	}
	if (Login.length() > 15)
	{
		ui.te_Main->append(u8"Ошибка! Длина Логина не должна превышать 15 символов!");
		return;
	}
	if (Login.indexOf("%&?") != -1)
	{
		ui.te_Main->append(u8"Ошибка! Логин содержит запрещенную комбинацию символов - <%&?>!");
		return;
	}
	QString Pass1 = ui.le_Pass_1->text();
	if (Pass1 == "")
	{
		ui.te_Main->append(u8"Ошибка! Поле 'Введите Пароль' не должно быть пустым!");
		return;
	}
	if (Pass1.indexOf(" ") != -1)
	{
		ui.te_Main->append(u8"Ошибка! Пароль не должен содержать пробелы!");
		return;
	}
	if (Pass1.indexOf("%&?") != -1)
	{
		ui.te_Main->append(u8"Ошибка! Пароль содержит запрещенную комбинацию символов - <%&?>!");
		return;
	}
	if (Pass1.length() > 15)
	{
		ui.te_Main->append(u8"Ошибка! Длина Пароля не должна превышать 15 символов!");
		return;
	}
	QString Pass2 = ui.le_Pass_2->text();
	if (Pass2 == "")
	{
		ui.te_Main->append(u8"Ошибка! Поле 'Повторите Пароль' не должно быть пустым!");
		return;
	}
	if (Pass1 != Pass2)
	{
		ui.te_Main->append(u8"Ошибка! Пароли не совпадают!");
		return;
	} 
	QByteArray hash_pass = QCryptographicHash::hash(Pass1.toUtf8(), (QCryptographicHash::Algorithm) 1);
	Pass1 = QString(hash_pass);
	QString registration = u8"AA7A " + Login + u8" " + Pass1;
	QByteArray ba_registration;
	ba_registration.append(registration);
	regis_socket->write(ba_registration);
}



void MyRegi::my_Back()
{
	regis_socket = NULL;
	recieved_socket = NULL;
	this->close();
	emit Return();
}
