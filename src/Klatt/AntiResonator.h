#ifndef MILION_MILIONANTIRESONATOR_H_
#define MILION_MILIONANTIRESONATOR_H_

#include <cmath>

/* Anti filter - output is given by :
 * y(n) = A*x(n) + B*y(n-1) + C*y(n-2) 
 */

class AntiResonator : public AudioProcessor {
 public:
    AntiResonator();
    ~AntiResonator();

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

    void setCenterFrequency(double frequency);
    void setBandwidth(double bandwidth);

 private:
    double m_centerFrequency;
    double m_bandwidth;

    double m_A;
    double m_B;
    double m_C;
    double m_x[2];
    double m_xx[2];
};
#endif  // MILION_MILIONANTIRESONATOR_H
