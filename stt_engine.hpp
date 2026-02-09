#ifndef STT_ENGINE_HPP
#define STT_ENGINE_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include "keyboard.hpp"

extern GlobalKeyboard kbd; 

using json = nlohmann::json;

inline void process_vosk_result(const std::string& raw_json) {
    if (raw_json.empty()) return;
    try {
        auto data = json::parse(raw_json);
        if (data.contains("text")) {
            std::string text = data["text"];
            if (!text.empty()) {
                std::cout << "Titan Typing: " << text << std::endl;
                kbd.type(text + " "); 
            }
        }
    } catch (const std::exception& e) {
        // Quietly catch parse errors
    }
}

#endif // STT_ENGINE_HPP
