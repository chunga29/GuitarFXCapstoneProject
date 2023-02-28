/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Guitarfxcapstone4AudioProcessorEditor  : public juce::AudioProcessorEditor
                                               //public juce::Slider::Listener,
                                               //public juce::ComboBox::Listener
{
public:
    Guitarfxcapstone4AudioProcessorEditor (Guitarfxcapstone4AudioProcessor&);
    ~Guitarfxcapstone4AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //void sliderValueChanged(Slider* slider);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Guitarfxcapstone4AudioProcessor& audioProcessor;

    // ----- Compressor Knobs ----- //
    ScopedPointer<Slider> attackKnob;
    ScopedPointer<Slider> releaseKnob;
    ScopedPointer<Slider> threshKnob;
    ScopedPointer<ComboBox> ratioMenu;
    ScopedPointer<Slider> compGainKnob;
    // ScopedPointer<Slider> bypass;

    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> threshAttachment;
    ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> ratioAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> compGainAttachment;

    // ----- Distortion Knobs ----- //
    ScopedPointer<Slider> driveKnob;
    ScopedPointer<Slider> rangeKnob;
    ScopedPointer<Slider> blendKnob;
    ScopedPointer<Slider> volumeKnob;

    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> rangeAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> blendAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;

    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Guitarfxcapstone4AudioProcessorEditor)
};
