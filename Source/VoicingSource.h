#ifndef MILION_MILIONVOICINGSOURCE_H_
#define MILION_MILIONVOICINGSOURCE_H_

#include "ImpulseGenerator.h"
#include "DigitalResonator.h"
#include "../JuceLibraryCode/JuceHeader.h"

class VoicingSource : public AudioProcessor {
 public:
    VoicingSource();
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

    void setFrequency(int frequency);

 private:
    ImpulseGenerator* m_pImpulseGenerator;
    DigitalResonator* m_pRGP;
    DigitalResonator* m_pRGZ;
    DigitalResonator* m_pRGS;

    AudioSampleBuffer m_RGZBuffer;

    double m_AV;
    double m_AVS;

    int m_sampleRate;
};

#endif  // MILION_MILIONVOICINGSOURCE_H_
