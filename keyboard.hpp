#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <string>

class GlobalKeyboard {
private:
    Display *display;
public:
    GlobalKeyboard() { display = XOpenDisplay(NULL); }
    ~GlobalKeyboard() { if (display) XCloseDisplay(display); }

    void type(const std::string& text) {
        if (!display) return;
        for (char c : text) {
            KeySym sym = (c == ' ') ? XK_space : XStringToKeysym(std::string(1, c).c_str());
            KeyCode code = XKeysymToKeycode(display, sym);
            if (code != 0) {
                XTestFakeKeyEvent(display, code, True, 0);  // Press
                XTestFakeKeyEvent(display, code, False, 0); // Release
                XFlush(display);
            }
        }
    }
};
#endif
