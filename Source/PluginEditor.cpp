/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define WIDTH_DIV 9
#define HEIGHT_DIV 6

//==============================================================================
Guitarfxcapstone4AudioProcessorEditor::Guitarfxcapstone4AudioProcessorEditor(Guitarfxcapstone4AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    verticalGradientMeterInput([&]() {return audioProcessor.getRmsValue(0); }),
    verticalGradientMeterOutput([&]() {return audioProcessor.getRmsValue(1); })
{
    addAndMakeVisible(verticalGradientMeterInput);
    addAndMakeVisible(verticalGradientMeterOutput);

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

    addAndMakeVisible(ratioKnob = new Slider("Ratio"));
    ratioKnob->setSliderStyle(Slider::Rotary);
    ratioKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    ratioKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(compGainKnob = new Slider("Comp Gain"));
    compGainKnob->setSliderStyle(Slider::Rotary);
    compGainKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    compGainKnob->setLookAndFeel(&customLookAndFeel);

    attackAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "attack", *attackKnob);
    releaseAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "release", *releaseKnob);
    threshAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "thresh", *threshKnob);
    ratioAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "ratio", *ratioKnob);
    compGainAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "comp gain", *compGainKnob);

    addAndMakeVisible(bypassComp = new TextButton("Bypass Comp"));
    bypassComp->setButtonText("Compressor");
    bypassComp->setClickingTogglesState(true);
    bypassComp->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::cadetblue);
    bypassComp->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::dimgrey);
    bypassCompAttachment = new AudioProcessorValueTreeState::ButtonAttachment(p.getState(), "bypass comp", *bypassComp);

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

    addAndMakeVisible(bypassDist = new TextButton("Bypass Dist"));
    bypassDist->setButtonText("Distortion");
    bypassDist->setClickingTogglesState(true);
    bypassDist->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkred);
    bypassDist->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::dimgrey);
    bypassDistAttachment = new AudioProcessorValueTreeState::ButtonAttachment(p.getState(), "bypass dist", *bypassDist);

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

    addAndMakeVisible(bypassDelay = new TextButton("Bypass Delay"));
    bypassDelay->setButtonText("Delay");
    bypassDelay->setClickingTogglesState(true);
    bypassDelay->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::rebeccapurple);
    bypassDelay->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::dimgrey);
    bypassDelayAttachment = new AudioProcessorValueTreeState::ButtonAttachment(p.getState(), "bypass delay", *bypassDelay);


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

    addAndMakeVisible(bypassEQ = new TextButton("Bypass EQ"));
    bypassEQ->setButtonText("EQ");
    bypassEQ->setClickingTogglesState(true);
    bypassEQ->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkseagreen);
    bypassEQ->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::dimgrey);
    bypassEQAttachment = new AudioProcessorValueTreeState::ButtonAttachment(p.getState(), "bypass eq", *bypassEQ);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(800, 700);
    startTimerHz(24);
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

    //g.drawText("Compressor", ((getWidth() / WIDTH_DIV) * 1) - (100 / 2), (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100, Justification::centred, false);
    g.drawText("Attack", ((getWidth() / WIDTH_DIV) * 2) - (100 / 2), (getHeight() / HEIGHT_DIV) + 10, 100, 100, Justification::centred, false);
    g.drawText("Release", ((getWidth() / WIDTH_DIV) * 3) - (100 / 2), (getHeight() / HEIGHT_DIV) + 10, 100, 100, Justification::centred, false);
    g.drawText("Threshold", ((getWidth() / WIDTH_DIV) * 4) - (100 / 2), (getHeight() / HEIGHT_DIV) + 10, 100, 100, Justification::centred, false);
    g.drawText("Gain", ((getWidth() / WIDTH_DIV) * 5) - (100 / 2), (getHeight() / HEIGHT_DIV) + 10, 100, 100, Justification::centred, false);
    g.drawText("Ratio", ((getWidth() / WIDTH_DIV) * 6) - (100 / 2), (getHeight() / HEIGHT_DIV) + 10, 100, 100, Justification::centred, false);

    // g.drawText("Distortion", ((getWidth() / WIDTH_DIV) * 1) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100, Justification::centred, false);
    g.drawText("Drive", ((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) + 10, 100, 100, Justification::centred, false);
    g.drawText("Range", ((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) + 10, 100, 100, Justification::centred, false);
    g.drawText("Blend", ((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) + 10, 100, 100, Justification::centred, false);
    g.drawText("Volume", ((getWidth() / WIDTH_DIV) * 5) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) + 10, 100, 100, Justification::centred, false);

    // g.drawText("Delay", ((getWidth() / WIDTH_DIV) * 1) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100, Justification::centred, false);
    g.drawText("Feedback", ((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) + 10, 100, 100, Justification::centred, false);
    g.drawText("Time", ((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) + 10, 100, 100, Justification::centred, false);
    g.drawText("Volume", ((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) + 10, 100, 100, Justification::centred, false);

    // g.drawText("EQ", ((getWidth() / WIDTH_DIV) * 1) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100, Justification::centred, false);
    g.drawText("Low", ((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) + 10, 100, 100, Justification::centred, false);
    g.drawText("Mid", ((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) + 10, 100, 100, Justification::centred, false);
    g.drawText("High", ((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) + 10, 100, 100, Justification::centred, false);
}

void Guitarfxcapstone4AudioProcessorEditor::resized()
{
    verticalGradientMeterInput.setBounds(getWidth() - 100, getHeight() - 250, 15, 200);
    verticalGradientMeterOutput.setBounds(getWidth() - 80, getHeight() - 250, 15, 200);

    bypassComp->setBounds(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 10, (getHeight() / HEIGHT_DIV) - 25, 100, 30);
    attackKnob->setBounds(((getWidth() / WIDTH_DIV) * 2) - (100 / 2), (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);
    releaseKnob->setBounds(((getWidth() / WIDTH_DIV) * 3) - (100 / 2), (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);
    threshKnob->setBounds(((getWidth() / WIDTH_DIV) * 4) - (100 / 2), (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);
    compGainKnob->setBounds(((getWidth() / WIDTH_DIV) * 5) - (100 / 2), (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);
    ratioKnob->setBounds(((getWidth() / WIDTH_DIV) * 6) - (100 / 2), (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);

    bypassDist->setBounds(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 10, 2 * (getHeight() / HEIGHT_DIV) - 25, 100, 30);
    driveKnob->setBounds(((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);
    rangeKnob->setBounds(((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);
    blendKnob->setBounds(((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);
    volumeKnob->setBounds(((getWidth() / WIDTH_DIV) * 5) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);

    bypassDelay->setBounds(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 10, 3 * (getHeight() / HEIGHT_DIV) - 25, 100, 30);
    delayFeedbackKnob->setBounds(((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);
    delayTimeKnob->setBounds(((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);
    delayVolumeKnob->setBounds(((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);

    bypassEQ->setBounds(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 10, 4 * (getHeight() / HEIGHT_DIV) - 25, 100, 30);
    lowKnob->setBounds(((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);
    midKnob->setBounds(((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);
    highKnob->setBounds(((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);
}

void Guitarfxcapstone4AudioProcessorEditor::timerCallback() {
    verticalGradientMeterInput.repaint();
    verticalGradientMeterOutput.repaint();
}