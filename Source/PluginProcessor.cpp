#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DSP/DSPUtils.h"

NeveStripAudioProcessor::NeveStripAudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    // === PREAMP SECTION ===
    inputGain = apvts.getRawParameterValue("inputGain");
    outputTrim = apvts.getRawParameterValue("outputTrim");
    phase = apvts.getRawParameterValue("phase");
    hpfFreq = apvts.getRawParameterValue("hpfFreq");
    transformerDrive = apvts.getRawParameterValue("transformerDrive");

    // === EQ SECTION ===
    hfFreq = apvts.getRawParameterValue("hfFreq");
    hfGain = apvts.getRawParameterValue("hfGain");
    hmFreq = apvts.getRawParameterValue("hmFreq");
    hmGain = apvts.getRawParameterValue("hmGain");
    lmFreq = apvts.getRawParameterValue("lmFreq");
    lmGain = apvts.getRawParameterValue("lmGain");
    lfFreq = apvts.getRawParameterValue("lfFreq");
    lfGain = apvts.getRawParameterValue("lfGain");
    eqBypass = apvts.getRawParameterValue("eqBypass");
    eqPrePost = apvts.getRawParameterValue("eqPrePost");

    // === DYNAMICS SECTION ===
    compThreshold = apvts.getRawParameterValue("compThreshold");
    compRatio = apvts.getRawParameterValue("compRatio");
    compAttack = apvts.getRawParameterValue("compAttack");
    compRelease = apvts.getRawParameterValue("compRelease");
    compMakeup = apvts.getRawParameterValue("compMakeup");
    compSCHPF = apvts.getRawParameterValue("compSCHPF");
    compLink = apvts.getRawParameterValue("compLink");
    compBypass = apvts.getRawParameterValue("compBypass");

    limThreshold = apvts.getRawParameterValue("limThreshold");
    limBypass = apvts.getRawParameterValue("limBypass");

    // === OUTPUT ===
    outputLevel = apvts.getRawParameterValue("outputLevel");
    masterBypass = apvts.getRawParameterValue("masterBypass");
}

NeveStripAudioProcessor::~NeveStripAudioProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout NeveStripAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // === PREAMP SECTION ===
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("inputGain", 1), "Input Gain",
        juce::NormalisableRange<float>(0.0f, 60.0f, 0.1f), 0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("outputTrim", 1), "Output Trim",
        juce::NormalisableRange<float>(-20.0f, 10.0f, 0.1f), 0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("phase", 1), "Phase", false));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("hpfFreq", 1), "HPF Frequency",
        juce::StringArray{ "Off", "50 Hz", "80 Hz", "160 Hz", "300 Hz" }, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("transformerDrive", 1), "Transformer Drive",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    // === EQ SECTION ===
    // HF
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("hfFreq", 1), "HF Frequency",
        juce::StringArray{ "10 kHz", "12 kHz", "16 kHz" }, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("hfGain", 1), "HF Gain",
        juce::NormalisableRange<float>(-16.0f, 16.0f, 0.1f), 0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    // HM
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("hmFreq", 1), "HM Frequency",
        juce::StringArray{ "1.5 kHz", "2.4 kHz", "3.2 kHz", "4.8 kHz", "7.2 kHz" }, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("hmGain", 1), "HM Gain",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    // LM
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("lmFreq", 1), "LM Frequency",
        juce::StringArray{ "220 Hz", "360 Hz", "700 Hz", "1.6 kHz", "3.2 kHz" }, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("lmGain", 1), "LM Gain",
        juce::NormalisableRange<float>(-16.0f, 16.0f, 0.1f), 0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    // LF
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("lfFreq", 1), "LF Frequency",
        juce::StringArray{ "35 Hz", "60 Hz", "110 Hz", "220 Hz" }, 1));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("lfGain", 1), "LF Gain",
        juce::NormalisableRange<float>(-16.0f, 16.0f, 0.1f), 0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("eqBypass", 1), "EQ Bypass", false));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("eqPrePost", 1), "EQ Pre/Post", false));  // false = pre, true = post

    // === DYNAMICS SECTION ===
    // Compressor
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("compThreshold", 1), "Comp Threshold",
        juce::NormalisableRange<float>(-40.0f, 10.0f, 0.1f), -20.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("compRatio", 1), "Comp Ratio",
        juce::StringArray{ "1.5:1", "2:1", "3:1", "4:1", "6:1" }, 2));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("compAttack", 1), "Comp Attack",
        juce::StringArray{ "Fast (2ms)", "Med (8ms)", "Slow (20ms)" }, 1));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("compRelease", 1), "Comp Release",
        juce::StringArray{ "Fast (100ms)", "Med (400ms)", "Slow (1.2s)", "Auto" }, 1));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("compMakeup", 1), "Comp Makeup",
        juce::NormalisableRange<float>(0.0f, 20.0f, 0.1f), 0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("compSCHPF", 1), "Comp SC HPF", false));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("compLink", 1), "Comp Stereo Link", true));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("compBypass", 1), "Comp Bypass", true));

    // Limiter
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("limThreshold", 1), "Limiter Threshold",
        juce::NormalisableRange<float>(-20.0f, 0.0f, 0.1f), 0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("limBypass", 1), "Limiter Bypass", true));

    // === OUTPUT ===
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("outputLevel", 1), "Output Level",
        juce::NormalisableRange<float>(-20.0f, 10.0f, 0.1f), 0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("masterBypass", 1), "Master Bypass", false));

    return { params.begin(), params.end() };
}

