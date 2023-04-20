/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Guitarfxcapstone4AudioProcessorEditor::Guitarfxcapstone4AudioProcessorEditor (Guitarfxcapstone4AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Compression
    addAndMakeVisible(attackKnob = new Slider("Attack"));
    attackKnob->setSliderStyle(Slider::Rotary);
    attackKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    attackKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(releaseKnob = new Slider("Release"));
    releaseKnob->setSliderStyle(Slider::Rotary);
    releaseKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    releaseKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(threshKnob = new Slider("Threshold"));
    threshKnob->setSliderStyle(Slider::Rotary);
    threshKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    threshKnob->setLookAndFeel(&customLookAndFeel);

    /*addAndMakeVisible(ratioMenu = new ComboBox("Ratio"));
    ratioMenu->addItem("1", 1);
    ratioMenu->addItem("2", 2);
    ratioMenu->addItem("4", 3);
    ratioMenu->addItem("8", 4);
    ratioMenu->addItem("50", 5);*/

    addAndMakeVisible(compGainKnob = new Slider("Comp Gain"));
    compGainKnob->setSliderStyle(Slider::Rotary);
    compGainKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    compGainKnob->setLookAndFeel(&customLookAndFeel);

    attackAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "attack", *attackKnob);
    releaseAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "release", *releaseKnob);
    threshAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "thresh", *threshKnob);
    //// ratioAttachment = new AudioProcessorValueTreeState::ComboBoxAttachment(p.getState(), "ratio", *ratioMenu);
    compGainAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "comp gain", *compGainKnob);

    // Distortion
    addAndMakeVisible(driveKnob = new Slider("Drive"));
    driveKnob->setSliderStyle(Slider::Rotary);
    driveKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    driveKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(rangeKnob = new Slider("Range"));
    rangeKnob->setSliderStyle(Slider::Rotary);
    rangeKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    rangeKnob->setLookAndFeel(&customLookAndFeel);

    
    addAndMakeVisible(blendKnob = new Slider("Blend"));
    blendKnob->setSliderStyle(Slider::Rotary);
    blendKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    blendKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(volumeKnob = new Slider("Volume"));
    volumeKnob->setSliderStyle(Slider::Rotary);
    volumeKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    volumeKnob->setLookAndFeel(&customLookAndFeel);

    driveAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "drive", *driveKnob);
    rangeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "range", *rangeKnob);
    blendAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "blend", *blendKnob);
    volumeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "volume", *volumeKnob);

    // Delay
    addAndMakeVisible(delayFeedbackKnob = new Slider("Delay Feedback"));
    delayFeedbackKnob->setSliderStyle(Slider::Rotary);
    delayFeedbackKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    delayFeedbackKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(delayTimeKnob = new Slider("Delay Time"));
    delayTimeKnob->setSliderStyle(Slider::Rotary);
    delayTimeKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    delayTimeKnob->setLookAndFeel(&customLookAndFeel);


    addAndMakeVisible(delayVolumeKnob = new Slider("Delay Volume"));
    delayVolumeKnob->setSliderStyle(Slider::Rotary);
    delayVolumeKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    delayVolumeKnob->setLookAndFeel(&customLookAndFeel);

    delayFeedbackAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "delay feedback", *delayFeedbackKnob);
    delayTimeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "delay time", *delayTimeKnob);
    delayVolumeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "delay volume", *delayVolumeKnob);


    // EQ
    addAndMakeVisible(lowKnob = new Slider("EQ Low"));
    lowKnob->setSliderStyle(Slider::Rotary);
    lowKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    lowKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(midKnob = new Slider("EQ Mid"));
    midKnob->setSliderStyle(Slider::Rotary);
    midKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    midKnob->setLookAndFeel(&customLookAndFeel);


    addAndMakeVisible(highKnob = new Slider("EQ High"));
    highKnob->setSliderStyle(Slider::Rotary);
    highKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    highKnob->setLookAndFeel(&customLookAndFeel);

    lowAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "eq low", *lowKnob);
    midAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "eq mid", *midKnob);
    highAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "eq high", *highKnob);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(800, 700);
}

Guitarfxcapstone4AudioProcessorEditor::~Guitarfxcapstone4AudioProcessorEditor()
{
}

