#ifndef MILION_MILIONEDITOR_H_
#define MILION_MILIONEDITOR_H_

#include "MilionProcessor.h"
#include "Component/EnvelopeComponent.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class MilionEditor  : public AudioProcessorEditor,
                                    private Slider::Listener {
 public:
    explicit MilionEditor(MilionProcessor&, ValueTreeStates& vst);
    ~MilionEditor();

    void paint(Graphics&) override;
    void resized() override;
    void pushBuffer(const float* data, int numSamples);

 private:
    void sliderValueChanged(Slider* slider) override;

 private:
    MilionProcessor& m_processor;
    drow::TriggeredScope m_oscilloscope;
    drow::Spectroscope m_spectroscope;
    MidiKeyboardState m_keyboardState;
    MidiKeyboardComponent m_midiKeyboard;

    ValueTreeStates& m_valueTreeStates;
    EnvelopeComponent m_envelopes[NUM_OPERATOR];

    Label m_freqMultiplierLabel1;
    Slider m_freqMultiplierSlider1;
    Label m_freqMultiplierLabel2;
    Slider m_freqMultiplierSlider2;
    ScopedPointer<SliderAttachment> m_freqMultiplierAttachment1;
    ScopedPointer<SliderAttachment> m_freqMultiplierAttachment2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MilionEditor)
};

#endif  // MILION_MILIONEDITOR_H_
