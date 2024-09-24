#pragma once

#include <JuceHeader.h>
#include "OscVoice.h"

class Oscillator
{
public:
	void processAudioBlock(juce::AudioBuffer<float>& buffer, int numInputChannels, int numOutputChannels);
	float writeSample();
//========================================================================
	void setSampleRate(double _sampleRate) { sampleRate = _sampleRate; };
	void setGain(float _gain) { gain = _gain; };
	void setPan(float _pan) { panValue = _pan; };
	void setOnState(bool _isOn) { isOn = _isOn; };
	void setWaveShape(int _waveShapeID) { waveShapeID = _waveShapeID; };
	bool checkIfOn() { return isOn; };
//========================================================================
	void assignVoice(int noteDownNumber);
	void freeVoice(int noteUpNumber);
	float numHarmonics{ 1 };

private:
//========================================================================
	double sampleRate{};
	double currentPhase{};
	double phaseStep{};
//========================================================================
	float gain{ 1.0 };
	float previousGain{ 1.0 };
	float panValue{ 0.0 };
	float previousPanValue{ 0.0 };
//========================================================================
	bool isOn{};
	int numVoicesOn{}; // currently unused...
	int waveShapeID{}; // enum......................

	void applyPanning(juce::AudioBuffer<float>& buffer, bool stereo);
	std::array<OscVoice, 12> voices;
};
