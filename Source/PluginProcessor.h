#pragma once

#include <JuceHeader.h>
#include "DSP/Transformer.h"
#include "DSP/HighPassFilter.h"
#include "DSP/NeveEQ.h"
#include "DSP/NeveCompressor.h"
#include "DSP/NeveLimiter.h"

class NeveStripAudioProcessor : public juce::AudioProcessor
{
public:
    NeveStripAudioProcessor();
    ~NeveStripAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }

    // Metering
    float getInputLevel() const { return inputLevelMeter.load(); }
    float getOutputLevel() const { return outputLevelMeter.load(); }
    float getCompressorGR() const { return compressor.getGainReduction(); }
    float getLimiterGR() const { return limiter.getGainReduction(); }

private:
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // DSP Modules
    Transformer transformer;
    HighPassFilter hpf;
    NeveEQ eq;
    NeveCompressor compressor;
    NeveLimiter limiter;

    // === PREAMP SECTION ===
    std::atomic<float>* inputGain = nullptr;
    std::atomic<float>* outputTrim = nullptr;
    std::atomic<float>* phase = nullptr;
    std::atomic<float>* hpfFreq = nullptr;
    std::atomic<float>* transformerDrive = nullptr;

    // === EQ SECTION ===
    std::atomic<float>* hfFreq = nullptr;
    std::atomic<float>* hfGain = nullptr;
    std::atomic<float>* hmFreq = nullptr;
    std::atomic<float>* hmGain = nullptr;
    std::atomic<float>* lmFreq = nullptr;
    std::atomic<float>* lmGain = nullptr;
    std::atomic<float>* lfFreq = nullptr;
    std::atomic<float>* lfGain = nullptr;
    std::atomic<float>* eqBypass = nullptr;
    std::atomic<float>* eqPrePost = nullptr;

    // === DYNAMICS SECTION ===
    std::atomic<float>* compThreshold = nullptr;
    std::atomic<float>* compRatio = nullptr;
    std::atomic<float>* compAttack = nullptr;
    std::atomic<float>* compRelease = nullptr;
    std::atomic<float>* compMakeup = nullptr;
    std::atomic<float>* compSCHPF = nullptr;
    std::atomic<float>* compLink = nullptr;
    std::atomic<float>* compBypass = nullptr;

    std::atomic<float>* limThreshold = nullptr;
    std::atomic<float>* limBypass = nullptr;

    // === OUTPUT ===
    std::atomic<float>* outputLevel = nullptr;
    std::atomic<float>* masterBypass = nullptr;

    // Metering
    std::atomic<float> inputLevelMeter { 0.0f };
    std::atomic<float> outputLevelMeter { 0.0f };

    // Smoothed parameters
    juce::SmoothedValue<float> smoothInputGain;
    juce::SmoothedValue<float> smoothOutputTrim;
    juce::SmoothedValue<float> smoothOutputLevel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeveStripAudioProcessor)
};
