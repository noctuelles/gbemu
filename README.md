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

### Optional

- **Google Test** (automatically fetched by CMake for testing)

## Installation

### Linux (Ubuntu/Debian)

1. **Install dependencies:**

You must have installed Qt 6.10.

### Installation

#### Linux (Ubuntu/Debian)

1. **Clone the repository and build:**

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
