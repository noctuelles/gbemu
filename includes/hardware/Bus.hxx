#ifndef BUS_HXX
#define BUS_HXX

#include <array>
#include <memory>
#include <vector>

#include "Addressable.hxx"

class Bus final : public Addressable
{
  public:
    Bus() = default;

    void    write(uint16_t address, uint8_t value) override;
    uint8_t read(uint16_t address) override;
    [[nodiscard]] AddressableRange get_addressable_range() const override;

    void attach(Addressable& addressable);

  private:
    std::array<Addressable *, 0xFFFF> memory_map{};
};

#endif  // BUS_HXX
