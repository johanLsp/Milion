#ifndef INCLUDE_MILIONENVELOPEGENERATOR_H_
#define INCLUDE_MILIONENVELOPEGENERATOR_H_

#include "JuceHeader.h"

class EnvelopeGenerator : public AudioProcessor {
 public:
    EnvelopeGenerator();
    ~EnvelopeGenerator();

    void setParameters(double attack, 
                         double attackLevel,
                         double decay,
                         double sustainLevel,
                         double release);

    void handleNoteOn(int midiChannel, int midiNoteNumber, float velocity);
    void handleNoteOff(int midiChannel, int midiNoteNumber, float velocity);
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;

    void releaseResources() override {};
    AudioProcessorEditor* createEditor() override {}

    // Configuration
    bool hasEditor() const override {return false;}
    const String getName() const override {return "Envelope Generator";}
    double getTailLengthSeconds() const override {return 0.0;}
    bool acceptsMidi() const override {return true;}
    bool producesMidi() const override {return false;}
    // Presets
    int getNumPrograms() override {return 1;}
    int getCurrentProgram() override {return 0;}
    void setCurrentProgram(int index) override {}
    const String getProgramName(int index) override {return {};}
    void changeProgramName(int index, const String& newName) override {}

    void getStateInformation(MemoryBlock& destData) override {};
    void setStateInformation(const void* data, int sizeInBytes) override {};


 private:
    enum class State {IDLE, ATTACK, DECAY, SUSTAIN, RELEASE} m_state;
    double m_attack;
    double m_attackLevel;
    double m_decay;
    double m_sustain;
    double m_sustainLevel;
    double m_release;
    double m_currentLevel;
};

#endif  // INCLUDE_MILIONENVELOPEGENERATOR_H_
