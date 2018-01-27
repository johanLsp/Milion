#ifndef MILION_MILIONEDITOR_H_
#define MILION_MILIONEDITOR_H_

#include "MilionProcessor.h"
#include "Component/EnvelopeComponent.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

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
    ComboBox m_waveforms[NUM_OPERATOR];
    ScopedPointer<ComboBoxAttachment> m_comboboxAttachment[NUM_OPERATOR];

    
    Slider m_bandwidth[NUM_OPERATOR];
    ScopedPointer<SliderAttachment> m_bandwidthAttachment[NUM_OPERATOR];
    Slider m_skirt[NUM_OPERATOR];
    ScopedPointer<SliderAttachment> m_skirtAttachment[NUM_OPERATOR];

    Label m_freqMultiplierLabel[NUM_OPERATOR];
    Slider m_freqMultiplierSlider[NUM_OPERATOR];
    ScopedPointer<SliderAttachment> m_freqMultiplierAttachment[NUM_OPERATOR];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MilionEditor)
};

#endif  // MILION_MILIONEDITOR_H_
