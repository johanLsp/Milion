#include "EnvelopeComponent.h"

void EnvelopeComponent::EnvelopeData::drawDebugInformation(Graphics & g,
                                                    juce::Rectangle<float> area) const
{
    g.setColour(Colours::grey.withAlpha(0.5f));
    area.setHeight(area.getHeight() / 5.0f);
    
    show(g, "AttackLevel", attackLevel, area);
    show(g, "attackTime",  attackTime, area);
    show(g, "decay",       decay, area);
    show(g, "sustain",     sustain, area);
    show(g, "release",     release, area);
}

void  EnvelopeComponent::EnvelopeData::show(Graphics & g,
                                     const String & title,
                                     float value,
                                     juce::Rectangle<float> & area) const
{
    float halfWidth = area.getWidth() / 2.0f + 5.0f;

    g.drawText(title,
               area.withTrimmedRight(halfWidth),
               Justification::right, false);

    g.drawText(String(value),
               area.withTrimmedLeft(halfWidth),
               Justification::left, false);

    area.translate(0.0f, area.getHeight());
}



EnvelopeComponent::EnvelopeComponent()
{
    /* Attack. Y axis drag controls right side attack level. */
    segments.add(new Segment(this, nullptr));
    segments[kAttack]->setYAxisControls(false, true);
                
    /* Decay. Y axis drag controls left side attack level. */
    segments.add(new Segment(this, segments[kAttack]));
    segments[kDecay]->setYAxisControls(true, false);
    
    /* Sustain.  Y axis drag controls both left and right sides, which
     remain in step. */
    segments.add(new Segment(this, segments[kDecay]));
    segments[kSustain]->setYAxisControls(true, true);
    segments[kSustain]->setFixedDuration(0.5f);
    
    /* Release.  Y axis drag controls left hand sustain segment. */
    segments.add(new Segment(this, segments[kSustain]));
    segments[kRelease]->setYAxisControls(true, false);
    
    for (auto s: segments)
        addAndMakeVisible(s);
    
    /* Some initial data. */
    data = { 0.5f, 0.5f, 0.5f, 0.5f, 0.5f };
    
    /* Set the initial positions. */
    update();
}


void EnvelopeComponent::setValueTreeState(AudioProcessorValueTreeState* vst) {
    m_valueTreeState = vst;
    m_attackAttachment.reset(
        new SliderAttachment(*(m_valueTreeState), "env_attack", m_attackSlider));
    m_attackLevelAttachment.reset(
        new SliderAttachment(*(m_valueTreeState), "env_attackLevel", m_attackLevelSlider));
    m_decayAttachment.reset(
        new SliderAttachment(*(m_valueTreeState), "env_decay", m_decaySlider));
    m_sustainAttachment.reset(
        new SliderAttachment(*(m_valueTreeState), "env_sustain", m_sustainSlider));
    m_releaseAttachment.reset(
        new SliderAttachment(*(m_valueTreeState),  "env_release", m_releaseSlider));
}

void EnvelopeComponent::resized()
{
    updateSegmentPositions();
}


void EnvelopeComponent::updateFromSegments()
{
    updateSegmentPositions();
    data.attackLevel    = segments[kAttack]->getRightLevel();
    data.attackTime     = segments[kAttack]->getDuration();
    data.decay          = segments[kDecay]->getDuration();
    data.sustain        = segments[kSustain]->getLeftLevel();
    data.release        = segments[kRelease]->getDuration();
    m_attackSlider.setValue(data.attackTime);
    m_attackLevelSlider.setValue(data.attackLevel);
    m_decaySlider.setValue(data.decay);
    m_sustainSlider.setValue(data.sustain);
    m_releaseSlider.setValue(data.release);
}

void EnvelopeComponent::updateSegmentPositions()
{
    float totalDuration = 0.0f;
    const float minDuration = 0.01f;
    
    for (auto s: segments)
        totalDuration += (s->getDuration() + minDuration);
    
    int width = getWidth();
    int height = getHeight();
    
    float xpos = 0.0f;
    
    for (auto s: segments)
    {
        float dur = s->getDuration();
        float pw = ((dur + minDuration) / totalDuration) * width;
        
        s->setBounds(int(xpos), 0, int(pw), height);
        
        xpos += int(pw);
    }
}

