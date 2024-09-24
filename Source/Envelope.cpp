#include "Envelope.h"

void DW::Envelope::applyEnvelope(juce::RangedAudioParameter& targetParam)
{
	if (!initialized)
		init(targetParam);

	float attackStep = ( bufferSize / (adsrValues.attack / 1000)) / sampleRate;
	float decayStep = (bufferSize / (adsrValues.decay / 1000)) / sampleRate;
	float releaseStep = (bufferSize / (adsrValues.release / 1000)) / sampleRate;

	switch (currentEnvState)
	{
	case A:
		// It seems like this function multiplies the argument by the normalisable range (max value) on the other end
		targetParam.setValueNotifyingHost(targetValue * currentADSRMultipliers.attack);
		targetParam.getValue() < targetValue ? currentADSRMultipliers.attack += attackStep : currentEnvState = D;
		break;

	case D:
		targetParam.setValueNotifyingHost(targetValue * currentADSRMultipliers.decay);
		targetParam.getValue() > adsrValues.sustain ? currentADSRMultipliers.decay -= decayStep : currentEnvState = S;
		break;

	case S:
		targetParam.setValueNotifyingHost(targetValue * currentADSRMultipliers.sustain);
		break;

	case R:
		targetParam.setValueNotifyingHost(targetValue * currentADSRMultipliers.release);
		targetParam.getValue() > 0 ? currentADSRMultipliers.release -= releaseStep : reset();
		break;
	}
}

void DW::Envelope::init(juce::RangedAudioParameter& targetParam)
{
	targetValue = targetParam.getValue();
	targetParam.setValueNotifyingHost(0);
	initialized = true;
}

void DW::Envelope::reset()
{
	currentADSRMultipliers.attack = 0.0;
	currentADSRMultipliers.attack = 1.0;

	currentEnvState = A;
	initialized = false;
	isOn = false;
}


