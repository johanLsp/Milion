#include "VoicingSource.h"



VoicingSource::VoicingSource() {
    m_pImpulseGenerator = new ImpulseGenerator(44100);
}



void VoicingSource::processBlock(AudioSampleBuffer& buffer) {
    m_pImpulseGenerator->processBlock(buffer);
}
