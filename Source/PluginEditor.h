/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Bitcrusher1AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Bitcrusher1AudioProcessorEditor (Bitcrusher1AudioProcessor&);
    ~Bitcrusher1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider sliderDownsampleAmount;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentDownsampleAmount;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Bitcrusher1AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Bitcrusher1AudioProcessorEditor)
};
