#ifndef MILION_MILIONVOICINGSOURCE_H_
#define MILION_MILIONVOICINGSOURCE_H_

#include "ImpulseGenerator.h"
#include "DigitalResonator.h"
#include "GainProcessor.h"
#include "../JuceLibraryCode/JuceHeader.h"

class VoicingSource : public AudioProcessor {
 public:
    VoicingSource();
    ~VoicingSource();
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
    AudioProcessorGraph m_graph;

    ImpulseGenerator m_impulseGenerator;
    DigitalResonator m_RGP;
    DigitalResonator m_RGZ;
    DigitalResonator m_RGS;
    GainProcessor m_AV;
    GainProcessor m_AVS;
};

#endif  // MILION_MILIONVOICINGSOURCE_H_
