#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <string>
#include <iostream>

class GlobalKeyboard {
public:
    GlobalKeyboard() {
        m_display = XOpenDisplay(NULL);
        if (!m_display) std::cerr << "X11 Display Error!" << std::endl;
    }
    
    ~GlobalKeyboard() {
        if (m_display) XCloseDisplay(m_display);
    }

    void type(const std::string& text) {
        if (!m_display) return;

        for (char c : text) {
            KeySym ks;
            // Explicitly handle space
            if (c == ' ') {
                ks = XK_space;
            } else {
                ks = XStringToKeysym(std::string(1, c).c_str());
            }

            if (ks != NoSymbol) {
                KeyCode code = XKeysymToKeycode(m_display, ks);
                if (code) {
                    XTestFakeKeyEvent(m_display, code, True, 0);  // Press
                    XTestFakeKeyEvent(m_display, code, False, 0); // Release
                }
            }
        }
        XFlush(m_display);
    }

private:
    Display* m_display;
};

#endif
