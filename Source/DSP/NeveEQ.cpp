#include "NeveEQ.h"
#include "DSPUtils.h"

constexpr float NeveEQ::hfFreqs[3];
constexpr float NeveEQ::hmFreqs[5];
constexpr float NeveEQ::lmFreqs[5];
constexpr float NeveEQ::lfFreqs[4];

NeveEQ::NeveEQ()
{
}

void NeveEQ::prepare(double sampleRate, int /*samplesPerBlock*/)
{
    currentSampleRate = sampleRate;
    updateHFCoefficients();
    updateHMCoefficients();
    updateLMCoefficients();
    updateLFCoefficients();
    reset();
}

void NeveEQ::reset()
{
    hfX1L = hfX2L = hfY1L = hfY2L = 0.0f;
    hfX1R = hfX2R = hfY1R = hfY2R = 0.0f;
    hmX1L = hmX2L = hmY1L = hmY2L = 0.0f;
    hmX1R = hmX2R = hmY1R = hmY2R = 0.0f;
    lmX1L = lmX2L = lmY1L = lmY2L = 0.0f;
    lmX1R = lmX2R = lmY1R = lmY2R = 0.0f;
    lfX1L = lfX2L = lfY1L = lfY2L = 0.0f;
    lfX1R = lfX2R = lfY1R = lfY2R = 0.0f;
}

float NeveEQ::calculateProportionalQ(float gainDb, float baseQ)
{
    // Neve-style proportional Q: Q narrows as gain increases
    // This creates the characteristic "musical" sound
    float absGain = std::abs(gainDb);
    if (absGain < 0.5f)
        return baseQ;

    // Q increases (narrower) with more gain
    float qMultiplier = 1.0f + (absGain / 16.0f) * 1.5f;
    return baseQ * qMultiplier;
}

// HF Section (High Shelf)
void NeveEQ::setHFFreq(int index)
{
    hfFreqIndex = std::clamp(index, 0, 2);
    updateHFCoefficients();
}

void NeveEQ::setHFGain(float gainDb)
{
    hfGain = std::clamp(gainDb, -16.0f, 16.0f);
    updateHFCoefficients();
}

void NeveEQ::updateHFCoefficients()
{
    if (std::abs(hfGain) < 0.1f)
    {
        hfB0 = 1.0f; hfB1 = 0.0f; hfB2 = 0.0f; hfA1 = 0.0f; hfA2 = 0.0f;
        return;
    }

    float freq = hfFreqs[hfFreqIndex];
    // Neve shelves have a gentle slope with smooth Q
    float q = 0.6f;

    DSPUtils::calculateHighShelf(freq, hfGain, q, currentSampleRate,
                                  hfB0, hfB1, hfB2, hfA1, hfA2);
}

// HM Section (Parametric Bell)
void NeveEQ::setHMFreq(int index)
{
    hmFreqIndex = std::clamp(index, 0, 4);
    updateHMCoefficients();
}

void NeveEQ::setHMGain(float gainDb)
{
    hmGain = std::clamp(gainDb, -12.0f, 12.0f);
    updateHMCoefficients();
}

void NeveEQ::updateHMCoefficients()
{
    if (std::abs(hmGain) < 0.1f)
    {
        hmB0 = 1.0f; hmB1 = 0.0f; hmB2 = 0.0f; hmA1 = 0.0f; hmA2 = 0.0f;
        return;
    }

    float freq = hmFreqs[hmFreqIndex];
    float q = calculateProportionalQ(hmGain, 1.0f);

    DSPUtils::calculatePeakingEQ(freq, hmGain, q, currentSampleRate,
                                  hmB0, hmB1, hmB2, hmA1, hmA2);
}

// LM Section (Parametric Bell)
void NeveEQ::setLMFreq(int index)
{
    lmFreqIndex = std::clamp(index, 0, 4);
    updateLMCoefficients();
}

void NeveEQ::setLMGain(float gainDb)
{
    lmGain = std::clamp(gainDb, -16.0f, 16.0f);
    updateLMCoefficients();
}

void NeveEQ::updateLMCoefficients()
{
    if (std::abs(lmGain) < 0.1f)
    {
        lmB0 = 1.0f; lmB1 = 0.0f; lmB2 = 0.0f; lmA1 = 0.0f; lmA2 = 0.0f;
        return;
    }

    float freq = lmFreqs[lmFreqIndex];
    float q = calculateProportionalQ(lmGain, 0.8f);

    DSPUtils::calculatePeakingEQ(freq, lmGain, q, currentSampleRate,
                                  lmB0, lmB1, lmB2, lmA1, lmA2);
}

// LF Section (Low Shelf)
void NeveEQ::setLFFreq(int index)
{
    lfFreqIndex = std::clamp(index, 0, 3);
    updateLFCoefficients();
}

