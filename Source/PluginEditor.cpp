/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define WIDTH_DIV 8
#define HEIGHT_DIV 6
#define B_UP 5

//==============================================================================
Guitarfxcapstone4AudioProcessorEditor::Guitarfxcapstone4AudioProcessorEditor(Guitarfxcapstone4AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    verticalGradientMeterInput([&]() {return audioProcessor.getRmsValue(0); }),
    verticalGradientMeterOutput([&]() {return audioProcessor.getRmsValue(1); })
{
    addAndMakeVisible(bypassAll = new TextButton("Bypass All"));
    bypassAll->setButtonText("POWER");
    bypassAll->setClickingTogglesState(true);
    bypassAll->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::indianred);
    bypassAll->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::dimgrey);
    bypassAllAttachment = new AudioProcessorValueTreeState::ButtonAttachment(p.getState(), "bypass all", *bypassAll);

    addAndMakeVisible(inputKnob = new Slider("Input"));
    inputKnob->setSliderStyle(Slider::Rotary);
    inputKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    inputKnob->setLookAndFeel(&customLookAndFeel);
    inputAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "input gain", *inputKnob);

    addAndMakeVisible(outputKnob = new Slider("Output"));
    outputKnob->setSliderStyle(Slider::Rotary);
    outputKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    outputKnob->setLookAndFeel(&customLookAndFeel);
    outputAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "output gain", *outputKnob);

    Font helvetica = customLookAndFeel.getTypefaceForFont(Font());
    // helvetica.setBold(true);

    addAndMakeVisible(verticalGradientMeterInput);
    addAndMakeVisible(verticalGradientMeterOutput);

    // Compression
    addAndMakeVisible(attackKnob = new Slider("Attack"));
    attackKnob->setSliderStyle(Slider::Rotary);
    //attackKnob->s etTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    attackKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    attackKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(releaseKnob = new Slider("Release"));
    releaseKnob->setSliderStyle(Slider::Rotary);
    releaseKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    releaseKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(threshKnob = new Slider("Threshold"));
    threshKnob->setSliderStyle(Slider::Rotary);
    threshKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    threshKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(ratioKnob = new Slider("Ratio"));
    ratioKnob->setSliderStyle(Slider::Rotary);
    ratioKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    ratioKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(compGainKnob = new Slider("Comp Gain"));
    compGainKnob->setSliderStyle(Slider::Rotary);
    compGainKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    compGainKnob->setLookAndFeel(&customLookAndFeel);

    attackAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "attack", *attackKnob);
    releaseAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "release", *releaseKnob);
    threshAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "thresh", *threshKnob);
    ratioAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "ratio", *ratioKnob);
    compGainAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "comp gain", *compGainKnob);

    addAndMakeVisible(bypassComp = new TextButton("Bypass Comp"));
    bypassComp->setButtonText("COMPRESSOR");
    bypassComp->setClickingTogglesState(true);
    bypassComp->setColour(juce::TextButton::ColourIds::textColourOnId, Colours::darkslategrey);
    bypassComp->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::whitesmoke);
    // dark slate blue
    bypassComp->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::dimgrey);
    bypassCompAttachment = new AudioProcessorValueTreeState::ButtonAttachment(p.getState(), "bypass comp", *bypassComp);

    // Distortion
    addAndMakeVisible(driveKnob = new Slider("Drive"));
    driveKnob->setSliderStyle(Slider::Rotary);
    driveKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    driveKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(rangeKnob = new Slider("Range"));
    rangeKnob->setSliderStyle(Slider::Rotary);
    rangeKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    rangeKnob->setLookAndFeel(&customLookAndFeel);

    
    addAndMakeVisible(blendKnob = new Slider("Blend"));
    blendKnob->setSliderStyle(Slider::Rotary);
    blendKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    blendKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(volumeKnob = new Slider("Volume"));
    volumeKnob->setSliderStyle(Slider::Rotary);
    volumeKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    volumeKnob->setLookAndFeel(&customLookAndFeel);

    driveAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "drive", *driveKnob);
    rangeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "range", *rangeKnob);
    blendAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "blend", *blendKnob);
    volumeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "volume", *volumeKnob);

    addAndMakeVisible(bypassDist = new TextButton("Bypass Dist"));
    bypassDist->setButtonText("DISTORTION");
    bypassDist->setClickingTogglesState(true);
    bypassDist->setColour(juce::TextButton::ColourIds::textColourOnId, Colours::darkslategrey);
    bypassDist->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::whitesmoke);
    bypassDist->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::dimgrey);
    bypassDistAttachment = new AudioProcessorValueTreeState::ButtonAttachment(p.getState(), "bypass dist", *bypassDist);

    // Delay
    addAndMakeVisible(delayFeedbackKnob = new Slider("Delay Feedback"));
    delayFeedbackKnob->setSliderStyle(Slider::Rotary);
    delayFeedbackKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    delayFeedbackKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(delayTimeKnob = new Slider("Delay Time"));
    delayTimeKnob->setSliderStyle(Slider::Rotary);
    delayTimeKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    delayTimeKnob->setLookAndFeel(&customLookAndFeel);


    addAndMakeVisible(delayVolumeKnob = new Slider("Delay Volume"));
    delayVolumeKnob->setSliderStyle(Slider::Rotary);
    delayVolumeKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    delayVolumeKnob->setLookAndFeel(&customLookAndFeel);

    delayFeedbackAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "delay feedback", *delayFeedbackKnob);
    delayTimeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "delay time", *delayTimeKnob);
    delayVolumeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "delay volume", *delayVolumeKnob);

    addAndMakeVisible(bypassDelay = new TextButton("Bypass Delay"));
    bypassDelay->setButtonText("DELAY");
    bypassDelay->setClickingTogglesState(true);
    bypassDelay->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::whitesmoke);
    bypassDelay->setColour(juce::TextButton::ColourIds::textColourOnId, Colours::darkslategrey);
    bypassDelay->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::dimgrey);
    bypassDelayAttachment = new AudioProcessorValueTreeState::ButtonAttachment(p.getState(), "bypass delay", *bypassDelay);

    // Reverb
    addAndMakeVisible(reverbSizeKnob = new Slider("Reverb Size"));
    reverbSizeKnob->setSliderStyle(Slider::Rotary);
    reverbSizeKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    reverbSizeKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(reverbDecayKnob = new Slider("Reverb Decay"));
    reverbDecayKnob->setSliderStyle(Slider::Rotary);
    reverbDecayKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    reverbDecayKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(reverbMixKnob = new Slider("Reverb Mix"));
    reverbMixKnob->setSliderStyle(Slider::Rotary);
    reverbMixKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    reverbMixKnob->setLookAndFeel(&customLookAndFeel);

    reverbSizeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "reverb size", *reverbSizeKnob);
    reverbDecayAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "reverb decay", *reverbDecayKnob);
    reverbMixAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "reverb mix", *reverbMixKnob);

    addAndMakeVisible(bypassReverb = new TextButton("Bypass Reverb"));
    bypassReverb->setButtonText("REVERB");
    bypassReverb->setClickingTogglesState(true);
    bypassReverb->setColour(juce::TextButton::ColourIds::textColourOnId, Colours::darkslategrey);
    bypassReverb->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::whitesmoke);
    bypassReverb->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::dimgrey);
    bypassReverbAttachment = new AudioProcessorValueTreeState::ButtonAttachment(p.getState(), "bypass reverb", *bypassReverb);

    // EQ
    addAndMakeVisible(lowKnob = new Slider("EQ Low"));
    lowKnob->setSliderStyle(Slider::Rotary);
    lowKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    lowKnob->setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(midKnob = new Slider("EQ Mid"));
    midKnob->setSliderStyle(Slider::Rotary);
    midKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    midKnob->setLookAndFeel(&customLookAndFeel);


    addAndMakeVisible(highKnob = new Slider("EQ High"));
    highKnob->setSliderStyle(Slider::Rotary);
    highKnob->setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
    highKnob->setLookAndFeel(&customLookAndFeel);

    lowAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "eq low", *lowKnob);
    midAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "eq mid", *midKnob);
    highAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "eq high", *highKnob);

    addAndMakeVisible(bypassEQ = new TextButton("Bypass EQ"));
    bypassEQ->setButtonText("EQ");
    bypassEQ->setColour(juce::TextButton::ColourIds::textColourOnId, Colours::darkslategrey);
    bypassEQ->setClickingTogglesState(true);
    bypassEQ->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::whitesmoke);
    bypassEQ->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::dimgrey);
    bypassEQAttachment = new AudioProcessorValueTreeState::ButtonAttachment(p.getState(), "bypass eq", *bypassEQ);


    // Cabinets
    addAndMakeVisible(cabinetMenu = new ComboBox("Cabinets"));
    cabinetMenu->addItem("NONE", 1);
    cabinetMenu->addItem("MARSHALL 1960VB", 2);
    cabinetMenu->addItem("TWIN FENDER REVERB", 3);
    cabinetMenu->addItem("RANDALL RT412", 4);
    cabinetMenu->addItem("KOCH MT184S", 5);
    cabinetMenu->setColour(0x1000b00, Colours::floralwhite);
    cabinetMenu->setColour(0x1000c00, Colours::darkslategrey);
    cabinetMenu->setColour(0x1000e00, Colours::darkslategrey);
    cabinetMenu->setColour(0x1000a00, Colours::darkslategrey);
    cabinetMenuAttachment = new AudioProcessorValueTreeState::ComboBoxAttachment(p.getState(), "cabinet menu", *cabinetMenu);


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    // setLookAndFeel(&customLookAndFeel);
    setSize(800, 700);
    startTimerHz(24);
}

