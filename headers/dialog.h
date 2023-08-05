#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    bool getBungeecord();
    bool getCompression();
    QString getIP();
    int getPort();
    ~Dialog();

private slots:
    void on_Ok_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
