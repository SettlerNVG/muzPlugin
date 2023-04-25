/*
  ==============================================================================

    WaveThumbnail.h
    Created: 25 Apr 2023 5:56:03pm
    Author:  Nikitoooooozy)

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/*
*/
class WaveThumbnail  : public juce::Component,
                       public juce::FileDragAndDropTarget
{
public:
    WaveThumbnail(EQAudioProcessor& p);
    ~WaveThumbnail() override;

    bool isInterestedInFileDrag (const juce::StringArray& files) override;

    void filesDropped (const juce::StringArray& files, int x, int y) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    std::vector<float> mAudioPoints;
    bool mShouldBePainting {false};
    
    juce::String mFileName{""};
    
    EQAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveThumbnail)
};
