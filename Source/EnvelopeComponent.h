#pragma once
#include <JuceHeader.h>

class EnvelopeComponent : public juce::Component
{
public:
	EnvelopeComponent();

	void setID(int _envID);
	void paint(juce::Graphics& g) override;
	void resized() override;
	juce::LookAndFeel_V4 dolphinScheme;

	juce::Slider attackSlider;
	juce::Slider decaySlider;
	juce::Slider sustainSlider;
	juce::Slider releaseSlider;

private:
	void initSlider(juce::Slider& slider);
	void initLabels();

	int envID{};
	std::string envTitle{};

	juce::Colour dwDarkerGrey = juce::Colour::fromRGB(31, 31, 31);
	juce::Colour dwDarkGrey = juce::Colour::fromRGB(49, 49, 49);
	juce::Colour dwLightGrey = juce::Colour::fromRGB(67, 67, 67);
	juce::Colour dwDarkPurple = juce::Colour::fromRGB(133, 18, 133);
	juce::Colour dwLightPurple = juce::Colour::fromRGB(153, 25, 153);

	juce::Label attackLabel;
	juce::Label decayLabel;
	juce::Label sustainLabel;
	juce::Label releaseLabel;
};