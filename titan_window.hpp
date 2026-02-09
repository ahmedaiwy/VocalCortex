#ifndef TITAN_WINDOW_HPP
#define TITAN_WINDOW_HPP

#include <gtkmm.h>

class TitanWindow : public Gtk::Window {
public:
    TitanWindow();
    virtual ~TitanWindow();
    
    void update_mic_level(double level);
    Gtk::ToggleButton& get_power_btn() { return m_power_btn; }
    Gtk::Switch& get_mode_switch() { return m_mode_switch; } // The new toggle

protected:
    bool on_animation_tick();

private:
    Gtk::Box m_vbox;
    Gtk::Image m_mic_image;
    Gtk::Label m_status_label;
    Gtk::ToggleButton m_power_btn;
    Gtk::ProgressBar m_audio_meter;
    
    // New Online/Offline UI elements
    Gtk::Box m_mode_box;
    Gtk::Label m_mode_label;
    Gtk::Switch m_mode_switch;

    double m_current_audio_level = 0.0;
};

#endif
