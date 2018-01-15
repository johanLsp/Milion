#ifndef MILION_MILIONFMOPERATOR_H_
#define MILION_MILIONFMOPERATOR_H_

#include "BaseOperator.h"

/*
    Implements a Frequency Modulation operator
    This is actually done using phase modulation
*/

class FMOperator : public BaseOperator {

 public:
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    void handleNoteOn(int midiChannel, int midiNoteNumber, float velocity);

    void setFrequencyMultiplier(double multiplier);

 private:
    double m_basePhase;
    double m_modulatedPhase;

    double m_frequency;
    double m_frequencyMultiplier;
    double m_feedbackGain;

};

#endif  // MILION_MILIONFMOPERATOR_H_