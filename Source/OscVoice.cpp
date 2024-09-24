#include "OscVoice.h"

void OscVoice::initVoice(int _noteNumber, int _sampleRate)
{
    sampleRate       = _sampleRate;
    noteNumber       = _noteNumber;
    fundamentalFreq  = juce::MidiMessage::getMidiNoteInHertz(noteNumber);
    samplesPerPeriod = (2.0 * juce::MathConstants<double>::pi) / sampleRate;
    maxFrequency     = sampleRate / 2;
    phaseStep        = samplesPerPeriod * fundamentalFreq;

    isOn = true;
}

float OscVoice::getSample(float _numHarmonics, int waveShapeID)
{
    int   numFullHarmonics    = (int)_numHarmonics;
    float highestHarmonicGain = _numHarmonics - numFullHarmonics; // Amplitude of highest harmonic = value after decimal

    float sample{};
    for (int n = 1; n <= numFullHarmonics + 1; ++n)
    {
        FourierCoefficients fourier = findFourierCoefficients(n, waveShapeID); // could/should be a function pointer involved here
        sample += fourier.a_n * std::sin(fourier.freqCoefficient * currentPhase) * currentHarmonicGains[n].currentGain;
    }

    applyGainRamp(numFullHarmonics + 1, highestHarmonicGain);
    currentPhase += phaseStep;
    wrapPhase();
    return sample;   
}

OscVoice::FourierCoefficients OscVoice::findFourierCoefficients(int n, int waveShapeID)
{
    switch (waveShapeID)
    {
    // Square
    case 0:
        return { (float)1/((2 * n) - 1), (2 * n) - 1 };
        break;
    // Saw
    case 1:
        return { (float)1 / (2 * n), n };
        break;
    // Tri
    case 2:
        return { std::powf(-1, (float)((2 * n) - 2) / 2) / std::powf((2 * n) - 1, 2), (2 * n) - 1 };
        break;
    }
}

void OscVoice::wrapPhase()
{
    if (currentPhase >= (2.0 * juce::MathConstants<double>::pi))
    {
        currentPhase -= (2.0 * juce::MathConstants<double>::pi);
    }
}

// The gain ramp prevents clicking at the beginning and end of the note, and when harmonics are added or removed
void OscVoice::applyGainRamp(int numHarmonicsActive, float highestHarmonicGain)
{
    int nonZeroGainCount{};
    for (auto& harmonicGain : currentHarmonicGains)
    {
        int harmonic       = harmonicGain.harmonic;
        float& currentGain = harmonicGain.currentGain;

        if (stopping)
        {
            if (currentGain > 0)
            {
                currentGain -= gainRampStep;
                nonZeroGainCount++;
            }    
            continue;
        }

        if (harmonic < numHarmonicsActive && currentGain < MAX_GAIN)
        {
            currentGain += gainRampStep;
        }
        else if (harmonic == numHarmonicsActive)
        {
            if (currentGain > highestHarmonicGain)
            {
                currentGain -= gainRampStep;
            }
            else if (currentGain < highestHarmonicGain)
            {
                currentGain += gainRampStep;
            }
        }
        if (harmonic > numHarmonicsActive)
        {
            currentGain > 0 ? currentGain -= gainRampStep : currentGain = 0;
        }
    }
    if (stopping && nonZeroGainCount == 0)
        reset();
}

void OscVoice::reset()
{
    isOn = false;
    stopping = false;
    noteNumber = -1;
    phaseStep = 0;
}