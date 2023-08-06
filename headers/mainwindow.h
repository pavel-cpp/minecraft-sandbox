#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QEvent>

#include <thread>
#include <fstream>

#include "realonline.h"
#include "dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    void on_send_clicked();

private slots:
    void on_listOfPlayers_doubleClicked(const QModelIndex &index);

    void on_logout_clicked();

    void on_login_clicked();

    void on_CALL_clicked();

    void on_DALL_clicked();

    void on_RO_stateChanged(int arg1);

private:

    void checkOnline();
    void nickList();

    Dialog dialog_server_info;

    Ui::MainWindow *ui;

    QVector<Bot> bots;

    unique_ptr<RealOnline> real_online_system_thread;

    int last_selected_row = -1;
};
#endif // MAINWINDOW_H
