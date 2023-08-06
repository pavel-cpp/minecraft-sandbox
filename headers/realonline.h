#ifndef REALONLINE_H
#define REALONLINE_H

#include <array>
#include <ctime>

#include <QThread>
#include <QVector>
#include <QTime>

#include "Bot.h"

using namespace std;

class RealOnline : public QThread {
public:
    RealOnline(QObject *parent, QVector<Bot> &botThreads);

    void run() override;

    void SetServerParams(const std::string &ip, int port);

    void SetCurTraffic(int traffic);

    void SetStatus(bool value);

    bool SetStatus();

    QVector<Bot> &bot_threads;
private:
    std::array<float, 24> graph = {
            0.2153, 0.1127, 0.1371, 0.0783, 0.0923, 0.1234,
            0.1662, 0.2912, 0.4198, 0.4523, 0.8456, 0.5487,
            0.7521, 0.9145, 1.0000, 0.8478, 0.8932, 0.9812,
            0.8668, 0.8384, 0.8052, 0.6726, 0.5021, 0.3126
    };

    int port;

    int current_traffic;

    std::string ip;

    inline int TimeToMs();

    void ChangeOnline(float point);

    bool status = false;
};

#endif // REALONLINE_H
