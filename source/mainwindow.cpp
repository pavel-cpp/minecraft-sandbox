#include "../headers/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->Chat->setReadOnly(true);
    ui->login->setEnabled(false);
    ui->logout->setEnabled(false);

    WSADATA WSA;
    WSAStartup(WINSOCK_VERSION, &WSA);

    QIcon icon;
    icon.addFile("icon.png");
    setWindowIcon(icon);
    dialog_server_info.setWindowIcon(icon);

    dialog_server_info.setWindowTitle("MCSB");
    close();
    dialog_server_info.exec();
    show();

    nickList();
    real_online_system_thread = make_unique<RealOnline>(nullptr, bots);
    real_online_system_thread->SetCurTraffic(0);
    real_online_system_thread->SetServerParams(dialog_server_info.GetIP(), dialog_server_info.GetPort());
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::nickList() {
    FakePlayer fp;
    fp.bungeecord = dialog_server_info.GetBungeecord();
    fp.compression = dialog_server_info.GetCompression();
    ifstream nicks("Nicks.txt");
    ui->Chat->setText(ui->Chat->toPlainText() + '\n' + "Player Initialization: ");
    while (!nicks.eof()) {
        Bot buffer;
        nicks >> buffer.nick;
        ui->Chat->setText(ui->Chat->toPlainText() + '\n' + "    " + QString::fromStdString(buffer.nick) +
                          " has been initialized");
        buffer.attributes = fp;
        buffer.onlineStatus = buffer.isBusy = false;
        bots.push_back(buffer);
        ui->listOfPlayers->addItem(QString::fromStdString(buffer.nick));
    }
    nicks.close();
}

void MainWindow::on_send_clicked() {
    if (bots[ui->listOfPlayers->currentRow()].isBusy) {
        SendCommandFP(ui->chatLine->toPlainText().toStdString().c_str(),
                      &bots[ui->listOfPlayers->currentRow()].attributes);
    } else if (last_selected_row != -1) {
        if (bots[last_selected_row].isBusy) {
            SendCommandFP(ui->chatLine->toPlainText().toStdString().c_str(), &bots[last_selected_row].attributes);
        }
    }
    ui->chatLine->setPlainText("");
}


void MainWindow::on_listOfPlayers_doubleClicked(const QModelIndex &index) {
    int row = index.row();
    if (bots[row].isBusy) return;
    bots[row].isBusy = true;
    if (last_selected_row != row) {
        if (last_selected_row != -1) {
            bots[last_selected_row].isBusy = false;
        }
        last_selected_row = row;
    }
    ui->Chat->setText(
            ui->Chat->toPlainText() + '\n' + QString::fromStdString(bots[ui->listOfPlayers->currentRow()].nick) +
            " selected!");
    ui->bots->setText("bots: " + QString::fromStdString(bots[ui->listOfPlayers->currentRow()].nick));
    checkOnline();
}

void MainWindow::on_login_clicked() {
    bots[ui->listOfPlayers->currentRow()].onlineStatus = true;
    CreateFP(bots[ui->listOfPlayers->currentRow()].nick.c_str(), &bots[ui->listOfPlayers->currentRow()].attributes,
             dialog_server_info.GetIP().c_str(), dialog_server_info.GetPort());
    checkOnline();
}

void MainWindow::on_logout_clicked() {
    bots[ui->listOfPlayers->currentRow()].onlineStatus = false;
    DestroyFP(&bots[ui->listOfPlayers->currentRow()].attributes);
    checkOnline();
}

void MainWindow::checkOnline() {
    if (bots[ui->listOfPlayers->currentRow()].onlineStatus) {
        ui->login->setEnabled(false);
        ui->logout->setEnabled(true);
    } else {
        ui->login->setEnabled(true);
        ui->logout->setEnabled(false);
    }
};

void MainWindow::on_CALL_clicked() {
    ui->Chat->setText(ui->Chat->toPlainText() + '\n' + "Player Connecting:");
    for (Bot bot: bots) {
        if (!bot.onlineStatus) {
            CreateFP(bot.nick.c_str(), &bot.attributes, dialog_server_info.GetIP().c_str(),
                     dialog_server_info.GetPort());
            ui->Chat->setText(
                    ui->Chat->toPlainText() + '\n' + "    " + QString::fromStdString(bot.nick) + " has been connected");
            bot.onlineStatus = true;
        }
    }
    real_online_system_thread->SetCurTraffic(bots.size() - 1);
}

void MainWindow::on_DALL_clicked() {
    ui->Chat->setText(ui->Chat->toPlainText() + '\n' + "Player Connecting:");
    for (Bot bot: bots) {
        if (bot.onlineStatus) {
            DestroyFP(&bot.attributes);
            ui->Chat->setText(ui->Chat->toPlainText() + '\n' + "    " + QString::fromStdString(bot.nick) +
                              " has been disconnected");
            bot.onlineStatus = false;
        }
    }
    real_online_system_thread->SetCurTraffic(0);
}


void MainWindow::on_RO_stateChanged(int arg1) {
    if (ui->real_online_system_thread->checkState()) {
        real_online_system_thread->SetStatus(true);
        real_online_system_thread->start();
        ui->Chat->setText(ui->Chat->toPlainText() + '\n' + "Real Online Activated!");
    } else {
        real_online_system_thread->SetStatus(false);
        ui->Chat->setText(ui->Chat->toPlainText() + '\n' + "Real Online Deactivated!");
    }
}
