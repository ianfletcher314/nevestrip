#include "NeveLimiter.h"
#include "DSPUtils.h"

NeveLimiter::NeveLimiter()
{
}

void NeveLimiter::prepare(double sampleRate, int /*samplesPerBlock*/)
{
    currentSampleRate = sampleRate;

    // Very fast attack (0.1ms) for limiting
    attackCoeff = DSPUtils::calculateCoefficient(currentSampleRate, 0.1f);
    // Moderate release (50ms)
    releaseCoeff = DSPUtils::calculateCoefficient(currentSampleRate, 50.0f);

    reset();
}

void NeveLimiter::reset()
{
    envelopeL = 0.0f;
    envelopeR = 0.0f;
    currentGainReduction = 0.0f;
}

void NeveLimiter::setThreshold(float thresholdDb)
{
    threshold = std::clamp(thresholdDb, -20.0f, 0.0f);
    thresholdLinear = DSPUtils::decibelsToLinear(threshold);
}

void NeveLimiter::process(juce::AudioBuffer<float>& buffer)
{
    if (bypassed)
        return;

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    if (numChannels == 0)
        return;

    float* left = buffer.getWritePointer(0);
    float* right = numChannels > 1 ? buffer.getWritePointer(1) : nullptr;

    float maxGR = 0.0f;

    for (int i = 0; i < numSamples; ++i)
    {
        float inputL = left[i];
        float inputR = right ? right[i] : inputL;

        // Peak detection
        float peakL = std::abs(inputL);
        float peakR = std::abs(inputR);
        float peak = std::max(peakL, peakR);

        // Envelope follower
        if (peak > envelopeL)
            envelopeL += attackCoeff * (peak - envelopeL);
        else
            envelopeL += releaseCoeff * (peak - envelopeL);

        // Calculate gain reduction
        float gain = 1.0f;
        if (envelopeL > thresholdLinear)
        {
            gain = thresholdLinear / envelopeL;

            // Soft saturation on the gain reduction for more musical limiting
            // This mimics diode bridge behavior
            float overRatio = envelopeL / thresholdLinear;
            if (overRatio > 1.5f)
            {
                // Add soft clipping character for extreme limiting
                float excess = overRatio - 1.5f;
                gain *= (1.0f / (1.0f + excess * 0.5f));
            }
        }

        // Track GR for metering
        float gr = DSPUtils::linearToDecibels(gain);
        if (std::abs(gr) > maxGR)
            maxGR = std::abs(gr);

        // Apply limiting
        left[i] = inputL * gain;
        if (right)
            right[i] = inputR * gain;

        // Final soft clip safety (prevents any overs)
        if (std::abs(left[i]) > thresholdLinear * 1.1f)
            left[i] = std::tanh(left[i] / thresholdLinear) * thresholdLinear;
        if (right && std::abs(right[i]) > thresholdLinear * 1.1f)
            right[i] = std::tanh(right[i] / thresholdLinear) * thresholdLinear;
    }

    currentGainReduction = maxGR;
}
