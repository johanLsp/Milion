#ifndef MILION_BASEOPERATORCOMPONENT_H_
#define MILION_BASEOPERATORCOMPONENT_H_

#include "JuceHeader.h"

class OperatorComponent : public juce::Component {
 public:
    enum class Operator {FM, Formant};
    virtual Operator getType() = 0;
};

#endif //  MILION_BASEOPERATORCOMPONENT_H_
