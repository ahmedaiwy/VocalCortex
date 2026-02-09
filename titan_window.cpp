#include "titan_window.hpp"
#include <glibmm/main.h>

TitanWindow::TitanWindow() 
    : m_vbox(Gtk::ORIENTATION_VERTICAL, 10),
      m_mode_box(Gtk::ORIENTATION_HORIZONTAL, 10)
{
    set_title("VocalCortex@STT");
    set_default_size(240, 380);
    set_border_width(15);
    set_keep_above(true); 

    // 1. Pulsing Mic Icon
    m_mic_image.set_from_icon_name("audio-input-microphone", Gtk::ICON_SIZE_DIALOG);
    m_mic_image.set_pixel_size(48);
    m_vbox.pack_start(m_mic_image, Gtk::PACK_SHRINK, 10);

    // 2. Status Label
    m_status_label.set_markup("<b>Ready</b>");
    m_vbox.pack_start(m_status_label, Gtk::PACK_SHRINK);

    // 3. Power Button
    m_power_btn.set_label("Start Global Titan");
    m_vbox.pack_start(m_power_btn, Gtk::PACK_SHRINK);

    // 4. Mode Switch (The requested Online/Offline toggle)
    m_mode_label.set_text("Gemini Online Mode:");
    m_mode_box.pack_start(m_mode_label);
    m_mode_box.pack_end(m_mode_switch, Gtk::PACK_SHRINK);
    m_vbox.pack_start(m_mode_box, Gtk::PACK_SHRINK);

    // 5. Audio Meter
    m_audio_meter.set_text("Mic Sensitivity");
    m_audio_meter.set_show_text(true);
    m_vbox.pack_start(m_audio_meter, Gtk::PACK_SHRINK);

    add(m_vbox);
    show_all_children();

    Glib::signal_timeout().connect(sigc::mem_fun(*this, &TitanWindow::on_animation_tick), 30);
}

bool TitanWindow::on_animation_tick() {
    if (m_power_btn.get_active()) {
        int pulse_size = 48 + static_cast<int>(m_current_audio_level * 32.0);
        m_mic_image.set_pixel_size(pulse_size);
        m_mic_image.set_opacity(1.0);
    } else {
        m_mic_image.set_pixel_size(48);
        m_mic_image.set_opacity(0.3);
    }
    return true; 
}

void TitanWindow::update_mic_level(double level) {
    m_current_audio_level = level;
    m_audio_meter.set_fraction(level);
}

TitanWindow::~TitanWindow() {}
