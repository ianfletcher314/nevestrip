#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
// Neve-style LookAndFeel
//==============================================================================
class NeveLookAndFeel : public juce::LookAndFeel_V4
{
public:
    NeveLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider& slider) override;

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                          bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown) override;

    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonW, int buttonH,
                      juce::ComboBox& box) override;

    juce::Font getComboBoxFont(juce::ComboBox& box) override;
    juce::Font getLabelFont(juce::Label& label) override;

    void setKnobColour(juce::Colour c) { knobColour = c; }

private:
    juce::Colour knobColour;
};

//==============================================================================
// VU Meter Component
//==============================================================================
class VUMeter : public juce::Component
{
public:
    VUMeter();
    void paint(juce::Graphics& g) override;
    void setLevel(float newLevel);
    void setGainReduction(float gr);
    void setShowGR(bool show) { showGR = show; }

private:
    float level = 0.0f;
    float gainReduction = 0.0f;
    float smoothedLevel = 0.0f;
    float smoothedGR = 0.0f;
    bool showGR = false;
};

//==============================================================================
// Section Header Component
//==============================================================================
class SectionHeader : public juce::Component
{
public:
    SectionHeader(const juce::String& title);
    void paint(juce::Graphics& g) override;

private:
    juce::String title;
};

//==============================================================================
// Main Editor
//==============================================================================
class NeveStripAudioProcessorEditor : public juce::AudioProcessorEditor,
                                       public juce::Timer
{
public:
    NeveStripAudioProcessorEditor(NeveStripAudioProcessor&);
    ~NeveStripAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    NeveStripAudioProcessor& audioProcessor;

    NeveLookAndFeel neveLookAndFeel;
    NeveLookAndFeel orangeKnobLookAndFeel;

    // === PREAMP SECTION ===
    SectionHeader preampHeader { "PREAMP" };
    juce::Slider inputGainSlider;
    juce::Slider outputTrimSlider;
    juce::Slider transformerDriveSlider;
    juce::ComboBox hpfCombo;
    juce::ToggleButton phaseButton { "Phase" };

    juce::Label inputGainLabel { {}, "INPUT" };
    juce::Label outputTrimLabel { {}, "TRIM" };
    juce::Label transformerLabel { {}, "DRIVE" };
    juce::Label hpfLabel { {}, "HPF" };

    // === EQ SECTION ===
    SectionHeader eqHeader { "EQ" };

    // HF
    juce::ComboBox hfFreqCombo;
    juce::Slider hfGainSlider;
    juce::Label hfLabel { {}, "HF" };

    // HM
    juce::ComboBox hmFreqCombo;
    juce::Slider hmGainSlider;
    juce::Label hmLabel { {}, "HM" };

    // LM
    juce::ComboBox lmFreqCombo;
    juce::Slider lmGainSlider;
    juce::Label lmLabel { {}, "LM" };

    // LF
    juce::ComboBox lfFreqCombo;
    juce::Slider lfGainSlider;
    juce::Label lfLabel { {}, "LF" };

    juce::ToggleButton eqBypassButton { "EQ In" };
    juce::ToggleButton eqPrePostButton { "Post" };

    // === DYNAMICS SECTION ===
    SectionHeader dynamicsHeader { "DYNAMICS" };

    juce::Slider compThresholdSlider;
    juce::ComboBox compRatioCombo;
    juce::ComboBox compAttackCombo;
    juce::ComboBox compReleaseCombo;
    juce::Slider compMakeupSlider;
    juce::ToggleButton compSCHPFButton { "SC HPF" };
    juce::ToggleButton compLinkButton { "Link" };
    juce::ToggleButton compBypassButton { "Comp" };

    juce::Label compThreshLabel { {}, "THRESH" };
    juce::Label compRatioLabel { {}, "RATIO" };
    juce::Label compAttackLabel { {}, "ATTACK" };
    juce::Label compReleaseLabel { {}, "RELEASE" };
    juce::Label compMakeupLabel { {}, "MAKEUP" };

    // Limiter
    juce::Slider limThresholdSlider;
    juce::ToggleButton limBypassButton { "Limit" };
    juce::Label limThreshLabel { {}, "LIMIT" };

    // === OUTPUT SECTION ===
    SectionHeader outputHeader { "OUTPUT" };
    juce::Slider outputLevelSlider;
    juce::ToggleButton masterBypassButton { "BYPASS" };
    juce::Label outputLabel { {}, "LEVEL" };

    // VU Meters
    VUMeter inputMeter;
    VUMeter outputMeter;
    VUMeter grMeter;
    juce::Label inputMeterLabel { {}, "IN" };
    juce::Label outputMeterLabel { {}, "OUT" };
    juce::Label grMeterLabel { {}, "GR" };

    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputTrimAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> transformerDriveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> hpfAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> phaseAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> hfFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hfGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> hmFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hmGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lmFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lmGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqBypassAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqPrePostAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> compThresholdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> compRatioAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> compAttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> compReleaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> compMakeupAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> compSCHPFAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> compLinkAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> compBypassAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> limThresholdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> limBypassAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputLevelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> masterBypassAttachment;

    void setupSlider(juce::Slider& slider, juce::Label& label);
    void setupComboBox(juce::ComboBox& combo);

    // Neve colors
    static constexpr uint32_t neveBlueGrey = 0xFF4A5568;
    static constexpr uint32_t neveDarkGrey = 0xFF2D3748;
    static constexpr uint32_t neveOrange = 0xFFED8936;
    static constexpr uint32_t neveRed = 0xFFE53E3E;
    static constexpr uint32_t neveCream = 0xFFF7FAFC;
    static constexpr uint32_t neveGold = 0xFFD69E2E;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeveStripAudioProcessorEditor)
};
