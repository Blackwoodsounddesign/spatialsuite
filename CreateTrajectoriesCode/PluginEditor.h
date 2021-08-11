/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "xypad.h"
//#include "CustomTextEditor.h"

//==============================================================================
/**
*/

class CustomlookandFeel : public juce::LookAndFeel_V4
{
    public :
    ~CustomlookandFeel() override;
    CustomlookandFeel()
    {
        setColour(juce::Slider::trackColourId, juce::Colours::lightgrey);
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    }


};

class CreateTrajectoriesAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    CreateTrajectoriesAudioProcessorEditor (CreateTrajectoriesAudioProcessor&);
    ~CreateTrajectoriesAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void showConnectionErrorMessage (const juce::String& messageText)
        {
            juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::WarningIcon,
                                                    "Connection Error",
                                                    messageText,
                                                    "OK");
            opened = true;
        }
    
    void createOSCMessage(juce::String OSClabel)
    {
        float scaleNB = scalenumberBox.getValue();
        
        if (! sender.send (OSClabel,
                           (int) SLnumberBox.getValue(),
                           (float) xaxisslider.getValue() * scaleNB,
                           (float) yaxisslider.getValue() * scaleNB,
                           (float) zaxisslider.getValue() * scaleNB) && opened == false)
            showConnectionErrorMessage ("Check your IP address and port number.");
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CreateTrajectoriesAudioProcessor& audioProcessor;
    CustomlookandFeel otherlookandfeel;
    
    juce::Slider yaxisslider;
    juce::Slider xaxisslider;
    juce::Slider zaxisslider;
    juce::Slider SLnumberBox;
    juce::Slider scalenumberBox;
    juce::Slider port;
    juce::OSCSender sender;
    XYPad trajectorypad;

public:
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> xslidervalue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> yslidervalue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> zslidervalue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> scalenumberBoxvalue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> portvalue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> SLnumberBoxValue;
    //std::unique_ptr<juce::AudioProcessorValueTreeState::Parameter> ipaddressvalue;
    
    bool opened = false;
    juce::TextEditor ipaddress;
    //juce::TextPropertyComponent ipaddressv2(juce::String,"steve",12,false,true);
    juce::String testString;
    
    /*
     juce::Identifier nonparameterstring (nodes);
    juce::ValueTree nodes (nonparameterstring);
    nonparameterstring.setProperty (nodesstringID, "Test", nullptr);
     */
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CreateTrajectoriesAudioProcessorEditor)
};
