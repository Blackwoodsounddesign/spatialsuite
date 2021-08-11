/*
  ==============================================================================

    xypad.h
    Created: 9 Apr 2021 5:40:42pm
    Author:  Stewart Blackwood

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>

class XYPad  : public juce::AnimatedAppComponent
{
public:

    XYPad();
    //public methods
    int xsliderconversion (float sliderval);
    int ysliderconversion (float sliderval);
    
    //public variables
    float yslidervalue = NULL;
    float xslidervalue = NULL;
    float radius = 20.0f;
    bool clicked = false;
    float returnxsliderval;
    float returnysliderval;

    private:
    void paint (juce::Graphics& g) override;
    void update() override;
    void mouseDrag (const juce::MouseEvent& event) override;
    void mouseDown (const juce::MouseEvent& event) override;
    void mouseUp (const juce::MouseEvent& event) override;
    void mouseDoubleClick (const juce::MouseEvent& event) override;
    
    int x = NULL;
    int y = NULL;
    juce::Point<int> k;
    juce::Point<float> p;
    bool started = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XYPad)
};
