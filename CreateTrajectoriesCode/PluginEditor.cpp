/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <string>

//==============================================================================
CreateTrajectoriesAudioProcessorEditor::CreateTrajectoriesAudioProcessorEditor (CreateTrajectoriesAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 400);
    setResizable(true, true);
    setResizeLimits(350, 350, 10000, 10000);
    setScaleFactor(1.25);
    ipaddress.setFont (10.0f);
    ipaddress.setText("127.0.0.1");
    xaxisslider.setValue(0.0f);
    //ipaddressv2.TextPropertyComponent(testString,"steve",12,false,true);
    //p.treestate.state.addChild(nodes, 0, nullptr);
}

CreateTrajectoriesAudioProcessorEditor::~CreateTrajectoriesAudioProcessorEditor()
{
    
}

CustomlookandFeel::~CustomlookandFeel()
{
    
}

//==============================================================================
void CreateTrajectoriesAudioProcessorEditor::paint (juce::Graphics& g)
{
    xslidervalue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treestate, X_ID, xaxisslider);
    yslidervalue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treestate, Y_ID, yaxisslider);
    zslidervalue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treestate, Z_ID, zaxisslider);
    scalenumberBoxvalue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treestate, Scale_ID, scalenumberBox);
    SLnumberBoxValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treestate, SOURCE_ID, SLnumberBox);
    portvalue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treestate, PORT_ID, port);
    
    
    
    //audioProcessor.treestate.createAndAddParameter(std::unique_ptr<RangedAudioParameter> parameter);
    //ipaddressvalue = std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(audioProcessor.treestate,"ipaddress", ipaddress);
    //textvalue = std::make_unique<juce::AudioProcessorValueTreeState::text>(audioProcessor.treestate, PORT_ID, port);
    
    int w = getWidth();
    int h = getHeight();
    
    g.fillAll (juce::Colours::snow);
    g.setFont (15.0f);
    g.drawRect(10, 10, (getWidth() - 20), (getHeight() -20)); 
    g.drawRect(w/3, h/6, (1.5*w/4), (h/2), 5);
    
    //Y Axis slider
    yaxisslider.setSliderStyle(juce::Slider::LinearVertical);
    yaxisslider.setRange(-1.0f, 1.0f, 0.01f);
    yaxisslider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible(yaxisslider);
    yaxisslider.setTooltip("y-axis control");
    yaxisslider.setLookAndFeel(&otherlookandfeel);
    if(trajectorypad.clicked)
    yaxisslider.setValue(trajectorypad.yslidervalue);
    
    //X Axis slider
    xaxisslider.setSliderStyle(juce::Slider::LinearHorizontal);
    xaxisslider.setRange(-1.0f, 1.0f, 0.01f);
    addAndMakeVisible(xaxisslider);
    xaxisslider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 40);
    xaxisslider.setTooltip("x-axis control");
    xaxisslider.setLookAndFeel(&otherlookandfeel);
    if(trajectorypad.clicked) 
    xaxisslider.setValue(trajectorypad.xslidervalue);
    
    //z Axis slider
    zaxisslider.setSliderStyle(juce::Slider::LinearVertical);
    zaxisslider.setRange(-1.0f, 1.0f, 0.01f);
    addAndMakeVisible(zaxisslider); 
    zaxisslider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    zaxisslider.setTooltip("z-axis control");
    zaxisslider.setLookAndFeel(&otherlookandfeel);
    
    //SLnumber box
    SLnumberBox.setSliderStyle(juce::Slider::LinearBarVertical);
    SLnumberBox.setRange(1, 64, 1);
    SLnumberBox.setSliderSnapsToMousePosition(false);
    SLnumberBox.setTextBoxStyle(juce::Slider::TextBoxRight, false, 40, 20);
    SLnumberBox.setColour(1, juce::Colours::transparentBlack);
    addAndMakeVisible(SLnumberBox);
    SLnumberBox.setSize(w * .1, 20);
    SLnumberBox.setLookAndFeel(&otherlookandfeel);
    SLnumberBox.setTooltip("Label");
    
    //Scale Numberbox
    scalenumberBox.setSliderStyle(juce::Slider::LinearBarVertical);
    scalenumberBox.setRange(1, 20, 0.1);
    scalenumberBox.setSliderSnapsToMousePosition(false);
    scalenumberBox.setTextBoxStyle(juce::Slider::TextBoxRight, false, 40, 20);
    scalenumberBox.setColour(1, juce::Colours::transparentBlack);
    addAndMakeVisible(scalenumberBox);
    scalenumberBox.setSize(w * .1, 20);
    scalenumberBox.setLookAndFeel(&otherlookandfeel);
    scalenumberBox.setTooltip("Label");
    
    //IP Address default 127.0.0.1
    addAndMakeVisible(ipaddress);
    ipaddress.setSize(w*.18, 25);
    ipaddress.setInputRestrictions(13);
    ipaddress.setFont(9.0f);
    
    //addAndMakeVisible(ipaddressv2);
    //ipaddressv2.setSize(w*.18, 25);
    //ipaddressv2.setInputRestrictions(13);
    //ipaddressv2.setFont(9.0f);
    
    //port
    addAndMakeVisible(port);
    port.setSize(w * .13, 20);
    port.setSliderStyle(juce::Slider::LinearBarVertical);
    port.setRange(1000, 10000, 1);
    port.setTextBoxStyle(juce::Slider::TextBoxRight, false, 40, 20);
    port.setSliderSnapsToMousePosition(false);
    port.setColour(1, juce::Colours::transparentBlack);
    port.setLookAndFeel(&otherlookandfeel);
    
    //custom xypad
    addAndMakeVisible(trajectorypad);
    trajectorypad.setSize(getWidth()/2, getHeight()/2);
    trajectorypad.radius = 20.0f + (zaxisslider.getValue() * 4);
    trajectorypad.returnxsliderval = xaxisslider.getValue();
    trajectorypad.returnysliderval = yaxisslider.getValue();
    
    //OSC setup
    if (! sender.connect (ipaddress.getText(), port.getValue()) && opened == false){
        showConnectionErrorMessage ("Error: could not connect to UDP port 9001.");
        opened = true;
    }
    
    juce::String OSClabel = "/XYZ/";
    
    //OSC on any change
    zaxisslider.onValueChange = [this, OSClabel] {createOSCMessage(OSClabel); };
    yaxisslider.onValueChange = [this, OSClabel] {createOSCMessage(OSClabel); };
    xaxisslider.onValueChange = [this, OSClabel] {createOSCMessage(OSClabel); };
    scalenumberBox.onValueChange = [this, OSClabel] {createOSCMessage(OSClabel); }; 
    
    //text
    g.setFont(w/19);
    g.drawText("Spatial Controller", w/3.5, h * .03, w/2, h/8, juce::Justification::centred, true);
    g.setFont(w/27);
    g.drawText("X Axis", w/4, h * .8, w/2, h/8, juce::Justification::centred, true);
    g.drawText("Y Axis", w/500, h * .87, w/2, h/8, juce::Justification::centred, true);
    g.drawText("Z Axis", w/1.55, h * .87, w/2, h/8, juce::Justification::centred, true);
    g.drawText("Label #", -1 * w/7.6, h * .51, w/2, h/8, juce::Justification::centred, true);
    g.drawText("Scale", -1 * w/7.2, h * .405, w/2, h/8, juce::Justification::centred, true);
    g.drawText("Port", -1 * w/7.2, h * .255, w/2, h/8, juce::Justification::centred, true);
    g.drawText("IP address", -1 * w/8, h * .15, w/2, h/8, juce::Justification::centred, true);
}

void CreateTrajectoriesAudioProcessorEditor::resized()
{
    int w = getWidth();
    int h = getHeight();
    
    
    SLnumberBox.setBounds(w * .06f, h * 0.5f, 35, 40);
    scalenumberBox.setBounds(w * .06f, h * 0.4f, 35, 40);
    ipaddress.setBounds(w * 0.04f, h * 0.13f, w * 0.075, 20);
    //ipaddressv2.setBounds(w * 0.04f, h * 0.13f, w * 0.075, 200);
    port.setBounds(w * 0.055f, h * 0.25f, w * 0.2, 20);
    
    yaxisslider.setBounds(w * 0.2f, h * 0.1f, w/10, h * 0.8f);
    zaxisslider.setBounds(w * 0.84f, h * 0.1f, w/10, h * 0.8f);
    xaxisslider.setBounds(w * 0.25f, h * 0.75f, w /2, 30);
    trajectorypad.setBounds(w/3.5, h/6, (w/4), (h/2));
    
}
