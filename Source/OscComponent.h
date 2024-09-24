#pragma once

#include <JuceHeader.h>

class OscComponent : public juce::Component
{
public:
	OscComponent();

	void setID(int _oscID);
	void paint(juce::Graphics& g) override;
	void resized() override;
	juce::LookAndFeel_V4 dolphinScheme;

private:
	void initSliders();
	void initLabels();

	int oscID{};
	std::string oscTitle{};

	juce::Colour dwDarkerGrey = juce::Colour::fromRGB(31, 31, 31);
	juce::Colour dwDarkGrey = juce::Colour::fromRGB(49, 49, 49);
	juce::Colour dwLightGrey = juce::Colour::fromRGB(67, 67, 67);
	juce::Colour dwDarkPurple = juce::Colour::fromRGB(133, 18, 133);
	juce::Colour dwLightPurple = juce::Colour::fromRGB(153, 25, 153);

public:
	juce::Slider gainSlider;
	juce::Slider panSlider;
	juce::Slider harmonicsSlider;
	juce::ComboBox waveShapeDropdown;
	juce::ToggleButton onButton;
	juce::Label harmonicsLabel;
	juce::Label gainLabel;
	juce::Label panLabel;
};