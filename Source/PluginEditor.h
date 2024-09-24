#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "OscComponent.h"
#include "EnvelopeComponent.h"

//==============================================================================
/**
*/
class DolphinwavveAudioProcessorEditor  : public juce::AudioProcessorEditor

{
public:
    DolphinwavveAudioProcessorEditor (DolphinwavveAudioProcessor&);
    ~DolphinwavveAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    DolphinwavveAudioProcessor& audioProcessor;

    std::array<OscComponent, 4> oscComponents;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 4> gainSliderAttachments;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 4> panSliderAttachments;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 4> harmonicsSliderAttachments;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>, 4> onButtonAttachments;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>, 4> waveShapeDropdownAttachments;

    EnvelopeComponent env1Component;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackSliderAttachment;

    juce::Slider masterGainSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment masterGainSliderAttachment{ audioProcessor.apvts, "MSTR_GAIN", masterGainSlider };

    void initBackground();
    void initOscAttachments();

    juce::ImageComponent bgImage;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DolphinwavveAudioProcessorEditor)
};
