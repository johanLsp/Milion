#ifndef MILION_MILIONVOICINGSOURCE_H_
#define MILION_MILIONVOICINGSOURCE_H_

#include "ImpulseGenerator.h"
#include "../JuceLibraryCode/JuceHeader.h"


class VoicingSource {
 public:
    VoicingSource();
    void processBlock(AudioSampleBuffer& buffer);


 private:
    ImpulseGenerator* m_pImpulseGenerator;
};

#endif  // MILION_MILIONVOICINGSOURCE_H_
