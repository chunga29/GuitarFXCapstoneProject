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
    state->createAndAddParameter("attack", "Attack", "Attack", NormalisableRange<float>(1.f, 200.f, 0.0001f), 10.0, nullptr, nullptr);
    state->createAndAddParameter("release", "Release", "Release", NormalisableRange<float>(1.f, 500.f, 0.0001f), 100.0, nullptr, nullptr);
    state->createAndAddParameter("thresh", "Threshold", "Threshold", NormalisableRange<float>(0.f, 50.f, 0.0001f), 0.0, nullptr, nullptr);
    state->createAndAddParameter("comp gain", "Comp Gain", "Comp Gain", NormalisableRange<float>(0.f, 15.f, 0.0001f), 0.0, nullptr, nullptr);

    state->createAndAddParameter("drive", "Drive", "Drive", NormalisableRange<float>(0.0001f, 1.f, 0.0001f), 0.0001f, nullptr, nullptr);
    state->createAndAddParameter("range", "Range", "Range", NormalisableRange<float>(0.0001f, 3000.f, 0.0001f), 0.0001f, nullptr, nullptr);
    state->createAndAddParameter("blend", "Blend", "Blend", NormalisableRange<float>(0.f, 1.f, 0.0001f), 0.0, nullptr, nullptr);
    state->createAndAddParameter("volume", "Volume", "Volume", NormalisableRange<float>(0.f, 7.f, 0.0001f), 3.0, nullptr, nullptr);

    state->createAndAddParameter("delay feedback", "Delay Feedback", "Delay Feedback", NormalisableRange<float>(0.f, 1.f, 0.0001f), 0.0, nullptr, nullptr);
    state->createAndAddParameter("delay time", "Delay Time", "Delay Time", NormalisableRange<float>(0.05f, 500.f, 0.0001f), 60.0, nullptr, nullptr);
    state->createAndAddParameter("delay volume", "Delay Volume", "Delay Volume", NormalisableRange<float>(0.f, 1.f, 0.0001f), 0.0, nullptr, nullptr);

    state->createAndAddParameter("eq low", "EQ Low", "EQ Low", NormalisableRange<float>(0.05f, 4.f, 0.1f, 1.0f), 1.0, nullptr, nullptr);
    state->createAndAddParameter("eq mid", "EQ Mid", "EQ Mid", NormalisableRange<float>(0.05f, 4.f, 0.1f, 1.0f), 1.0, nullptr, nullptr);
    state->createAndAddParameter("eq high", "EQ High", "EQ High", NormalisableRange<float>(0.05f, 4.f, 0.1f, 1.0f), 1.0, nullptr, nullptr);

    state->state = ValueTree("attack");
    state->state = ValueTree("release");
    state->state = ValueTree("threshold");
    state->state = ValueTree("comp gain");
    
    state->state = ValueTree("drive");
    state->state = ValueTree("range");
    state->state = ValueTree("blend");
    state->state = ValueTree("volume");

    state->state = ValueTree("delay feedback");
    state->state = ValueTree("delay time");
    state->state = ValueTree("delay volume");

    state->state = ValueTree("eq low");
    state->state = ValueTree("eq mid");
    state->state = ValueTree("eq high");
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
    dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;

    if (getTotalNumOutputChannels() > 0) {
        compressor.prepare(spec);
    }

    const int numInputChannels = getNumInputChannels();
    const int delayBufferSize = 2 * (sampleRate * samplesPerBlock); // saves two seconds of past data
    
    mDelayBuffer.setSize(numInputChannels, delayBufferSize);
    mSampleRate = sampleRate;

    drySignal.setSize(1, spec.maximumBlockSize);
    wetBuffer.setSize(1, spec.maximumBlockSize);

    // EQ
    spec.numChannels = 1;
    leftChain.prepare(spec);
    rightChain.prepare(spec);
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

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto block = dsp::AudioBlock<float>(buffer);
    auto context = dsp::ProcessContextReplacing<float>(block);

    // Noise Gating


    // Compressor
    float attack = *state->getRawParameterValue("attack");
    float release = *state->getRawParameterValue("release");
    float threshold = *state->getRawParameterValue("thresh");
    float compgain = *state->getRawParameterValue("comp gain");

    compressor.setAttack(attack);
    compressor.setRelease(release);
    compressor.setThreshold(threshold * -1.0);
    compressor.setRatio(25.0f);
    compressor.process(context);

    // Distortion Variables
    float drive = *state->getRawParameterValue("drive");
    float range = *state->getRawParameterValue("range");
    float blend = *state->getRawParameterValue("blend");
    float volume = *state->getRawParameterValue("volume");

    // Delay Variables
    float delayFbk = *state->getRawParameterValue("delay feedback");
    float delayTme = *state->getRawParameterValue("delay time");
    float delayVol = *state->getRawParameterValue("delay volume");

    const int bufferLength = buffer.getNumSamples();
    const int delayBufferLength = mDelayBuffer.getNumSamples();

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        // buffer carries all of the audio samples
        auto* channelData = buffer.getWritePointer (channel);

        // Distortion
        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
            // Pre-Distortion Signal
            float cleanSig = *channelData;

            // Range is a drive multiplier
            *channelData *= drive * range;

            // Distortion Formula: (2.f / float_Pi) * atan(*channelData)
            // (Filtering) (Wah Pedal)
            // Signal Forumla: (B% * Distorted + (1 - B)% * Clean) * V
            *channelData = ((((2.f / float_Pi) * atan(*channelData)) * blend) + (cleanSig * (1.f - blend))) * volume;

            channelData++;
        }

        // Delay
        drySignal.makeCopyOf(buffer, true);
        const float* bufferData = buffer.getReadPointer(channel);   // dry buffer
        const float* delayBufferData = mDelayBuffer.getReadPointer(channel); // delay buffer
        const float* wetBufferData = wetBuffer.getReadPointer(channel);
        float* dryBuffer = buffer.getWritePointer(channel);

        // Write Dry Buffer into Delay Buffer
        fillDelayBuffer(channel, bufferLength, delayBufferLength, bufferData, delayBufferData, delayVol);

        // Write Delay Buffer into Wet Buffer
        getFromDelayBuffer(buffer, channel, bufferLength, delayBufferLength, bufferData, delayBufferData, wetBufferData, delayTme);

        // Add Feedback from Wet Buffer to Delay Buffer
        feedbackDelay(channel, bufferLength, delayBufferLength, wetBufferData, delayFbk);
    }

    mWritePosition += bufferLength;
    mWritePosition %= delayBufferLength;


    // EQ (Equalizer)
    float lowGain = *state->getRawParameterValue("eq low");
    float midGain = *state->getRawParameterValue("eq mid");
    float highGain = *state->getRawParameterValue("eq high");

    auto leftBlock = block.getSingleChannelBlock(0);
    // auto rightBlock = block.getSingleChannelBlock(1);

    dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    // dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    leftChain.process(leftContext);
    // rightChain.process(rightContext);
    
    auto midCoefficients = dsp::IIR::Coefficients<float>::makePeakFilter(mSampleRate, 1150.0f, 1.0, midGain);
    auto lowCoefficients = dsp::IIR::Coefficients<float>::makeLowShelf(mSampleRate, 300.0f, 1.0f, lowGain);
    auto highCoefficients = dsp::IIR::Coefficients<float>::makeHighShelf(mSampleRate, 2000.0f, 1.0f, highGain);
    leftChain.get<ChainPositions::Peak>().coefficients = *midCoefficients;
    leftChain.get<ChainPositions::LowShelf>().coefficients = *lowCoefficients;
    leftChain.get<ChainPositions::HighShelf>().coefficients = *highCoefficients;
    // rightChain.get<ChainPositions::Peak>().coefficients = *midCoefficients;
}

