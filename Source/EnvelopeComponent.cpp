#include "EnvelopeComponent.h"

EnvelopeComponent::EnvelopeComponent()
{
    setSize(400, 200);

    dolphinScheme.setColour(juce::Slider::thumbColourId, dwDarkPurple);
    dolphinScheme.setColour(juce::Slider::rotarySliderOutlineColourId, dwDarkGrey);
    dolphinScheme.setColour(juce::Slider::rotarySliderFillColourId, dwDarkerGrey);
    dolphinScheme.setColour(juce::Slider::trackColourId, dwDarkGrey);

    initSlider(attackSlider);
    initSlider(decaySlider);
    initSlider(sustainSlider);
    initSlider(releaseSlider);
    initLabels();
}

void EnvelopeComponent::setID(int _envID)
{
    envID = _envID;
    envTitle = "Envelope " + std::to_string(envID);
}

void EnvelopeComponent::paint(juce::Graphics& g)
{
    g.setColour(dwDarkGrey);
    g.drawRoundedRectangle(0, 0, 400, 200, 5, 2);

    g.setFont(30.0f);
    g.setColour(dwDarkerGrey);
    g.drawText(envTitle, getLocalBounds(), juce::Justification::centredTop, true);
}

void EnvelopeComponent::resized()
{
    attackSlider.setCentrePosition(10, 35);
    decaySlider.setCentrePosition(attackSlider.getRight()+60, 35);
    sustainSlider.setCentrePosition(decaySlider.getRight() + 60, 35);
    releaseSlider.setCentrePosition(sustainSlider.getRight() + 60, 35);
}

void EnvelopeComponent::initSlider(juce::Slider& slider)
{
    slider.setSize(30, 100);
    slider.setLookAndFeel(&dolphinScheme);
    slider.setSliderStyle(juce::Slider::LinearBarVertical);
    slider.setTextBoxStyle(juce::Slider::NoTextBox(), true, 0, 0);
    addAndMakeVisible(&slider);
}

void EnvelopeComponent::initLabels()
{
    attackLabel.setText("A", juce::dontSendNotification);
    attackLabel.setJustificationType(juce::Justification::centred);
    attackLabel.setSize(100, 50);
    //addAndMakeVisible(&attackLabel);

    decayLabel.setText("D", juce::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centred);
    decayLabel.setSize(100, 50);
    //addAndMakeVisible(&decayLabel);

    sustainLabel.setText("S", juce::dontSendNotification);
    sustainLabel.setJustificationType(juce::Justification::centred);
    sustainLabel.setSize(100, 50);
    //addAndMakeVisible(&sustainLabel);

    releaseLabel.setText("R", juce::dontSendNotification);
    releaseLabel.setJustificationType(juce::Justification::centred);
    releaseLabel.setSize(100, 50);
    //addAndMakeVisible(&releaseLabel);
}
