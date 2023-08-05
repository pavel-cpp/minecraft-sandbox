#ifndef REALONLINE_H
#define REALONLINE_H

#include <QThread>
#include <QVector>
#include <QTime>
#include <QDebug>

#include <time.h>

#include "bot.h"

using namespace std;

class realOnline : public QThread
{
public:
    float graph[24] = {0.2153, 0.1127, 0.1371, 0.0783, 0.0923, 0.1234, 0.1662, 0.2912, 0.4198, 0.4523, 0.8456, 0.5487, 0.7521, 0.9145, 1.0000, 0.8478, 0.8932, 0.9812, 0.8668, 0.8384, 0.8052, 0.6726, 0.5021, 0.3126 };
    void run();
    void  changeOnline(float persent);
    QVector<bot>* BotThread;
    bool status = false;
    QString ip;
    int port;
    int userAmount;
    int userPersent;
    QTime realTime;
};

#endif // REALONLINE_H
