/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define WIDTH_DIV 10
#define HEIGHT_DIV 7

//==============================================================================
Guitarfxcapstone4AudioProcessorEditor::Guitarfxcapstone4AudioProcessorEditor(Guitarfxcapstone4AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    verticalGradientMeterInput([&]() {return audioProcessor.getRmsValue(0); }),
    verticalGradientMeterOutput([&]() {return audioProcessor.getRmsValue(1); })
{
    addAndMakeVisible(bypassAll = new TextButton("Bypass All"));
    bypassAll->setButtonText("POWER ON");
    bypassAll->setClickingTogglesState(true);
    bypassAll->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::red);
    bypassAll->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::dimgrey);
    bypassAllAttachment = new AudioProcessorValueTreeState::ButtonAttachment(p.getState(), "bypass all", *bypassAll);


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
    bypassComp->setButtonText("Compressor");
    bypassComp->setClickingTogglesState(true);
    bypassComp->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkslateblue);
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
    bypassDist->setButtonText("Distortion");
    bypassDist->setClickingTogglesState(true);
    bypassDist->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::orange);
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
    bypassDelay->setButtonText("Delay");
    bypassDelay->setClickingTogglesState(true);
    bypassDelay->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkred);
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
    bypassReverb->setButtonText("Reverb");
    bypassReverb->setClickingTogglesState(true);
    bypassReverb->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkgrey);
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
    bypassEQ->setColour(juce::TextButton::ColourIds::textColourOnId, Colours::black);
    bypassEQ->setClickingTogglesState(true);
    bypassEQ->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::bisque);
    bypassEQ->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::dimgrey);
    bypassEQAttachment = new AudioProcessorValueTreeState::ButtonAttachment(p.getState(), "bypass eq", *bypassEQ);


    // Cabinets
    addAndMakeVisible(cabinetMenu = new ComboBox("Cabinets"));
    cabinetMenu->addItem("None", 1);
    cabinetMenu->addItem("Marshall 1960VB", 2);
    cabinetMenu->addItem("Twin Fender Reverb", 3);
    cabinetMenu->setColour(0x1000b00, Colours::darkolivegreen);
    cabinetMenu->setColour(0x1000c00, Colours::darkolivegreen);
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
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    // g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::beige);
    g.setColour (juce::Colours::black);
    Font helvetica = customLookAndFeel.getTypefaceForFont(Font());
    // helvetica.setBold(true);
    g.setFont(helvetica);

    g.setFont (15.0f);

    //g.drawText("Compressor", ((getWidth() / WIDTH_DIV) * 1) - (100 / 2), (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100, Justification::centred, false);
    g.drawText("Attack", ((getWidth() / WIDTH_DIV) * 2) - (100 / 2), (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("Release", ((getWidth() / WIDTH_DIV) * 3) - (100 / 2), (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("Threshold", ((getWidth() / WIDTH_DIV) * 4) - (100 / 2), (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("Gain", ((getWidth() / WIDTH_DIV) * 5) - (100 / 2), (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("Ratio", ((getWidth() / WIDTH_DIV) * 6) - (100 / 2), (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);

    // g.drawText("Distortion", ((getWidth() / WIDTH_DIV) * 1) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100, Justification::centred, false);
    g.drawText("Drive", ((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("Range", ((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("Blend", ((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("Volume", ((getWidth() / WIDTH_DIV) * 5) - (100 / 2), 2 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);

    // g.drawText("Delay", ((getWidth() / WIDTH_DIV) * 1) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100, Justification::centred, false);
    g.drawText("Feedback", ((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("Time", ((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("Volume", ((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 3 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);

    g.drawText("Size", ((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("Decay", ((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("Mix", ((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);

    // g.drawText("EQ", ((getWidth() / WIDTH_DIV) * 1) - (100 / 2), 5 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100, Justification::centred, false);
    g.drawText("Low", ((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 5 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("Mid", ((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 5 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
    g.drawText("High", ((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 5 * (getHeight() / HEIGHT_DIV) - 95, 100, 100, Justification::centred, false);
}

void Guitarfxcapstone4AudioProcessorEditor::resized()
{
    bypassAll->setBounds(getWidth() - 120, 20, 100, 30);

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

    bypassReverb->setBounds(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 10, 4 * (getHeight() / HEIGHT_DIV) - 25, 100, 30);
    reverbSizeKnob->setBounds(((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);
    reverbDecayKnob->setBounds(((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);
    reverbMixKnob->setBounds(((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 4 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);

    bypassEQ->setBounds(((getWidth() / WIDTH_DIV) * 1) - (100 / 2) - 10, 5 * (getHeight() / HEIGHT_DIV) - 25, 100, 30);
    lowKnob->setBounds(((getWidth() / WIDTH_DIV) * 2) - (100 / 2), 5 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);
    midKnob->setBounds(((getWidth() / WIDTH_DIV) * 3) - (100 / 2), 5 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);
    highKnob->setBounds(((getWidth() / WIDTH_DIV) * 4) - (100 / 2), 5 * (getHeight() / HEIGHT_DIV) - (100 / 2), 100, 100);

    cabinetMenu->setBounds((getWidth() / 2) - 75, getHeight() - 70, 150, 50);
}

void Guitarfxcapstone4AudioProcessorEditor::timerCallback() {
    verticalGradientMeterInput.repaint();
    verticalGradientMeterOutput.repaint();
}