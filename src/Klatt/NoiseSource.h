#ifndef MILION_MILIONNOISESOURCE_H_
#define MILION_MILIONNOISESOURCE_H_

#include "RandomGenerator.h"
#include "DifferenceProcessor.h"
#include "BaseAudioProcessor.h"

class NoiseSource : public BaseAudioProcessor {
 public:
    NoiseSource() = default;
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    const String getName() const override { return "Voicing Source Processor"; }
    void setFrequency(int frequency);

 private:
    AudioProcessorGraph m_graph;

    RandomGenerator* m_randomGenerator;
    DifferenceProcessor* m_LPF;
};

#endif  // MILION_MILIONNOISESOURCE_H_
