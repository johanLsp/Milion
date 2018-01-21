#include "EnveloppeGenerator.h"

EnveloppeGenerator::EnveloppeGenerator()
    : m_state(State::IDLE),
      m_attack(1.0),
      m_attackLevel(1.0),
      m_decay(1.0),
      m_sustain(0.5),
      m_sustainLevel(1.0),
      m_release(1.0),
      m_currentLevel(0.0) {
}

EnveloppeGenerator::~EnveloppeGenerator() {
}

void EnveloppeGenerator::setParameters(double attack,
                         double attackLevel,
                         double decay,
                         double sustainLevel,
                         double release) {
    double sampleRate = getSampleRate();
    m_attack = attack * sampleRate;
    m_attackLevel = attackLevel;
    m_decay = decay * sampleRate;
    m_sustainLevel = sustainLevel;
    m_release = release * sampleRate;
}

void EnveloppeGenerator::prepareToPlay(double sampleRate, int samplesPerBlock) {
}

void EnveloppeGenerator::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    float* channelData = buffer.getWritePointer(0);
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        switch (m_state) {
            case State::IDLE:
                break;
            case State::ATTACK:
                m_currentLevel += m_attackLevel / m_attack;
                if (m_currentLevel >= m_attackLevel)
                    m_state = State::DECAY;
                break;
            case State::DECAY:
                m_currentLevel -= (m_attackLevel - m_sustainLevel) / m_decay;
                if (m_currentLevel <= m_sustainLevel)
                    m_state = State::SUSTAIN;
                break;
            case State::SUSTAIN:
                m_currentLevel = m_sustainLevel;
                break;
            case State::RELEASE:
                m_currentLevel -= m_sustainLevel / m_release;
                if (m_currentLevel <= 0)
                    m_state = State::IDLE;
                break;
        }
        channelData[i] *= m_currentLevel;
    }
}

void EnveloppeGenerator::handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) {
    m_state = State::ATTACK;
}

void EnveloppeGenerator::handleNoteOff(int midiChannel, int midiNoteNumber, float velocity) {
    m_state = State::RELEASE;
}

