/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveThumbnail.h"
#include "ADSRComponent.h"

//==============================================================================
/**
*/
class EQAudioProcessorEditor  : public juce::AudioProcessorEditor
                                                              
{
public:
    EQAudioProcessorEditor (EQAudioProcessor&);
    ~EQAudioProcessorEditor();
    

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
   
    
    
private:
   
    WaveThumbnail mWaveThumbnail;
    
    ADSRComponent mADSR;
    
    juce::ImageComponent mImageComponent;
    
    EQAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQAudioProcessorEditor)
};
