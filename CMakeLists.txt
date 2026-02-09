cmake_minimum_required(VERSION 3.5)
project(VocalCortex)

# THIS IS THE CRITICAL FIX:
# It tells the compiler to use modern C++ so it understands 'noexcept' and 'override'
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force the compiler to find GTKMM and ALSA
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
pkg_check_modules(ALSA REQUIRED alsa)

# Include directories for X11, GTK, and ALSA
include_directories(/usr/include /usr/local/include ${GTKMM_INCLUDE_DIRS} ${ALSA_INCLUDE_DIRS})

# Vosk Paths
set(VOSK_DIR "$ENV{HOME}/c++/vosk-linux")
include_directories(${VOSK_DIR})
link_directories(${VOSK_DIR})

# Define the binary name as vocal_cortex
add_executable(vocal_cortex 
    main.cpp 
    titan_window.cpp 
    speechtotextservice.cpp
)

# Link all libraries to the new target name
target_link_libraries(vocal_cortex 
    ${GTKMM_LIBRARIES} 
    ${ALSA_LIBRARIES}
    curl 
    vosk
    X11
    Xtst 
    pthread
)

# Ensure the app finds libvosk.so at runtime
set_target_properties(vocal_cortex PROPERTIES INSTALL_RPATH "${VOSK_DIR}")
