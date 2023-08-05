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

    Dialog serverInfo;

    void nickList();

    Ui::MainWindow *ui;

    QVector<bot> Bot;

    realOnline RO;

    int lastRow = -1;
};
#endif // MAINWINDOW_H
