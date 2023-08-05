#ifndef FAKEPLAYER_H
#define FAKEPLAYER_H

#include "fplib.h"
#include <string>

struct bot
{
    std::string nick;
    FakePlayer attributes;
    bool onlineStatus = false;
    bool isBusy = false;
};

#endif // FAKEPLAYER_H
