//
// Created by plouvel on 12/8/24.
//

#include <Bus.hxx>
int main(void)
{
    Bus bus{};
    uint8_t program[] = {0x12U, 0x32U};

    bus.write(0x0000, program);

    return (0);
}