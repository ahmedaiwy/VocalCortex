#include <gtkmm.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <regex>
#include <unistd.h>
#include "titan_window.hpp"
#include "speechtotextservice.h"
#include "nlohmann/json.hpp" // Path fixed for your local directory

TitanWindow* global_win = nullptr;

// --- THE MASTER PARSER ---
std::string absolute_parser(std::string text, const std::vector<std::string>& banned) {
    // Escape single quotes for shell safety
    text = std::regex_replace(text, std::regex("'"), "'\\''");

    // 1. HARDWARE SHORTCUTS
    if (std::regex_search(text, std::regex("\\b(capture screen|take a screenshot|snap it)\\b", std::regex_constants::icase))) return "CMD_SCREENSHOT";
    if (std::regex_search(text, std::regex("\\b(go left|move left|lift|go lift)\\b", std::regex_constants::icase))) return "CMD_LEFT";
    if (std::regex_search(text, std::regex("\\b(go up|move up)\\b", std::regex_constants::icase))) return "CMD_UP";
    if (std::regex_search(text, std::regex("\\b(go down|move down)\\b", std::regex_constants::icase))) return "CMD_DOWN";
    if (std::regex_search(text, std::regex("\\b(go left|move left)\\b", std::regex_constants::icase))) return "CMD_LEFT";
    if (std::regex_search(text, std::regex("\\b(go right|move right)\\b", std::regex_constants::icase))) return "CMD_RIGHT";
    if (std::regex_search(text, std::regex("\\b(page up|scroll up)\\b", std::regex_constants::icase))) return "CMD_PGUP";
    if (std::regex_search(text, std::regex("\\b(page down|scroll down)\\b", std::regex_constants::icase))) return "CMD_PGDN";
    if (std::regex_search(text, std::regex("\\b(go home|start of line)\\b", std::regex_constants::icase))) return "CMD_HOME";
    if (std::regex_search(text, std::regex("\\b(insert)\\b", std::regex_constants::icase))) return "CMD_INSERT";

    // AUDIO & BRIGHTNESS
    if (std::regex_search(text, std::regex("\\b(volume up|louder)\\b", std::regex_constants::icase))) return "CMD_VOL_UP";
    if (std::regex_search(text, std::regex("\\b(volume down|quieter)\\b", std::regex_constants::icase))) return "CMD_VOL_DOWN";
    if (std::regex_search(text, std::regex("\\b(mute|silent)\\b", std::regex_constants::icase))) return "CMD_MUTE";
    if (std::regex_search(text, std::regex("\\b(brightness up|screen brighter)\\b", std::regex_constants::icase))) return "CMD_BRIGHT_UP";
    if (std::regex_search(text, std::regex("\\b(brightness down|dim screen)\\b", std::regex_constants::icase))) return "CMD_BRIGHT_DOWN";
    if (std::regex_search(text, std::regex("\\b(brightness full|max brightness)\\b", std::regex_constants::icase))) return "CMD_BRIGHT_FULL";

    // SYSTEM TOOLS & NETWORK
    if (std::regex_search(text, std::regex("\\b(calculator|calc)\\b", std::regex_constants::icase))) return "CMD_CALC";
    if (std::regex_search(text, std::regex("\\b(wifi up|network on)\\b", std::regex_constants::icase))) return "CMD_WIFI_ON";
    if (std::regex_search(text, std::regex("\\b(wifi down|network off)\\b", std::regex_constants::icase))) return "CMD_WIFI_OFF";
    if (std::regex_search(text, std::regex("\\b(restart network|fix internet)\\b", std::regex_constants::icase))) return "CMD_NET_RESTART";
    // SYSTEM TOGGLES
    if (std::regex_search(text, std::regex("\\b(num lock|number lock)\\b", std::regex_constants::icase))) return "CMD_NUMLOCK";
    if (std::regex_search(text, std::regex("\\b(caps lock|cups lock|capital lock)\\b", std::regex_constants::icase))) return "CMD_CAPSLOCK";
    if (std::regex_search(text, std::regex("\\b(firefox)\\b", std::regex_constants::icase))) return "CMD_BROWSER";
    if (std::regex_search(text, std::regex("\\b(control|controlled) (see|sea|c)\\b", std::regex_constants::icase))) return "CMD_COPY";
    if (std::regex_search(text, std::regex("\\b(select all|grab all|choose all)\\b", std::regex_constants::icase))) return "CMD_SELECT_ALL";
    if (std::regex_search(text, std::regex("\\b(control|controlled) (x|ex|cut)\\b", std::regex_constants::icase))) return "CMD_CUT";
    if (std::regex_search(text, std::regex("\\b(task manager|system monitor|check processes)\\b", std::regex_constants::icase))) return "CMD_TASK_MANAGER";
    if (std::regex_search(text, std::regex("\\b(file manager|open folders|my files)\\b", std::regex_constants::icase))) return "CMD_FILE_MANAGER";
    if (std::regex_search(text, std::regex("\\b(open camera|launch camera|webcam)\\b", std::regex_constants::icase))) return "CMD_CAMERA";
    if (std::regex_search(text, std::regex("\\b(restart titan|reboot titan)\\b", std::regex_constants::icase))) return "CMD_RESTART";
    if (std::regex_search(text, std::regex("\\b(control|controlled) (v|vee|victor|paste)\\b", std::regex_constants::icase))) return "CMD_PASTE";
    if (std::regex_search(text, std::regex("\\b(switch|swing|sway|switched|which)\\b", std::regex_constants::icase))) return "CMD_ALT_TAB";
    if (std::regex_search(text, std::regex("\\b(undo|ando|under|andrew|andoo|i do)\\b", std::regex_constants::icase))) return "CMD_UNDO";
    if (std::regex_search(text, std::regex("\\b(enter|execute|send|send it)\\b", std::regex_constants::icase))) return "CMD_ENTER";
    if (std::regex_search(text, std::regex("\\b(redo|read do|riddle)\\b", std::regex_constants::icase))) return "CMD_REDO";
    if (std::regex_search(text, std::regex("\\b(strike|delete)\\b", std::regex_constants::icase))) return "CMD_SINGLE_BACKSPACE";
    if (std::regex_search(text, std::regex("\\b(backspace|delete last|erase that)\\b", std::regex_constants::icase))) return "CMD_BACKSPACE";
    if (std::regex_search(text, std::regex("\\b(backspace|delete last|erase that|iris|areas|areas or ease|iris iris)\\b", std::regex_constants::icase))) {
    return "CMD_BACKSPACE";
}
    // 2. DATA MAPS
    static const std::map<std::string, std::string> ordinal_map = {
        {"first", "1st"}, {"second", "2nd"}, {"third", "3rd"}, {"fourth", "4th"},
        {"fifth", "5th"}, {"sixth", "6th"}, {"seventh", "7th"}, {"eighth", "8th"},
        {"nineth", "9th"}, {"tenth", "10th"}, {"eleventh", "11th"}, {"twelfth", "12th"},
        {"thirteenth", "13th"}, {"fourteenth", "14th"}, {"fifteenth", "15th"}
    };

    static const std::set<std::string> date_keywords = {
        "july", "november", "december", "january", "february", "march", "april", 
        "may", "june", "august", "september", "october", "of", "the"
    };

    static const std::map<std::string, int> val_map = {
    {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5},
    {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}, {"zero", 0}, {"ten", 10},
    {"eleven", 11}, {"twelve", 12}, {"thirteen", 13}, {"fourteen", 14}, {"fifteen", 15},
    {"sixteen", 16}, {"seventeen", 17}, {"eighteen", 18}, {"nineteen", 19},
    {"twenty", 20}, {"thirty", 30}, {"forty", 40}, {"fifty", 50}, 
    {"sixty", 60}, {"seventy", 70}, {"eighty", 80}, {"ninety", 90},
    // ORDINAL ALIASES (The secret sauce for dates)
    {"tenth", 10}, {"eleventh", 11}, {"twelfth", 12}, {"thirteenth", 13}, 
    {"fourteenth", 14}, {"fifteenth", 15}, {"sixteenth", 16}, 
    {"seventeenth", 17}, {"eighteenth", 18}, {"nineteenth", 19}, {"twentieth", 20}
    };
    
    // UPDATED SYM_MAP with Math Operators
    static const std::map<std::string, std::string> sym_map = {
        {"plus", "+"}, {"minus", "-"}, {"times", "*"}, {"equals", "="},
        {"hyphen", "-"}, {"dash", "-"}, {"slash", "/"}, {"backslash", "\\"}, 
        {"star", "*"}, {"hashtag", "#"}, {"pound", "#"}, {"at", "@"}, {"point", "."}, 
        {"dot", "."}, {"comma", ","}, {"question", "?"}, {"exclamation", "!"},
        {"quote", "\""}, {"quotation", "\""}, {"apostrophe", "'"}, {"underscore", "_"}, {"space", " "}
    };

    std::stringstream ss(text);
    std::string word;
    std::vector<std::string> tokens;
    
    long long total_sum = 0, current_grp = 0;
    bool in_num = false, money_mode = false;
    // --- THE TEEN PICKLE FIXER ---
    auto get_ordinal_suffix = [](long long n) -> std::string {
        long long m = n % 100;
        if (m >= 11 && m <= 13) return "th"; // Specifically handles 11th, 12th, 13th
        switch (n % 10) {
            case 1:  return "st";
            case 2:  return "nd";
            case 3:  return "rd";
            default: return "th";
        }
    };

    auto flush_num = [&](bool force_ordinal = false) {
        if (in_num) {
            long long final_val = total_sum + current_grp;
            std::string s = std::to_string(final_val);
            if (force_ordinal) s += get_ordinal_suffix(final_val);
            if (money_mode) s = "$" + s;
            tokens.push_back(s);
            total_sum = 0; current_grp = 0; in_num = false; money_mode = false;
        } else if (money_mode) {
            tokens.push_back("$"); money_mode = false;
        }
    };

    while (ss >> word) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);

        // A. CENSORSHIP
        for (const auto& b : banned) {
            if (!b.empty() && word.find(b) != std::string::npos) {
                flush_num(); tokens.push_back(word.substr(0, 1) + "***"); goto next_word;
            }
        }

        // B. MULTI-WORD TRIGGERS (Added "divided by")
        if (word == "come") {
            std::string next;
            if (ss >> next) {
                std::transform(next.begin(), next.end(), next.begin(), ::tolower);
                if (next == "on" || next == "in") { flush_num(); tokens.push_back(","); continue; }
                else { flush_num(); tokens.push_back("come"); word = next; }
            }
        }
        if (word == "divided") {
            std::string next;
            if (ss >> next) {
                std::transform(next.begin(), next.end(), next.begin(), ::tolower);
                if (next == "by") { flush_num(); tokens.push_back("/"); continue; }
                else { flush_num(); tokens.push_back("/"); word = next; } // Assume division even if "by" is missed
            }
        }

        // C. CONTEXTUAL ORDINALS & DATES
        if (ordinal_map.count(word)) {
            flush_num(); tokens.push_back(ordinal_map.at(word));
        }
        else if (date_keywords.count(word)) {
            if (in_num) flush_num(true); 
            tokens.push_back(word);
        }
        // D. MONEY TRIGGERS
        else if (word == "dollar" || word == "dollars" || word == "bucks" || word == "bike" || word == "$") {
            money_mode = true; if (in_num) flush_num(); 
        }
        // E. NUMBER LOGIC
        else if (val_map.count(word)) {
            current_grp += val_map.at(word); in_num = true;
        }
        else if (word == "hundred") {
            current_grp = (current_grp == 0) ? 100 : current_grp * 100; in_num = true;
        }
        else if (word == "thousand") {
            total_sum += (current_grp == 0 ? 1 : current_grp) * 1000;
            current_grp = 0; in_num = true;
        }
        else if (word == "million") {
            total_sum += (current_grp == 0 ? 1 : current_grp) * 1000000;
            current_grp = 0; in_num = true;
        }
        // F. SYMBOLS (Math logic is inside here now)
        else if (sym_map.count(word)) {
            flush_num(); tokens.push_back(sym_map.at(word));
        }
        else if (word == "and") {
            if (!in_num) tokens.push_back("and");
        }
        else {
            flush_num(); tokens.push_back(word);
        }
        next_word:;
    }
    flush_num();

    // 3. MASTER GLUE LOGIC
    std::string result = "";
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].empty()) continue;
        result += tokens[i];
        if (i < tokens.size() - 1) {
            bool next_is_punct = (tokens[i+1] == "," || tokens[i+1] == "." || tokens[i+1] == "?" || tokens[i+1] == "!" || tokens[i+1] == ":" || tokens[i+1] == ";");
            bool curr_is_sticky = (tokens[i] == "$" || tokens[i] == "\\" || tokens[i] == "#");
            bool connecting_nums = (isdigit(tokens[i].back()) && (isdigit(tokens[i+1][0]) || tokens[i+1] == "st" || tokens[i+1] == "nd" || tokens[i+1] == "rd" || tokens[i+1] == "th"));
            
            // New: Don't put spaces around math operators if they are between numbers (e.g., 5+5)
            bool math_op = (tokens[i] == "+" || tokens[i] == "-" || tokens[i] == "*" || tokens[i] == "/");
            bool next_is_math = (tokens[i+1] == "+" || tokens[i+1] == "-" || tokens[i+1] == "*" || tokens[i+1] == "/");

            if (next_is_punct || curr_is_sticky || connecting_nums || math_op || next_is_math) continue;
            result += " ";
        }
    }
    return result; 
}

