/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define X_ID "X_value"
#define X_NAME "X Value"

#define Y_ID "Y_value"
#define Y_NAME "Y Value"

#define Z_ID "Z_value"
#define Z_NAME "Z Value"

#define Scale_ID "scale"
#define Scale_NAME "Scale"

#define PORT_ID "port"
#define PORT_NAME "PORT"

#define SOURCE_ID "souce"
#define SOURCE_NAME "Source"



//==============================================================================
/**
*/
class CreateTrajectoriesAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    CreateTrajectoriesAudioProcessor();
    ~CreateTrajectoriesAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

public:
    juce::AudioProcessorValueTreeState treestate;
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CreateTrajectoriesAudioProcessor)
};
