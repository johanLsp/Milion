#ifndef MILION_MILIONDIFFERENCEPROCESSOR_H_
#define MILION_MILIONDIFFERENCEPROCESSOR_H_

#include <cmath>

#include "JuceHeader.h"

/* Digital filter - output is given by :
 * y(n) = A*x(n) + B*y(n-1) + C*y(n-2) 
 */

class DifferenceProcessor : public AudioProcessor {
 public:
    enum class Type {LOWPASS, HIGHPASS};
 public:
    DifferenceProcessor();
    ~DifferenceProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;
    const String getName() const override;
    void releaseResources() override;
    double getTailLengthSeconds() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String& newName) override;

    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    void setType(Type type);

 private:
    double m_xx[2];
    int m_sign;
};

#endif  // MILION_MILIONDIFFERENCEPROCESSOR_H
