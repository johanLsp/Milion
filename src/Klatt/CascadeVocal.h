#ifndef MILION_MILIONCASCADEVOCAL_H_
#define MILION_MILIONCASCADEVOCAL_H_

#include "AntiResonator.h"
#include "BaseAudioProcessor.h"
#include "DigitalResonator.h"

class CascadeVocal : public BaseAudioProcessor {
 public:
    CascadeVocal() = default;
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    const String getName() const override { return "Voicing Source Processor"; }

 private:
    AudioProcessorGraph m_graph;

    DigitalResonator* m_RNP;
    AntiResonator* m_RNZ;
    DigitalResonator* m_R1;
    DigitalResonator* m_R2;
    DigitalResonator* m_R3;
    DigitalResonator* m_R4;
    DigitalResonator* m_R5;
};

#endif  // MILION_MILIONCASCADEVOCAL_H_
