#include "BaseOperator.h"

BaseOperator::BaseOperator() {
}

BaseOperator::~BaseOperator() {
}

const String BaseOperator::getName() const {
    return "Anti Resonator Processor";
}

void BaseOperator::releaseResources() {
}

double BaseOperator::getTailLengthSeconds() const {
    return 0.0;
}

bool BaseOperator::acceptsMidi() const {
    return false;
}

bool BaseOperator::producesMidi() const {
    return false;
}

AudioProcessorEditor* BaseOperator::createEditor() {

}

bool BaseOperator::hasEditor() const {
    return false;
}

int BaseOperator::getNumPrograms() {
    return 1;
}

int BaseOperator::getCurrentProgram() {
    return 0;
}

void BaseOperator::setCurrentProgram(int index) {
}

const String BaseOperator::getProgramName(int index) {
    return {};
}

void BaseOperator::changeProgramName(int index, const String& newName) {
}

void BaseOperator::getStateInformation(MemoryBlock& destData) {
}

void BaseOperator::setStateInformation(const void* data, int sizeInBytes) {
}