Guitarfxcapstone4AudioProcessorEditor::~Guitarfxcapstone4AudioProcessorEditor()
{
}

//==============================================================================
void Guitarfxcapstone4AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::whitesmoke);
    g.setColour (juce::Colours::darkslategrey);
    Font helvetica = customLookAndFeel.getTypefaceForFont(Font());
    helvetica.setBold(true);
    g.setFont(helvetica);
    g.setFont (15.0f);

    //g.drawText("Compressor", ((getWidth() / WIDTH_DIV) * 1) - (100 / 2), (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100, Justification::centred, false);
    g.drawText("ATTACK", ((getWidth() / WIDTH_DIV) * 2) - (100 / 2), (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("RELEASE", ((getWidth() / WIDTH_DIV) * 3) - (100 / 2), (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("THRESHOLD", ((getWidth() / WIDTH_DIV) * 4) - (100 / 2), (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("GAIN", ((getWidth() / WIDTH_DIV) * 6) - (100 / 2), (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("RATIO", ((getWidth() / WIDTH_DIV) * 5) - (100 / 2), (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false); 
    g.drawRoundedRectangle(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 20, (getHeight() / HEIGHT_DIV) - 60, (getWidth() / WIDTH_DIV) * 6 + 20, 100, 10, 2);
    // g.fillRoundedRectangle(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 20, (getHeight() / HEIGHT_DIV) - 60, (getWidth() / WIDTH_DIV) * 6 + 20, 100, 10);

    // g.drawText("Distortion", ((getWidth() / WIDTH_DIV) * 1) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100, Justification::centred, false);
    g.drawText("DRIVE", ((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("RANGE", ((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("BLEND", ((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("VOLUME", ((getWidth() / WIDTH_DIV) * 5) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawRoundedRectangle(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 20, 2 * (getHeight() / HEIGHT_DIV) - 60, (getWidth() / WIDTH_DIV) * 6 + 20, 100, 10, 2);

    // g.drawText("Delay", ((getWidth() / WIDTH_DIV) * 1) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100, Justification::centred, false);
    g.drawText("FEEDBACK", ((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("TIME", ((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("MIX", ((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawRoundedRectangle(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 20, 3 * (getHeight() / HEIGHT_DIV) - 60, (getWidth() / WIDTH_DIV) * 6 + 20, 100, 10, 2);

    // REVERB
    g.drawText("SIZE", ((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("DECAY", ((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("DRY / WET", ((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawRoundedRectangle(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 20, 4 * (getHeight() / HEIGHT_DIV) - 60, (getWidth() / WIDTH_DIV) * 6 + 20, 100, 10, 2);


    // g.drawText("EQ", ((getWidth() / WIDTH_DIV) * 1) - (100 / 2), 5 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100, Justification::centred, false);
    g.drawText("LOW", ((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 5 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("MID", ((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 5 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("HIGH", ((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 5 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawRoundedRectangle(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 20, 5 * (getHeight() / HEIGHT_DIV) - 60, (getWidth() / WIDTH_DIV) * 6 + 20, 100, 10, 2);


    g.drawText("INPUT", getWidth() - 120, 2 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("OUTPUT", getWidth() - 120, 3 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawRoundedRectangle(getWidth() - 120, 2 * (getHeight() / HEIGHT_DIV) - 60, 100, (getHeight() / HEIGHT_DIV) + 100, 10, 2);
}

void Guitarfxcapstone4AudioProcessorEditor::resized()
{
    bypassAll->setBounds(getWidth() - 120, (getHeight() / HEIGHT_DIV) - 60, 100, 100);

    inputKnob->setBounds(getWidth() - 120, 2 * (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);
    outputKnob->setBounds(getWidth() - 120, 3 * (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);

    verticalGradientMeterInput.setBounds(getWidth() - 88, 5 * (getHeight() / HEIGHT_DIV) - 160, 15, 200);
    verticalGradientMeterOutput.setBounds(getWidth() - 68, 5 * (getHeight() / HEIGHT_DIV) - 160, 15, 200);

    bypassComp->setBounds(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 10, (getHeight() / HEIGHT_DIV) - 25, 105, 30);
    attackKnob->setBounds(((getWidth() / WIDTH_DIV) * 2) - (100 / 2), (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);
    releaseKnob->setBounds(((getWidth() / WIDTH_DIV) * 3) - (100 / 2), (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);
    threshKnob->setBounds(((getWidth() / WIDTH_DIV) * 4) - (100 / 2), (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);
    compGainKnob->setBounds(((getWidth() / WIDTH_DIV) * 6) - (100 / 2), (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);
    ratioKnob->setBounds(((getWidth() / WIDTH_DIV) * 5) - (100 / 2), (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);

    bypassDist->setBounds(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 10, 2 * (getHeight() / HEIGHT_DIV) - 25, 105, 30);
    driveKnob->setBounds(((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);
    rangeKnob->setBounds(((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);
    blendKnob->setBounds(((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);
    volumeKnob->setBounds(((getWidth() / WIDTH_DIV) * 5) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);

    bypassDelay->setBounds(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 10, 3 * (getHeight() / HEIGHT_DIV) - 25, 105, 30);
    delayFeedbackKnob->setBounds(((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);
    delayTimeKnob->setBounds(((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);
    delayVolumeKnob->setBounds(((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);

    bypassReverb->setBounds(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 10, 4 * (getHeight() / HEIGHT_DIV) - 25, 105, 30);
    reverbSizeKnob->setBounds(((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);
    reverbDecayKnob->setBounds(((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);
    reverbMixKnob->setBounds(((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);

    bypassEQ->setBounds(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 10, 5 * (getHeight() / HEIGHT_DIV) - 25, 105, 30);
    lowKnob->setBounds(((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 5 * (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);
    midKnob->setBounds(((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 5 * (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);
    highKnob->setBounds(((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 5 * (getHeight() / HEIGHT_DIV) - (100 / 2) - B_UP, 100, 100);

    cabinetMenu->setBounds(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 20, getHeight() - 65, 750, 50);
}

void Guitarfxcapstone4AudioProcessorEditor::timerCallback() {
    verticalGradientMeterInput.repaint();
    verticalGradientMeterOutput.repaint();
}