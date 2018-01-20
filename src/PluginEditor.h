#ifndef MILION_MILIONPLUGINEDITOR_H_
#define MILION_MILIONPLUGINEDITOR_H_

#include "PluginProcessor.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class MilionAudioProcessorEditor  : public AudioProcessorEditor,
                                    private Slider::Listener {
 public:
    explicit MilionAudioProcessorEditor(MilionAudioProcessor&, AudioProcessorValueTreeState& vst);
    ~MilionAudioProcessorEditor();

    void paint(Graphics&) override;
    void resized() override;

    void pushBuffer(const float* data, int numSamples);


 private:
    void sliderValueChanged(Slider* slider) override;


 private:
    MilionAudioProcessor& processor;
    drow::TriggeredScope m_oscilloscope;
    drow::Spectroscope m_spectroscope;
    MidiKeyboardState m_keyboardState;
    MidiKeyboardComponent m_midiKeyboard;

    AudioProcessorValueTreeState& m_valueTreeState;
    Label m_freqMultiplierLabel;
    Slider m_freqMultiplierSlider;
    ScopedPointer<SliderAttachment> m_freqMultiplierAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MilionAudioProcessorEditor)
};

#endif  // MILION_MILIONPLUGINEDITOR_H_
