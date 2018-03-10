#ifndef MILION_MILIONFMOPERATOR_H_
#define MILION_MILIONFMOPERATOR_H_

#include "BaseOperator.h"
#include "EnvelopeGenerator.h"
#include "../Utils/Wavetable.h"
/*
    Implements a Frequency Modulation operator
    This is actually done using phase modulation
*/

class FMOperator : public BaseOperator {

 public:
    FMOperator(AudioProcessorValueTreeState* vst);
    ~FMOperator();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    void handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(int midiChannel, int midiNoteNumber, float velocity) override;

    const String getName() const override {return "FM Operator";}

 private:
    AudioProcessorValueTreeState* m_valueTreeState;
    EnvelopeGenerator m_envelope;
    Wavetable m_wavetable;
    double m_basePhase;
    double m_modulatedPhase;
    double m_frequency;
    double m_feedbackGain;
};

#endif  // MILION_MILIONFMOPERATOR_H_
