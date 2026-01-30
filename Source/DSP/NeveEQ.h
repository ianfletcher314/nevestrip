#pragma once

#include <JuceHeader.h>

/**
 * Neve-style 4-band EQ (1073/1084 inspired)
 *
 * HF: High Frequency Shelf (10kHz, 12kHz, 16kHz) +/-16dB
 * HM: High-Mid Parametric (1.5k, 2.4k, 3.2k, 4.8k, 7.2kHz) +/-12dB
 * LM: Low-Mid Parametric (220Hz, 360Hz, 700Hz, 1.6kHz, 3.2kHz) +/-16dB
 * LF: Low Frequency Shelf (35Hz, 60Hz, 110Hz, 220Hz) +/-16dB
 *
 * Features:
 * - Proportional Q (Q narrows with boost - Neve characteristic)
 * - Musical, inductor-like curves
 * - Smooth, never harsh
 */
class NeveEQ
{
public:
    // HF frequency options
    enum HFFreq { HF_10K = 0, HF_12K, HF_16K };

    // HM frequency options
    enum HMFreq { HM_1K5 = 0, HM_2K4, HM_3K2, HM_4K8, HM_7K2 };

    // LM frequency options
    enum LMFreq { LM_220 = 0, LM_360, LM_700, LM_1K6, LM_3K2 };

    // LF frequency options
    enum LFFreq { LF_35 = 0, LF_60, LF_110, LF_220 };

    NeveEQ();

    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    // HF Section
    void setHFFreq(int index);
    void setHFGain(float gainDb);

    // HM Section
    void setHMFreq(int index);
    void setHMGain(float gainDb);

    // LM Section
    void setLMFreq(int index);
    void setLMGain(float gainDb);

    // LF Section
    void setLFFreq(int index);
    void setLFGain(float gainDb);

    // Bypass
    void setBypass(bool shouldBypass) { bypassed = shouldBypass; }
    bool isBypassed() const { return bypassed; }

private:
    void updateHFCoefficients();
    void updateHMCoefficients();
    void updateLMCoefficients();
    void updateLFCoefficients();

    // Calculate proportional Q based on gain
    float calculateProportionalQ(float gainDb, float baseQ);

    double currentSampleRate = 44100.0;
    bool bypassed = false;

    // HF parameters and state
    int hfFreqIndex = 0;
    float hfGain = 0.0f;
    float hfB0 = 1.0f, hfB1 = 0.0f, hfB2 = 0.0f, hfA1 = 0.0f, hfA2 = 0.0f;
    float hfX1L = 0.0f, hfX2L = 0.0f, hfY1L = 0.0f, hfY2L = 0.0f;
    float hfX1R = 0.0f, hfX2R = 0.0f, hfY1R = 0.0f, hfY2R = 0.0f;

    // HM parameters and state
    int hmFreqIndex = 0;
    float hmGain = 0.0f;
    float hmB0 = 1.0f, hmB1 = 0.0f, hmB2 = 0.0f, hmA1 = 0.0f, hmA2 = 0.0f;
    float hmX1L = 0.0f, hmX2L = 0.0f, hmY1L = 0.0f, hmY2L = 0.0f;
    float hmX1R = 0.0f, hmX2R = 0.0f, hmY1R = 0.0f, hmY2R = 0.0f;

    // LM parameters and state
    int lmFreqIndex = 0;
    float lmGain = 0.0f;
    float lmB0 = 1.0f, lmB1 = 0.0f, lmB2 = 0.0f, lmA1 = 0.0f, lmA2 = 0.0f;
    float lmX1L = 0.0f, lmX2L = 0.0f, lmY1L = 0.0f, lmY2L = 0.0f;
    float lmX1R = 0.0f, lmX2R = 0.0f, lmY1R = 0.0f, lmY2R = 0.0f;

    // LF parameters and state
    int lfFreqIndex = 0;
    float lfGain = 0.0f;
    float lfB0 = 1.0f, lfB1 = 0.0f, lfB2 = 0.0f, lfA1 = 0.0f, lfA2 = 0.0f;
    float lfX1L = 0.0f, lfX2L = 0.0f, lfY1L = 0.0f, lfY2L = 0.0f;
    float lfX1R = 0.0f, lfX2R = 0.0f, lfY1R = 0.0f, lfY2R = 0.0f;

    // Frequency lookup tables
    static constexpr float hfFreqs[3] = { 10000.0f, 12000.0f, 16000.0f };
    static constexpr float hmFreqs[5] = { 1500.0f, 2400.0f, 3200.0f, 4800.0f, 7200.0f };
    static constexpr float lmFreqs[5] = { 220.0f, 360.0f, 700.0f, 1600.0f, 3200.0f };
    static constexpr float lfFreqs[4] = { 35.0f, 60.0f, 110.0f, 220.0f };
};
