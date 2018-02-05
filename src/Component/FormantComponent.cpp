#include "FormantComponent.h"

FormantComponent::FormantComponent(AudioProcessorValueTreeState* vts)
: m_valueTreeState(vts) {
}

OperatorComponent::Operator FormantComponent::getType() {
    return OperatorComponent::Operator::Formant;
}
FormantComponent::~FormantComponent() {
}
