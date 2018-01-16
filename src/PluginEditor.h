#ifndef MILION_MILIONPLUGINEDITOR_H_
#define MILION_MILIONPLUGINEDITOR_H_

#include "PluginProcessor.h"

class MilionAudioProcessorEditor  : public AudioProcessorEditor,
                                    private Slider::Listener {
 public:
    explicit MilionAudioProcessorEditor(MilionAudioProcessor&);
    ~MilionAudioProcessorEditor();

    void paint(Graphics&) override;
    void resized() override;

    void pushBuffer(const float* data, int numSamples);


 private:
    void sliderValueChanged(Slider* slider) override;


 private:
    MilionAudioProcessor& processor;
    drow::TriggeredScope m_oscilloscope;
    
    MidiKeyboardState m_keyboardState;
    MidiKeyboardComponent m_midiKeyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MilionAudioProcessorEditor)
};

#endif  // MILION_MILIONPLUGINEDITOR_H_
