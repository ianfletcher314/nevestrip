#pragma once

#include <JuceHeader.h>

/**
 * Neve-style Compressor (2254/33609 inspired)
 *
 * Features:
 * - Classic Neve ratios: 1.5:1, 2:1, 3:1, 4:1, 6:1
 * - Stepped attack times: Fast (2ms), Medium (8ms), Slow (20ms)
 * - Stepped release times: Fast (100ms), Medium (400ms), Slow (1.2s), Auto
 * - Diode bridge detection (soft knee characteristic)
 * - Sidechain high-pass filter
 * - Stereo linking option
 */
class NeveCompressor
{
public:
    // Ratio options
    enum Ratio { RATIO_1_5 = 0, RATIO_2, RATIO_3, RATIO_4, RATIO_6 };

    // Attack options
    enum Attack { ATTACK_FAST = 0, ATTACK_MED, ATTACK_SLOW };

    // Release options
    enum Release { RELEASE_FAST = 0, RELEASE_MED, RELEASE_SLOW, RELEASE_AUTO };

    NeveCompressor();

    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    // Parameters
    void setThreshold(float thresholdDb);    // -40 to +10 dB
    void setRatio(int ratioIndex);           // 0-4 (1.5:1 to 6:1)
    void setAttack(int attackIndex);         // 0-2 (Fast/Med/Slow)
    void setRelease(int releaseIndex);       // 0-3 (Fast/Med/Slow/Auto)
    void setMakeup(float makeupDb);          // 0 to +20 dB
    void setSidechainHPF(bool enabled);      // Enable/disable sidechain HPF
    void setStereoLink(bool enabled);        // Stereo linking

    // Bypass
    void setBypass(bool shouldBypass) { bypassed = shouldBypass; }
    bool isBypassed() const { return bypassed; }

    // Metering
    float getGainReduction() const { return currentGainReduction; }

private:
    void updateCoefficients();
    float computeGain(float inputLevel);

    double currentSampleRate = 44100.0;
    bool bypassed = false;

    // Parameters
    float threshold = -20.0f;
    int ratioIndex = 2;      // Default: 3:1
    int attackIndex = 1;     // Default: Medium
    int releaseIndex = 1;    // Default: Medium
    float makeup = 0.0f;
    bool sidechainHPF = false;
    bool stereoLink = true;

    // Derived values
    float ratio = 3.0f;
    float attackMs = 8.0f;
    float releaseMs = 400.0f;
    float attackCoeff = 0.0f;
    float releaseCoeff = 0.0f;
    float makeupLinear = 1.0f;

    // State
    float envelopeL = 0.0f;
    float envelopeR = 0.0f;
    float currentGainReduction = 0.0f;
    float autoReleaseEnv = 0.0f;

    // Sidechain HPF state (high-pass at ~150Hz)
    float scHpfStateL = 0.0f;
    float scHpfStateR = 0.0f;
    float scHpfCoeff = 0.0f;

    // Lookup tables
    static constexpr float ratios[5] = { 1.5f, 2.0f, 3.0f, 4.0f, 6.0f };
    static constexpr float attackTimes[3] = { 2.0f, 8.0f, 20.0f };
    static constexpr float releaseTimes[3] = { 100.0f, 400.0f, 1200.0f };
};
