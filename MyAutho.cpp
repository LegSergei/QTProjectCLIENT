#include "MyAutho.h"
#include <qdatetime.h>
#include <QCryptographicHash>
#include <QString>

MyAutho::MyAutho(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.te_Main->append(u8"Добро пожаловать!");
	ui.te_Main->append(u8"Здесь вы можете войти в свою учетную запись.");
	connect(ui.pb_back, &QPushButton::clicked, this, &MyAutho::my_Back);
	connect(ui.pb_autho, &QPushButton::clicked, this, &MyAutho::my_Authorization);
}

MyAutho::~MyAutho()
{
}

void MyAutho::ServMessage_2(QString String)
{
	int i = 0;
	if (String.startsWith(u8"Вы"))
	{
		QString login_name = ui.le_Login->text();
		emit Success(login_name);
	}
	ui.te_Main->append(String);
	MyAutho::my_Back();
}

void MyAutho::recieveData_2(QTcpSocket* recieved_socket_2)
{
	if (recieved_socket_2 == nullptr)
	{
		ui.te_Main->append(u8": ОШИБКА! Переданный указатель не работает");
		return;
	}
	if (!recieved_socket_2->isValid())
	{
		ui.te_Main->append(u8": ОШИБКА! Переданный сокет не валиден");
		return;
	}
	if (!recieved_socket_2->isOpen())
	{
		ui.te_Main->append(u8": ОШИБКА! Переданный сокет закрыт");
		return;
	}
	autho_socket = recieved_socket_2;
}

void MyAutho::my_Authorization()
{
	QString Login = ui.le_Login->text();
	if (Login == "")
	{
		ui.te_Main->append(u8"Ошибка! Поле 'Введите Логин' не должно быть пустым!");
		return;
	}
	if (Login.length() > 15)
	{
		ui.te_Main->append(u8"Ошибка! Длина Логина не может превышать 15 символов!");
		return;
	}
	QString Pass1 = ui.le_Pass->text();
	if (Pass1 == "")
	{
		ui.te_Main->append(u8"Ошибка! Поле 'Введите Пароль' не должно быть пустым!");
		return;
	}
	if (Pass1.length() > 15)
	{
		ui.te_Main->append(u8"Ошибка! Длина Пароля не может превышать 15 символов!");
		return;
	}
	QByteArray hash_pass = QCryptographicHash::hash(Pass1.toUtf8(), (QCryptographicHash::Algorithm) 1);
	Pass1 = QString(hash_pass);
	QString Authorization = u8"AA9A " + Login + u8" " + Pass1;
	QByteArray ba_authorization;
	ba_authorization.append(Authorization);
	autho_socket->write(ba_authorization);
}

void MyAutho::my_Back()
{
	autho_socket = NULL;
	recieved_socket_2 = NULL;
	this->close();
	emit Return_2();
}