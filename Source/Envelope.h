#pragma once
#include <JuceHeader.h>
#include "Oscillator.h"

namespace DW
{
	class Envelope
	{
	public:
		void applyEnvelope(juce::RangedAudioParameter& targetParam);
		void setOn(bool _on) { isOn = _on; };
		bool checkIfOn() { return isOn; };
		void setSampleRate(int _sampleRate) { sampleRate = _sampleRate; };
		void setBufferSize(int _bufferSize) { bufferSize = _bufferSize; };
		void reset();
		void init(juce::RangedAudioParameter& targetParam);
		void release() { currentEnvState = R; };

	private:
		std::unique_ptr<Oscillator> osc;

		int sampleRate{};
		int bufferSize{};
		
		struct ADSRValues
		{
			float attack{};
			float decay{};
			float sustain{};
			float release{};
		};
		
		ADSRValues adsrValues{1000, 350, 1, 500};
		ADSRValues currentADSRMultipliers{0, 0, 0, 1};

		float targetValue{};
		float unnormalizedTargetValue{};
		float currentMultiplier{};
		float bpm{};
		bool isOn{};
		bool initialized{};

		enum envState
		{
			A, D, S, R
		};
		envState currentEnvState{};
	};
}
