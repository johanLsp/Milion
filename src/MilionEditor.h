#ifndef MILION_MILIONEDITOR_H_
#define MILION_MILIONEDITOR_H_

#include "MilionProcessor.h"
#include "Component/EnvelopeComponent.h"
#include "Component/ComponentContainer.h"
#include "dRowAudio/dRowAudio.h"

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
    MidiKeyboardState m_keyboardState;
    MidiKeyboardComponent m_midiKeyboard;

    ValueTreeStates& m_valueTreeStates;

    drow::Spectroscope m_spectroscope;
    drow::TriggeredScope m_oscilloscope;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MilionEditor)
};

#endif  // MILION_MILIONEDITOR_H_
