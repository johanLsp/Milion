#ifndef MILION_MILIONRANDOMGENERATOR_H_
#define MILION_MILIONRANDOMGENERATOR_H_


class RandomGenerator : public AudioProcessor {
 public:
    RandomGenerator();
    ~RandomGenerator();

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
    int m_frequency;
    uint64_t m_currentTick;
    Random m_random;
};

#endif  // MILION_MILIONRANDOMGENERATOR_H_