void Guitarfxcapstone4AudioProcessor::fillDelayBuffer(int channel, const int bufferLength, const int delayBufferLength, 
                                                        const float* bufferData, const float* delayBufferData, const float gain) {

    if (delayBufferLength >= bufferLength + mWritePosition) {
        mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferLength, gain, gain);
    }
    else {
        const int bufferRemaining = delayBufferLength - mWritePosition;
        mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferRemaining, gain, gain);
        mDelayBuffer.copyFromWithRamp(channel, 0, bufferData, bufferLength - bufferRemaining, gain, gain);
    }
}

void Guitarfxcapstone4AudioProcessor::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int bufferLength,
    const int delayBufferLength, const float* bufferData, const float* delayBufferData, const float* wetBufferData, int delayTime) {

    const int readPosition = static_cast<int>(delayBufferLength + mWritePosition - (mSampleRate * delayTime / 1000)) % delayBufferLength;

    if (delayBufferLength >= bufferLength + readPosition) {
        wetBuffer.copyFrom(channel, 0, delayBufferData + readPosition, bufferLength);
    }
    else {
        const int bufferRemaining = delayBufferLength - readPosition;
        wetBuffer.copyFrom(channel, 0, delayBufferData + readPosition, bufferRemaining);
        wetBuffer.copyFrom(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining);
    }

    buffer.addFrom(channel, 0, wetBufferData, bufferLength);
}

void Guitarfxcapstone4AudioProcessor::feedbackDelay(int channel, const int bufferLength,
        const int delayBufferLength, const float* dryBuffer, float fdbk_amt) {

    if (delayBufferLength > bufferLength + mWritePosition) {
        mDelayBuffer.addFromWithRamp(channel, mWritePosition, dryBuffer, bufferLength, fdbk_amt, fdbk_amt);
    }
    else {
        const int bufferRemaining = delayBufferLength - mWritePosition;
        mDelayBuffer.addFromWithRamp(channel, bufferRemaining, dryBuffer, bufferRemaining, fdbk_amt, fdbk_amt);
        mDelayBuffer.addFromWithRamp(channel, 0, dryBuffer, bufferLength - bufferRemaining, fdbk_amt, fdbk_amt);
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

EQChainSettings getEQChainSettings(juce::AudioProcessorValueTreeState& apvts) {
    EQChainSettings settings;
    return settings;
}
