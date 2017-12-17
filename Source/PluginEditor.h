#ifndef MILION_MILIONPLUGINEDITOR_H_
#define MILION_MILIONPLUGINEDITOR_H_

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class MilionAudioProcessorEditor  : public AudioProcessorEditor,
                                    private Slider::Listener {
 public:
    explicit MilionAudioProcessorEditor(MilionAudioProcessor&);
    ~MilionAudioProcessorEditor();

    void paint(Graphics&) override;
    void resized() override;

 private:
    void sliderValueChanged(Slider* slider) override;


 private:
    MilionAudioProcessor& processor;
    Slider m_frequencyKnob;
    MidiKeyboardState m_keyboardState;
    MidiKeyboardComponent m_midiKeyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MilionAudioProcessorEditor)
};

#endif  // MILION_MILIONPLUGINEDITOR_H_
