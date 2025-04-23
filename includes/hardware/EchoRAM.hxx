//
// Created by plouvel on 3/13/25.
//

#ifndef ECHORAM_HXX
#define ECHORAM_HXX
#include "Addressable.hxx"

class EchoRAM final : public Addressable
{
  public:
    explicit EchoRAM(Addressable& ram);

    uint8_t read(uint16_t address) override;
    void    write(uint16_t address, uint8_t value) override;

    [[nodiscard]] AddressableRange get_addressable_range() const noexcept override;

  private:
    Addressable& content;
};

#endif  // ECHORAM_HXX
