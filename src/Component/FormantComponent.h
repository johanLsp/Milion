#ifndef MILION_FORMANTCOMPONENT_H_
#define MILION_FORMANTCOMPONENT_H_

#include "JuceHeader.h"
#include "OperatorComponent.h"

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
    std::unique_ptr<ComboBoxAttachment> m_waveformAttachment;
    Slider m_bandwidth;
    std::unique_ptr<SliderAttachment> m_bandwidthAttachment;
    Label m_bandwidthLabel;
    Slider m_skirt;
    std::unique_ptr<SliderAttachment> m_skirtAttachment;
    Label m_skirtLabel;
};

#endif //  MILION_FORMANTCOMPONENT_H_
