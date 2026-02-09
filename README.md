# 🎙️ VocalCortex
**A High-Performance Phonetic Command Engine for Linux.**

VocalCortex turns your voice into direct system hardware commands. Unlike standard STT tools, it uses a custom regex parser to map speech patterns to X11 events and system signals.

## 🚀 Key Features
- **Zero Latency**: Local offline processing via Vosk.
- **Hardware Nerves**: Control Volume, Brightness, and WiFi via voice.
- **Smart Editing**: "Iris" erases words, "Strike" deletes characters.
- **Phonetic Mapping**: Handles tricky dates and math (e.g., "July 17th", "2 + 2").

## 🛠️ Build Instructions
Requires `gtkmm-3.0`, `alsa`, `xdotool`, and `libvosk`.

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
./vocal_cortex
# VocalCortex
