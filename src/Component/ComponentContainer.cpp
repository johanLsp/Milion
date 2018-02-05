#include "ComponentContainer.h"

ComponentContainer::ComponentContainer(AudioProcessorValueTreeState* vst)
: m_valueTreeState(vst) {
    m_FMButton = new TextButton ("FM");
    m_FMButton->setClickingTogglesState(true);
    m_FMButton->setRadioGroupId(34567);
    m_FMButton->setBounds(20, 20, 50, 25);
    m_FMButton->setConnectedEdges(Button::ConnectedOnRight);
    m_FMButton->setToggleState(true, dontSendNotification);
    m_FMButton->addListener(this);
    addAndMakeVisible(m_FMButton);

    m_FormantButton = new TextButton ("Formant");
    m_FormantButton->setComponentID("formant");
    m_FormantButton->setClickingTogglesState(true);
    m_FormantButton->setRadioGroupId(34567);
    m_FormantButton->setBounds(70, 20, 50, 25);
    m_FormantButton->setConnectedEdges(Button::ConnectedOnLeft);
    m_FormantButton->setToggleState(false, dontSendNotification);
    m_FormantButton->addListener(this);
    addAndMakeVisible(m_FormantButton);

    m_envelope.setValueTreeState(m_valueTreeState);
    addAndMakeVisible(m_envelope);
    m_pOperator = new FMComponent(m_valueTreeState);
    addAndMakeVisible(m_pOperator);



}


void ComponentContainer::buttonClicked(Button* button) {
    if (button == m_FMButton && m_pOperator->getType() != OperatorComponent::Operator::FM) {
        setOperator(OperatorComponent::Operator::FM);
        Value type = m_valueTreeState->getParameterAsValue("operator_type");
        type = 0;
    } else if (button == m_FormantButton && m_pOperator->getType() != OperatorComponent::Operator::Formant) {
        setOperator(OperatorComponent::Operator::Formant);
        Value type = m_valueTreeState->getParameterAsValue("operator_type");
        type = 1;
    }
}

void ComponentContainer::resized() {
    Rectangle<int> area = getLocalBounds();
    Rectangle<int> knobs = area.removeFromTop(100);
    knobs.removeFromLeft(150);
    m_pOperator->setBounds(knobs);
    m_envelope.setBounds(area);
}

void ComponentContainer::setOperator(OperatorComponent::Operator op) {
    removeChildComponent(m_pOperator);
    delete m_pOperator;

    switch (op) {
        case OperatorComponent::Operator::FM:
            m_pOperator = new FMComponent(m_valueTreeState);
            break;
        case OperatorComponent::Operator::Formant:
            m_pOperator = new FormantComponent(m_valueTreeState);
            break;
    }
    addAndMakeVisible(m_pOperator);
    resized();
}

ComponentContainer::~ComponentContainer() {
    delete m_pOperator;
    delete m_FMButton;
    delete m_FormantButton;
}
