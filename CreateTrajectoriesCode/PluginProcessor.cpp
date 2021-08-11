/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CreateTrajectoriesAudioProcessor::CreateTrajectoriesAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
treestate(*this, nullptr, "Parameters", {std::make_unique<juce::AudioParameterFloat>(X_ID, X_NAME, -1.0f, 1.0f, 0.0f),
                                        std::make_unique<juce::AudioParameterFloat>(Y_ID, Y_NAME, -1.0f, 1.0f, 0.0f),
                                         std::make_unique<juce::AudioParameterFloat>(Z_ID, Z_NAME, -1.0f, 1.0f, 0.0f),
                                         std::make_unique<juce::AudioParameterFloat>(Scale_ID, Scale_NAME, 1.0f, 20, 0.0f),
                                        std::make_unique<juce::AudioParameterInt>(SOURCE_ID, SOURCE_NAME, 1, 61, 1),
                                        std::make_unique<juce::AudioParameterInt>(PORT_ID, PORT_NAME, 1000, 10000, 9001)
                                        //std::make_unique<juce::AudioProcessorParameterWithID("ipaddress", "IPaddress")
})
#endif
{
}

CreateTrajectoriesAudioProcessor::~CreateTrajectoriesAudioProcessor()
{
}

//==============================================================================
const juce::String CreateTrajectoriesAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CreateTrajectoriesAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CreateTrajectoriesAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CreateTrajectoriesAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CreateTrajectoriesAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CreateTrajectoriesAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CreateTrajectoriesAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CreateTrajectoriesAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CreateTrajectoriesAudioProcessor::getProgramName (int index)
{
    return {};
}

void CreateTrajectoriesAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CreateTrajectoriesAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void CreateTrajectoriesAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CreateTrajectoriesAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CreateTrajectoriesAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool CreateTrajectoriesAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CreateTrajectoriesAudioProcessor::createEditor()
{
    return new CreateTrajectoriesAudioProcessorEditor (*this);
}

//==============================================================================
void CreateTrajectoriesAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = treestate.copyState();
    //auto ipstate = CreateTrajectoriesAudioProcessorEditor.ipaddress.getValue();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    for (int index = 0; index < 261; index++)
       {
           juce::String oscaddress("IPaddress");
           oscaddress += juce::String(index);
           //xml->setAttribute(name, guiTableL[index]);
       }
    
    copyXmlToBinary(*xml, destData);
}

void CreateTrajectoriesAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
       if (xmlState.get() != nullptr)
       {
           if (xmlState->hasTagName(treestate.state.getType()))
           {
               treestate.replaceState(juce::ValueTree::fromXml(*xmlState));
               
           }
       }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CreateTrajectoriesAudioProcessor();
}
