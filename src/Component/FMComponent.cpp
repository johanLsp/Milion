#include "FMComponent.h"


FMComponent::FMComponent(AudioProcessorValueTreeState* vts)
: m_valueTreeState(vts) {
    StringArray waveformList;
    waveformList.add("Sine");
    waveformList.add("1");
    waveformList.add("2");
    waveformList.add("3");
    waveformList.add("4");
    waveformList.add("5");
    waveformList.add("6");
    m_waveform.addItemList(waveformList, 1);
    addAndMakeVisible(&m_waveform);
    m_waveformAttachment = new ComboBoxAttachment(*(m_valueTreeState),
                                                        "waveform",
                                                        m_waveform);

    m_freqLabel.setText("Frequency", dontSendNotification);
    addAndMakeVisible(m_freqLabel);
    m_frequency.setSliderStyle(Slider::Rotary);
    m_frequency.setSkewFactor(0.1);
    m_frequency.setTextBoxStyle(Slider::NoTextBox, true, 50, 20);
    addAndMakeVisible(m_frequency);
    m_frequencyAttachment = new SliderAttachment(*(m_valueTreeState),
                                                        "freq_multiplier",
                                                        m_frequency);
}

OperatorComponent::Operator FMComponent::getType() {
    return OperatorComponent::Operator::FM;
}


void FMComponent::resized() {
    Rectangle<int> area = getLocalBounds();
    m_waveform.setBounds(area.removeFromLeft(100).reduced(0, 25));
    area.removeFromLeft(50);
    m_freqLabel.setBounds(area.removeFromLeft(100));
    m_frequency.setBounds(area.removeFromLeft(100));
}

FMComponent::~FMComponent() {
}
