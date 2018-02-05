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
    m_comboboxAttachment = new ComboBoxAttachment(*(m_valueTreeState),
                                                        "waveform",
                                                        m_waveform);
}

OperatorComponent::Operator FMComponent::getType() {
    return OperatorComponent::Operator::FM;
}


void FMComponent::resized() {
    Rectangle<int> area = getLocalBounds();
    m_waveform.setBounds(area);
}

FMComponent::~FMComponent() {
}
