#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MilionAudioProcessorEditor::MilionAudioProcessorEditor(MilionAudioProcessor& p)
    : AudioProcessorEditor(&p),
      processor(p),
      m_spectroscope(12),
      m_midiKeyboard(m_keyboardState, MidiKeyboardComponent::horizontalKeyboard)
    {
    m_keyboardState.addListener(&processor);

    m_spectroscope.setLogFrequencyDisplay(true);
    addAndMakeVisible(&m_oscilloscope);
    addAndMakeVisible(&m_spectroscope);
    addAndMakeVisible(&m_midiKeyboard);
    setSize(800, 400);
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
}

void MilionAudioProcessorEditor::resized() {
    Rectangle<int> area = getLocalBounds();
    m_midiKeyboard.setBounds(area.removeFromBottom(100));
    m_oscilloscope.setBounds(area.removeFromRight(400));
    m_spectroscope.setBounds(area);
}


void MilionAudioProcessorEditor::pushBuffer(const float* data, int numSamples) {
    m_oscilloscope.addSamples(data, numSamples);
    m_spectroscope.copySamples(data, numSamples);
}