void EnvelopeComponent::update()
{
    segments[kAttack]->setLeftLevel(0.0);
    segments[kAttack]->setRightLevel(data.attackLevel);
    segments[kAttack]->setDuration(data.attackTime);
    segments[kDecay]->setDuration(data.decay);
    segments[kSustain]->setLeftLevel(data.sustain);
    segments[kSustain]->setRightLevel(data.sustain);
    segments[kRelease]->setDuration(data.release);
    segments[kRelease]->setRightLevel(0.0);
    
    updateSegmentPositions();
}


void EnvelopeComponent::Segment::paint(Graphics & g)
{
    float height = (float) getHeight();
    float width = (float) getWidth();

    /** Pop some edges on our segment. */
    g.setColour(Colours::grey.withAlpha(0.5f));
    g.drawLine(width, 0.0f, width, height, 2.0f);
    g.drawLine(0.0f, 0.0f, 0.0f, height, 2.0f);

    /* Calculate the end positions. */
    float yleft = height - (float(leftLevel) * height);
    float yright = height - (float(rightLevel) * height);
    
    float ycurve = jmax(yleft, yright);
    
    /* Shade under our line. */
    {
        const Colour c1 = Colours::white.withAlpha(0.05f);
        const Colour c2 = Colours::white.withAlpha(0.2f);
        g.setGradientFill(ColourGradient(c2, 0.0, jmin(yleft, yright),
                                         c1, 0.0, height,
                                         false));
        Path p;
        p.startNewSubPath(0.0f, yleft);
        p.quadraticTo(width / 2.0, ycurve, width, yright);
        p.lineTo(width, height);
        p.lineTo(0.0, height);
        p.closeSubPath();
        
        g.fillPath(p);
    }
    
    /* And draw a nice solid line on top. */
    {
        g.setColour(Colours::white);
        
        Path p;
        p.startNewSubPath(0.0f, yleft);
        p.quadraticTo(width / 2.0, ycurve, width, yright);
        
        g.strokePath(p, PathStrokeType(4.0f));
    }
}


void EnvelopeComponent::Segment::setLeftLevel(float newLevel, ChainDirection direction)
{
    leftLevel = newLevel;
    
    if ((direction == kLeft || direction == kBoth) && left)
        left->setRightLevel(newLevel, kLeft);
    
    if ((direction == kRight || direction == kBoth) && leftRightLinked())
        setRightLevel(newLevel, kRight);
    
    repaint();
}

void EnvelopeComponent::Segment::setRightLevel(float newLevel, ChainDirection direction)
{
    rightLevel = newLevel;
    
    if ((direction == kRight || direction == kBoth) && right)
        right->setLeftLevel(newLevel, kRight);
    
    if ((direction == kLeft || direction == kBoth) && leftRightLinked())
        setLeftLevel(newLevel, kLeft);
    
    repaint();
}

void EnvelopeComponent::Segment::mouseDown(const MouseEvent & e)
{
    mouseDownData.left = leftLevel;
    mouseDownData.right = rightLevel;
    mouseDownData.duration = duration;
}

void EnvelopeComponent::Segment::mouseDrag(const MouseEvent & e)
{
    if (allowDurationChange)
    {
        float dx = float(e.getDistanceFromDragStartX()) / mouseSensitivity;
        duration = jlimit(0.0f, 1.0f, mouseDownData.duration + dx);
    }
    
    float dy = float(-1 * e.getDistanceFromDragStartY()) / mouseSensitivity;
    
    if (controllingLeft)
        setLeftLevel(jlimit(0.0f, 1.0f, mouseDownData.left + dy));
    else if (controllingRight)
        setRightLevel(jlimit(0.0f, 1.0f, mouseDownData.right + dy));
    
    owner->updateFromSegments();
}


void EnvelopeComponent::Segment::setYAxisControls(bool leftSide, bool rightSide)
{
    controllingLeft = leftSide;
    controllingRight = rightSide;
}
