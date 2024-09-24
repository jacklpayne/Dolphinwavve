#pragma once

#include <JuceHeader.h>

class OscVoice
{
public:
	void initVoice(int _noteNumber, int _sampleRate);
	void reset();

	float  getSample(float _numHarmonics, int waveShapeID);
	double getPhaseStep() { return phaseStep; };
	int    getNoteNumber() { return noteNumber; };
	void   setMaxFrequency(int _maxFrequency) { maxFrequency = _maxFrequency; };
	bool   checkIfOn() { return isOn; };

	bool stopping{ 0 };

private:
	double fundamentalFreq{};
	double samplesPerPeriod{}; // 2pi / sampleRate
	double phaseStep{};        // samplesPerPeriod * frequency
	double currentPhase{};
	int sampleRate{};
	int maxFrequency{};

	void wrapPhase();
	void applyGainRamp(int numHarmonicsActive, float highestHarmonicGain);

	const float MAX_GAIN{ 0.125f };
	const float gainRampStep{ 0.001 };

	int noteNumber{}; // Should Oscillator have a map of oscvoice -> note number instead?
	bool isOn{ 0 };

	struct FourierCoefficients
	{
		float a_n{};
		int freqCoefficient{};
	};
	struct currentHarmonicGain
	{
		int harmonic{};
		float currentGain{};
	};
	struct ADSRms
	{
		float attack{100};
		float decay{};
		float sustain{};
		float release{};
	};

	ADSRms adsr{};

	FourierCoefficients findFourierCoefficients(int n, int waveShapeID);
	std::array<currentHarmonicGain, 32> currentHarmonicGains{};
	
};
	
