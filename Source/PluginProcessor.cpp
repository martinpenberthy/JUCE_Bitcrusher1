/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Bitcrusher1AudioProcessor::Bitcrusher1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, juce::Identifier("PARAMETERS"), createParameterLayout())
#endif
{
    treeState.state.addListener(this);

}

Bitcrusher1AudioProcessor::~Bitcrusher1AudioProcessor()
{    treeState.state.removeListener(this);

}


juce::AudioProcessorValueTreeState::ParameterLayout Bitcrusher1AudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    //Input and Output parameters
    params.push_back(std::make_unique<juce::AudioParameterInt>(juce::ParameterID {"DOWNSAMPLEAMOUNT", 1}, "DownsampleAmount", 1, 30, 1));

    return {params.begin(), params.end()};
}


//==============================================================================
const juce::String Bitcrusher1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Bitcrusher1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Bitcrusher1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Bitcrusher1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Bitcrusher1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Bitcrusher1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Bitcrusher1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Bitcrusher1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Bitcrusher1AudioProcessor::getProgramName (int index)
{
    return {};
}

void Bitcrusher1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Bitcrusher1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Bitcrusher1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Bitcrusher1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Bitcrusher1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
    
    int downsampAmount = *treeState.getRawParameterValue("DOWNSAMPLEAMOUNT");
    if(downsampAmount > 1)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float avg = 0.0f;
            //Move through all samples
            for(int samp = 0; samp < buffer.getNumSamples(); samp++)
            {
                //When downsampAmount samples have been added together
                if(samp % downsampAmount == 0)
                {
                    //Calculate the average
                    avg /= downsampAmount;
                    //Set last downsampAmount samples to the average
                    for(int i = samp - downsampAmount; i < samp; i++)
                        buffer.setSample(channel, i, avg);
                    
                    //Reset avg variable
                    avg = 0.0f;
                }
                else if(samp + downsampAmount > buffer.getNumSamples())
                {
                    for(int i = samp; i < buffer.getNumSamples(); i++)
                        
                }
                else
                    avg += buffer.getSample(channel, samp); //Add sample values
            }//End samp loop
        }
    }
}

//==============================================================================
bool Bitcrusher1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Bitcrusher1AudioProcessor::createEditor()
{
    return new Bitcrusher1AudioProcessorEditor (*this);
}

//==============================================================================
void Bitcrusher1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Bitcrusher1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Bitcrusher1AudioProcessor();
}
