#include "../headers/realonline.h"

void realOnline::changeOnline(float persent){
    bool vector;
    int needOnline = (BotThread->size() - 1) * persent;
    if(userAmount > needOnline) vector = false;
    else vector = true;
    int nowTime = 3600000 - ((realTime.currentTime().minute()) * 60000) - ((realTime.currentTime().second()) * 1000);
    int period = nowTime/abs(needOnline-userAmount);
    qDebug() << (60 - realTime.currentTime().minute()) << nowTime << period << endl;
    while(userAmount != needOnline){
        int botNum = rand()%BotThread->size();
        Sleep(period);
        if(vector && (*BotThread)[botNum].onlineStatus == false){
            CreateFP((*BotThread)[botNum].nick.c_str(), &(*BotThread)[botNum].attributes, ip.toStdString().c_str(), port);
            (*BotThread)[botNum].onlineStatus = true;
            userAmount++;
        }
        if(vector == false && (*BotThread)[botNum].onlineStatus && (*BotThread)[botNum].isBusy == false){
            DestroyFP(&(*BotThread)[botNum].attributes);
            (*BotThread)[botNum].onlineStatus = false;
            userAmount--;
        }
    }
}

void realOnline::run(){
    srand(time(NULL));
    userPersent = 1;
    while(status){
        int currentHour;
        if(realTime.currentTime().hour() == 23) currentHour = 0;
        else currentHour = realTime.currentTime().hour() + 1;
        changeOnline(graph[currentHour]);
    }
}
