#include "MilionProcessor.h"
#include "MilionEditor.h"

//==============================================================================
MilionEditor::MilionEditor(
    MilionProcessor& parent, ValueTreeStates& vts)
    : AudioProcessorEditor(&parent),
      m_valueTreeStates(vts),
      m_processor(parent),
      //m_spectroscope(12),
      m_midiKeyboard(m_keyboardState, MidiKeyboardComponent::horizontalKeyboard),
      m_tabLayout(TabbedButtonBar::TabsAtTop)
    {
    m_keyboardState.addListener(&m_processor);
    //m_oscilloscope.setNumSamplesPerPixel(2);
    //m_spectroscope.setLogFrequencyDisplay(true);
    //addAndMakeVisible(&m_oscilloscope);
    //addAndMakeVisible(&m_spectroscope);
    addAndMakeVisible(&m_midiKeyboard);
    setSize(800, 800);


    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_containers[i] = new ComponentContainer(vts[i]);
        m_tabLayout.addTab("Operator " + i, Colour(), m_containers[i], false);
        m_tabLayout.setTabBackgroundColour (i, getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    }

    addAndMakeVisible(&m_tabLayout);
/*

    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_envelopes[i].setValueTreeState(vts[i]);
        addAndMakeVisible(&m_envelopes[i]);
        m_waveforms[i].addItemList(waveformList, 1);
        addAndMakeVisible(&m_waveforms[i]);
        m_comboboxAttachment[i] = new ComboBoxAttachment(*(m_valueTreeStates[i]),
                                                        "waveform",
                                                        m_waveforms[i]);

        addAndMakeVisible(m_bandwidth[i]);
        m_bandwidthAttachment[i] = new SliderAttachment(*(m_valueTreeStates[i]),
                                                        "bandwidth",
                                                        m_bandwidth[i]);

        addAndMakeVisible(m_skirt[i]);
        m_skirtAttachment[i] = new SliderAttachment(*(m_valueTreeStates[i]),
                                                        "skirt",
                                                        m_skirt[i]);

        m_freqMultiplierLabel[i].setText("Frequency Multiplier", dontSendNotification);
        addAndMakeVisible(m_freqMultiplierLabel[i]);
        m_freqMultiplierSlider[i].setSliderStyle(Slider::Rotary);
        m_freqMultiplierSlider[i].setSkewFactor(0.1);

        m_freqMultiplierSlider[i].setTextBoxStyle(Slider::TextBoxBelow, true, 40, 20);
        addAndMakeVisible(m_freqMultiplierSlider[i]);
        m_freqMultiplierAttachment[i] = new SliderAttachment(*(m_valueTreeStates[i]),
                                                        "freq_multiplier",
                                                        m_freqMultiplierSlider[i]);
    }
*/
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
    
    m_tabLayout.setBounds(area.removeFromBottom(400));
    
    /*
    Rectangle<int> operators = area.removeFromBottom(200);
    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_envelopes[i].setBounds(operators.removeFromLeft(area.getWidth()/NUM_OPERATOR));
    }

    Rectangle<int> waveforms = area.removeFromBottom(50);
    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_waveforms[i].setBounds(waveforms.removeFromLeft(area.getWidth()/NUM_OPERATOR));
    }

    Rectangle<int> formant = area.removeFromBottom(50);
    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_bandwidth[i].setBounds(formant.removeFromLeft(area.getWidth()/NUM_OPERATOR/2));
        m_skirt[i].setBounds(formant.removeFromLeft(area.getWidth()/NUM_OPERATOR/2));
    }

    Rectangle<int> knobs = area.removeFromBottom(100);
    for (int i = 0; i < NUM_OPERATOR; i++) {
        m_freqMultiplierSlider[i].setBounds(knobs.removeFromLeft(area.getWidth()/NUM_OPERATOR));
    }
    */
   // m_oscilloscope.setBounds(area.removeFromRight(400));
   // m_spectroscope.setBounds(area);
}

void MilionEditor::pushBuffer(const float* data, int numSamples) {
   // m_oscilloscope.addSamples(data, numSamples);
   // m_spectroscope.copySamples(data, numSamples);
}
