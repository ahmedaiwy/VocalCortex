#include "speechtotextservice.h"
#include "vosk_api.h"
#include "titan_window.hpp"
#include <glibmm/main.h>
#include <iostream>
#include <vector>
#include <cmath>

extern TitanWindow* global_win; // This tells the compiler "it exists in main.cpp"
SpeechToTextService::SpeechToTextService(TranscribedTextCallback cb)
    : m_transcribed_text_callback(cb), m_should_run_audio_thread(false), 
      m_alsa_handle(nullptr), m_model(nullptr), m_recognizer(nullptr) {}

SpeechToTextService::~SpeechToTextService() {
    stop_listening();
    if(m_recognizer) vosk_recognizer_free(m_recognizer);
    if(m_model) vosk_model_free(m_model);
    if(m_alsa_handle) snd_pcm_close(m_alsa_handle);
}

bool SpeechToTextService::init(const std::string& model_path, const std::string& api_key) {
    m_model = vosk_model_new(model_path.c_str());
    if (!m_model) return false;
    m_recognizer = vosk_recognizer_new(m_model, 16000.0f);
    if (snd_pcm_open(&m_alsa_handle, "default", SND_PCM_STREAM_CAPTURE, 0) < 0) return false;
    snd_pcm_set_params(m_alsa_handle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, 16000, 1, 500000);
    return true;
}

void SpeechToTextService::start_listening() {
    if (m_should_run_audio_thread) return;
    m_should_run_audio_thread = true;
    m_audio_processing_thread = std::thread(&SpeechToTextService::audio_processing_loop, this);
}

void SpeechToTextService::stop_listening() {
    m_should_run_audio_thread = false;
    if (m_audio_processing_thread.joinable()) m_audio_processing_thread.join();
}

void SpeechToTextService::audio_processing_loop() {
    std::vector<short> buffer(3200); 
    while (m_should_run_audio_thread) {
        int err = snd_pcm_readi(m_alsa_handle, buffer.data(), buffer.size());
        if (err < 0) { snd_pcm_prepare(m_alsa_handle); continue; }

        long long sum = 0;
        for (short s : buffer) sum += std::abs(s);
        double level = (double)sum / (buffer.size() * 3000.0);
        Glib::signal_idle().connect_once([level]() { 
            if (global_win) global_win->update_mic_level(level); 
        });

        if (vosk_recognizer_accept_waveform(m_recognizer, (const char*)buffer.data(), err * 2)) {
            m_transcribed_text_callback(vosk_recognizer_result(m_recognizer));
        }
    }
}
