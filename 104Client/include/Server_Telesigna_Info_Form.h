#ifndef SERVER_TELESIGNA_INFO_FORM_H
#define SERVER_TELESIGNA_INFO_FORM_H

#include <QDialog>

namespace Ui {
    class Server_Telesigna_Info_Form;
}

class Server_Telesigna_Info_Form : public QDialog {
    Q_OBJECT

  public:
    explicit Server_Telesigna_Info_Form(QWidget* parent = nullptr);
    ~Server_Telesigna_Info_Form();

    static Server_Telesigna_Info_Form* getInstance(QWidget* parent = nullptr);
    // 初始化界面，以及左上角名字
    void init();
    // 更新界面
    void updateInfo(QString str);

  private:
    Ui::Server_Telesigna_Info_Form* ui;
    static Server_Telesigna_Info_Form* self;
};

// 单例模式
inline Server_Telesigna_Info_Form* Server_Telesigna_Info_Form::getInstance(QWidget* parent) {
    if (self == nullptr) {
        self = new Server_Telesigna_Info_Form(parent);
        self->init();
    }
    return self;
}
#endif  // SERVER_TELESIGNA_INFO_FORM_H
