#ifndef MILION_FORMANTCOMPONENT_H_
#define MILION_FORMANTCOMPONENT_H_

#include "JuceHeader.h"
#include "OperatorComponent.h"

class FormantComponent : public OperatorComponent {
 public:
    FormantComponent(AudioProcessorValueTreeState* vts);
    ~FormantComponent();
    OperatorComponent::Operator getType() override;
 private:
    AudioProcessorValueTreeState* m_valueTreeState;

};

#endif //  MILION_FORMANTCOMPONENT_H_
