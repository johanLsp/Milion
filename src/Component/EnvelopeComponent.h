#ifndef MILION_ENVELOPECOMPONENT_H_
#define MILION_ENVELOPECOMPONENT_H_

/*
The MIT License (MIT)
Copyright (c) 2014, 2015 Jim Credland / Credland Technical Limited. 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "JuceHeader.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class EnvelopeComponent : public Component {
 public:
    EnvelopeComponent();
    ~EnvelopeComponent() {}
    
    void update();
    void setValueTreeState(AudioProcessorValueTreeState* vst);

    
    struct EnvelopeData {
     public:
        float attackLevel;
        float attackTime;
        float decay;
        float sustain; /** < 0.0 to 1.0. */
        float release;
        
        void drawDebugInformation(Graphics & g, Rectangle<float> area) const;
     private:
        void show(Graphics & g, const String & title, float value,
                  Rectangle<float> & area) const;
    };

    
    EnvelopeData & getData() { return data; }
    
    /** Several of these are arranged horizontally to make up the envelope. */
    class Segment : public Component {
     public:
        Segment(EnvelopeComponent * owner, Segment * segmentOnLeft)
        : owner(owner), right(nullptr), allowDurationChange(true) {
            if (segmentOnLeft != nullptr)
                segmentOnLeft->right = this;
            
            left = segmentOnLeft;
        }
        void mouseDown(const MouseEvent &) override;
        void mouseDrag(const MouseEvent &) override;
        void paint(Graphics &) override;
        
        enum ChainDirection {
            kLeft,
            kRight,
            kBoth
        };
        /** Returns the level, i.e. the Y amount, for this shape.
         Values are normalised from 0.0 to 1.0. */
        float getLeftLevel() const { return leftLevel; }
        float getRightLevel() const { return rightLevel; }
        
        void setLeftLevel(float newLevel, ChainDirection d = kBoth);
        void setRightLevel(float newLevel, ChainDirection d = kBoth);
        void setDuration(float newDuration) { duration = newDuration; }
        
        void setFixedDuration(float fixedDuration) { duration = fixedDuration; allowDurationChange = false; }
        
        /** 
         Returns the length of this segment.  Values are
         normalised from 0.0 to 1.0.
         */
        float getDuration() const { return duration; }
        
        void setYAxisControls(bool leftSide, bool rightSide);
        
    private:

        

        bool leftRightLinked() const { return controllingLeft && controllingRight; }


        EnvelopeComponent * owner;
        
        Segment * left;
        Segment * right;
        
        float leftLevel;
        float rightLevel;
        float duration;
        
        bool controllingLeft;
        bool controllingRight;
        bool allowDurationChange;
        
        struct MouseDownInfo
        {
            float left;
            float right;
            float duration;
        } mouseDownData;
        
        const float mouseSensitivity = 200.0f; /**< Increase to reduce senstivity. */
    };

private:
    void resized() override;
    /** Called by the segments when some values have changed. */
    void updateFromSegments();
    /** Updates the bounds of the segment components. */
    void updateSegmentPositions();
    
    EnvelopeData data;
    enum SegmentOrder
    {
        kAttack,
        kDecay,
        kSustain,
        kRelease
    };
    OwnedArray<Segment> segments;

    AudioProcessorValueTreeState* m_valueTreeState;
    Slider m_attackSlider;
    ScopedPointer<SliderAttachment> m_attackAttachment;

    Slider m_attackLevelSlider;
    ScopedPointer<SliderAttachment> m_attackLevelAttachment;

    Slider m_decaySlider;
    ScopedPointer<SliderAttachment> m_decayAttachment; 

    Slider m_sustainSlider;
    ScopedPointer<SliderAttachment> m_sustainAttachment;

    Slider m_releaseSlider;
    ScopedPointer<SliderAttachment> m_releaseAttachment;

};

#endif  // MILION_ENVELOPECOMPONENT_H_

