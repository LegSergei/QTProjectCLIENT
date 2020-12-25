#include "qtclient.h"
#include "MyRegi.h"
#include "MyAutho.h"
#include <QTcpSocket>
#include <qdatetime.h>
#include "Windows.h"

QtCLIENT::QtCLIENT(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    my_reg = new MyRegi();
    my_auth = new MyAutho();
    connect(my_reg, &MyRegi::Return, this, &QtCLIENT::show);
    connect(my_auth, &MyAutho::Return_2, this, &QtCLIENT::show);
    connect(this, &QtCLIENT::sendData, my_reg, &MyRegi::recieveData);
    connect(this, &QtCLIENT::send_to_Registration, my_reg, &MyRegi::ServMessage);
    connect(this, &QtCLIENT::sendData_2, my_auth, &MyAutho::recieveData_2);
    connect(this, &QtCLIENT::send_to_Authorization, my_auth, &MyAutho::ServMessage_2);
    connect(my_auth, &MyAutho::Success, this, &QtCLIENT::res_Authorization);
    connect(ui.pb_connect, &QPushButton::clicked, this, &QtCLIENT::my_Connect);
    connect(ui.pb_send, &QPushButton::clicked, this, &QtCLIENT::my_SendMessage);
    connect(ui.pb_Registration, &QPushButton::clicked, this, &QtCLIENT::my_Registration);
    connect(ui.pb_Authorization, &QPushButton::clicked, this, &QtCLIENT::my_Authorization);
    connect(ui.pb_Ref, &QPushButton::clicked, this, &QtCLIENT::Refresh);
    connect(ui.pb_exit, &QPushButton::clicked, this, &QtCLIENT::Exit);
//    connect(ui.pb_change, &QPushButton::clicked, this, &QtCLIENT::ChatChange);
    connect(ui.tabWidget, &QTabWidget::currentChanged, this, &QtCLIENT::ChatChange);
    UserName = u8"Аноним";
    Chat_number = 1;
    ui.le_username->setText(UserName);
    my_socket = nullptr;
}


void QtCLIENT::my_Connect()
{
    if (my_socket != nullptr)
    {
        if (my_socket->isValid() && my_socket->isOpen())
        {
            my_socket->abort();
            my_socket->deleteLater();
            my_socket = nullptr;
        }
    }
    my_socket = new QTcpSocket(this);

    connect(my_socket, &QTcpSocket::connected, this, &QtCLIENT::my_Connected); //Сигнал об установлении соединения подключен к соотв блоку кода
    connect(my_socket, &QTcpSocket::disconnected, this, &QtCLIENT::my_DConnected);//Сигнал о разрыве соединения подключен к соотв блоку кода
    connect(my_socket, &QTcpSocket::stateChanged, this, &QtCLIENT::my_stateChanged);//Сигнал об изменении состояния соединения подключен к соотв блоку кода
    connect(my_socket, &QTcpSocket::readyRead, this, &QtCLIENT::my_readyRead);//Сигнал о готовности к передачи подключен к соотв блоку кода

    int port = 4110; // Номер порта который будет слушать сервер.
    QString address = "127.0.0.1"; //Адрес сервера в сети
    QDateTime date = QDateTime::currentDateTime();
    ui.te_support->append(date.toString("dd.MM.yyyy hh:mm:ss") + u8": Подключение...");
    my_socket->connectToHost(address, port);
}

void QtCLIENT::my_SendMessage()
{
    if (my_socket == nullptr)
    {
        QDateTime date = QDateTime::currentDateTime();
        ui.te_support->append(date.toString("dd.MM.yyyy hh:mm:ss") + u8": ОШИБКА! Указатель не работает");
        return;
    }
    if (!my_socket->isValid())
    {
        QDateTime date = QDateTime::currentDateTime();
        ui.te_support->append(date.toString("dd.MM.yyyy hh:mm:ss") + u8": ОШИБКА! Сокет не валиден");
        return;
    }
    if (!my_socket->isOpen())
    { 
        QDateTime date = QDateTime::currentDateTime();
        ui.te_support->append(date.toString("dd.MM.yyyy hh:mm:ss") + u8": ОШИБКА! Сокет закрыт");
        return;
    }

    QString new_message = "AA0A%&?" + UserName + "%&?" + ui.le_message->text();
    my_socket->write(new_message.toUtf8());
    ui.le_message->clear();
    my_socket->flush();
    QtCLIENT::Refresh();

}

