#ifndef MILION_MILIONBASEOPERATOR_H_
#define MILION_MILIONBASEOPERATOR_H_

#include <cmath>
#include "JuceHeader.h"

class BaseOperator : public AudioProcessor {
 public:
    BaseOperator();
    ~BaseOperator();

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
};
#endif  // MILION_MILIONBASEOPERATOR_H_
