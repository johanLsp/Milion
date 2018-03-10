#ifndef MILION_FORMANTCOMPONENT_H_
#define MILION_FORMANTCOMPONENT_H_

#include "JuceHeader.h"
#include "OperatorComponent.h"
#include "../Utils/Wavetable.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

class FormantComponent : public OperatorComponent {
 public:
    FormantComponent(AudioProcessorValueTreeState* vts);
    ~FormantComponent();
    void resized() override;
    OperatorComponent::Operator getType() override;

 private:
    AudioProcessorValueTreeState* m_valueTreeState;

    ComboBox m_waveform;
    ScopedPointer<ComboBoxAttachment> m_waveformAttachment;
    Slider m_bandwidth;
    ScopedPointer<SliderAttachment> m_bandwidthAttachment;
    Label m_bandwidthLabel;
    Slider m_skirt;
    ScopedPointer<SliderAttachment> m_skirtAttachment;
    Label m_skirtLabel;
};

#endif //  MILION_FORMANTCOMPONENT_H_
