#include "main_window.h"
#include "ui_main_window.h"
#include "json_highlighter.h"
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

    Highlighter * json_req = new Highlighter(ui->jsonRequest->document());
    Highlighter * json_res = new Highlighter(ui->jsonResponse->document());
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
    QString data;
    if (ui->tabWidget->currentIndex() == 0) {
        data = onSendRawData();
    } else {
        data = onSendJson();
    }
    Client c = Client();
    QHostAddress addr = QHostAddress(ui->AddressValue->text().left(ui->AddressValue->text().indexOf(':')));
    QString address_str = ui->AddressValue->text().mid(ui->AddressValue->text().indexOf(':') + 1, ui->AddressValue->text().indexOf('/') - ui->AddressValue->text().indexOf(':') - 1);
    int port = address_str.toInt();
    if (!c.setupConnection(addr, port)) {
        QMessageBox::critical(this, address_str, c.what());
        return;
    }
    if (!c.sendData(data)) {
        QMessageBox::critical(this, "error", c.what());
        return;
    }
    QString recieved_from_host;
    if (!c.reciveData(recieved_from_host)) {
        QMessageBox::critical(this, "error", c.what());
        return;
    }
    c.closeConnection();
    if (ui->tabWidget->currentIndex() == 0) {
        ui->responseData->setPlainText(recieved_from_host);
    } else {
        int posStart = recieved_from_host.indexOf("\r\n\r\n");
        QString received = recieved_from_host.mid(posStart+4);
        ui->jsonResponse->setPlainText(received);
    }
}

QString MainWindow::onSendRawData() {
    QString data = ui->firstLineValue->text();
    data += "\r\n";
    data += prepeareToSend();
    data += "\r\n\r\n";
    data += ui->bodeRequest->toPlainText();
    return data;
}

QString MainWindow::onSendJson() {
    QString data = ui->firstLineValue->text();
    data += "\r\n";
    int count = ui->jsonRequest->toPlainText().length();
    data += "Content-Length: " + QString::number(count);
    data += "\r\n";
    data += "Content-Type: application/json";
    data += "\r\n\r\n";
    data += ui->jsonRequest->toPlainText();
    return data;
}

void MainWindow::updateLineFirstRequest() {
    QString firstLine;
    firstLine +=  ui->MethodValue->itemText(ui->MethodValue->currentIndex());
    firstLine += " ";
    firstLine += ui->AddressValue->text().mid(ui->AddressValue->text().indexOf('/'));
    firstLine += " HTTP/1.1";
    ui->firstLineValue->setText(firstLine);
}

QString MainWindow::prepeareToSend() {
     QString data_headers = ui->headersRequest->toPlainText();
     QString parsed;
     for (auto & c : data_headers) {
        if (c != '\n') {
            parsed += c;
        } else {
            parsed += '\r';
            parsed += c;
        }
     }
     if (parsed.endsWith("\r\n")) {
         parsed.left(parsed.length() - 2);
     }
     return parsed;
}
