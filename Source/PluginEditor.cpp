#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MilionAudioProcessorEditor::MilionAudioProcessorEditor(MilionAudioProcessor& p)
    : AudioProcessorEditor(&p),
      processor(p),
      m_midiKeyboard(m_keyboardState, MidiKeyboardComponent::horizontalKeyboard)
    {
    m_frequencyKnob.setSliderStyle(Slider::Rotary);
    m_frequencyKnob.setRange(100, 10000, 1);
    m_frequencyKnob.setSkewFactor(0.5);
    m_frequencyKnob.setTextBoxStyle(Slider::TextBoxBelow, true, 60, 15);
    m_frequencyKnob.setTextValueSuffix(" Hz");
    m_frequencyKnob.addListener(this);

    m_keyboardState.addListener(&processor);


    addAndMakeVisible(&m_frequencyKnob);
    addAndMakeVisible(&m_midiKeyboard);
    setSize(400, 300);
}

MilionAudioProcessorEditor::~MilionAudioProcessorEditor() {
}

void MilionAudioProcessorEditor::sliderValueChanged(Slider* slider) {
}

//==============================================================================
void MilionAudioProcessorEditor::paint(Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

    g.setColour(Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Frequency", getLocalBounds(), Justification::centred, 1);
}

void MilionAudioProcessorEditor::resized() {
    Rectangle<int> area = getLocalBounds();
    m_midiKeyboard.setBounds(area.removeFromBottom(50));
    m_frequencyKnob.setBounds(area.reduced(50));
}
