/*
  ==============================================================================

    ADSRComponent.h
    Created: 25 Apr 2023 6:48:09pm
    Author:  Nikitoooooozy)

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/*
*/
class ADSRComponent  : public juce::Component,
                       public juce::Slider::Listener //abstact class
{
public:
    ADSRComponent(EQAudioProcessor& p);
    ~ADSRComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* slider) override;


private:
    
    juce::Slider mAttackSlider, mDecaySlider, mSustainSlider, mReleaseSlider;
    
    juce::Label mAttackLabel, mDecayLabel, mSustainLabel, mReleaseLabel;
    
    EQAudioProcessor& audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
