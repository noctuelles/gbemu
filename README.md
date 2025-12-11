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

   If you don't pass a ROM on the command-line, the GUI will start and you can load a cartridge from the "File → Open"
   menu.

---

#### macOS (Homebrew)

1. **Install dependencies:**

   ```bash
   brew update
   brew install cmake boost qt@6 git
   ```

   You may need to add Qt to your PKG_CONFIG_PATH or specify `-DQt6_DIR` when running CMake. Example:

   ```bash
   cmake .. -DCMAKE_BUILD_TYPE=Release -DQt6_DIR="$(brew --prefix qt@6)/lib/cmake/Qt6"
   ```

2. **Build and run:**

   ```bash
   git clone https://github.com/<your-org>/gbemu.git
   cd gbemu
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build . -- -j$(sysctl -n hw.ncpu)
   ./gbemu ../roms/<game>.gb
   ```

---

#### Windows (MSVC / Visual Studio)

1. **Install prerequisites:**

* Visual Studio 2022 (Desktop development with C++)
* CMake (bundled with Visual Studio or installed separately)
* Qt6 (install via the Qt installer or use `vcpkg` to install Qt/Boost)
* Boost (install via vcpkg or use precompiled libraries)

2. **Build with CMake in a Visual Studio generator:**

   ```powershell
   git clone https://github.com/<your-org>/gbemu.git
   cd gbemu
   mkdir build; cd build
   cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release -DQt6_DIR="C:/Qt/6.x.x/msvc2019_64/lib/cmake/Qt6"
   cmake --build . --config Release
   ```

   Then run the produced executable from the `Release` folder.

---

### Development / Debug build

To build a debug-enabled version (with symbols and assertions enabled):

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . -- -j$(nproc)
```

This build enables extra logging and the integrated debugger UI features.

### Tests

If Google Test is enabled, CMake will automatically fetch it. Run the test-suite from the build directory:

```bash
ctest --output-on-failure
```

Or run specific test executables generated under `build/tests`.

---

### Usage

#### Command-line options

```
Usage: gbemu [options] [rom-file]

Options:
  -h, --help        Show help message
  -d, --debug       Start with debugger attached (break on start)
  -s, --scale <n>   Window scale multiplier (1..4)
  --headless        Run without GUI (for unit tests / automation)
```

#### GUI features

* **File → Open**: load a Game Boy ROM (.gb)
* **Emulation controls**: Play / Pause / Step Frame / Step Instruction
* **Debugger panel**: view registers, memory, disassembly, and set breakpoints
* **Palette & Controls**: configure color palettes and key bindings from the Settings dialog

#### Keyboard mapping (default)

* Arrow keys: D-pad
* Z: A
* X: B
* Enter: Start
* Right Shift: Select
* F5: Toggle pause / resume
* F10: Step instruction (when paused)
* F11: Step frame (when paused)

(These can be changed in Settings → Input)

---

### Debugger

The built-in debugger provides:

* CPU register view
* Disassembly window with current PC highlight
* Memory inspector
* Breakpoints (address or memory access where supported)
* Step into / over / out controls

**Note:** The debugger is "work-in-progress" — some complex breakpoint types and memory watchpoints may be incomplete.

---

### Configuration

Configuration files are stored in `~/.config/gbemu/` (Linux) or the equivalent platform-specific config folder. Settings
you can change include:

* Window size and scale
* Key bindings
* Color palettes (several built-in palettes + ability to add custom palettes)
* Last opened ROMs

### Cartridge / ROM support

* The emulator currently supports standard ROM banking for ROM1 (no MBC2/3/5 advanced mapper support yet). ROMs that
  rely on advanced mappers, battery-backed saves, or special hardware may not work correctly.

### Known limitations

* A complete PPU FIFO is not implemented — rendering is done per-scanline. Some games that rely on advanced mid-scanline
  behavior may display glitches.
* Some MBC types and cartridge features are not implemented yet (see the `docs/` folder for the current feature matrix).
* Link cable and multiplayer features are not supported.

---

### Contributing

Contributions are welcome! Please follow these guidelines:

1. Open an issue to discuss major changes before implementing them.
2. Fork the repository and create feature branches named like `feature/my-feature` or `fix/bug-xyz`.
3. Follow the project's code style (modern C++ with clear ownership semantics). Keep functions short and write unit
   tests for logic where possible.
4. Submit pull requests with a clear description and link to the related issue.

See `CONTRIBUTING.md` for more details (if present).

---

### Troubleshooting

* **Qt runtime errors:** Make sure Qt6 runtime libraries are discoverable. On Linux you may need to set
  `LD_LIBRARY_PATH` or install Qt via your package manager. On macOS the `@rpath` may need fixing — use `macdeployqt`
  for bundling.
* **Missing Boost includes:** Ensure `libboost-all-dev` or the appropriate Boost packages are installed and visible to
  CMake.
* **ROM fails to load:** Verify the ROM is a valid Game Boy ROM and not an archived file. The emulator supports `.gb`
  files; `.gbc` support is partial/experimental.

---

### Acknowledgements

* Thanks to the authors of the Game Boy documentation and Pan Docs for the hardware references.
* Portions of the testing workflow use Google Test.

---
