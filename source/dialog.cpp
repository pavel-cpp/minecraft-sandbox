#include "../headers/dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Dialog) {
    ui->setupUi(this);
}

Dialog::~Dialog() {
    delete ui;
}

bool Dialog::GetBungeecord() {
    return ui->bungeecord->checkState();
}

bool Dialog::GetCompression() {
    return ui->compression->checkState();
}

std::string Dialog::GetIP() {
    return ui->IP->toPlainText().toStdString();
}

int Dialog::GetPort() {
    return ui->Port->toPlainText().toInt();
}

void Dialog::on_Ok_clicked() {
    close();
}