//==============================================================================
void Guitarfxcapstone4AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    // g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    g.drawText("Compressor", ((getWidth() / 7) * 1) - (100 / 2), (getHeight() / 6) - (100 / 2), 100, 100, Justification::centred, false);
    g.drawText("Attack", ((getWidth() / 7) * 2) - (100 / 2), (getHeight() / 6) + 10, 100, 100, Justification::centred, false);
    g.drawText("Release", ((getWidth() / 7) * 3) - (100 / 2), (getHeight() / 6) + 10, 100, 100, Justification::centred, false);
    g.drawText("Threshold", ((getWidth() / 7) * 4) - (100 / 2), (getHeight() / 6) + 10, 100, 100, Justification::centred, false);
    g.drawText("Gain", ((getWidth() / 7) * 5) - (100 / 2), (getHeight() / 6) + 10, 100, 100, Justification::centred, false);

    g.drawText("Distortion", ((getWidth() / 7) * 1) - (100 / 2), 2 * (getHeight() / 6) - (100 / 2), 100, 100, Justification::centred, false);
    g.drawText("Drive", ((getWidth() / 7) * 2) - (100 / 2), 2 * (getHeight() / 6) + 10, 100, 100, Justification::centred, false);
    g.drawText("Range", ((getWidth() / 7) * 3) - (100 / 2), 2 * (getHeight() / 6) + 10, 100, 100, Justification::centred, false);
    g.drawText("Blend", ((getWidth() / 7) * 4) - (100 / 2), 2 * (getHeight() / 6) + 10, 100, 100, Justification::centred, false);
    g.drawText("Volume", ((getWidth() / 7) * 5) - (100 / 2), 2 * (getHeight() / 6) + 10, 100, 100, Justification::centred, false);

    g.drawText("Delay", ((getWidth() / 7) * 1) - (100 / 2), 3 * (getHeight() / 6) - (100 / 2), 100, 100, Justification::centred, false);
    g.drawText("Feedback", ((getWidth() / 7) * 2) - (100 / 2), 3 * (getHeight() / 6) + 10, 100, 100, Justification::centred, false);
    g.drawText("Time", ((getWidth() / 7) * 3) - (100 / 2), 3 * (getHeight() / 6) + 10, 100, 100, Justification::centred, false);
    g.drawText("Volume", ((getWidth() / 7) * 4) - (100 / 2), 3 * (getHeight() / 6) + 10, 100, 100, Justification::centred, false);

    g.drawText("EQ", ((getWidth() / 7) * 1) - (100 / 2), 4 * (getHeight() / 6) - (100 / 2), 100, 100, Justification::centred, false);
    g.drawText("Low", ((getWidth() / 7) * 2) - (100 / 2), 4 * (getHeight() / 6) + 10, 100, 100, Justification::centred, false);
    g.drawText("Mid", ((getWidth() / 7) * 3) - (100 / 2), 4 * (getHeight() / 6) + 10, 100, 100, Justification::centred, false);
    g.drawText("High", ((getWidth() / 7) * 4) - (100 / 2), 4 * (getHeight() / 6) + 10, 100, 100, Justification::centred, false);
}

void Guitarfxcapstone4AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    attackKnob->setBounds(((getWidth() / 7) * 2) - (100 / 2), (getHeight() / 6) - (100 / 2), 100, 100);
    releaseKnob->setBounds(((getWidth() / 7) * 3) - (100 / 2), (getHeight() / 6) - (100 / 2), 100, 100);
    threshKnob->setBounds(((getWidth() / 7) * 4) - (100 / 2), (getHeight() / 6) - (100 / 2), 100, 100);
    compGainKnob->setBounds(((getWidth() / 7) * 5) - (100 / 2), (getHeight() / 6) - (100 / 2), 100, 100);
    // ratioMenu->setBounds(((getWidth() / 7) * 6) - (100 / 2), (getHeight() / 3) - (100 / 2), 100, 100);

    driveKnob->setBounds(((getWidth() / 7) * 2) - (100 / 2), 2 * (getHeight() / 6) - (100 / 2), 100, 100);
    rangeKnob->setBounds(((getWidth() / 7) * 3) - (100 / 2), 2 * (getHeight() / 6) - (100 / 2), 100, 100);
    blendKnob->setBounds(((getWidth() / 7) * 4) - (100 / 2), 2 * (getHeight() / 6) - (100 / 2), 100, 100);
    volumeKnob->setBounds(((getWidth() / 7) * 5) - (100 / 2), 2 * (getHeight() / 6) - (100 / 2), 100, 100);

    delayFeedbackKnob->setBounds(((getWidth() / 7) * 2) - (100 / 2), 3 * (getHeight() / 6) - (100 / 2), 100, 100);
    delayTimeKnob->setBounds(((getWidth() / 7) * 3) - (100 / 2), 3 * (getHeight() / 6) - (100 / 2), 100, 100);
    delayVolumeKnob->setBounds(((getWidth() / 7) * 4) - (100 / 2), 3 * (getHeight() / 6) - (100 / 2), 100, 100);

    lowKnob->setBounds(((getWidth() / 7) * 2) - (100 / 2), 4 * (getHeight() / 6) - (100 / 2), 100, 100);
    midKnob->setBounds(((getWidth() / 7) * 3) - (100 / 2), 4 * (getHeight() / 6) - (100 / 2), 100, 100);
    highKnob->setBounds(((getWidth() / 7) * 4) - (100 / 2), 4 * (getHeight() / 6) - (100 / 2), 100, 100);
}

//void Guitarfxcapstone4AudioProcessorEditor::sliderValueChanged(Slider* slider) 
//{
//    if (slider == driveKnob) {
//        
//    }
//}