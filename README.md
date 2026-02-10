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




➜  c++ git:(main) ✗ ls
build                keyboardbutton.h  README.md                vocal_cortex
censor.txt           keyboard.h        speechtotextservice.cpp  vosk-linux
censor.txt.          keyboard.hpp      speechtotextservice.h    vosk_models
cmake_install.cmake  main.cpp          stt_engine.hpp           word_list.hpp
CMakeLists.txt       Makefile          titan_window.cpp
json.hpp             nlohmann          titan_window.hpp
➜  c++ git:(main) ✗ ls -la vosk_models
total 20
drwxrwxr-x 5 android android 4096 Feb  2 03:58 .
drwxrwxrwx 7 android android 4096 Feb 10 01:27 ..
drwxrwxr-x 3 android android 4096 Feb  1 05:44 medium_english_model
drwxr-xr-x 2 android android 4096 Feb  9 06:27 vosk-model-en-us-daanzu-20200905-lgraph
drwxrwxr-x 2 android android 4096 Feb  3 03:59 vosk-model-small-en-us-0.15
➜  c++ git:(main) ✗ ls -la vosk_models/vosk-model-en-us-daanzu-20200905-lgraph
total 8
drwxr-xr-x 2 android android 4096 Feb  9 06:27 .
drwxrwxr-x 5 android android 4096 Feb  2 03:58 ..
➜  c++ git:(main) ✗ ls -la vosk_models/vosk-model-en-us-daanzu-20200905-lgraph
total 68
drwxr-xr-x 6 android android  4096 Feb 10 01:33 .
drwxrwxr-x 5 android android  4096 Feb  2 03:58 ..
drwxr-xr-x 2 android android  4096 Feb 10 01:33 am
-rw-r--r-- 1 android android    31 Sep  5  2020 AUTHORS.txt
drwxr-xr-x 2 android android  4096 Feb 10 01:33 conf
drwxr-xr-x 3 android android  4096 Feb 10 01:33 graph
drwxr-xr-x 2 android android  4096 Feb 10 01:33 ivector
-rw-r--r-- 1 android android 34523 Sep  5  2020 LICENSE.txt
-rw-r--r-- 1 android android    94 Sep  5  2020 README
➜  c++ git:(main) ✗                                                           
cmake ..
make -j$(nproc)
./vocal_cortex
cmake: /usr/local/lib/libcurl.so.4: no version information available (required by cmake)
CMake Error: The source directory "/home/android" does not appear to contain CMakeLists.txt.
Specify --help for usage, or press the help button on the CMake GUI.
/usr/bin/cmake: /usr/local/lib/libcurl.so.4: no version information available (required by /usr/bin/cmake)
CMake Error: The source directory "" does not exist.
Specify --help for usage, or press the help button on the CMake GUI.
Makefile:233: recipe for target 'cmake_check_build_system' failed
make: *** [cmake_check_build_system] Error 1
LOG (VoskAPI:ReadDataFiles():model.cc:213) Decoding params beam=13 max-active=7000 lattice-beam=6
LOG (VoskAPI:ReadDataFiles():model.cc:216) Silence phones 1:2:3:4:5:6:7:8:9:10:11:12:13:14:15
LOG (VoskAPI:RemoveOrphanNodes():nnet-nnet.cc:948) Removed 1 orphan nodes.
LOG (VoskAPI:RemoveOrphanComponents():nnet-nnet.cc:847) Removing 2 orphan components.
LOG (VoskAPI:Collapse():nnet-utils.cc:1488) Added 1 components, removed 2
LOG (VoskAPI:ReadDataFiles():model.cc:248) Loading i-vector extractor from /home/android/c++/vosk_models/vosk-model-en-us-daanzu-20200905-lgraph/ivector/final.ie
LOG (VoskAPI:ComputeDerivedVars():ivector-extractor.cc:183) Computing derived variables for iVector extractor
LOG (VoskAPI:ComputeDerivedVars():ivector-extractor.cc:204) Done.
LOG (VoskAPI:ReadDataFiles():model.cc:282) Loading HCL and G from /home/android/c++/vosk_models/vosk-model-en-us-daanzu-20200905-lgraph/graph/HCLr.fst /home/android/c++/vosk_models/vosk-model-en-us-daanzu-20200905-lgraph/graph/Gr.fst
LOG (VoskAPI:ReadDataFiles():model.cc:297) Loading words from /home/android/c++/vosk_models/vosk-model-en-us-daanzu-20200905-lgraph/graph/words.txt
LOG (VoskAPI:ReadDataFiles():model.cc:308) Loading winfo /home/android/c++/vosk_models/vosk-model-en-us-daanzu-20200905-lgraph/graph/phones/word_boundary.int
--- VOCAL CORTEX ONLINE ---
VOCALCORTEX LOG: swift 
swift VOCALCORTEX LOG: CMD_ALT_TAB 
VOCALCORTEX LOG: CMD_ALT_TAB 
VOCALCORTEX LOG: CMD_ALT_TAB 
VOCALCORTEX LOG: CMD_ENTER 
VOCALCORTEX LOG: hello was going on 
hello was going on VOCALCORTEX LOG: CMD_VOL_UP 
VOCALCORTEX LOG: CMD_VOL_UP 
VOCALCORTEX LOG: CMD_VOL_DOWN 
VOCALCORTEX LOG: CMD_BRIGHT_UP 
VOCALCORTEX LOG: CMD_BRIGHT_UP 
VOCALCORTEX LOG: CMD_BRIGHT_DOWN 
VOCALCORTEX LOG: CMD_BRIGHT_FULL 
VOCALCORTEX LOG: all voice commands 
all voice commands VOCALCORTEX LOG: as you can see you 
as you can see VOCALCORTEX LOG: beautiful 
beautiful VOCALCORTEX LOG: okay a $100 
okay a $100 VOCALCORTEX LOG: a $1000 
a $1000 VOCALCORTEX LOG: a $1000000 
a $1000000 VOCALCORTEX LOG: $1555555 
$1555555 VOCALCORTEX LOG: boom 
boom VOCALCORTEX LOG: as you can see handle numbers money 
as you can see handle numbers money VOCALCORTEX LOG: everything 
everything VOCALCORTEX LOG: happy 4th of july 
happy 4th of july VOCALCORTEX LOG: boom 
boom VOCALCORTEX LOG: advocacy also dates 
advocacy also dates VOCALCORTEX LOG: get a bigger model 
get a bigger model VOCALCORTEX LOG: for high accuracy 
for high accuracy VOCALCORTEX LOG: abuse in this 1 as you can see 
abuse in this 1 as you can see VOCALCORTEX LOG: so small so fast 
so small so fast VOCALCORTEX LOG: speak the text like never before mean it 
speak the text like never before mean it VOCALCORTEX LOG: i mean it 
i mean it on linux VOCALCORTEX LOG: okay more money 
okay more money VOCALCORTEX LOG: $5 
$5 VOCALCORTEX LOG: $25 
$25 VOCALCORTEX LOG: $65 
$65 VOCALCORTEX LOG: a $155 
a $155 VOCALCORTEX LOG: go ah [unk] handled 
go ah [unk] handled VOCALCORTEX LOG: please of the above my work help me to improve it 
please of the above my work help me to improve it VOCALCORTEX LOG: please if you love my work help me to improve it 
please if you love my work help me to improve it VOCALCORTEX LOG: i'\''m using the s*** microphone ever look @ the a curse 
i'm using the s*** microphone ever look @ the accuracy VOCALCORTEX LOG: also signs handle 
also signs handle VOCALCORTEX LOG: /\*!. 
/\*!. VOCALCORTEX LOG: ? 
? VOCALCORTEX LOG: -die 
- VOCALCORTEX LOG: - 
- VOCALCORTEX LOG: ' 
' VOCALCORTEX LOG: " 
" 

