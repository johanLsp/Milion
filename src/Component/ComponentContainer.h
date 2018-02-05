#ifndef MILION_COMPONENTCONTAINER_H_
#define MILION_COMPONENTCONTAINER_H_

#include "JuceHeader.h"
#include "EnvelopeComponent.h"
#include "FMComponent.h"
#include "FormantComponent.h"


class ComponentContainer  : public Component,
                            public Button::Listener {
 public:
    ComponentContainer(AudioProcessorValueTreeState* vts);
    ~ComponentContainer();
 private:
    void resized();
    void setOperator(OperatorComponent::Operator op);
    void buttonClicked(Button* button) override;


    AudioProcessorValueTreeState* m_valueTreeState;
    EnvelopeComponent m_envelope;
    OperatorComponent* m_pOperator;
    TextButton* m_FMButton;
    TextButton* m_FormantButton;
};



#endif //  MILION_COMPONENTCONTAINER_H_
