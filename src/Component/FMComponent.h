#ifndef MILION_FMCOMPONENT_H_
#define MILION_FMCOMPONENT_H_

#include "JuceHeader.h"
#include "OperatorComponent.h"

typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;
typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class FMComponent : public OperatorComponent {
 public:
    FMComponent(AudioProcessorValueTreeState* vts);
    ~FMComponent();
    void resized() override;
    OperatorComponent::Operator getType() override;

 private:
    AudioProcessorValueTreeState* m_valueTreeState;
    ComboBox m_waveform;
    std::unique_ptr<ComboBoxAttachment> m_waveformAttachment;
    Slider m_frequency;
    std::unique_ptr<SliderAttachment> m_frequencyAttachment;
    Label m_freqLabel;
};

#endif //  MILION_FMCOMPONENT_H_