void NeveEQ::setLFGain(float gainDb)
{
    lfGain = std::clamp(gainDb, -16.0f, 16.0f);
    updateLFCoefficients();
}

void NeveEQ::updateLFCoefficients()
{
    if (std::abs(lfGain) < 0.1f)
    {
        lfB0 = 1.0f; lfB1 = 0.0f; lfB2 = 0.0f; lfA1 = 0.0f; lfA2 = 0.0f;
        return;
    }

    float freq = lfFreqs[lfFreqIndex];
    // Neve low shelves have a characteristic gentle slope
    float q = 0.5f;

    DSPUtils::calculateLowShelf(freq, lfGain, q, currentSampleRate,
                                 lfB0, lfB1, lfB2, lfA1, lfA2);
}

void NeveEQ::process(juce::AudioBuffer<float>& buffer)
{
    if (bypassed)
        return;

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    if (numChannels == 0)
        return;

    // Check if all bands are at unity (no processing needed)
    bool needsProcessing = (std::abs(hfGain) >= 0.1f ||
                            std::abs(hmGain) >= 0.1f ||
                            std::abs(lmGain) >= 0.1f ||
                            std::abs(lfGain) >= 0.1f);

    if (!needsProcessing)
        return;

    // Process left channel
    float* left = buffer.getWritePointer(0);
    for (int i = 0; i < numSamples; ++i)
    {
        float sample = left[i];

        // LF Shelf
        if (std::abs(lfGain) >= 0.1f)
        {
            float y = lfB0 * sample + lfB1 * lfX1L + lfB2 * lfX2L - lfA1 * lfY1L - lfA2 * lfY2L;
            lfX2L = lfX1L; lfX1L = sample;
            lfY2L = lfY1L; lfY1L = y;
            sample = y;
        }

        // LM Bell
        if (std::abs(lmGain) >= 0.1f)
        {
            float y = lmB0 * sample + lmB1 * lmX1L + lmB2 * lmX2L - lmA1 * lmY1L - lmA2 * lmY2L;
            lmX2L = lmX1L; lmX1L = sample;
            lmY2L = lmY1L; lmY1L = y;
            sample = y;
        }

        // HM Bell
        if (std::abs(hmGain) >= 0.1f)
        {
            float y = hmB0 * sample + hmB1 * hmX1L + hmB2 * hmX2L - hmA1 * hmY1L - hmA2 * hmY2L;
            hmX2L = hmX1L; hmX1L = sample;
            hmY2L = hmY1L; hmY1L = y;
            sample = y;
        }

        // HF Shelf
        if (std::abs(hfGain) >= 0.1f)
        {
            float y = hfB0 * sample + hfB1 * hfX1L + hfB2 * hfX2L - hfA1 * hfY1L - hfA2 * hfY2L;
            hfX2L = hfX1L; hfX1L = sample;
            hfY2L = hfY1L; hfY1L = y;
            sample = y;
        }

        left[i] = sample;
    }

    // Process right channel if stereo
    if (numChannels > 1)
    {
        float* right = buffer.getWritePointer(1);
        for (int i = 0; i < numSamples; ++i)
        {
            float sample = right[i];

            // LF Shelf
            if (std::abs(lfGain) >= 0.1f)
            {
                float y = lfB0 * sample + lfB1 * lfX1R + lfB2 * lfX2R - lfA1 * lfY1R - lfA2 * lfY2R;
                lfX2R = lfX1R; lfX1R = sample;
                lfY2R = lfY1R; lfY1R = y;
                sample = y;
            }

            // LM Bell
            if (std::abs(lmGain) >= 0.1f)
            {
                float y = lmB0 * sample + lmB1 * lmX1R + lmB2 * lmX2R - lmA1 * lmY1R - lmA2 * lmY2R;
                lmX2R = lmX1R; lmX1R = sample;
                lmY2R = lmY1R; lmY1R = y;
                sample = y;
            }

            // HM Bell
            if (std::abs(hmGain) >= 0.1f)
            {
                float y = hmB0 * sample + hmB1 * hmX1R + hmB2 * hmX2R - hmA1 * hmY1R - hmA2 * hmY2R;
                hmX2R = hmX1R; hmX1R = sample;
                hmY2R = hmY1R; hmY1R = y;
                sample = y;
            }

            // HF Shelf
            if (std::abs(hfGain) >= 0.1f)
            {
                float y = hfB0 * sample + hfB1 * hfX1R + hfB2 * hfX2R - hfA1 * hfY1R - hfA2 * hfY2R;
                hfX2R = hfX1R; hfX1R = sample;
                hfY2R = hfY1R; hfY1R = y;
                sample = y;
            }

            right[i] = sample;
        }
    }
}
