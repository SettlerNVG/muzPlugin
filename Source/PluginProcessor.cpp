/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
EQAudioProcessor::EQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    mFormatManager.registerBasicFormats();
    for (int i = 0; i<mNumVoices;i++)
    {
        mSampler.addVoice(new juce::SamplerVoice());
    }
}

EQAudioProcessor::~EQAudioProcessor()
{
    mFormatReader = nullptr;
}

//==============================================================================
const juce::String EQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String EQAudioProcessor::getProgramName (int index)
{
    return {};
}

void EQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void EQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampler.setCurrentPlaybackSampleRate(sampleRate);
    
    updateADSR();
}

void EQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void EQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

}

//==============================================================================
bool EQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* EQAudioProcessor::createEditor()
{
    return new EQAudioProcessorEditor (*this);
}

//==============================================================================
void EQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void EQAudioProcessor::loadFile()
{
    mSampler.clearSounds () ;

    juce::FileChooser chooser {"Please load file"};
    if(chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        mFormatReader = mFormatManager.createReaderFor(file); // how we get file
    }
    juce::BigInteger range;
    range.setRange(0, 128, true);
    
    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1, 0.1,10));
}

void EQAudioProcessor::loadFile (const juce::String& path)
{
    mSampler.clearSounds () ;
    auto file = juce::File (path);
    mFormatReader = mFormatManager. createReaderFor (file);
    
    auto sampleLength = static_cast<int>(mFormatReader->lengthInSamples);
    
    mWaveForm.setSize(1, sampleLength);
    mFormatReader -> read(&mWaveForm, 0,sampleLength, 0, true, false); //waveform has data

    //auto buffer = mWaveForm.getReadPointer(0);
    
    
    juce::BigInteger range;
    range. setRange (0, 128, true);

    mSampler.addSound (new juce::SamplerSound ("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10.0));
}

void EQAudioProcessor::updateADSR()
{
    for (int i = 0; i<mSampler.getNumSounds();i++)
    {
        if(auto sound = dynamic_cast<juce::SamplerSound*>(mSampler.getSound(i).get()))//gets a sound of class from synthesizer of local class samplerSound == Synthesizer
        {
            sound->setEnvelopeParameters(mADSRParams);
        }
    }}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EQAudioProcessor();
}
