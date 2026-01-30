#pragma once

#include <JuceHeader.h>

/**
 * Neve-style Transformer Saturation
 *
 * Models the characteristics of Neve input/output transformers:
 * - Asymmetric soft saturation (even harmonics)
 * - Low-frequency "weight" from core saturation
 * - High-frequency "silk" from transformer resonance
 * - Subtle low-frequency phase shift
 */
class Transformer
{
public:
    Transformer();

    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    // Drive amount (0-100%)
    void setDrive(float drivePercent);

private:
    float processSample(float input);
    void updateCoefficients();

    // Parameters
    float drive = 0.0f;          // 0-1 range
    float driveGain = 1.0f;      // Linear gain from drive

    // State for low-frequency coloration (one-pole filter)
    float lpStateL = 0.0f;
    float lpStateR = 0.0f;

    // State for high-frequency "silk" (subtle resonance)
    float hpStateL = 0.0f;
    float hpStateR = 0.0f;

    // Filter coefficients
    float lpCoeff = 0.0f;
    float hpCoeff = 0.0f;

    // Sample rate
    double currentSampleRate = 44100.0;

    // DC blocking filter state
    float dcBlockStateL = 0.0f;
    float dcBlockStateR = 0.0f;
    float dcBlockCoeff = 0.995f;
};
