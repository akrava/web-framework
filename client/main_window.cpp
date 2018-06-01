#include "main_window.h"
#include "ui_main_window.h"
#include <QHostAddress>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QString portRange = "(?:[0-9]+)";
    QRegExp ipRegex ("^" + ipRange  + "\\." + ipRange + "\\." + ipRange  + "\\." + ipRange + "\\:" + portRange + "\\/.*" + "$");
    QRegExpValidator * ipValidator = new QRegExpValidator(ipRegex, this);
    ui->AddressValue->setValidator(ipValidator);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_AddressValue_textEdited(const QString &arg1) {
    QString val = arg1;
    int pos = 0;
    bool isValid = ui->AddressValue->validator()->validate(val, pos) == QValidator::Acceptable;
    ui->sendButton->setEnabled(isValid);
    if (isValid) updateLineFirstRequest();
    else ui->firstLineValue->setText("");
}

void MainWindow::on_MethodValue_currentIndexChanged(const QString &arg1) {
    QString val = ui->AddressValue->text();
    int pos = 0;
    if (ui->AddressValue->validator()->validate(val, pos) == QValidator::Acceptable) updateLineFirstRequest();
    else ui->firstLineValue->setText("");
}

void MainWindow::on_sendButton_clicked() {
    QString data = ui->firstLineValue->text();
    data += "\r\n";
    data += ui->headersRequest->toPlainText();
    data += "\r\n\r\n";
    data += ui->bodeRequest->toPlainText();
    Client c = Client();
    QHostAddress addr = QHostAddress(ui->AddressValue->text().left(ui->AddressValue->text().indexOf(':')));
    QString datas = ui->AddressValue->text().mid(ui->AddressValue->text().indexOf(':') + 1, ui->AddressValue->text().indexOf('/') - ui->AddressValue->text().indexOf(':') - 1);
    int port = datas.toInt();
    if (!c.setupConnection(addr, port)) {
        QMessageBox::critical(this, datas, c.what());
        return;
    }

    if (!c.sendData(data)) {
        QMessageBox::critical(this, "err", c.what());
        return;
    }
    QString ddd;
    if (!c.reciveData(ddd)) {
        QMessageBox::critical(this, "err", c.what());
        return;
    }
    c.closeConnection();
    ui->responseData->setPlainText(ddd);
}

void MainWindow::updateLineFirstRequest() {
    QString firstLine;
    firstLine +=  ui->MethodValue->itemText(ui->MethodValue->currentIndex());
    firstLine += " ";
    firstLine += ui->AddressValue->text().mid(ui->AddressValue->text().indexOf('/'));
    firstLine += " HTTP/1.1";
    ui->firstLineValue->setText(firstLine);
}
