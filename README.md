# Dolphinwavve

Dolphinwavve is an additive synthesizer project that leverages the JUCE library. It allows users to generate waveforms by summing partials of Fourier series for various wave types, providing a powerful tool for sound design.

## Features

- **Custom Oscillators**: Generates Fourier series for different wave types per oscillator, enabling the creation of complex sounds.
- **Partial Summation**: Summing of partials is configurable up to a user-defined number while adhering to the Nyquist limit to prevent aliasing.
- **Audio Buffer Management**: Manually fills an audio buffer with floating-point samples that are efficiently passed to the JUCE library for playback.
- **Per-Oscillator Control**: Includes per-oscillator ADR envelopes for shaping the sound, as well as individual panning and gain adjustments.
- **Planned Improvements**: Future iterations will focus on implementing SIMD acceleration for oscillators to enhance performance and refining the behavior of envelopes and panning.

## Getting Started

Ensure you have the JUCE library and its Projucer project manger installed, and then point it to the cloned directory. Alternatively, access the .vst3 from the build directory and use your preferred plugin host.

## License

This project is licensed under the MIT License.