// --- HARDWARE COMMAND EXECUTOR ---
void execute_hardware_command(const std::string& input) {
    if (input.empty()) return;

    // 1. STRIP THE TRAILING SPACE (Fixes the "Catch Fix" trap)
    std::string clean_input = input;
    size_t last = clean_input.find_last_not_of(" \n\r\t");
    if (std::string::npos != last) {
        clean_input.erase(last + 1);
    }

    // 2. PHYSICAL COMMAND INTERCEPTORS
    if (clean_input == "CMD_SCREENSHOT") {
    // We use '&&' so the sound only plays if the screenshot command starts successfully
    std::string cmd = "mkdir -p ~/Pictures/Screenshots && "
                      "gnome-screenshot -f ~/Pictures/Screenshots/VocalCortex_$(date +%Y%m%d_%H%M%S).png && "
                      "canberra-gtk-play -i camera-shutter &";
    
    std::system(cmd.c_str());
    std::cout << "📸 SHUTTER FIRED: Check ~/Pictures/Screenshots" << std::endl;
    return;
    }
    if (clean_input == "CMD_VOL_UP")   { std::system("amixer -D pulse sset Master 5%+ > /dev/null 2>&1"); return; }
    if (clean_input == "CMD_VOL_DOWN") { std::system("amixer -D pulse sset Master 5%- > /dev/null 2>&1"); return; }
    if (clean_input == "CMD_MUTE")     { std::system("amixer -D pulse sset Master toggle > /dev/null 2>&1"); return; }

    // Brightness (Using brightnessctl - common on Ubuntu/Debian)
    if (clean_input == "CMD_BRIGHT_UP")   { std::system("xbacklight -set -inc +1 > /dev/null 2>&1"); return; }
    if (clean_input == "CMD_BRIGHT_DOWN") { std::system("xbacklight -set -dec -1 > /dev/null 2>&1"); return; }
    if (clean_input == "CMD_BRIGHT_FULL") { std::system("xbacklight -set 100 > /dev/null 2>&1"); return; }

    // Network & Apps
    if (clean_input == "CMD_CALC")        { std::system("gnome-calculator &"); return; }
    if (clean_input == "CMD_WIFI_ON")      { std::system("nmcli radio wifi on"); return; }
    if (clean_input == "CMD_WIFI_OFF")     { std::system("nmcli radio wifi off"); return; }
    if (clean_input == "CMD_NET_RESTART")  { std::system("sudo systemctl restart NetworkManager"); return; }
    // ARROWS & NAV
    if (clean_input == "CMD_UP")    { std::system("xdotool key --clearmodifiers Up"); return; }
    if (clean_input == "CMD_DOWN")  { std::system("xdotool key --clearmodifiers Down"); return; }
    if (clean_input == "CMD_LEFT")  { std::system("xdotool key --clearmodifiers Left"); return; }
    if (clean_input == "CMD_RIGHT") { std::system("xdotool key --clearmodifiers Right"); return; }
    if (clean_input == "CMD_PGUP")  { std::system("xdotool key --clearmodifiers Prior"); return; }
    if (clean_input == "CMD_PGDN")  { std::system("xdotool key --clearmodifiers Next"); return; }
    if (clean_input == "CMD_HOME")  { std::system("xdotool key --clearmodifiers Home"); return; }
    if (clean_input == "CMD_INSERT"){ std::system("xdotool key --clearmodifiers Insert"); return; }

    // LOCKS
    if (clean_input == "CMD_NUMLOCK")  { std::system("xdotool key Num_Lock"); return; }
    if (clean_input == "CMD_CAPSLOCK") { std::system("xdotool key Caps_Lock"); return; }
    if (clean_input == "CMD_BROWSER") { std::system("firefox &"); return; }
    if (clean_input == "CMD_TASK_MANAGER") { std::system("gnome-system-monitor &"); return; }
    if (clean_input == "CMD_FILE_MANAGER") { std::system("pcmanfm &"); return; }
    if (clean_input == "CMD_CAMERA")       { std::system("cheese &"); return; } // 'cheese' is the standard linux camera app
    if (clean_input == "CMD_ALT_TAB")    { std::system("xdotool key alt+Tab"); usleep(100000); return; }
    if (clean_input == "CMD_UNDO")       { std::system("xdotool key ctrl+z"); usleep(100000); return; }
    if (clean_input == "CMD_ENTER")      { std::system("xdotool key Return"); usleep(100000); return; }
    if (clean_input == "CMD_COPY")       { std::system("xdotool key ctrl+c"); usleep(100000); return; }
    if (clean_input == "CMD_PASTE")      { std::system("xdotool key ctrl+v"); usleep(100000); return; }
    if (clean_input == "CMD_CUT")        { std::system("xdotool key ctrl+x"); usleep(100000); return; }
    if (clean_input == "CMD_SELECT_ALL") { std::system("xdotool key ctrl+a"); usleep(100000); return; }
    if (clean_input == "CMD_REDO")       { std::system("xdotool key --clearmodifiers ctrl+y"); return; }
    // --- THE NEW "WORD-EATER" LOGIC ---
    if (clean_input == "CMD_BACKSPACE") { 
        // We repeat the BackSpace key 6 times to erase a full word
        std::system("xdotool key --clearmodifiers BackSpace BackSpace BackSpace BackSpace BackSpace BackSpace"); 
        return; 
    }
    if (clean_input == "CMD_SINGLE_BACKSPACE") { 
        std::system("xdotool key --clearmodifiers BackSpace"); 
        return; 
    }

    // 3. REGULAR TYPING ENGINE
    std::string text_to_type = input;
    text_to_type = std::regex_replace(text_to_type, std::regex("'\\\\''"), "'"); 
    
    std::string cmd = "xdotool type --clearmodifiers --delay 10 -- " + std::string("'") + 
                      std::regex_replace(text_to_type, std::regex("'"), "'\\''") + 
                      std::string("'");

    std::system(cmd.c_str());
}

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.VOCALCORTEX.commandcenter");
    TitanWindow window;
    global_win = &window;

    // Load Banned Words
    std::vector<std::string> banned;
    std::ifstream b_file("/home/android/c++/censor.txt");
    if (b_file.is_open()) {
        std::string line;
        while (std::getline(b_file, line)) {
            if(!line.empty()) {
                line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
                banned.push_back(line);
            }
        }
    }

    SpeechToTextService titan_service([&](const std::string& res) {
        try {
            auto j = nlohmann::json::parse(res);
            std::string raw = j.value("text", "");
            if (!raw.empty()) {
                std::string processed = absolute_parser(raw, banned);
                processed += " "; // The Catch Fix

                std::cout << "VOCALCORTEX LOG: " << processed << std::endl;
                
                Glib::signal_idle().connect_once([processed]() { 
                    execute_hardware_command(processed); 
                });
            }
        } catch (...) {}
    });

    if (!titan_service.init("/home/android/c++/vosk_models/vosk-model-en-us-daanzu-20200905-lgraph", "")) return -1;

    window.get_power_btn().signal_toggled().connect([&]() {
        if (window.get_power_btn().get_active()) titan_service.start_listening();
        else titan_service.stop_listening();
    });

    std::cout << "--- VOCAL CORTEX ONLINE ---" << std::endl;
    return app->run(window);
}
