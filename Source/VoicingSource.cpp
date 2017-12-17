#include "VoicingSource.h"

VoicingSource::VoicingSource() {
    m_sampleRate = 44100;
    m_pImpulseGenerator = new ImpulseGenerator(m_sampleRate);
    m_pRGP = new DigitalResonator(m_sampleRate);
    m_pRGP->setCenterFrequency(0);
    m_pRGP->setBandwidth(100);

    m_pRGS = new DigitalResonator(m_sampleRate);
    m_pRGS->setCenterFrequency(0);
    m_pRGS->setBandwidth(200);

    m_pRGZ = new DigitalResonator(m_sampleRate);
    m_pRGZ->setCenterFrequency(1500);
    m_pRGZ->setBandwidth(6000);

    //  20dB
    m_AV = pow(10, 0/10);
    m_AVS = pow(10, 20/10);
}


void VoicingSource::setFrequency(int frequency) {
    m_pImpulseGenerator->setFrequency(frequency);
}


void VoicingSource::prepareToPlay(int numChannels, double sampleRate, int samplesPerBlock) {
    m_RGZBuffer.setSize(numChannels, samplesPerBlock);
}


void VoicingSource::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    m_pImpulseGenerator->processBlock(buffer);
    m_pRGP->processBlock(buffer);

    AudioSampleBuffer tmpBuffer(m_RGZBuffer.getArrayOfWritePointers(), buffer.getNumChannels(), buffer.getNumSamples());
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        tmpBuffer.copyFrom(ch, 0, buffer, ch, 0, buffer.getNumSamples());

    m_pRGS->processBlock(buffer);
    m_pRGZ->processBlock(tmpBuffer);

    buffer.applyGain(m_AVS);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        buffer.addFrom(ch, 0, tmpBuffer, ch, 0, buffer.getNumSamples(), m_AV); 
}
