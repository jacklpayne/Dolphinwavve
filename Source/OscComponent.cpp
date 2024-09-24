#include "OscComponent.h"

OscComponent::OscComponent()
{
    setSize(200, 200);

    dolphinScheme.setColour(juce::Slider::thumbColourId, dwDarkPurple);
    dolphinScheme.setColour(juce::Slider::rotarySliderOutlineColourId, dwDarkGrey);
    dolphinScheme.setColour(juce::Slider::rotarySliderFillColourId, dwDarkerGrey);
    dolphinScheme.setColour(juce::Slider::trackColourId, dwDarkGrey);


    waveShapeDropdown.setSize(80, 20);
    waveShapeDropdown.addItemList({ "Square", "Saw", "Triangle" }, 1);
    waveShapeDropdown.setColour(juce::ComboBox::backgroundColourId, dwLightGrey);
    waveShapeDropdown.setColour(juce::ComboBox::buttonColourId, dwLightGrey);
    waveShapeDropdown.setColour(juce::ComboBox::outlineColourId, dwDarkGrey);
    waveShapeDropdown.setColour(juce::ComboBox::focusedOutlineColourId, dwDarkGrey);

    onButton.setSize(20, 20);
    waveShapeDropdown.setSelectedId(1);

    initSliders();
    initLabels();

    addAndMakeVisible(&waveShapeDropdown);
    addAndMakeVisible(&onButton);
}

void OscComponent::setID(int _oscID)
{
    oscID = _oscID;
    oscTitle = "Osc " + std::to_string(oscID);
}

void OscComponent::paint(juce::Graphics& g)
{
    g.setColour(dwDarkGrey);
    g.drawRoundedRectangle(0,0,200,200,5,2);

    g.setFont(30.0f);
    g.setColour(dwDarkerGrey);
    g.drawText(oscTitle, getLocalBounds(), juce::Justification::centredTop, true);
}

void OscComponent::resized()
{
    onButton.setTopLeftPosition(8, 8);
    gainSlider.setCentrePosition(95, (getHeight()/ 2));
    panSlider.setCentrePosition(15, (getHeight() / 2));
    harmonicsSlider.setCentrePosition(8, 35);
    waveShapeDropdown.setCentrePosition(harmonicsSlider.getX() + 90, harmonicsSlider.getY() + 30);

    gainLabel.setCentrePosition(gainSlider.getX()-10, gainSlider.getY() -10);
    panLabel.setCentrePosition(panSlider.getX() - 10, panSlider.getY() - 10);
    harmonicsLabel.setTopLeftPosition(harmonicsSlider.getX() + 42, harmonicsSlider.getY() -4);

}

void OscComponent::initSliders()
{
    gainSlider.setSize(80, 80);
    gainSlider.setLookAndFeel(&dolphinScheme);
    gainSlider.setSliderStyle(juce::Slider::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox(), true, 0, 0);
    addAndMakeVisible(gainSlider);

    panSlider.setSize(80, 80);
    panSlider.setLookAndFeel(&dolphinScheme);
    panSlider.setSliderStyle(juce::Slider::Rotary);
    panSlider.setTextBoxStyle(juce::Slider::NoTextBox(), true, 0, 0);
    addAndMakeVisible(panSlider);

    harmonicsSlider.setSize(180, 40);
    harmonicsSlider.setLookAndFeel(&dolphinScheme);
    harmonicsSlider.setTextBoxStyle(juce::Slider::NoTextBox(), true, 0, 0);
    harmonicsSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    addAndMakeVisible(harmonicsSlider);
}

void OscComponent::initLabels()
{
    harmonicsLabel.setText("Harmonics", juce::dontSendNotification);
    harmonicsLabel.setJustificationType(juce::Justification::centred);
    harmonicsLabel.setSize(100, 20);
    addAndMakeVisible(harmonicsLabel);

    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.setSize(100, 20);
    addAndMakeVisible(gainLabel);

    panLabel.setText("Pan", juce::dontSendNotification);
    panLabel.setJustificationType(juce::Justification::centred);
    panLabel.setSize(100, 20);
    addAndMakeVisible(panLabel);
}
