#include "MilionProcessor.h"
#include "MilionEditor.h"

//==============================================================================
MilionEditor::MilionEditor(
    MilionProcessor& parent, ValueTreeStates& vts)
    : AudioProcessorEditor(&parent),
      m_valueTreeStates(vts),
      m_processor(parent),
      m_spectroscope(12),
      m_midiKeyboard(m_keyboardState, MidiKeyboardComponent::horizontalKeyboard)
    {
    m_keyboardState.addListener(&m_processor);

    m_spectroscope.setLogFrequencyDisplay(true);
    addAndMakeVisible(&m_oscilloscope);
    addAndMakeVisible(&m_spectroscope);
    addAndMakeVisible(&m_midiKeyboard);
    setSize(800, 600);


    StringArray waveformList;
    waveformList.add("Sine");
    waveformList.add("1");
    waveformList.add("2");
    waveformList.add("3");
    waveformList.add("4");
    waveformList.add("5");
    waveformList.add("6");
    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_envelopes[i].setValueTreeState(vts[i]);
        addAndMakeVisible(&m_envelopes[i]);
        m_waveforms[i].addItemList(waveformList, 1);
        addAndMakeVisible(&m_waveforms[i]);
        m_comboboxAttachment[i] = new ComboBoxAttachment(*(m_valueTreeStates[i]),
                                                        "waveform",
                                                        m_waveforms[i]);
        
    }

    m_freqMultiplierLabel1.setText("Frequency Multiplier", dontSendNotification);
    addAndMakeVisible(m_freqMultiplierLabel1);
    m_freqMultiplierSlider1.setSliderStyle(Slider::Rotary);
    m_freqMultiplierSlider1.setTextBoxStyle(Slider::TextBoxBelow, true, 40, 20);
    addAndMakeVisible(m_freqMultiplierSlider1);
    m_freqMultiplierAttachment1 = new SliderAttachment(*(m_valueTreeStates[0]),
                                                        "freq_multiplier",
                                                        m_freqMultiplierSlider1);
    m_freqMultiplierSlider2.setSliderStyle(Slider::Rotary);
    m_freqMultiplierSlider2.setTextBoxStyle(Slider::TextBoxBelow, true, 40, 20);

    m_freqMultiplierLabel2.setText("Frequency Multiplier", dontSendNotification);
    addAndMakeVisible (m_freqMultiplierLabel2);
    addAndMakeVisible (m_freqMultiplierSlider2);
    m_freqMultiplierAttachment2 = new SliderAttachment(*(m_valueTreeStates[1]),
                                                        "freq_multiplier",
                                                        m_freqMultiplierSlider2);
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
    Rectangle<int> area = getLocalBounds();
    m_midiKeyboard.setBounds(area.removeFromBottom(100));
    Rectangle<int> operators = area.removeFromBottom(200);
    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_envelopes[i].setBounds(operators.removeFromLeft(area.getWidth()/NUM_OPERATOR));
    }

    Rectangle<int> waveforms = area.removeFromBottom(50);
    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_waveforms[i].setBounds(waveforms.removeFromLeft(area.getWidth()/NUM_OPERATOR));
    }
    Rectangle<int> knobs = area.removeFromBottom(100);
    m_freqMultiplierSlider1.setBounds(knobs.removeFromLeft(100));
    m_freqMultiplierSlider2.setBounds(knobs.removeFromLeft(100));
    m_oscilloscope.setBounds(area.removeFromRight(400));
    m_spectroscope.setBounds(area);
}

void MilionEditor::pushBuffer(const float* data, int numSamples) {
    m_oscilloscope.addSamples(data, numSamples);
    m_spectroscope.copySamples(data, numSamples);
}
