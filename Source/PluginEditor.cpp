/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EQAudioProcessorEditor::EQAudioProcessorEditor (EQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    mLoadButton.onClick = [&]() { audioProcessor.loadFile(); }; //UI for button
    addAndMakeVisible(mLoadButton);
    setSize (200, 200);
}

EQAudioProcessorEditor::~EQAudioProcessorEditor()
{
}

//==============================================================================
void EQAudioProcessorEditor::paint (juce::Graphics& g)
{
    g. fillAll (juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    if (audioProcessor.getNumSamplerSounds() > 0)
    {
        g. fillAll (juce::Colours::red) ;
        g. drawText ("Sound Loaded", getWidth() / 2 - 50, getHeight () / 2 - 10, 100, 20, juce::Justification::centred);
    }
    else
    {
        g.drawText ("Load a Sound", getWidth() / 2 - 50, getHeight () / 2 - 10, 100, 20, juce::Justification::centred);
    }
}

void EQAudioProcessorEditor::resized()
{
    //mLoadButton.setBounds(getWidth()/2-50, getHeight()/2-50, 100, 100);
    
}
bool EQAudioProcessorEditor::isInterestedInFileDrag (const juce::StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains (".wav") || file.contains (".mp3") || file.contains (".aif"))
        {
            return true;
        }
    }
    return false;
}


void EQAudioProcessorEditor::filesDropped (const juce::StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag (file))
        {
            //load a file
            audioProcessor.loadFile (file);
        }
    }
    repaint();
}
