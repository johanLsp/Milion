#ifndef MILION_MILIONPARALLELVOCAL_H_
#define MILION_MILIONPARALLELVOCAL_H_

#include "DifferenceProcessor.h"
#include "DigitalResonator.h"
#include "GainProcessor.h"
#include "BaseAudioProcessor.h"

class ParallelVocal : public BaseAudioProcessor {
 public:
    ParallelVocal() = default;
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    const String getName() const override { return "Voicing Source Processor"; };

 private:
    AudioProcessorGraph m_graph;

    GainProcessor* m_A1;
    DigitalResonator* m_R1;
    GainProcessor* m_AN;
    DigitalResonator* m_RNP;
    GainProcessor* m_A2;
    DigitalResonator* m_R2;
    GainProcessor* m_A3;
    DigitalResonator* m_R3;
    GainProcessor* m_A4;
    DigitalResonator* m_R4;
    GainProcessor* m_A5;
    DigitalResonator* m_R5;
    GainProcessor* m_A6;
    DigitalResonator* m_R6;
    GainProcessor* m_AB;

    DifferenceProcessor* m_DIFF;
};

#endif  // MILION_MILIONPARALLELVOCAL_H_
