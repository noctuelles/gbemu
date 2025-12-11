# gbemu - Game Boy Emulator

A Game Boy (DMG) emulator written in modern C++23 with a Qt6-based user interface.

### General QT Architecture

There is a GUI thread and an emulation thread. Both communicate via Qt Signals / Slots mechanism with a
QueuedConnection, making it thread-safe by design. No further synchronization mechanism is used.

The emulator thread emits a signal whenever the PPU enters in VBlank mode, meaning that a full frame can be displayed.
The emulation is stopped at this point.
The GUI thread then displays that frame and asks the emulator thread to emulate enough to actually display another
frame. This process repeats itself until the emulation is paused / breakpoint is triggered, at a rate of roughly 59.7
Hz.

## Features

- Full SM83 CPU emulation
- PPU (Picture Processing Unit) implementation, per scanline-rendering, not a full fifo yet.
- Timer and interrupt handling
- Cartridge loading (only ROM1 supported)
- Built-in debugger with breakpoints, WIP.
- Configurable color palettes and key bindings.

## Requirements

### Build Dependencies

- **CMake** 3.20 or higher
- **C++ Compiler** with C++23 support:
    - GCC 12+ or
    - Clang 16+ or
    - MSVC 2022+
- **Qt6** (Widgets component)
- **Boost** libraries

### Optional

- **Google Test** (automatically fetched by CMake for testing)

## Installation

### Linux (Ubuntu/Debian)

1. **Install dependencies:**

### Installation

#### Linux (Ubuntu/Debian)

1. **Install dependencies:**

   ```bash
   sudo apt update
   sudo apt install -y build-essential cmake git pkg-config \
       libboost-all-dev qt6-base-dev qt6-tools-dev qt6-declarative-dev \
       libxcb-keysyms1-dev libxcb-image0-dev libxcb-icccm4-dev libxcb-randr0-dev \
       libxcb-xfixes0-dev
   ```

   Notes:

* Package names may vary between distributions and Ubuntu versions. If `qt6-*` packages are not available, use your
  distribution's Qt6 packages or install Qt6 from the official Qt installer.
* If you prefer a more lightweight install just for building the application, install the `*-dev` packages required for
  Qt Widgets and Qt6's CMake config.

2. **Clone the repository and build:**

   ```bash
   git clone https://github.com/noctuelles/gbemu.git
   cd gbemu
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="<Your_Qt_Installation_Path>/Qt/6.10.1/gcc_64"
   cmake --build . -- -j$(nproc)
   ```

3. **Run the application:**

   ```bash
   ./gbemu
   ```
---
