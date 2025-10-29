//
// Created by plouvel on 12/8/24.
//


#include "GbEmu.hxx"

int main(int argc, char* args[])
{
    (void) argc;
    (void) args;
    GbEmu emu{};

    emu.loop();

    return 0;
}