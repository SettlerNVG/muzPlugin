/*
  ==============================================================================

    WaveThumbnail.cpp
    Created: 25 Apr 2023 5:56:03pm
    Author:  Nikitoooooozy)

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveThumbnail.h"

//==============================================================================
WaveThumbnail::WaveThumbnail(EQAudioProcessor& p) : audioProcessor(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

WaveThumbnail::~WaveThumbnail()
{
}

void WaveThumbnail::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::blue.brighter());
    
    auto waveForm = audioProcessor.getWaveForm();

    if(waveForm.getNumSamples()>0)
    {
        
        juce::Path p;
        mAudioPoints.clear();
        auto ratio = waveForm.getNumSamples() / getWidth();
        auto buffer = waveForm.getReadPointer(0);
        //scale audio file to win on x axis
        
        for (int sample = 0; sample<waveForm.getNumSamples();sample+=ratio)
        {
            mAudioPoints.push_back(buffer[sample]);
        }
        
        p.startNewSubPath(0, getHeight() / 2);
        
        //scale on y axis
        
        for (int sample = 0; sample<mAudioPoints.size();++sample)
        {
            auto point = juce::jmap<float>(mAudioPoints[sample], -1.0f, 1.0f, getHeight(), 0);
            
            p.lineTo(sample, point);
        }
        g.setColour(juce::Colours::white);
        
        g.strokePath(p, juce::PathStrokeType (2));
        
        g.setColour(juce::Colours::white.darker());
        g.setFont(15.0f);
        
        auto textBounds = getLocalBounds().reduced(10, 10);
        
        g.drawFittedText(mFileName, textBounds, juce::Justification::topRight, 1);
    }
    else
    {
        g.setColour(juce::Colours::white);
        g.setFont(40.0f);
        g.drawFittedText("Drop an Audio File to load", getLocalBounds(), juce::Justification::centred, 1);
    }
    //new file dropped?
        //if yes
    
            //get the waveform from the processor
            // sample = 44100 (1sec) ... x axis of window = 600 == sampleLength / getWidth() // cant displaey so many pixels\
            // the same for y axis for 200 pixels
            // user the ratio to tale values of audio buffer and put in vector to display
            //draw the waveform
            
//    g.setColour (juce::Colours::white);
//    g.setFont (15.0f);
//    if (audioProcessor.getNumSamplerSounds() > 0)
//    {
//        g. fillAll (juce::Colours::red) ;
//        g. drawText ("Sound Loaded", getWidth() / 2 - 50, getHeight () / 2 - 10, 100, 20, juce::Justification::centred);
//    }
//    else
//    {
//        g.drawText ("Load a Sound", getWidth() / 2 - 50, getHeight () / 2 - 10, 100, 20, juce::Justification::centred);
//    }

//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
//
//    g.setColour (juce::Colours::grey);
//    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
//
//    g.setColour (juce::Colours::white);
//    g.setFont (14.0f);
//    g.drawText ("WaveThumbnail", getLocalBounds(),
//                juce::Justification::centred, true);   // draw some placeholder text
}

void WaveThumbnail::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

bool WaveThumbnail::isInterestedInFileDrag (const juce::StringArray& files)
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


void WaveThumbnail::filesDropped (const juce::StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag (file))
        {
            
            auto myFile = std::make_unique<juce::File>(file);
            
            mFileName = myFile->getFileNameWithoutExtension();
            //load a file
            audioProcessor.loadFile (file);
        }
    }
    repaint();
}
