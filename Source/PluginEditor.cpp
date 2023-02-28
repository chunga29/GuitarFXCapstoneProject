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

    addAndMakeVisible(releaseKnob = new Slider("Release"));
    releaseKnob->setSliderStyle(Slider::Rotary);
    releaseKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);

    addAndMakeVisible(threshKnob = new Slider("Threshold"));
    threshKnob->setSliderStyle(Slider::Rotary);
    threshKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);

    /*addAndMakeVisible(ratioMenu = new ComboBox("Ratio"));
    ratioMenu->addItem("1", 1);
    ratioMenu->addItem("2", 2);
    ratioMenu->addItem("4", 3);
    ratioMenu->addItem("8", 4);
    ratioMenu->addItem("50", 5);*/

    addAndMakeVisible(compGainKnob = new Slider("Comp Gain"));
    compGainKnob->setSliderStyle(Slider::Rotary);
    compGainKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);

    attackAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "attack", *attackKnob);
    releaseAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "release", *releaseKnob);
    threshAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "thresh", *threshKnob);
    //// ratioAttachment = new AudioProcessorValueTreeState::ComboBoxAttachment(p.getState(), "ratio", *ratioMenu);
    compGainAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "comp gain", *compGainKnob);

    // Distortion
    addAndMakeVisible(driveKnob = new Slider("Drive"));
    driveKnob->setSliderStyle(Slider::Rotary);
    driveKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);

    addAndMakeVisible(rangeKnob = new Slider("Range"));
    rangeKnob->setSliderStyle(Slider::Rotary);
    rangeKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);

    
    addAndMakeVisible(blendKnob = new Slider("Blend"));
    blendKnob->setSliderStyle(Slider::Rotary);
    blendKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);

    addAndMakeVisible(volumeKnob = new Slider("Volume"));
    volumeKnob->setSliderStyle(Slider::Rotary);
    volumeKnob->setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);

    
    driveAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "drive", *driveKnob);
    rangeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "range", *rangeKnob);
    blendAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "blend", *blendKnob);
    volumeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "volume", *volumeKnob);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(500, 400);
}

Guitarfxcapstone4AudioProcessorEditor::~Guitarfxcapstone4AudioProcessorEditor()
{
}

//==============================================================================
void Guitarfxcapstone4AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    g.drawText("Attack", ((getWidth() / 6) * 1) - (100 / 2), (getHeight() / 3) + 10, 100, 100, Justification::centred, false);
    g.drawText("Release", ((getWidth() / 6) * 2) - (100 / 2), (getHeight() / 3) + 10, 100, 100, Justification::centred, false);
    g.drawText("Threshold", ((getWidth() / 6) * 3) - (100 / 2), (getHeight() / 3) + 10, 100, 100, Justification::centred, false);
    g.drawText("Gain", ((getWidth() / 6) * 4) - (100 / 2), (getHeight() / 3) + 10, 100, 100, Justification::centred, false);

    g.drawText("Drive", ((getWidth() / 5) * 1) - (100 / 2), 2 * (getHeight() / 3) + 10, 100, 100, Justification::centred, false);
    g.drawText("Range", ((getWidth() / 5) * 2) - (100 / 2), 2 * (getHeight() / 3) + 10, 100, 100, Justification::centred, false);
    g.drawText("Blend", ((getWidth() / 5) * 3) - (100 / 2), 2 * (getHeight() / 3) + 10, 100, 100, Justification::centred, false);
    g.drawText("Volume", ((getWidth() / 5) * 4) - (100 / 2), 2 * (getHeight() / 3) + 10, 100, 100, Justification::centred, false);

}

void Guitarfxcapstone4AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    attackKnob->setBounds(((getWidth() / 6) * 1) - (100 / 2), (getHeight() / 3) - (100 / 2), 100, 100);
    releaseKnob->setBounds(((getWidth() / 6) * 2) - (100 / 2), (getHeight() / 3) - (100 / 2), 100, 100);
    threshKnob->setBounds(((getWidth() / 6) * 3) - (100 / 2), (getHeight() / 3) - (100 / 2), 100, 100);
    compGainKnob->setBounds(((getWidth() / 6) * 4) - (100 / 2), (getHeight() / 3) - (100 / 2), 100, 100);
    // ratioMenu->setBounds(((getWidth() / 6) * 5) - (100 / 2), (getHeight() / 3) - (100 / 2), 100, 100);


    driveKnob->setBounds(((getWidth() / 5) * 1) - (100 / 2), 2 * (getHeight() / 3) - (100 / 2), 100, 100);
    rangeKnob->setBounds(((getWidth() / 5) * 2) - (100 / 2), 2 * (getHeight() / 3) - (100 / 2), 100, 100);
    blendKnob->setBounds(((getWidth() / 5) * 3) - (100 / 2), 2 * (getHeight() / 3) - (100 / 2), 100, 100);
    volumeKnob->setBounds(((getWidth() / 5) * 4) - (100 / 2), 2 * (getHeight() / 3) - (100 / 2), 100, 100);
}

//void Guitarfxcapstone4AudioProcessorEditor::sliderValueChanged(Slider* slider) 
//{
//    if (slider == driveKnob) {
//        
//    }
//}