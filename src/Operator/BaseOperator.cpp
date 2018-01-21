#include "BaseOperator.h"

BaseOperator::BaseOperator() {
}

BaseOperator::~BaseOperator() {
}


void BaseOperator::releaseResources() {
}

void BaseOperator::getStateInformation(MemoryBlock& destData) {
}

void BaseOperator::setStateInformation(const void* data, int sizeInBytes) {
}

void BaseOperator::handleNoteOn(int midiChannel, int midiNoteNumber, float velocity) {
}
