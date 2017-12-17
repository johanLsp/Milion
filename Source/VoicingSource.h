#ifndef MILION_MILIONVOICINGSOURCE_H_
#define MILION_MILIONVOICINGSOURCE_H_

#include "ImpulseGenerator.h"
#include "DigitalResonator.h"
#include "../JuceLibraryCode/JuceHeader.h"


class VoicingSource {
 public:
    VoicingSource();
    void prepareToPlay(int numChannels, double sampleRate, int samplesPerBlock);
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages);
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
