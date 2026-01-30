#include "NeveCompressor.h"
#include "DSPUtils.h"

constexpr float NeveCompressor::ratios[5];
constexpr float NeveCompressor::attackTimes[3];
constexpr float NeveCompressor::releaseTimes[3];

NeveCompressor::NeveCompressor()
{
}

void NeveCompressor::prepare(double sampleRate, int /*samplesPerBlock*/)
{
    currentSampleRate = sampleRate;
    updateCoefficients();
    reset();
}

void NeveCompressor::reset()
{
    envelopeL = 0.0f;
    envelopeR = 0.0f;
    currentGainReduction = 0.0f;
    autoReleaseEnv = 0.0f;
    scHpfStateL = 0.0f;
    scHpfStateR = 0.0f;
}

void NeveCompressor::updateCoefficients()
{
    ratio = ratios[ratioIndex];
    attackMs = attackTimes[attackIndex];

    if (releaseIndex < 3)
        releaseMs = releaseTimes[releaseIndex];
    // Auto release handled in processing

    attackCoeff = DSPUtils::calculateCoefficient(currentSampleRate, attackMs);
    releaseCoeff = DSPUtils::calculateCoefficient(currentSampleRate, releaseMs);
    makeupLinear = DSPUtils::decibelsToLinear(makeup);

    // Sidechain HPF coefficient (~150Hz)
    scHpfCoeff = std::exp(-2.0f * 3.14159265359f * 150.0f / static_cast<float>(currentSampleRate));
}

void NeveCompressor::setThreshold(float thresholdDb)
{
    threshold = std::clamp(thresholdDb, -40.0f, 10.0f);
}

void NeveCompressor::setRatio(int index)
{
    ratioIndex = std::clamp(index, 0, 4);
    ratio = ratios[ratioIndex];
}

void NeveCompressor::setAttack(int index)
{
    attackIndex = std::clamp(index, 0, 2);
    attackMs = attackTimes[attackIndex];
    attackCoeff = DSPUtils::calculateCoefficient(currentSampleRate, attackMs);
}

void NeveCompressor::setRelease(int index)
{
    releaseIndex = std::clamp(index, 0, 3);
    if (releaseIndex < 3)
    {
        releaseMs = releaseTimes[releaseIndex];
        releaseCoeff = DSPUtils::calculateCoefficient(currentSampleRate, releaseMs);
    }
}

void NeveCompressor::setMakeup(float makeupDb)
{
    makeup = std::clamp(makeupDb, 0.0f, 20.0f);
    makeupLinear = DSPUtils::decibelsToLinear(makeup);
}

void NeveCompressor::setSidechainHPF(bool enabled)
{
    sidechainHPF = enabled;
}

void NeveCompressor::setStereoLink(bool enabled)
{
    stereoLink = enabled;
}

float NeveCompressor::computeGain(float inputLevel)
{
    float inputDb = DSPUtils::linearToDecibels(inputLevel);

    if (inputDb < threshold)
        return 1.0f;

    // Soft knee characteristic (diode bridge style)
    // Knee width of about 6dB for smooth compression onset
    float kneeWidth = 6.0f;
    float overThreshold = inputDb - threshold;

    float gainReductionDb;
    if (overThreshold < kneeWidth)
    {
        // Soft knee region - gradual onset
        float kneeRatio = overThreshold / kneeWidth;
        float effectiveRatio = 1.0f + (ratio - 1.0f) * kneeRatio * kneeRatio;
        gainReductionDb = overThreshold * (1.0f - 1.0f / effectiveRatio);
    }
    else
    {
        // Above knee - full ratio
        float softKneeGR = kneeWidth * (1.0f - 1.0f / ratio) * 0.25f;
        gainReductionDb = softKneeGR + (overThreshold - kneeWidth) * (1.0f - 1.0f / ratio);
    }

    currentGainReduction = gainReductionDb;

    return DSPUtils::decibelsToLinear(-gainReductionDb);
}

void NeveCompressor::process(juce::AudioBuffer<float>& buffer)
{
    if (bypassed)
        return;

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    if (numChannels == 0)
        return;

    float* left = buffer.getWritePointer(0);
    float* right = numChannels > 1 ? buffer.getWritePointer(1) : nullptr;

    for (int i = 0; i < numSamples; ++i)
    {
        float inputL = left[i];
        float inputR = right ? right[i] : inputL;

        // Sidechain signal (optionally high-passed)
        float scL = inputL;
        float scR = inputR;

        if (sidechainHPF)
        {
            // Simple high-pass filter on sidechain
            float hpL = inputL - scHpfStateL;
            scHpfStateL = scHpfStateL + (1.0f - scHpfCoeff) * hpL;
            scL = hpL;

            if (right)
            {
                float hpR = inputR - scHpfStateR;
                scHpfStateR = scHpfStateR + (1.0f - scHpfCoeff) * hpR;
                scR = hpR;
            }
        }

        // Envelope detection
        float levelL = std::abs(scL);
        float levelR = std::abs(scR);

        float targetLevel;
        if (stereoLink && right)
        {
            // Use max of both channels for stereo link
            targetLevel = std::max(levelL, levelR);
        }
        else
        {
            targetLevel = levelL;  // For mono or unlinked left
        }

        // Auto-release calculation
        float currentReleaseCoeff = releaseCoeff;
        if (releaseIndex == 3)  // Auto release
        {
            // Program-dependent release: faster for transients, slower for sustained
            float autoReleaseFast = DSPUtils::calculateCoefficient(currentSampleRate, 50.0f);
            float autoReleaseSlow = DSPUtils::calculateCoefficient(currentSampleRate, 800.0f);

            // Track program density
            float targetAutoEnv = targetLevel;
            if (targetAutoEnv > autoReleaseEnv)
                autoReleaseEnv += 0.001f * (targetAutoEnv - autoReleaseEnv);
            else
                autoReleaseEnv += 0.0001f * (targetAutoEnv - autoReleaseEnv);

            // Mix fast/slow based on density
            float density = std::min(1.0f, autoReleaseEnv * 10.0f);
            currentReleaseCoeff = autoReleaseFast * (1.0f - density) + autoReleaseSlow * density;
        }

        // Envelope follower with attack/release
        if (targetLevel > envelopeL)
            envelopeL += attackCoeff * (targetLevel - envelopeL);
        else
            envelopeL += currentReleaseCoeff * (targetLevel - envelopeL);

        // Compute gain
        float gain = computeGain(envelopeL);

        // Apply gain with makeup
        left[i] = inputL * gain * makeupLinear;

        if (right)
        {
            if (stereoLink)
            {
                // Same gain for both channels
                right[i] = inputR * gain * makeupLinear;
            }
            else
            {
                // Independent envelope for right channel
                if (levelR > envelopeR)
                    envelopeR += attackCoeff * (levelR - envelopeR);
                else
                    envelopeR += currentReleaseCoeff * (levelR - envelopeR);

                float gainR = computeGain(envelopeR);
                right[i] = inputR * gainR * makeupLinear;
            }
        }
    }
}
