//
// Created by plouvel on 12/18/24.
//

#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <string_view>

class Cartridge
{
  public:
    const std::string_view& get_game_published_name() const;
};

#endif  // CARTRIDGE_H
