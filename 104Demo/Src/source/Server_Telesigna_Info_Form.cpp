#include "Src/include/Server_Telesigna_Info_Form.h"

#include "Src/include/Client_Infomation.h"
#include "Src/include/mainwindow.h"
#include "ui_Server_Telesigna_Info_Form.h"

Server_Telesigna_Info_Form* Server_Telesigna_Info_Form::self = nullptr;

Server_Telesigna_Info_Form::Server_Telesigna_Info_Form(QWidget* parent)
    : QDialog(parent), ui(new Ui::Server_Telesigna_Info_Form) {
  ui->setupUi(this);
  this->setDefault();
  connect(ui->btn_Save, &QPushButton::clicked, this,
          &Server_Telesigna_Info_Form::Save, Qt::UniqueConnection);

  connect(ui->lineEdit_addr, &QLineEdit::textEdited, this,
          [&]() { emit textChanged(false); });
  connect(ui->lineEdit_clientID, &QLineEdit::textEdited, this,
          [&]() { emit textChanged(false); });
  connect(ui->lineEdit_clientTA, &QLineEdit::textEdited, this,
          [&]() { emit textChanged(false); });
  connect(ui->textEdit_server_des, &MyTextEdit::manualTextChanged, this,
          [&]() { emit textChanged(false); });
  ui->btn_Save->setToolTip("Ctrl+Shift+S");

  connect(this, &Server_Telesigna_Info_Form::clientInfo, ui->lineEdit_clientID,
          &CustomLineEdit::clientInfo);
  connect(this, &Server_Telesigna_Info_Form::clientInfo, ui->lineEdit_clientTA,
          &CustomLineEdit::clientInfo);

  connect(Client_Infomation::getInstance(), &Client_Infomation::teleSigna, this,
          [&](tvalue v) {
            ui->lineEdit_clientID->setText(v.first);
            client_id = v.second;
            emit textChanged(false);
            this->updateInfo_to_Client_Infomation();
          });
  connect(Client_Infomation::getInstance(),
          &Client_Infomation::clientTelesignaValue, this, [&](tvalue v) {
            ui->lineEdit_clientTA->setText(v.first);
            addr_id = v.second;
            emit textChanged(false);
          });
}

Server_Telesigna_Info_Form::~Server_Telesigna_Info_Form() { delete ui; }

void Server_Telesigna_Info_Form::init() {
  QFont font("Microsoft YaHei", FONTSIZE);

  ui->lab_addr->setFont(font);
  ui->lab_clientID->setFont(font);
  ui->lab_clientTA->setFont(font);
  ui->lab_describe->setFont(font);

  ui->lab_describe->setAlignment(Qt::AlignTop | Qt::AlignLeft);

  ui->lineEdit_addr->setFont(font);
  ui->lineEdit_clientID->setFont(font);
  ui->lineEdit_clientTA->setFont(font);
  ui->textEdit_server_des->setFont(font);

  ui->lineEdit_clientID->setType(telesigna);
  ui->lineEdit_clientTA->setType(client_telesignaID);

  // 设置自动换行
  QTextOption textOption;
  textOption.setWrapMode(QTextOption::WrapAnywhere);
  ui->textEdit_server_des->document()->setDefaultTextOption(textOption);
}

void Server_Telesigna_Info_Form::updateInfo(MAP_CFG_CLIENT client,
                                            MAP_CFG_CLIENT_TELESIGNA telesigna,
                                            CFG_SERVER_TELESIGNA str) {
  std::vector<QString> res, signa;
  for (const auto& [id, c] : client) {
    res.emplace_back(MainWindow::getLocaClientidx(c).first);
  }
  client_id = str.uiClientID;
  addr_id = str.uiClientTelesignaAddr;
  QString s;
  if (static_cast<size_t>(str.uiClientID) < res.size()) {
    s = res[str.uiClientID];
    for (const auto& c : telesigna[str.uiClientID]) {
      signa.emplace_back(
          QString("%1（%2）").arg(c.uiClientTelesignaAddr).arg(c.szDescribe));
    }
  } else {
    s = "此处索引无效，需要重新选择";
  }
  QString s1;
  if (static_cast<size_t>(str.uiClientTelesignaAddr) <= signa.size() &&
      str.uiClientTelesignaAddr) {
    s1 = signa[str.uiClientTelesignaAddr - 1];
  } else {
    s1 = "此处索引无效，需要重新选择";
  }

  ui->lineEdit_addr->setText(QString("%1").arg(str.uiServerTelesignaAddr));
  ui->lineEdit_clientID->setText(s);
  ui->lineEdit_clientTA->setText(s1);
  ui->textEdit_server_des->setText(QString::fromUtf8(str.szDescribe));
}

void Server_Telesigna_Info_Form::cleanInfo() const {
  ui->lineEdit_addr->clear();
  ui->lineEdit_clientID->clear();
  ui->lineEdit_clientTA->clear();
  ui->textEdit_server_des->clear();
}

void Server_Telesigna_Info_Form::Save() {
  CFG_SERVER_TELESIGNA t;

  memset(&t, 0, MAX_CFG_SERVER_TELESIGNA);
  t.uiServerTelesignaAddr = ui->lineEdit_addr->text().toUInt();
  t.uiClientID = client_id;
  t.uiClientTelesignaAddr = addr_id;
  qstrncpy(t.szDescribe,
           ui->textEdit_server_des->toPlainText().toUtf8().constData(),
           MAX_CFG_DESCRIBE);

  emit Values(t);
}

void Server_Telesigna_Info_Form::setDefault() {
  ui->lineEdit_addr->setText("0");
  ui->lineEdit_clientID->setText("0");
  ui->lineEdit_clientTA->setText("0");
  ui->textEdit_server_des->clear();
}

void Server_Telesigna_Info_Form::clientInfomation(
    MAP_CFG_CLIENT client, MAP_CFG_CLIENT_TELESIGNA client_telesigna) {
  local_client = client;
  local_client_telesigna = client_telesigna;
  this->updateInfo_to_Client_Infomation();
}

void Server_Telesigna_Info_Form::updateInfo_to_Client_Infomation() {
  auto op = local_client.begin()->first;
  tmap t1, t2;
  for (const auto& c : local_client) {
    if (c.second.uiClientID == client_id) {
      op = c.first;
    }
    auto mes = MainWindow::getLocaClientidx(c.second);
    t1[mes] = QString::fromUtf8(c.second.szDescribe);
  }

  for (const auto& c : local_client_telesigna[op]) {
    auto mes = std::make_pair(
        QString("%1（%2）").arg(c.uiClientTelesignaAddr).arg(c.szDescribe),
        c.uiClientTelesignaAddr);
    t2[mes] = QString::fromUtf8(c.szDescribe);
  }

  emit clientInfo(t1, t2);
}
