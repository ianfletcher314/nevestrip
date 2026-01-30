#pragma once

#include <JuceHeader.h>

/**
 * Neve-style High Pass Filter
 *
 * Stepped frequencies matching classic Neve 1073:
 * Off, 50Hz, 80Hz, 160Hz, 300Hz
 *
 * Uses 12dB/octave slope (2-pole) for smooth, musical filtering
 */
class HighPassFilter
{
public:
    enum Frequency
    {
        HPF_OFF = 0,
        HPF_50Hz,
        HPF_80Hz,
        HPF_160Hz,
        HPF_300Hz
    };

    HighPassFilter();

    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    // Set frequency (0=Off, 1=50Hz, 2=80Hz, 3=160Hz, 4=300Hz)
    void setFrequency(int freqIndex);

private:
    void updateCoefficients();

    // Parameters
    Frequency currentFreq = HPF_OFF;
    float cutoffHz = 0.0f;

    // Biquad coefficients
    float b0 = 1.0f, b1 = 0.0f, b2 = 0.0f;
    float a1 = 0.0f, a2 = 0.0f;

    // Biquad state (2 channels)
    float x1L = 0.0f, x2L = 0.0f;
    float y1L = 0.0f, y2L = 0.0f;
    float x1R = 0.0f, x2R = 0.0f;
    float y1R = 0.0f, y2R = 0.0f;

    double currentSampleRate = 44100.0;

    // Frequency lookup table
    static constexpr float frequencies[5] = { 0.0f, 50.0f, 80.0f, 160.0f, 300.0f };
};
