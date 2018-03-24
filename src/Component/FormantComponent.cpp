#include "FormantComponent.h"

FormantComponent::FormantComponent(AudioProcessorValueTreeState* vts)
: m_valueTreeState(vts) {

    StringArray waveformList;
    waveformList.add("Sine");
    waveformList.add("1");
    waveformList.add("2");
    waveformList.add("3");
    waveformList.add("4");
    waveformList.add("5");
    waveformList.add("Square");
    waveformList.add("Saw");
    m_waveform.addItemList(waveformList, 1);
    addAndMakeVisible(&m_waveform);
    m_waveformAttachment = new ComboBoxAttachment(*(m_valueTreeState),
                                                        "waveform",
                                                        m_waveform);
    addAndMakeVisible(&m_bandwidth);
    m_bandwidthAttachment = new SliderAttachment(*(m_valueTreeState),
                                                    "bandwidth",
                                                    m_bandwidth);
    m_bandwidth.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    m_bandwidth.setSliderStyle(Slider::Rotary);
    m_bandwidthLabel.setText("Bandwidth", dontSendNotification);

    addAndMakeVisible(m_bandwidthLabel);
    
    addAndMakeVisible(&m_skirt);
    m_skirtAttachment = new SliderAttachment(*(m_valueTreeState),
                                                    "skirt",
                                                    m_skirt);
    m_skirt.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    m_skirt.setSliderStyle(Slider::Rotary);

    m_skirtLabel.setText("Skirt", dontSendNotification);
    addAndMakeVisible(m_skirtLabel);

}

void FormantComponent::resized() {
    juce::Rectangle<int> area = getLocalBounds();
    m_waveform.setBounds(area.removeFromLeft(100).reduced(0, 25));
    area.removeFromLeft(50);
    m_bandwidthLabel.setBounds(area.removeFromLeft(100));
    m_bandwidth.setBounds(area.removeFromLeft(100));
    m_skirtLabel.setBounds(area.removeFromLeft(100));
    m_skirt.setBounds(area.removeFromLeft(100));
}

OperatorComponent::Operator FormantComponent::getType() {
    return OperatorComponent::Operator::Formant;
}
FormantComponent::~FormantComponent() {
}
