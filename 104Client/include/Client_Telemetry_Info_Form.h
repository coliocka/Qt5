#ifndef CLIENT_TELEMETRY_INFO_FORM_H
#define CLIENT_TELEMETRY_INFO_FORM_H

#include <QDialog>

namespace Ui {
    class Client_Telemetry_Info_Form;
}

class Client_Telemetry_Info_Form : public QDialog {
    Q_OBJECT

  public:
    explicit Client_Telemetry_Info_Form(QWidget* parent = nullptr);
    ~Client_Telemetry_Info_Form();

    static Client_Telemetry_Info_Form* getInstance(QWidget* parent = nullptr);
    // 初始化界面，以及左上角名字
    void init();
    // 更新界面
    void updateInfo(QString str);

  private:
    Ui::Client_Telemetry_Info_Form* ui;
    static Client_Telemetry_Info_Form* self;
};

// 单例模式
inline Client_Telemetry_Info_Form* Client_Telemetry_Info_Form::getInstance(QWidget* parent) {
    if (self == nullptr) {
        self = new Client_Telemetry_Info_Form(parent);
        self->init();
    }
    return self;
}
#endif  // CLIENT_TELEMETRY_INFO_FORM_H
