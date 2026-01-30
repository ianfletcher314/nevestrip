#pragma once

#include <cmath>
#include <algorithm>

namespace DSPUtils
{
    inline float linearToDecibels(float linear)
    {
        return linear > 0.0f ? 20.0f * std::log10(linear) : -100.0f;
    }

    inline float decibelsToLinear(float dB)
    {
        return std::pow(10.0f, dB / 20.0f);
    }

    inline float mapRange(float value, float inMin, float inMax, float outMin, float outMax)
    {
        return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
    }

    // Soft saturation using tanh
    inline float softClip(float sample)
    {
        return std::tanh(sample);
    }

    // Hard clip
    inline float hardClip(float sample, float threshold = 1.0f)
    {
        return std::clamp(sample, -threshold, threshold);
    }

    // Asymmetric soft clipping (transformer-like)
    inline float asymmetricSoftClip(float sample, float drive, float asymmetry = 0.1f)
    {
        // Add even harmonics via asymmetry
        float biased = sample + asymmetry * sample * sample;
        return std::tanh(biased * drive) / std::tanh(drive);
    }

    // Calculate one-pole filter coefficient for given time constant
    inline float calculateCoefficient(double sampleRate, float timeMs)
    {
        if (timeMs <= 0.0f) return 1.0f;
        return 1.0f - std::exp(-1.0f / (static_cast<float>(sampleRate) * timeMs * 0.001f));
    }

    // Smooth parameter interpolation
    inline float smoothParameter(float current, float target, float coeff)
    {
        return current + coeff * (target - current);
    }

    // Calculate biquad coefficients for a low shelf filter
    inline void calculateLowShelf(float freq, float gain, float q, double sampleRate,
                                   float& b0, float& b1, float& b2, float& a1, float& a2)
    {
        float A = std::pow(10.0f, gain / 40.0f);
        float w0 = 2.0f * 3.14159265359f * freq / static_cast<float>(sampleRate);
        float cosW0 = std::cos(w0);
        float sinW0 = std::sin(w0);
        float alpha = sinW0 / (2.0f * q);

        float a0 = (A + 1.0f) + (A - 1.0f) * cosW0 + 2.0f * std::sqrt(A) * alpha;
        b0 = (A * ((A + 1.0f) - (A - 1.0f) * cosW0 + 2.0f * std::sqrt(A) * alpha)) / a0;
        b1 = (2.0f * A * ((A - 1.0f) - (A + 1.0f) * cosW0)) / a0;
        b2 = (A * ((A + 1.0f) - (A - 1.0f) * cosW0 - 2.0f * std::sqrt(A) * alpha)) / a0;
        a1 = (-2.0f * ((A - 1.0f) + (A + 1.0f) * cosW0)) / a0;
        a2 = ((A + 1.0f) + (A - 1.0f) * cosW0 - 2.0f * std::sqrt(A) * alpha) / a0;
    }

    // Calculate biquad coefficients for a high shelf filter
    inline void calculateHighShelf(float freq, float gain, float q, double sampleRate,
                                    float& b0, float& b1, float& b2, float& a1, float& a2)
    {
        float A = std::pow(10.0f, gain / 40.0f);
        float w0 = 2.0f * 3.14159265359f * freq / static_cast<float>(sampleRate);
        float cosW0 = std::cos(w0);
        float sinW0 = std::sin(w0);
        float alpha = sinW0 / (2.0f * q);

        float a0 = (A + 1.0f) - (A - 1.0f) * cosW0 + 2.0f * std::sqrt(A) * alpha;
        b0 = (A * ((A + 1.0f) + (A - 1.0f) * cosW0 + 2.0f * std::sqrt(A) * alpha)) / a0;
        b1 = (-2.0f * A * ((A - 1.0f) + (A + 1.0f) * cosW0)) / a0;
        b2 = (A * ((A + 1.0f) + (A - 1.0f) * cosW0 - 2.0f * std::sqrt(A) * alpha)) / a0;
        a1 = (2.0f * ((A - 1.0f) - (A + 1.0f) * cosW0)) / a0;
        a2 = ((A + 1.0f) - (A - 1.0f) * cosW0 - 2.0f * std::sqrt(A) * alpha) / a0;
    }

    // Calculate biquad coefficients for a peaking EQ (bell)
    inline void calculatePeakingEQ(float freq, float gain, float q, double sampleRate,
                                    float& b0, float& b1, float& b2, float& a1, float& a2)
    {
        float A = std::pow(10.0f, gain / 40.0f);
        float w0 = 2.0f * 3.14159265359f * freq / static_cast<float>(sampleRate);
        float cosW0 = std::cos(w0);
        float sinW0 = std::sin(w0);
        float alpha = sinW0 / (2.0f * q);

        float a0 = 1.0f + alpha / A;
        b0 = (1.0f + alpha * A) / a0;
        b1 = (-2.0f * cosW0) / a0;
        b2 = (1.0f - alpha * A) / a0;
        a1 = (-2.0f * cosW0) / a0;
        a2 = (1.0f - alpha / A) / a0;
    }

    // Calculate biquad coefficients for a high-pass filter
    inline void calculateHighPass(float freq, float q, double sampleRate,
                                   float& b0, float& b1, float& b2, float& a1, float& a2)
    {
        float w0 = 2.0f * 3.14159265359f * freq / static_cast<float>(sampleRate);
        float cosW0 = std::cos(w0);
        float sinW0 = std::sin(w0);
        float alpha = sinW0 / (2.0f * q);

        float a0 = 1.0f + alpha;
        b0 = ((1.0f + cosW0) / 2.0f) / a0;
        b1 = (-(1.0f + cosW0)) / a0;
        b2 = ((1.0f + cosW0) / 2.0f) / a0;
        a1 = (-2.0f * cosW0) / a0;
        a2 = (1.0f - alpha) / a0;
    }
}
