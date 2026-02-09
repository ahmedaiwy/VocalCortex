#ifndef SPEECHTOTEXTSERVICE_H
#define SPEECHTOTEXTSERVICE_H

#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <alsa/asoundlib.h>

enum class TitanMode { OFFLINE, ONLINE };

class SpeechToTextService {
public:
    // This matches the lambda in your main.cpp
    typedef std::function<void(const std::string&)> TranscribedTextCallback;

    SpeechToTextService(TranscribedTextCallback cb);
    ~SpeechToTextService();

    bool init(const std::string& model_path, const std::string& api_key);
    void start_listening();
    void stop_listening();
    void set_mode(TitanMode mode);

private:
    void audio_processing_loop();
    // ... rest of your private members ...
    TranscribedTextCallback m_transcribed_text_callback;
    TitanMode m_current_mode;
    bool m_should_run_audio_thread;
    std::thread m_audio_processing_thread;
    snd_pcm_t* m_alsa_handle;
    struct VoskModel* m_model;
    struct VoskRecognizer* m_recognizer;
};

#endif
