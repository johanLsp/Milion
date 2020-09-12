#include "MilionProcessor.h"
#include "MilionEditor.h"

MilionEditor::MilionEditor(
    MilionProcessor& parent, ValueTreeStates& vts)
    : AudioProcessorEditor(&parent),
      m_valueTreeStates(vts),
      m_processor(parent),
      m_spectroscope(12),
      m_midiKeyboard(m_keyboardState, MidiKeyboardComponent::horizontalKeyboard),
      m_tabLayout(TabbedButtonBar::TabsAtTop)
    {
    m_keyboardState.addListener(&m_processor);
    addAndMakeVisible(&m_midiKeyboard);
    setSize(800, 800);

    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_containers[i].reset(new ComponentContainer(vts[i]));
        m_tabLayout.addTab("Operator " + std::to_string(i), Colour(), m_containers[i].get(), false);
        m_tabLayout.setTabBackgroundColour (i, getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    }

    addAndMakeVisible(&m_tabLayout);

    m_spectroscope.setLogFrequencyDisplay(true);
    addAndMakeVisible(&m_spectroscope);
    addAndMakeVisible(&m_oscilloscope);
    resized();
}

MilionEditor::~MilionEditor() {
}

void MilionEditor::sliderValueChanged(Slider* slider) {
}

void MilionEditor::paint(Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    g.setColour(Colours::white);
    g.setFont(15.0f);
}

void MilionEditor::resized() {
    juce::Rectangle<int> area = getLocalBounds();
    m_midiKeyboard.setBounds(area.removeFromBottom(100));
    m_tabLayout.setBounds(area.removeFromTop(400));
    m_oscilloscope.setBounds(area.removeFromRight(400));
    m_spectroscope.setBounds(area);
}

void MilionEditor::pushBuffer(const float* data, int numSamples) {
    m_oscilloscope.addSamples(data, numSamples);
    m_spectroscope.copySamples(data, numSamples);
}
