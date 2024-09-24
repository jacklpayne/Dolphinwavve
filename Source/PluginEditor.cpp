#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DolphinwavveAudioProcessorEditor::DolphinwavveAudioProcessorEditor (DolphinwavveAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (1280, 720);
    initBackground();
    initOscAttachments();

    attackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ENV1_ATTACK", env1Component.attackSlider);

    masterGainSlider.setSize(100, 100);
    masterGainSlider.setLookAndFeel(&oscComponents[0].dolphinScheme);
    masterGainSlider.setSliderStyle(juce::Slider::Rotary);
    masterGainSlider.setTextBoxStyle(juce::Slider::NoTextBox(), true, 0, 0);
    addAndMakeVisible(masterGainSlider);

    addAndMakeVisible(env1Component);
}

DolphinwavveAudioProcessorEditor::~DolphinwavveAudioProcessorEditor()
{
} 

//==============================================================================
void DolphinwavveAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    g.setColour(juce::Colours::darkviolet);
}

void DolphinwavveAudioProcessorEditor::resized()
{
    bgImage.setBounds(getLocalBounds());
    oscComponents[0].setCentrePosition(getWidth() / 10, getHeight() / 4);
    oscComponents[1].setCentrePosition(oscComponents[0].getRight() + 120, getHeight() / 4);
    oscComponents[2].setCentrePosition(getWidth() / 10, oscComponents[0].getBottom() + 120);
    oscComponents[3].setCentrePosition(oscComponents[0].getRight() + 120, oscComponents[0].getBottom() + 120);

    env1Component.setTopLeftPosition(getWidth() / 35, getHeight() - 215);

    masterGainSlider.setCentrePosition(getWidth() - 120, getHeight() - 120);
}

void DolphinwavveAudioProcessorEditor::initBackground()
{
    bgImage.setImage(juce::ImageCache::getFromMemory(BinaryData::dolphinwavve_png, BinaryData::dolphinwavve_pngSize));
    bgImage.setImagePlacement(juce::RectanglePlacement(juce::RectanglePlacement::stretchToFit));
    addAndMakeVisible(&bgImage);
}

void DolphinwavveAudioProcessorEditor::initOscAttachments()
{
    for (int i = 0; i < oscComponents.size(); ++i)
    {
        oscComponents[i].setID(i + 1);
        addAndMakeVisible(oscComponents[i]);

        std::string gainParamID = "OSC" + std::to_string(i + 1) + "_GAIN";
        std::string panParamID = "OSC" + std::to_string(i + 1) + "_PAN";
        std::string harmonicsParamID = "OSC" + std::to_string(i + 1) + "_HARMS";
        std::string onParamID = "OSC" + std::to_string(i + 1) + "_ON";
        std::string waveShapeParamID = "OSC" + std::to_string(i + 1) + "_SHAPE";

        gainSliderAttachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, gainParamID, oscComponents[i].gainSlider);
        panSliderAttachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, panParamID, oscComponents[i].panSlider);
        harmonicsSliderAttachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, harmonicsParamID, oscComponents[i].harmonicsSlider);
        onButtonAttachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, onParamID, oscComponents[i].onButton);
        waveShapeDropdownAttachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, waveShapeParamID, oscComponents[i].waveShapeDropdown);
    }
}