void QtCLIENT::my_Connected()
{
    QDateTime date = QDateTime::currentDateTime();
    ui.te_support->append(date.toString("dd.MM.yyyy hh:mm:ss") + u8": Соединение установлено");
    ui.pb_send->setEnabled(true); // Разблокировка кнопки отправить
    ui.pb_Ref->setEnabled(true);
    ui.pb_Authorization->setEnabled(true);
    ui.pb_Registration->setEnabled(true);
    ui.le_message->setEnabled(true);
    ui.tabWidget->setEnabled(true);
    QtCLIENT::Refresh();

    my_socket_2 = qobject_cast<QTcpSocket*>(sender()); //Сохранение сокета.sender - это указатель на объект, испустивший сигнал.

    ui.pb_connect->setEnabled(true);
}

void QtCLIENT::my_DConnected()
{
    ui.pb_send->setEnabled(false); // Блокировка кнопки отправить
    ui.le_message->setEnabled(false);
    ui.pb_Ref->setEnabled(false);
    ui.pb_Authorization->setEnabled(false);
    ui.pb_Registration->setEnabled(false);
    ui.tabWidget->setEnabled(false);
    my_socket->deleteLater();
    my_socket = nullptr;
}

void QtCLIENT::my_stateChanged(QAbstractSocket::SocketState socketState)
{
    QString state; // Состояние соединения 
    switch (socketState)
    {
    case QAbstractSocket::UnconnectedState:
        state = "UnconnectedState";
        break;
    case QAbstractSocket::HostLookupState:
        state = "HostLookupState";
        break;
    case QAbstractSocket::ConnectingState:
        state = "ConnectingState";
        break;
    case QAbstractSocket::ConnectedState:
        state = "ConnectedState";
        break;
    case QAbstractSocket::BoundState:
        state = "BoundState";
        break;
    case QAbstractSocket::ClosingState:
        state = "ClosingState";
        break;
    case QAbstractSocket::ListeningState:
        state = "ListeningState";
        break;
    }
    QDateTime date = QDateTime::currentDateTime();
    ui.te_support->append(date.toString("dd.MM.yyyy hh:mm:ss") + u8": Сокет изменил состояние на: " + state);
}

void QtCLIENT::my_readyRead()
{
// Как только мы получаем от сокета сигнал, что к нам пришло сообщение, активируется этот кусок кода, поскольку мы соединили
// появление этого сигнала с этим кодом через connect
    QByteArray ba_array = my_socket->readAll();
    QString new_message = QString(ba_array);
    if (new_message.startsWith("AA3A"))
    {
        MainMessageManufacturing(new_message);
    }
    else if (new_message.startsWith("AARA")) // Ответ окну регистрации
    {
        new_message.remove(0, 4);
        TranslateMessage(new_message);
    } 
    else if (new_message.startsWith("AAAA")) // Ответ окну авторизации
    {
        new_message.remove(0, 4);
        TranslateMessageA(new_message);
    }
    else
    {
        QDateTime date = QDateTime::currentDateTime();
        ui.te_support->append(date.toString("dd.MM.yyyy hh:mm:ss") + u8": Сервер ответил:\n " + QString(ba_array));
    }
}

void QtCLIENT::my_Registration()
{
    my_reg->show();
    this->close();
    emit sendData(my_socket_2);
}

void QtCLIENT::my_Authorization()
{

    my_auth->show();
    this->close();
    emit sendData_2(my_socket_2);
}

void QtCLIENT::Refresh()
{
    if (Chat_number == 1)
    {
        ui.te_main->clear();
        ui.te_main_2->clear();
        QString new_message = "AA1A";
        my_socket->write(new_message.toUtf8());
        return;
    }
    if (Chat_number == 2)
    {
        ui.te_main->clear();
        ui.te_main_2->clear();
        QString new_message = "AA2A";
        my_socket->write(new_message.toUtf8());
        return;
    }

}

void QtCLIENT::Exit()
{
    if (my_socket != nullptr)
    {
        my_socket->abort();
        my_socket->deleteLater();
        my_socket = nullptr;
    }
    QApplication::quit();
}

void QtCLIENT::MainMessageManufacturing(QString String)
{
    QStringList list = String.split(u8"$%&");
    for (int i = 0; i < list.size(); i++)
    {
        QString message = list[i];
        message.remove(0, 4);
        if (Chat_number == 1) {
            ui.te_main->append(message);
        }
        if (Chat_number == 2) {
            ui.te_main_2->append(message);
        }
    }

}

void QtCLIENT::ChatChange()
{
    if (ui.tabWidget->currentIndex() == 0)
    {
        Chat_number = 1;
        QtCLIENT::Refresh();
    }
    if (ui.tabWidget->currentIndex() == 1)
    {
        Chat_number = 2;
        QtCLIENT::Refresh();
    }

}

void QtCLIENT::res_Authorization(QString String)
{
    UserName = String;
    ui.le_username->setText(UserName);

}

void QtCLIENT::TranslateMessage(QString String)
{
    emit send_to_Registration(String);
}

void QtCLIENT::TranslateMessageA(QString String2)
{
    emit send_to_Authorization(String2);
}
