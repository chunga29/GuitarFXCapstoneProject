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
    state->createAndAddParameter("bypass all", "Bypass All", "Bypass All", NormalisableRange<float>(0.f, 1.f, 1.f), 0.0, nullptr, nullptr);

    state->createAndAddParameter("attack", "Attack", "Attack", NormalisableRange<float>(1.f, 200.f, 0.0001f), 10.0, nullptr, nullptr);
    state->createAndAddParameter("release", "Release", "Release", NormalisableRange<float>(1.f, 500.f, 0.0001f), 100.0, nullptr, nullptr);
    state->createAndAddParameter("thresh", "Threshold", "Threshold", NormalisableRange<float>(0.f, 50.f, 0.0001f), 0.0, nullptr, nullptr);
    state->createAndAddParameter("ratio", "Ratio", "Ratio", NormalisableRange<float>(1.f, 10.f, 1.f), 2.0, nullptr, nullptr);
    state->createAndAddParameter("comp gain", "Comp Gain", "Comp Gain", NormalisableRange<float>(0.f, 15.f, 0.0001f), 0.0, nullptr, nullptr);

    state->createAndAddParameter("drive", "Drive", "Drive", NormalisableRange<float>(0.0001f, 1.f, 0.0001f), 0.0001f, nullptr, nullptr);
    state->createAndAddParameter("range", "Range", "Range", NormalisableRange<float>(0.0001f, 1000.f, 0.0001f), 0.0001f, nullptr, nullptr);
    state->createAndAddParameter("blend", "Blend", "Blend", NormalisableRange<float>(0.f, 1.f, 0.0001f), 0.0, nullptr, nullptr);
    state->createAndAddParameter("volume", "Volume", "Volume", NormalisableRange<float>(0.01f, 1.f, 0.01f), 1.0, nullptr, nullptr);

    state->createAndAddParameter("delay feedback", "Delay Feedback", "Delay Feedback", NormalisableRange<float>(0.f, 1.f, 0.0001f), 0.0, nullptr, nullptr);
    state->createAndAddParameter("delay time", "Delay Time", "Delay Time", NormalisableRange<float>(0.05f, 500.f, 0.0001f), 60.0, nullptr, nullptr);
    state->createAndAddParameter("delay volume", "Delay Volume", "Delay Volume", NormalisableRange<float>(0.f, 1.f, 0.0001f), 0.0, nullptr, nullptr);

    state->createAndAddParameter("reverb size", "Reverb Size", "Reverb Size", NormalisableRange<float>(0.f, 2000.f, 5.0f), 1000.0, nullptr, nullptr);
    state->createAndAddParameter("reverb decay", "Reverb Decay", "Reverb Decay", NormalisableRange<float>(0.f, 0.25f, 0.01f), 0.0, nullptr, nullptr);
    state->createAndAddParameter("reverb mix", "Reverb Mix", "Reverb Mix", NormalisableRange<float>(0.0f, 1.f, 0.01f), 0.5, nullptr, nullptr);

    state->createAndAddParameter("eq low", "EQ Low", "EQ Low", NormalisableRange<float>(0.1f, 2.f, 0.1f, 1.0f), 1.0, nullptr, nullptr);
    state->createAndAddParameter("eq mid", "EQ Mid", "EQ Mid", NormalisableRange<float>(0.1f, 2.f, 0.1f, 1.0f), 1.0, nullptr, nullptr);
    state->createAndAddParameter("eq high", "EQ High", "EQ High", NormalisableRange<float>(0.1f, 2.f, 0.1f, 1.0f), 1.0, nullptr, nullptr);

    state->createAndAddParameter("bypass comp", "Bypass Comp", "Bypass Comp", NormalisableRange<float>(0.f, 1.f, 1.f), 0.0, nullptr, nullptr);
    state->createAndAddParameter("bypass dist", "Bypass Dist", "Bypass Dist", NormalisableRange<float>(0.f, 1.f, 1.f), 0.0, nullptr, nullptr);
    state->createAndAddParameter("bypass delay", "Bypass Delay", "Bypass Delay", NormalisableRange<float>(0.f, 1.f, 1.f), 0.0, nullptr, nullptr);
    state->createAndAddParameter("bypass reverb", "Bypass Reverb", "Bypass Reverb", NormalisableRange<float>(0.f, 1.f, 1.f), 0.0, nullptr, nullptr);
    state->createAndAddParameter("bypass eq", "Bypass EQ", "Bypass EQ", NormalisableRange<float>(0.f, 1.f, 1.f), 0.0, nullptr, nullptr);

    state->createAndAddParameter("cabinet menu", "Cabinet Menu", "Cabinet Menu", NormalisableRange<float>(1.f, 3.f, 1.f), 1.0, nullptr, nullptr);

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

    state->state = ValueTree("reverb size");
    state->state = ValueTree("reverb decay");
    state->state = ValueTree("reverb mix");

    state->state = ValueTree("eq low");
    state->state = ValueTree("eq mid");
    state->state = ValueTree("eq high");

    state->state = ValueTree("bypass all");
    state->state = ValueTree("bypass comp");
    state->state = ValueTree("bypass dist");
    state->state = ValueTree("bypass delay");
    state->state = ValueTree("bypass reverb");
    state->state = ValueTree("bypass eq");

    state->state = ValueTree("cabinet menu");
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
        noiseGate.prepare(spec);
    }

    const int numInputChannels = getNumInputChannels();
    const int delayBufferSize = 2 * (sampleRate * samplesPerBlock); // saves two seconds of past data
    
    mDelayBuffer.setSize(numInputChannels, delayBufferSize);
    mSampleRate = sampleRate;

    drySignal.setSize(1, spec.maximumBlockSize);
    wetBuffer.setSize(1, spec.maximumBlockSize);

    // Cabinets
    marshallConvolution.prepare(spec);
    const File& impResponseMarshall = File::getSpecialLocation
    (File::SpecialLocationType::userDesktopDirectory).getChildFile("Marshall 1960VB SM57 A -2 0 0 45.wav");
    marshallConvolution.loadImpulseResponse(impResponseMarshall, dsp::Convolution::Stereo::no, dsp::Convolution::Trim::no, 0, dsp::Convolution::Normalise::no);

    fenderConvolution.prepare(spec);
    const File& impResponseFender = File::getSpecialLocation
    (File::SpecialLocationType::userDesktopDirectory).getChildFile("Rocksta Reactions Fender Twin Reverb Seiren Pro R A 8 4 45 45.wav");
    fenderConvolution.loadImpulseResponse(impResponseFender, dsp::Convolution::Stereo::no, dsp::Convolution::Trim::no, 0, dsp::Convolution::Normalise::no);

    // EQ
    spec.numChannels = 1;
    leftChain.prepare(spec);
    rightChain.prepare(spec);

    // Reverb
    reverbConvolution.prepare(spec);
    const File& impResponseReverb = File::getSpecialLocation
    (File::SpecialLocationType::userDesktopDirectory).getChildFile("lyd3_000_ortf_48k.wav");
    reverbConvolution.loadImpulseResponse(impResponseReverb, dsp::Convolution::Stereo::no, dsp::Convolution::Trim::no, 0, dsp::Convolution::Normalise::no);

    ap1.prepare(spec);
    ap1.reset();

    ap2.prepare(spec);
    ap2.reset();

    ap3.prepare(spec);
    ap3.reset();

    comb1.prepare(spec);
    comb1.reset();

    comb2.prepare(spec);
    comb2.reset();

    comb3.prepare(spec);
    comb3.reset();

    comb4.prepare(spec);
    comb4.reset();

    mixer.prepare(spec);
    mixer.reset();

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

    rmsLevelLeft = Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto block = dsp::AudioBlock<float>(buffer);
    auto context = dsp::ProcessContextReplacing<float>(block);

    // Noise Gating
    /*
    noiseGate.setAttack(10.f);
    noiseGate.setRelease(100.f);
    noiseGate.setThreshold(5.f);
    noiseGate.setRatio(2.f);
    noiseGate.process(context);*/

    // POWER ON
    auto allby = state->getRawParameterValue("bypass all");
    if ((bool)allby->load()) {

        // Compressor
        float attack = *state->getRawParameterValue("attack");
        float release = *state->getRawParameterValue("release");
        float threshold = *state->getRawParameterValue("thresh");
        float ratio = *state->getRawParameterValue("ratio");
        float compgain = *state->getRawParameterValue("comp gain");

        auto compby = state->getRawParameterValue("bypass comp");
        if ((bool)compby->load()) {
            compressor.setAttack(attack);
            compressor.setRelease(release);
            compressor.setThreshold(threshold * -1.0);
            compressor.setRatio(ratio);
            compressor.process(context);

            makeupGain.setGainDecibels(compgain);
            makeupGain.process(context);
        }

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
            auto* channelData = buffer.getWritePointer(channel);

            auto distby = state->getRawParameterValue("bypass dist");
            if ((bool)distby->load()) {
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
            }

            auto delayby = state->getRawParameterValue("bypass delay");
            if ((bool)delayby->load()) {
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
        }

        mWritePosition += bufferLength;
        mWritePosition %= delayBufferLength;

        // Reverb
        // Set size, decay, mix

        auto revby = state->getRawParameterValue("bypass reverb");
        if ((bool)revby->load()) {

            float size = *state->getRawParameterValue("reverb size");
            float decay = *state->getRawParameterValue("reverb decay");

            auto outA = juce::Decibels::decibelsToGain(-5.f);
            auto outB = juce::Decibels::decibelsToGain(-5.f);
            auto outC = juce::Decibels::decibelsToGain(-5.f);
            auto outD = juce::Decibels::decibelsToGain(-5.f);

            float mix = *state->getRawParameterValue("reverb mix");
            mixer.setWetMixProportion(mix);

            juce::dsp::AudioBlock <float> block(buffer);
            mixer.pushDrySamples(block);

            ap1.setDelay(347.f);
            ap2.setDelay(113.f);
            ap3.setDelay(37.f);

            float allPassfbVal = 0.7;

            for (int channel = 0; channel < getTotalNumInputChannels(); ++channel)
            {
                auto* channelData = buffer.getWritePointer(channel);


                comb1.setDelay(1687 + size);
                comb2.setDelay(1601 + size);
                comb3.setDelay(2053 + size);
                comb4.setDelay(2251 + size);

                // Comb Filtering (parellel)
                for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
                {
                    auto x1 = channelData[sample] / 4;
                    auto x2 = channelData[sample] / 4;
                    auto x3 = channelData[sample] / 4;
                    auto x4 = channelData[sample] / 4;

                    output4 = comb1.popSample(channel);
                    comb1.pushSample(channel, x1 + output4 * (.673 + decay));
                    x1 = output4;

                    output5 = comb2.popSample(channel);
                    comb2.pushSample(channel, x2 + output5 * (.702 + decay));
                    x2 = output5;

                    output6 = comb3.popSample(channel);
                    comb3.pushSample(channel, x3 + output6 * (.653 + decay));
                    x3 = output6;

                    output7 = comb4.popSample(channel);
                    comb4.pushSample(channel, x4 + output7 * (.633 + decay));
                    x4 = output7;

                    //multiply each output by it's slider value
                    channelData[sample] = x1 * outA + x2 * outB + x3 * outC + x4 * outD;
                }


                // All-Pass Filtering (series)
                for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
                {
                    output1 = ap1.popSample(channel);
                    feedBack = output1 * allPassfbVal;
                    feedForward = -channelData[sample] - output1 * allPassfbVal;
                    ap1.pushSample(channel, channelData[sample] + feedBack);
                    channelData[sample] = output1 + feedForward;

                    output2 = ap2.popSample(channel);
                    feedBack = output2 * allPassfbVal;
                    feedForward = -channelData[sample] - output2 * allPassfbVal;
                    ap2.pushSample(channel, channelData[sample] + feedBack);
                    channelData[sample] = output2 + feedForward;

                    output3 = ap3.popSample(channel);
                    feedBack = output3 * allPassfbVal;
                    feedForward = -channelData[sample] - output3 * allPassfbVal;
                    ap3.pushSample(channel, channelData[sample] + feedBack);
                    channelData[sample] = output3 + feedForward;
                }

                // MM
                mixer.mixWetSamples(block);

            }

        }
        
        auto eqby = state->getRawParameterValue("bypass eq");
        if ((bool)eqby->load()) {

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

        // Cabinets
        auto cabmen = state->getRawParameterValue("cabinet menu");
        if ((int)cabmen->load() == 2) {
            cabinetGain.setGainDecibels(-17.f);
            cabinetGain.process(context);
            marshallConvolution.process(context);
        }
        else if ((int)cabmen->load() == 3) {
            cabinetGain.setGainDecibels(-19.f);
            cabinetGain.process(context);
            fenderConvolution.process(context);
        }
    }
    rmsLevelRight = Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
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

float Guitarfxcapstone4AudioProcessor::getRmsValue(const int channel) const {
    jassert(channel == 0 || channel == 1);
    if (channel == 0) {
        return rmsLevelLeft;
    }
    else if (channel == 1) {
        return rmsLevelRight;
    }
    else {
        return 0.f;
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
