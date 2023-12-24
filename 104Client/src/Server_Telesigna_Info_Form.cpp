#include "include/Server_Telesigna_Info_Form.h"

#include "include/mainwindow.h"
#include "ui_Server_Telesigna_Info_Form.h"

Server_Telesigna_Info_Form* Server_Telesigna_Info_Form::self = nullptr;

Server_Telesigna_Info_Form::Server_Telesigna_Info_Form(QWidget* parent) :
    QDialog(parent), ui(new Ui::Server_Telesigna_Info_Form) {
    ui->setupUi(this);
}

Server_Telesigna_Info_Form::~Server_Telesigna_Info_Form() { delete ui; }

void Server_Telesigna_Info_Form::init() {
    QFont font("Microsoft YaHei", FONTSIZE);
    ui->label->setFont(font);

    ui->lab_addr->setFont(font);
    ui->lab_clientID->setFont(font);
    ui->lab_clientTA->setFont(font);
    ui->lab_describe->setFont(font);

    ui->lineEdit_addr->setFont(font);
    ui->lineEdit_clientID->setFont(font);
    ui->lineEdit_clientTA->setFont(font);
    ui->lineEdit_describe->setFont(font);
}

void Server_Telesigna_Info_Form::updateInfo(QString str) {
#ifdef NDEBUG
    qDebug() << __func__;
    qDebug() << "curText:\t" << str;
#endif
    QStringList list = str.split(',');
    if (list.size() != server_telesigna_Size) {
        return qDebug() << "This new text does not conform to the specification", void();
    }
    QStringList t = list.at(0).split(':');
    QString addr = t[t.size() - 1];
    ui->lineEdit_addr->setText(addr);

    t = list.at(1).split(':');
    QString clientID = t[t.size() - 1];
    ui->lineEdit_clientID->setText(clientID);

    t = list.at(2).split(':');
    QString clientTA = t[t.size() - 1];
    ui->lineEdit_clientTA->setText(clientTA);

    t = list.at(3).split(':');
    QString describe = t[t.size() - 1];
    ui->lineEdit_describe->setText(describe);
}
