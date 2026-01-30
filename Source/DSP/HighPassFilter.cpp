#include "HighPassFilter.h"
#include "DSPUtils.h"

constexpr float HighPassFilter::frequencies[5];

HighPassFilter::HighPassFilter()
{
}

void HighPassFilter::prepare(double sampleRate, int /*samplesPerBlock*/)
{
    currentSampleRate = sampleRate;
    updateCoefficients();
    reset();
}

void HighPassFilter::reset()
{
    x1L = x2L = y1L = y2L = 0.0f;
    x1R = x2R = y1R = y2R = 0.0f;
}

void HighPassFilter::setFrequency(int freqIndex)
{
    if (freqIndex < 0 || freqIndex > 4)
        freqIndex = 0;

    currentFreq = static_cast<Frequency>(freqIndex);
    cutoffHz = frequencies[freqIndex];
    updateCoefficients();
}

void HighPassFilter::updateCoefficients()
{
    if (currentFreq == HPF_OFF || cutoffHz <= 0.0f)
    {
        // Pass-through (no filtering)
        b0 = 1.0f;
        b1 = 0.0f;
        b2 = 0.0f;
        a1 = 0.0f;
        a2 = 0.0f;
        return;
    }

    // Q of 0.707 for Butterworth response (flat passband)
    float q = 0.707f;

    DSPUtils::calculateHighPass(cutoffHz, q, currentSampleRate, b0, b1, b2, a1, a2);
}

void HighPassFilter::process(juce::AudioBuffer<float>& buffer)
{
    if (currentFreq == HPF_OFF)
        return;

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    if (numChannels == 0)
        return;

    // Process left channel
    float* left = buffer.getWritePointer(0);
    for (int i = 0; i < numSamples; ++i)
    {
        float x0 = left[i];
        float y0 = b0 * x0 + b1 * x1L + b2 * x2L - a1 * y1L - a2 * y2L;

        x2L = x1L;
        x1L = x0;
        y2L = y1L;
        y1L = y0;

        left[i] = y0;
    }

    // Process right channel if stereo
    if (numChannels > 1)
    {
        float* right = buffer.getWritePointer(1);
        for (int i = 0; i < numSamples; ++i)
        {
            float x0 = right[i];
            float y0 = b0 * x0 + b1 * x1R + b2 * x2R - a1 * y1R - a2 * y2R;

            x2R = x1R;
            x1R = x0;
            y2R = y1R;
            y1R = y0;

            right[i] = y0;
        }
    }
}
