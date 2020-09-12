#ifndef MILION_MILIONVOICINGSOURCE_H_
#define MILION_MILIONVOICINGSOURCE_H_

#include "ImpulseGenerator.h"
#include "DigitalResonator.h"
#include "AntiResonator.h"
#include "GainProcessor.h"
#include "BaseAudioProcessor.h"

class VoicingSource : public BaseAudioProcessor {
 public:
    VoicingSource() = default;
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    const String getName() const override { return "Voicing Source Processor"; }
    void setFrequency(int frequency);

 private:
    AudioProcessorGraph m_graph;

    ImpulseGenerator* m_impulseGenerator;
    DigitalResonator* m_RGP;
    AntiResonator* m_RGZ;
    DigitalResonator* m_RGS;
    GainProcessor* m_AV;
    GainProcessor* m_AVS;
};

#endif  // MILION_MILIONVOICINGSOURCE_H_
