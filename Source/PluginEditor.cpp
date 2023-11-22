/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Bitcrusher1AudioProcessorEditor::Bitcrusher1AudioProcessorEditor (Bitcrusher1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    addAndMakeVisible(sliderDownsampleAmount);
    
    sliderDownsampleAmount.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    
    sliderAttachmentDownsampleAmount = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "DOWNSAMPLEAMOUNT", sliderDownsampleAmount);
}

Bitcrusher1AudioProcessorEditor::~Bitcrusher1AudioProcessorEditor()
{
}

//==============================================================================
void Bitcrusher1AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void Bitcrusher1AudioProcessorEditor::resized()
{
    sliderDownsampleAmount.setBounds(50, getHeight() - 200, 200, 200);
}
