#include "../headers/dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

bool Dialog::getBungeecord(){
    return ui->bungeecord->checkState();
}

bool Dialog::getCompression(){
    return ui->compression->checkState();
}

QString Dialog::getIP(){
    return ui->IP->toPlainText();
}

int Dialog::getPort(){
    return ui->Port->toPlainText().toInt();
}

void Dialog::on_Ok_clicked()
{
    close();
}
