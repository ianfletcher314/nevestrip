#include "Transformer.h"
#include "DSPUtils.h"

Transformer::Transformer()
{
}

void Transformer::prepare(double sampleRate, int /*samplesPerBlock*/)
{
    currentSampleRate = sampleRate;
    updateCoefficients();
    reset();
}

void Transformer::reset()
{
    lpStateL = 0.0f;
    lpStateR = 0.0f;
    hpStateL = 0.0f;
    hpStateR = 0.0f;
    dcBlockStateL = 0.0f;
    dcBlockStateR = 0.0f;
}

void Transformer::updateCoefficients()
{
    // Low-frequency coloration around 100Hz
    float lpFreq = 100.0f;
    lpCoeff = 1.0f - std::exp(-2.0f * 3.14159265359f * lpFreq / static_cast<float>(currentSampleRate));

    // High-frequency "silk" around 8kHz
    float hpFreq = 8000.0f;
    hpCoeff = std::exp(-2.0f * 3.14159265359f * hpFreq / static_cast<float>(currentSampleRate));

    // DC blocking coefficient (very slow filter to remove DC)
    dcBlockCoeff = 1.0f - (20.0f / static_cast<float>(currentSampleRate));
}

void Transformer::setDrive(float drivePercent)
{
    drive = std::clamp(drivePercent / 100.0f, 0.0f, 1.0f);
    // Map drive 0-1 to gain 1-4 for saturation
    driveGain = 1.0f + drive * 3.0f;
}

float Transformer::processSample(float input)
{
    if (drive < 0.001f)
        return input;

    // Apply drive gain
    float driven = input * driveGain;

    // Asymmetric saturation (generates even harmonics like real transformers)
    // Positive half saturates differently than negative
    float asymmetry = 0.15f * drive;
    float biased = driven + asymmetry * driven * driven;

    // Soft clipping with transformer-like curve
    float saturated;
    if (biased >= 0.0f)
    {
        // Positive: softer saturation
        saturated = std::tanh(biased * (1.0f + drive));
    }
    else
    {
        // Negative: slightly harder saturation
        saturated = std::tanh(biased * (1.0f + drive * 0.8f));
    }

    // Normalize output to compensate for drive
    float output = saturated / driveGain;

    // Blend wet/dry based on drive amount
    return input * (1.0f - drive * 0.7f) + output * drive * 0.7f + output * 0.3f;
}

void Transformer::process(juce::AudioBuffer<float>& buffer)
{
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    if (numChannels == 0 || drive < 0.001f)
        return;

    // Process left channel
    float* left = buffer.getWritePointer(0);
    for (int i = 0; i < numSamples; ++i)
    {
        float sample = left[i];

        // Low-frequency enhancement (transformer "weight")
        float lowFreq = lpStateL + lpCoeff * (sample - lpStateL);
        lpStateL = lowFreq;
        float lowEnhance = (lowFreq - sample) * drive * 0.3f;

        // High-frequency "silk" (subtle presence boost)
        float highFreq = sample - hpStateL;
        hpStateL = hpStateL + (1.0f - hpCoeff) * highFreq;
        float highEnhance = highFreq * drive * 0.15f;

        // Apply saturation
        float processed = processSample(sample + lowEnhance);

        // Add high-frequency silk
        processed += highEnhance;

        // DC blocking
        float dcBlocked = processed - dcBlockStateL;
        dcBlockStateL = processed - dcBlocked * dcBlockCoeff;

        left[i] = dcBlocked;
    }

    // Process right channel if stereo
    if (numChannels > 1)
    {
        float* right = buffer.getWritePointer(1);
        for (int i = 0; i < numSamples; ++i)
        {
            float sample = right[i];

            // Low-frequency enhancement
            float lowFreq = lpStateR + lpCoeff * (sample - lpStateR);
            lpStateR = lowFreq;
            float lowEnhance = (lowFreq - sample) * drive * 0.3f;

            // High-frequency "silk"
            float highFreq = sample - hpStateR;
            hpStateR = hpStateR + (1.0f - hpCoeff) * highFreq;
            float highEnhance = highFreq * drive * 0.15f;

            // Apply saturation
            float processed = processSample(sample + lowEnhance);
            processed += highEnhance;

            // DC blocking
            float dcBlocked = processed - dcBlockStateR;
            dcBlockStateR = processed - dcBlocked * dcBlockCoeff;

            right[i] = dcBlocked;
        }
    }
}
