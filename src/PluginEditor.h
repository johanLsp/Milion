#ifndef MILION_MILIONPLUGINEDITOR_H_
#define MILION_MILIONPLUGINEDITOR_H_

#include "PluginProcessor.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class MilionAudioProcessorEditor  : public AudioProcessorEditor,
                                    private Slider::Listener {
 public:
    explicit MilionAudioProcessorEditor(MilionAudioProcessor&, ValueTreeStates& vst);
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

    ValueTreeStates& m_valueTreeStates;
    Label m_freqMultiplierLabel1;
    Slider m_freqMultiplierSlider1;
    Label m_freqMultiplierLabel2;
    Slider m_freqMultiplierSlider2;
    ScopedPointer<SliderAttachment> m_freqMultiplierAttachment1;
    ScopedPointer<SliderAttachment> m_freqMultiplierAttachment2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MilionAudioProcessorEditor)
};

#endif  // MILION_MILIONPLUGINEDITOR_H_
