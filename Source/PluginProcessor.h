/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/

struct EQChainSettings {
    float peakGain{ 0 };
    float lowShelfGain{ 0 };
    float highShelfGain{ 0 };
};

EQChainSettings getEQChainSettings(juce::AudioProcessorValueTreeState& apvts);

class Guitarfxcapstone4AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Guitarfxcapstone4AudioProcessor();
    ~Guitarfxcapstone4AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    float getRmsValue(const int channel) const;

    AudioProcessorValueTreeState& getState();
    void fillDelayBuffer(int channel, const int bufferLength, const int delayBufferLength,
        const float* bufferData, const float* delayBufferData, const float gain);

    void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int bufferLength,
        const int delayBufferLength, const float* bufferData, const float* delayBufferData, const float* wetBufferData, int delayTime);

    void feedbackDelay(int channel, const int bufferLength,
        const int delayBufferLength, const float* dryBuffer, float fdbk_amt);



private:
    ScopedPointer<AudioProcessorValueTreeState> state;
    dsp::Gain<float> inputGain;
    dsp::Gain<float> outputGain;

    dsp::Compressor<float> compressor;
    dsp::NoiseGate<float> noiseGate;
    dsp::Gain<float> makeupGain;

    dsp::Convolution reverbConvolution;

    // Cabinets
    dsp::Convolution marshallConvolution;
    dsp::Convolution fenderConvolution;
    dsp::Convolution randallConvolution;
    dsp::Convolution kochConvolution;
    dsp::Gain<float> cabinetGain;
    

    AudioBuffer<float> mDelayBuffer, wetBuffer, drySignal;
    int mWritePosition { 0 };
    int mSampleRate { 44100 };

    using Filter = juce::dsp::IIR::Filter<float>;
    using Shelf = juce::dsp::ProcessorChain<Filter>;
    using MonoChain = juce::dsp::ProcessorChain<Filter, Filter, Filter>;

    MonoChain leftChain, rightChain;

    enum ChainPositions {
        LowShelf,
        Peak,
        HighShelf
    };

    float rmsLevelLeft, rmsLevelRight;

    // REVERB
    juce::dsp::DelayLine <float> ap1{ 400000 };
    juce::dsp::DelayLine <float> ap2{ 400000 };
    juce::dsp::DelayLine <float> ap3{ 400000 };
    juce::dsp::DelayLine <float> comb1{ 400000 };
    juce::dsp::DelayLine <float> comb2{ 400000 };
    juce::dsp::DelayLine <float> comb3{ 400000 };
    juce::dsp::DelayLine <float> comb4{ 400000 };

    float output1 = 0;
    float output2 = 0;
    float output3 = 0;
    float output4 = 0;
    float output5 = 0;
    float output6 = 0;
    float output7 = 0;

    float feedBack;
    float feedForward;

    juce::dsp::DryWetMixer <float> mixer;



    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Guitarfxcapstone4AudioProcessor)
};