const juce::String NeveStripAudioProcessor::getName() const { return JucePlugin_Name; }
bool NeveStripAudioProcessor::acceptsMidi() const { return false; }
bool NeveStripAudioProcessor::producesMidi() const { return false; }
bool NeveStripAudioProcessor::isMidiEffect() const { return false; }
double NeveStripAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int NeveStripAudioProcessor::getNumPrograms() { return 1; }
int NeveStripAudioProcessor::getCurrentProgram() { return 0; }
void NeveStripAudioProcessor::setCurrentProgram(int index) { juce::ignoreUnused(index); }
const juce::String NeveStripAudioProcessor::getProgramName(int index) { juce::ignoreUnused(index); return {}; }
void NeveStripAudioProcessor::changeProgramName(int index, const juce::String& newName) { juce::ignoreUnused(index, newName); }

void NeveStripAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Prepare DSP modules
    transformer.prepare(sampleRate, samplesPerBlock);
    hpf.prepare(sampleRate, samplesPerBlock);
    eq.prepare(sampleRate, samplesPerBlock);
    compressor.prepare(sampleRate, samplesPerBlock);
    limiter.prepare(sampleRate, samplesPerBlock);

    // Prepare smoothed values
    smoothInputGain.reset(sampleRate, 0.02);
    smoothOutputTrim.reset(sampleRate, 0.02);
    smoothOutputLevel.reset(sampleRate, 0.02);
}

void NeveStripAudioProcessor::releaseResources()
{
    transformer.reset();
    hpf.reset();
    eq.reset();
    compressor.reset();
    limiter.reset();
}

bool NeveStripAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
    return true;
}

void NeveStripAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Check master bypass
    if (masterBypass->load() > 0.5f)
        return;

    const int numSamples = buffer.getNumSamples();

    // Measure input level
    float inLevel = 0.0f;
    for (int ch = 0; ch < totalNumInputChannels; ++ch)
        inLevel = std::max(inLevel, buffer.getMagnitude(ch, 0, numSamples));
    inputLevel.store(inLevel);

    // === PREAMP SECTION ===

    // Input Gain
    float targetInputGain = DSPUtils::decibelsToLinear(inputGain->load());
    smoothInputGain.setTargetValue(targetInputGain);

    // Output Trim
    float targetOutputTrim = DSPUtils::decibelsToLinear(outputTrim->load());
    smoothOutputTrim.setTargetValue(targetOutputTrim);

    // Apply input gain with smoothing
    for (int ch = 0; ch < totalNumInputChannels; ++ch)
    {
        float* data = buffer.getWritePointer(ch);
        for (int i = 0; i < numSamples; ++i)
        {
            float gain = (ch == 0) ? smoothInputGain.getNextValue() : smoothInputGain.getCurrentValue();
            data[i] *= gain;
        }
    }

    // Phase inversion
    if (phase->load() > 0.5f)
    {
        for (int ch = 0; ch < totalNumInputChannels; ++ch)
        {
            float* data = buffer.getWritePointer(ch);
            for (int i = 0; i < numSamples; ++i)
                data[i] = -data[i];
        }
    }

    // High-pass filter
    hpf.setFrequency(static_cast<int>(hpfFreq->load()));
    hpf.process(buffer);

    // Transformer drive
    transformer.setDrive(transformerDrive->load());
    transformer.process(buffer);

    // Apply output trim
    for (int ch = 0; ch < totalNumInputChannels; ++ch)
    {
        float* data = buffer.getWritePointer(ch);
        for (int i = 0; i < numSamples; ++i)
        {
            float trim = (ch == 0) ? smoothOutputTrim.getNextValue() : smoothOutputTrim.getCurrentValue();
            data[i] *= trim;
        }
    }

    // === EQ + DYNAMICS ===

    // Update EQ parameters
    eq.setBypass(eqBypass->load() > 0.5f);
    eq.setHFFreq(static_cast<int>(hfFreq->load()));
    eq.setHFGain(hfGain->load());
    eq.setHMFreq(static_cast<int>(hmFreq->load()));
    eq.setHMGain(hmGain->load());
    eq.setLMFreq(static_cast<int>(lmFreq->load()));
    eq.setLMGain(lmGain->load());
    eq.setLFFreq(static_cast<int>(lfFreq->load()));
    eq.setLFGain(lfGain->load());

    // Update compressor parameters
    compressor.setBypass(compBypass->load() > 0.5f);
    compressor.setThreshold(compThreshold->load());
    compressor.setRatio(static_cast<int>(compRatio->load()));
    compressor.setAttack(static_cast<int>(compAttack->load()));
    compressor.setRelease(static_cast<int>(compRelease->load()));
    compressor.setMakeup(compMakeup->load());
    compressor.setSidechainHPF(compSCHPF->load() > 0.5f);
    compressor.setStereoLink(compLink->load() > 0.5f);

    // Update limiter parameters
    limiter.setBypass(limBypass->load() > 0.5f);
    limiter.setThreshold(limThreshold->load());

    // EQ Pre/Post routing
    bool eqPost = eqPrePost->load() > 0.5f;

    if (!eqPost)
    {
        // EQ before dynamics (Pre)
        eq.process(buffer);
        compressor.process(buffer);
        limiter.process(buffer);
    }
    else
    {
        // EQ after dynamics (Post)
        compressor.process(buffer);
        limiter.process(buffer);
        eq.process(buffer);
    }

    // === OUTPUT SECTION ===

    float targetOutput = DSPUtils::decibelsToLinear(outputLevel->load());
    smoothOutputLevel.setTargetValue(targetOutput);

    for (int ch = 0; ch < totalNumInputChannels; ++ch)
    {
        float* data = buffer.getWritePointer(ch);
        for (int i = 0; i < numSamples; ++i)
        {
            float outGain = (ch == 0) ? smoothOutputLevel.getNextValue() : smoothOutputLevel.getCurrentValue();
            data[i] *= outGain;
        }
    }

    // Measure output level
    float outLevel = 0.0f;
    for (int ch = 0; ch < totalNumInputChannels; ++ch)
        outLevel = std::max(outLevel, buffer.getMagnitude(ch, 0, numSamples));
    outputLevelMeter.store(outLevel);
}

void NeveStripAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void NeveStripAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessorEditor* NeveStripAudioProcessor::createEditor()
{
    return new NeveStripAudioProcessorEditor(*this);
}

bool NeveStripAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NeveStripAudioProcessor();
}
