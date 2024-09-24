#include "Oscillator.h"

void Oscillator::processAudioBlock(juce::AudioBuffer<float>& buffer, int inputChannelsCount, int outputChannelsCount)
{
    juce::AudioBuffer<float> osc_buffer(outputChannelsCount, buffer.getNumSamples());
    for (int i = inputChannelsCount; i < outputChannelsCount; ++i)
        osc_buffer.clear(i, 0, buffer.getNumSamples());

    // Generate samples from voices and write them to the temporary buffer
    for (int sample = 0; sample < osc_buffer.getNumSamples(); ++sample) // Could be range based
    {
        for (int channel = 0; channel < outputChannelsCount; ++channel)
        {
            auto* channelData = osc_buffer.getWritePointer(channel);
            channelData[sample] += writeSample();
        }
    }

    // Apply panning ramps to the temporary buffer
    applyPanning(osc_buffer, outputChannelsCount == 2);

    // Copy temporary buffer to the main processor buffer
    buffer.addFrom(0, 0, osc_buffer, 0, 0, osc_buffer.getNumSamples());
    buffer.addFrom(1, 0, osc_buffer, 1, 0, osc_buffer.getNumSamples());
}

// Despicable 
void Oscillator::applyPanning(juce::AudioBuffer<float>& buffer, bool stereo)
{
    if (stereo)
    {
        if (previousPanValue != panValue)
        {
            if (panValue > 0)
            {
                buffer.applyGainRamp(0, 0, buffer.getNumSamples(), 1 - previousPanValue, 1 - panValue);
            }
            else if (panValue < 0)
            {
                buffer.applyGainRamp(1, 0, buffer.getNumSamples(), 1 + previousPanValue, 1 + panValue);
            }
        }
        else
        {
            if (panValue > 0)
            {
                buffer.applyGain(0, 0, buffer.getNumSamples(), 1 - panValue);
            }
            else if (panValue < 0)
            {
                buffer.applyGain(1, 0, buffer.getNumSamples(), 1 + panValue);
            };
        }
    }
    if (gain == previousGain)
    {
        buffer.applyGain(gain);
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, gain);
    }

    previousPanValue = panValue;
    previousGain = gain;
}


float Oscillator::writeSample()
{
    float sample{};
    for (auto& voice : voices)
    {
        if (voice.checkIfOn())
            sample += (voice.getSample(numHarmonics, waveShapeID));
    }
    return sample;
}

// Seems that if the sampleRate changes while a note is being held, it won't update in OscVoice until a new midiMessage
void Oscillator::assignVoice(int noteDownNumber)
{
    // Finds the first available OscVoice and initializes it
    for (auto& voice : voices)
    {
        if (!voice.checkIfOn())
        {
            voice.initVoice(noteDownNumber, sampleRate);
            numVoicesOn++;
            break;
        }
    }
}

void Oscillator::freeVoice(int noteUpNumber)
{
    for (auto& voice : voices)
    {
        if (voice.getNoteNumber() == noteUpNumber)
        {
            voice.stopping = true;
            numVoicesOn--;
            break;
        }
    }
}


