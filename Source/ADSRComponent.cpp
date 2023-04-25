/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 25 Apr 2023 6:48:09pm
    Author:  Nikitoooooozy)

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"

//==============================================================================
ADSRComponent::ADSRComponent(EQAudioProcessor& p) : audioProcessor(p)
{
    //Attack sliders
    
    mAttackSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mAttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mAttackSlider.setRange(0.0f, 5.0f,0.01f);
    mAttackSlider.addListener(this);
    addAndMakeVisible(mAttackSlider);
    
    mAttackLabel.setFont(10.0f);
    mAttackLabel.setText("Attack", juce::NotificationType::dontSendNotification);
    mAttackLabel.setJustificationType(juce::Justification::centredTop);
    mAttackLabel.attachToComponent(&mAttackSlider, false);
    
    //Decay sliders

    mDecaySlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mDecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mDecaySlider.setRange(0.0f, 5.0f,0.01f);
    mDecaySlider.addListener(this);
    addAndMakeVisible(mDecaySlider);
    
    mDecayLabel.setFont(10.0f);
    mDecayLabel.setText("Decay", juce::NotificationType::dontSendNotification);
    mDecayLabel.setJustificationType(juce::Justification::centredTop);
    mDecayLabel.attachToComponent(&mDecaySlider, false);
    
    //Sustain sliders

    mSustainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mSustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mSustainSlider.setRange(0.0f, 1.0f,0.01f);
    mSustainSlider.addListener(this);
    addAndMakeVisible(mSustainSlider);
    
    mSustainLabel.setFont(10.0f);
    mSustainLabel.setText("Sustain", juce::NotificationType::dontSendNotification);
    mSustainLabel.setJustificationType(juce::Justification::centredTop);
    mSustainLabel.attachToComponent(&mSustainSlider, false);
    
    //Release sliders

    mReleaseSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mReleaseSlider.setRange(0.0f, 5.0f,0.01f);
    mReleaseSlider.addListener(this);
    addAndMakeVisible(mReleaseSlider);
    
    mReleaseLabel.setFont(10.0f);
    mReleaseLabel.setText("Release", juce::NotificationType::dontSendNotification);
    mReleaseLabel.setJustificationType(juce::Justification::centredTop);
    mReleaseLabel.attachToComponent(&mReleaseSlider, false);


}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint (juce::Graphics& g)
{

    g.fillAll(juce::Colours::black);
}

void ADSRComponent::resized()
{
    const auto startX = 0.6f;
    const auto startY = 0.2f;
    const auto sliderWidth = 0.1f;
    const auto sliderHeight = 0.75f;

    mAttackSlider.setBoundsRelative(startX, startY, sliderWidth, sliderHeight);
    mDecaySlider.setBoundsRelative(startX + sliderWidth, startY, sliderWidth, sliderHeight);
    mSustainSlider.setBoundsRelative(startX + (2*sliderWidth), startY, sliderWidth, sliderHeight);
    mReleaseSlider.setBoundsRelative(startX + (3*sliderWidth), startY, sliderWidth, sliderHeight);

}

void ADSRComponent::sliderValueChanged(juce::Slider* slider)
{
    //& uses to take a memory
    if(slider == &mAttackSlider)
    {
        audioProcessor.getADSRParams().attack = mAttackSlider.getValue();
    }
    else if (slider == &mDecaySlider)
    {
        audioProcessor.getADSRParams().decay = mDecaySlider.getValue();

    }
    else if (slider == &mSustainSlider)
    {
        audioProcessor.getADSRParams().sustain = mSustainSlider.getValue();

    }
    else if (slider == &mReleaseSlider)
    {
        audioProcessor.getADSRParams().release = mReleaseSlider.getValue();

    }
    audioProcessor.updateADSR();
}
