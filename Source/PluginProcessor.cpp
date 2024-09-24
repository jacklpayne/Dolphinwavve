/*
            __|\
         .-'    '-.
        / .--, _ a L
      .J (  '-' "'--'
     '-'-.)  .~~~~~~~~~~~~~~~~~~~~.
             |    Dolphinwavve    |     __
             |                    | ,.-'e ''-'7
             |                    |  '--.    (
             |                    |      ),   \
             '~~~~~~~~~~~~~~~~~~~~'      ` )  :
                                      ,__.'_.'
                                      '-, (
                                        '--'

*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DolphinwavveAudioProcessor::DolphinwavveAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                     #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                     #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
}

DolphinwavveAudioProcessor::~DolphinwavveAudioProcessor()
{
}

//==============================================================================
const juce::String DolphinwavveAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DolphinwavveAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DolphinwavveAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DolphinwavveAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DolphinwavveAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DolphinwavveAudioProcessor::getNumPrograms()
{
    return 1;
}

int DolphinwavveAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DolphinwavveAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DolphinwavveAudioProcessor::getProgramName (int index)
{
    return {};
}

void DolphinwavveAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DolphinwavveAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (auto& osc : oscs)
    {
        osc.setSampleRate(sampleRate);
    }
    env1.setSampleRate(sampleRate);
    //juce::ignoreUnused(samplesPerBlock);
}

void DolphinwavveAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DolphinwavveAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DolphinwavveAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    processMidiBlock(midiMessages);
    processAudioBlock(buffer);

}

void DolphinwavveAudioProcessor::processMidiBlock(juce::MidiBuffer& midiMessages)
{
    for (const auto event : midiMessages)
    {
        juce::MidiMessage message = event.getMessage();
        if (message.isNoteOn())
        {
            for (auto& osc : oscs)
            {
                if(osc.checkIfOn())
                    osc.assignVoice(message.getNoteNumber());

                env1.setOn(true);
            }
        }
        if (message.isNoteOff())
        {
            for (auto& osc : oscs)
            {
                osc.freeVoice(message.getNoteNumber());

                //env1.setOn(false);
                env1.release();
            }
        }
    }
}

void DolphinwavveAudioProcessor::processAudioBlock(juce::AudioBuffer<float>& buffer)
{
    if (env1.checkIfOn())
    {
        env1.setBufferSize(buffer.getNumSamples());
        env1.applyEnvelope(*apvts.getParameter("OSC1_HARMS"));
    }

    updateOscillatorParams();

    auto inputChannelsCount = getTotalNumInputChannels();
    auto outputChannelsCount = getTotalNumOutputChannels();

    for (int i = inputChannelsCount; i < outputChannelsCount; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    for (auto& osc : oscs)
    {
        if (osc.checkIfOn())
            osc.processAudioBlock(buffer, inputChannelsCount, outputChannelsCount);
    }
        
    //*apvts.getRawParameterValue("MSTR_GAIN")->load();
}

void DolphinwavveAudioProcessor::updateOscillatorParams()
{
    for (int i = 0; i < oscs.size(); ++i)
    {
        std::string IDPrefix = "OSC" + std::to_string(i + 1) + "_";
        std::string harmonicsParamID =   IDPrefix + "HARMS";
        std::string gainParamID      =   IDPrefix + "GAIN";
        std::string panParamID       =   IDPrefix + "PAN";
        std::string onParamID        =   IDPrefix + "ON";
        std::string waveShapeID      =   IDPrefix + "SHAPE";

        oscs[i].numHarmonics = apvts.getRawParameterValue(harmonicsParamID)->load();
        oscs[i].setGain(apvts.getRawParameterValue(gainParamID)->load());
        oscs[i].setPan(apvts.getRawParameterValue(panParamID)->load());
        oscs[i].setOnState(apvts.getParameterAsValue(onParamID).getValue());
        oscs[i].setWaveShape(apvts.getParameterAsValue(waveShapeID).getValue());
    }
}

//==============================================================================
bool DolphinwavveAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* DolphinwavveAudioProcessor::createEditor()
{
    return new DolphinwavveAudioProcessorEditor (*this);
}

//==============================================================================
void DolphinwavveAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void DolphinwavveAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

juce::AudioProcessorValueTreeState::ParameterLayout DolphinwavveAudioProcessor::createParams()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1_GAIN", "Osc 1 Gain", 0.0, 1.0, 1.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1_PAN", "Osc 1 Pan", -1.0, 1.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1_HARMS", "Osc 1 Harmonics", 1, 30, 20));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1_SHAPE", "Osc 1 Shape", juce::StringArray{"Square", "Saw", "Triangle"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterBool>("OSC1_ON", "Osc 1 On", 1));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC2_GAIN", "Osc 2 Gain", 0.0, 1.0, 1.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC2_PAN", "Osc 2 Pan", -1.0, 1.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC2_HARMS", "Osc 2 Harmonics", 1, 30, 1));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2_SHAPE", "Osc 2 Shape", juce::StringArray{ "Square", "Saw", "Triangle" }, 1));
    params.push_back(std::make_unique<juce::AudioParameterBool>("OSC2_ON", "Osc 2 On", 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC3_GAIN", "Osc 3 Gain", 0.0, 1.0, 1.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC3_PAN", "Osc 3 Pan", -1.0, 1.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC3_HARMS", "Osc 3 Harmonics", 1, 30, 1));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC3_SHAPE", "Osc 3 Shape", juce::StringArray{ "Square", "Saw", "Triangle" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterBool>("OSC3_ON", "Osc 3 On", 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC4_GAIN", "Osc 4 Gain", 0.0, 1.0, 1.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC4_PAN", "Osc 4 Pan", -1.0, 1.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC4_HARMS", "Osc 4 Harmonics", 1, 30, 1));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC4_SHAPE", "Osc 4 Shape", juce::StringArray{ "Square", "Saw", "Triangle" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterBool>("OSC4_ON", "Osc 4 On", 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("ENV1_ATTACK", "Env 1 Attack", 0.0, 550.0, 5.0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("MSTR_GAIN", "Master Gain", 0.0, 1.0, 1.0));

    return { params.begin(), params.end() };
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DolphinwavveAudioProcessor();
}
