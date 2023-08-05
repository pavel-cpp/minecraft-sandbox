#include "../headers/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Chat->setReadOnly(true);
    ui->login->setEnabled(false);
    ui->logout->setEnabled(false);

    WSADATA WSA;
    WSAStartup(WINSOCK_VERSION, &WSA);

    QIcon icon;
    icon.addFile("icon.png");
    setWindowIcon(icon);
    serverInfo.setWindowIcon(icon);

    serverInfo.setWindowTitle("MCSB");
    close();
    serverInfo.exec();
    show();

    nickList();
    RO.BotThread = &Bot;
    RO.userAmount = 0;
    RO.ip = serverInfo.getIP();
    RO.port = serverInfo.getPort();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nickList(){
    FakePlayer fp;
    fp.bungeecord = serverInfo.getBungeecord();
    fp.compression = serverInfo.getCompression();
    ifstream nicks("Nicks.txt");
    ui->Chat->setText(ui->Chat->toPlainText() + '\n' + "Player Initialization: ");
    while(!nicks.eof()){
        bot buffer;
        nicks >> buffer.nick;
        ui->Chat->setText(ui->Chat->toPlainText() + '\n' + "    " + QString::fromStdString(buffer.nick) + " has been initialized");
        buffer.attributes = fp;
        buffer.isBusy = false;
        buffer.onlineStatus = false;
        Bot.push_back(buffer);
        ui->listOfPlayers->addItem(QString::fromStdString(buffer.nick));
    }
    nicks.close();
}

void MainWindow::on_send_clicked()
{
    if(Bot[ui->listOfPlayers->currentRow()].isBusy){
        SendCommandFP(ui->chatLine->toPlainText().toStdString().c_str(), &Bot[ui->listOfPlayers->currentRow()].attributes);
    }else if(lastRow != -1){
        if(Bot[lastRow].isBusy){
            SendCommandFP(ui->chatLine->toPlainText().toStdString().c_str(), &Bot[lastRow].attributes);
        }
    }
    ui->chatLine->setPlainText("");
}


void MainWindow::on_listOfPlayers_doubleClicked(const QModelIndex &index)
{
    if(Bot[ui->listOfPlayers->currentRow()].isBusy == false){
        Bot[ui->listOfPlayers->currentRow()].isBusy = true;
        if(lastRow != ui->listOfPlayers->currentRow()){
            if(lastRow != -1){
                Bot[lastRow].isBusy = false;
            }
            lastRow = ui->listOfPlayers->currentRow();
        }
    }
    ui->Chat->setText(ui->Chat->toPlainText() + '\n' + QString::fromStdString(Bot[ui->listOfPlayers->currentRow()].nick) + " selected!");
    ui->Bot->setText("Bot: " + QString::fromStdString(Bot[ui->listOfPlayers->currentRow()].nick));
    checkOnline();
}

void MainWindow::on_login_clicked()
{
    Bot[ui->listOfPlayers->currentRow()].onlineStatus = true;
    CreateFP(Bot[ui->listOfPlayers->currentRow()].nick.c_str(), &Bot[ui->listOfPlayers->currentRow()].attributes, serverInfo.getIP().toStdString().c_str(), serverInfo.getPort());
    checkOnline();
}

void MainWindow::on_logout_clicked()
{
    Bot[ui->listOfPlayers->currentRow()].onlineStatus = false;
    DestroyFP(&Bot[ui->listOfPlayers->currentRow()].attributes);
    checkOnline();
}

void MainWindow::checkOnline(){
    if(Bot[ui->listOfPlayers->currentRow()].onlineStatus){
        ui->login->setEnabled(false);
        ui->logout->setEnabled(true);
    }else{
        ui->login->setEnabled(true);
        ui->logout->setEnabled(false);
    }
};

void MainWindow::on_CALL_clicked()
{
    ui->Chat->setText(ui->Chat->toPlainText() + '\n' + "Player Connecting:");
    for (int i = 0; i < Bot.size(); i++){
        if(!Bot[i].onlineStatus){
            CreateFP(Bot[i].nick.c_str(), &Bot[i].attributes, serverInfo.getIP().toStdString().c_str(), serverInfo.getPort());
            ui->Chat->setText(ui->Chat->toPlainText() + '\n' + "    " + QString::fromStdString(Bot[i].nick) + " has been connected");
            Bot[i].onlineStatus = true;
        }
    }
    RO.userAmount = Bot.size() - 1;
}

void MainWindow::on_DALL_clicked()
{
    ui->Chat->setText(ui->Chat->toPlainText() + '\n' + "Player Connecting:");
    for (int i = 0; i < Bot.size(); i++){
        if(Bot[i].onlineStatus == true){
            DestroyFP(&Bot[i].attributes);
            ui->Chat->setText(ui->Chat->toPlainText() + '\n' + "    " + QString::fromStdString(Bot[i].nick) + " has been disconnected");
            Bot[i].onlineStatus = false;
        }
    }
    RO.userAmount = 0;
}


void MainWindow::on_RO_stateChanged(int arg1)
{
    if(ui->RO->checkState()){
         RO.status = true;
         RO.start();
         ui->Chat->setText(ui->Chat->toPlainText() + '\n' + "Real Online Activated!");
    }else{
        RO.status = false;
        ui->Chat->setText(ui->Chat->toPlainText() + '\n' + "Real Online Deactivated!");
    }
}
