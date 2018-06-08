#pragma once
#include "client.h"
#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString lineFirstRequest;
private slots:
    void on_AddressValue_textEdited(const QString &arg1);
    void on_MethodValue_currentIndexChanged(const QString &arg1);
    void on_sendButton_clicked();
    void updateLineFirstRequest();
private:
    Ui::MainWindow *ui;
    QString prepeareToSend();
    QString onSendRawData();
    QString onSendJson();
};
