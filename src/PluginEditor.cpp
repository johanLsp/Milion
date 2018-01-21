#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MilionAudioProcessorEditor::MilionAudioProcessorEditor(
    MilionAudioProcessor& parent, ValueTreeStates& vts)
    : AudioProcessorEditor(&parent),
      m_valueTreeStates(vts),
      processor(parent),
      m_spectroscope(12),
      m_midiKeyboard(m_keyboardState, MidiKeyboardComponent::horizontalKeyboard)
    {
    m_keyboardState.addListener(&processor);

    m_spectroscope.setLogFrequencyDisplay(true);
    addAndMakeVisible(&m_oscilloscope);
    addAndMakeVisible(&m_spectroscope);
    addAndMakeVisible(&m_midiKeyboard);
    setSize(800, 400);

    m_freqMultiplierLabel1.setText("Frequency Multiplier", dontSendNotification);
    addAndMakeVisible(m_freqMultiplierLabel1);
    addAndMakeVisible(m_freqMultiplierSlider1);
    m_freqMultiplierAttachment1 = new SliderAttachment(*(m_valueTreeStates[0]),
                                                        "freq_multiplier",
                                                        m_freqMultiplierSlider1);

    m_freqMultiplierLabel2.setText("Frequency Multiplier", dontSendNotification);
    addAndMakeVisible (m_freqMultiplierLabel2);
    addAndMakeVisible (m_freqMultiplierSlider2);
    m_freqMultiplierAttachment2 = new SliderAttachment(*(m_valueTreeStates[1]),
                                                        "freq_multiplier",
                                                        m_freqMultiplierSlider2);
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
    m_freqMultiplierSlider2.setBounds(area.removeFromBottom(50));
    m_freqMultiplierSlider1.setBounds(area.removeFromBottom(50));
    m_oscilloscope.setBounds(area.removeFromRight(400));
    m_spectroscope.setBounds(area);
}

void MilionAudioProcessorEditor::pushBuffer(const float* data, int numSamples) {
    m_oscilloscope.addSamples(data, numSamples);
    m_spectroscope.copySamples(data, numSamples);
}
