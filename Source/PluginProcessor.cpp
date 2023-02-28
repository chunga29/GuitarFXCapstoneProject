/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Guitarfxcapstone4AudioProcessor::Guitarfxcapstone4AudioProcessor()
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
    state = new AudioProcessorValueTreeState(*this, nullptr);
    state->createAndAddParameter("attack", "Attack", "Attack", NormalisableRange<float>(1.f, 100.f, 0.0001f), 1.0, nullptr, nullptr);
    state->createAndAddParameter("release", "Release", "Release", NormalisableRange<float>(1.f, 300.f, 0.0001f), 1.0, nullptr, nullptr);
    state->createAndAddParameter("thresh", "Threshold", "Threshold", NormalisableRange<float>(0.f, 100.f, 0.0001f), 1.0, nullptr, nullptr);
    state->createAndAddParameter("comp gain", "Comp Gain", "Comp Gain", NormalisableRange<float>(0.f, 10.f, 0.0001f), 1.0, nullptr, nullptr);

    state->createAndAddParameter("drive", "Drive", "Drive", NormalisableRange<float>(0.f, 1.f, 0.0001f), 1.0, nullptr, nullptr);
    state->createAndAddParameter("range", "Range", "Range", NormalisableRange<float>(0.f, 3000.f, 0.0001f), 1.0, nullptr, nullptr);
    state->createAndAddParameter("blend", "Blend", "Blend", NormalisableRange<float>(0.f, 1.f, 0.0001f), 1.0, nullptr, nullptr);
    state->createAndAddParameter("volume", "Volume", "Volume", NormalisableRange<float>(0.f, 3.f, 0.0001f), 1.0, nullptr, nullptr);

    state->state = ValueTree("attack");
    state->state = ValueTree("release");
    state->state = ValueTree("threshold");
    state->state = ValueTree("comp gain");
    
    state->state = ValueTree("drive");
    state->state = ValueTree("range");
    state->state = ValueTree("blend");
    state->state = ValueTree("volume");
}

Guitarfxcapstone4AudioProcessor::~Guitarfxcapstone4AudioProcessor()
{
}

//==============================================================================
const juce::String Guitarfxcapstone4AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Guitarfxcapstone4AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Guitarfxcapstone4AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Guitarfxcapstone4AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Guitarfxcapstone4AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Guitarfxcapstone4AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Guitarfxcapstone4AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Guitarfxcapstone4AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Guitarfxcapstone4AudioProcessor::getProgramName (int index)
{
    return {};
}

void Guitarfxcapstone4AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Guitarfxcapstone4AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;

    if (getTotalNumOutputChannels() > 0) {
        compressor.prepare(spec);
    }
    
}

void Guitarfxcapstone4AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Guitarfxcapstone4AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Guitarfxcapstone4AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto block = dsp::AudioBlock<float>(buffer);
    auto context = dsp::ProcessContextReplacing<float>(block);

    float attack = *state->getRawParameterValue("attack");
    float release = *state->getRawParameterValue("release");
    float threshold = *state->getRawParameterValue("thresh");
    float compgain = *state->getRawParameterValue("comp gain");

    compressor.process(context);
    compressor.setAttack(attack);
    compressor.setRelease(release);
    compressor.setThreshold(threshold * -1.0);
    compressor.setRatio(25.0f);

    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    float drive = *state->getRawParameterValue("drive");
    float range = *state->getRawParameterValue("range");
    float blend = *state->getRawParameterValue("blend");
    float volume = *state->getRawParameterValue("volume");

    // Distortion
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        // buffer carries all of the audio samples
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {

            // Pre-Distortion Signal
            float cleanSig = *channelData; 

            // Range is a drive multiplier
            *channelData *= drive * range;

            // Distortion Formula: (2.f / float_Pi) * atan(*channelData)
            // Signal Forumla: (B% * Distorted + (1 - B)% * Clean) * V
            *channelData = ((((2.f / float_Pi) * atan(*channelData)) * blend) + (cleanSig * (1.f - blend))) * volume;

            channelData++;
        }
    }
}

AudioProcessorValueTreeState& Guitarfxcapstone4AudioProcessor::getState() {
    return *state;
}

//==============================================================================
bool Guitarfxcapstone4AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Guitarfxcapstone4AudioProcessor::createEditor()
{
    return new Guitarfxcapstone4AudioProcessorEditor (*this);
}

//==============================================================================
void Guitarfxcapstone4AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    MemoryOutputStream stream(destData, false);
    state->state.writeToStream(stream);
}

void Guitarfxcapstone4AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    ValueTree tree = ValueTree::readFromData(data, sizeInBytes);

    if (tree.isValid()) {
        state->state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Guitarfxcapstone4AudioProcessor();
}
