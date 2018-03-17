#include "EnvelopeGenerator.h"

EnvelopeGenerator::EnvelopeGenerator()
    : m_state(State::IDLE),
      m_attack(1.0),
      m_attackLevel(1.0),
      m_decay(1.0),
      m_sustain(0.5),
      m_sustainLevel(1.0),
      m_release(1.0),
      m_currentLevel(0.0),
      m_targetLevel(0.0) {
}

EnvelopeGenerator::~EnvelopeGenerator() {
}

void EnvelopeGenerator::setParameters(double attack,
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

void EnvelopeGenerator::prepareToPlay(double sampleRate, int samplesPerBlock) {
}

void EnvelopeGenerator::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    float* channelData = buffer.getWritePointer(0);
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        switch (m_state) {
            case State::IDLE:
                break;
            case State::ATTACK:
                m_targetLevel += m_attackLevel / m_attack;
                if (m_targetLevel >= m_attackLevel)
                    m_state = State::DECAY;
                break;
            case State::DECAY:
                m_targetLevel -= (m_attackLevel - m_sustainLevel) / m_decay;
                if (m_targetLevel <= m_sustainLevel)
                    m_state = State::SUSTAIN;
                break;
            case State::SUSTAIN:
                m_targetLevel = m_sustainLevel;
                break;
            case State::RELEASE:
                m_targetLevel -= m_sustainLevel / m_release;
                if (m_targetLevel <= 0) {
                    m_targetLevel = 0;
                    m_state = State::IDLE;
                }
                break;
        }

        if (m_currentLevel - m_targetLevel > 0.01) {
            m_currentLevel -= 0.01;
        } else if (m_currentLevel - m_targetLevel < -0.01) {
            m_currentLevel += 0.01;
        } else {
            m_currentLevel = m_targetLevel;
        }

        channelData[i] *= m_currentLevel;
    }
}

void EnvelopeGenerator::handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) {
    m_state = State::ATTACK;
}

void EnvelopeGenerator::handleNoteOff(int midiChannel, int midiNoteNumber, float velocity) {
    m_state = State::RELEASE;
    if (m_targetLevel > m_sustainLevel) {
        m_targetLevel = m_sustainLevel;
    }
}

