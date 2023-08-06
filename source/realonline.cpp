#include "../headers/realonline.h"

void RealOnline::ChangeOnline(float point){
    int traffic_request = (bot_threads.size() - 1) * point;
    bool traffic_move = current_traffic > traffic_request;
    int current_time_ms = TimeToMs();
    int time_delta = current_time_ms / abs(traffic_request - current_traffic);

    while(current_traffic != traffic_request){
        int selected_bot_index = rand() % bot_threads.size();
        Sleep(time_delta);
        if(traffic_move && !bot_threads[selected_bot_index].onlineStatus){
            CreateFP(bot_threads[selected_bot_index].nick.c_str(), &bot_threads[selected_bot_index].attributes, ip.c_str(), port);
            bot_threads[selected_bot_index].onlineStatus = true;
            ++current_traffic;
        }
        if(!traffic_move && bot_threads[selected_bot_index].onlineStatus && !bot_threads[selected_bot_index].isBusy){
            DestroyFP(&bot_threads[selected_bot_index].attributes);
            bot_threads[selected_bot_index].onlineStatus = false;
            --current_traffic;
        }
    }
}

void RealOnline::run(){
    srand(time(NULL));
    while(status){
        int currentHour = QTime::currentTime().hour() % 24;
        ChangeOnline(graph[currentHour]);
    }
}

RealOnline::RealOnline(QObject *parent, QVector<Bot> &botThreads)
        : QThread(parent), bot_threads(botThreads) {}

inline int RealOnline::TimeToMs() {
    return 3600000 - ((QTime::currentTime().minute()) * 60000) - ((QTime::currentTime().second()) * 1000);
}

void RealOnline::SetServerParams(const std::string &ip, int port) {
    this->ip = ip;
    this->port = port;
}

void RealOnline::SetStatus(bool value) {
    status = value;
}

void RealOnline::SetCurTraffic(int traffic) {
    current_traffic = traffic;
}

bool RealOnline::SetStatus() {
    return status;
}
