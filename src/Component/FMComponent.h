#ifndef MILION_FMCOMPONENT_H_
#define MILION_FMCOMPONENT_H_

#include "JuceHeader.h"
#include "OperatorComponent.h"

typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

class FMComponent : public OperatorComponent {
 public:
    FMComponent(AudioProcessorValueTreeState* vts);
    ~FMComponent();
    void resized();
    OperatorComponent::Operator getType() override;

 private:
    AudioProcessorValueTreeState* m_valueTreeState;
    ComboBox m_waveform;
    ScopedPointer<ComboBoxAttachment> m_comboboxAttachment;
};

#endif //  MILION_FMCOMPONENT_H_
