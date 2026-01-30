#pragma once

#include <JuceHeader.h>

/**
 * Neve-style Limiter
 *
 * Inspired by the diode bridge limiting in Neve dynamics units.
 * Features:
 * - Fast-acting limiting with musical character
 * - Soft clipping at threshold
 * - Minimal artifacts
 */
class NeveLimiter
{
public:
    NeveLimiter();

    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    // Threshold in dB (-20 to 0)
    void setThreshold(float thresholdDb);

    // Bypass
    void setBypass(bool shouldBypass) { bypassed = shouldBypass; }
    bool isBypassed() const { return bypassed; }

    // Metering
    float getGainReduction() const { return currentGainReduction; }

private:
    double currentSampleRate = 44100.0;
    bool bypassed = false;

    float threshold = 0.0f;        // dB
    float thresholdLinear = 1.0f;  // Linear
    float currentGainReduction = 0.0f;

    // State
    float envelopeL = 0.0f;
    float envelopeR = 0.0f;

    // Very fast attack for limiting
    float attackCoeff = 0.0f;
    float releaseCoeff = 0.0f;
};
