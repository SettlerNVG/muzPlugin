/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EQAudioProcessorEditor::EQAudioProcessorEditor (EQAudioProcessor& p)
    : AudioProcessorEditor (&p), mWaveThumbnail(p), mADSR(p), audioProcessor (p)
{
    //mLoadButton.onClick = [&]() { audioProcessor.loadFile(); }; //UI for button
    auto logoPic = juce::ImageCache::getFromMemory (BinaryData:: Logo_png, BinaryData:: Logo_pngSize) ;
    addAndMakeVisible(mADSR);

    if (!logoPic.isNull()) mImageComponent.setImage(logoPic, juce::RectanglePlacement::stretchToFit);
    else jassert (!logoPic.isNull ());
    
    addAndMakeVisible(mWaveThumbnail);
    addAndMakeVisible(mImageComponent);
    setSize (600, 400); //window size
}

EQAudioProcessorEditor::~EQAudioProcessorEditor()
{
}

//==============================================================================
void EQAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    
    g.setColour(juce::Colours::white);
    
   // p.clear();

   
}

void EQAudioProcessorEditor::resized()
{
    //mLoadButton.setBounds(getWidth()/2-50, getHeight()/2-50, 100, 100);
    
    mWaveThumbnail.setBoundsRelative(0.0f, 0.25f, 1.0f, 0.5);
    mADSR.setBoundsRelative(0.0f, 0.75f, 1.0f, 0.25f);
    mImageComponent.setBoundsRelative (0.0f, 0.0f, 0.25f, 0.25f);
}



