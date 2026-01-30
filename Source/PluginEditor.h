#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
// Neve-style LookAndFeel with Chicken Head Knobs
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
    void setIsChickenHead(bool ch) { isChickenHead = ch; }

private:
    juce::Colour knobColour;
    bool isChickenHead = true;
};

//==============================================================================
// Classic VU Meter Component (Analog Style)
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
// Section Header Component (Metal Strip Style)
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
// Main Editor - Vertical Channel Strip Layout
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
    NeveLookAndFeel redKnobLookAndFeel;

    // === PREAMP/INPUT SECTION (Top) ===
    SectionHeader preampHeader { "INPUT" };
    juce::Slider inputGainSlider;
    juce::Slider outputTrimSlider;
    juce::Slider transformerDriveSlider;
    juce::ComboBox hpfCombo;
    juce::ToggleButton phaseButton { "Phase" };

    juce::Label inputGainLabel { {}, "GAIN" };
    juce::Label outputTrimLabel { {}, "TRIM" };
    juce::Label transformerLabel { {}, "DRIVE" };
    juce::Label hpfLabel { {}, "HPF" };

    // === EQ SECTION (Middle) ===
    SectionHeader eqHeader { "EQUALISER" };

    // HF
    juce::ComboBox hfFreqCombo;
    juce::Slider hfGainSlider;
    juce::Label hfLabel { {}, "HF" };

    // HM
    juce::ComboBox hmFreqCombo;
    juce::Slider hmGainSlider;
    juce::Label hmLabel { {}, "HI MID" };

    // LM
    juce::ComboBox lmFreqCombo;
    juce::Slider lmGainSlider;
    juce::Label lmLabel { {}, "LO MID" };

    // LF
    juce::ComboBox lfFreqCombo;
    juce::Slider lfGainSlider;
    juce::Label lfLabel { {}, "LF" };

    juce::ToggleButton eqBypassButton { "EQ IN" };
    juce::ToggleButton eqPrePostButton { "PRE" };

    // === DYNAMICS SECTION ===
    SectionHeader dynamicsHeader { "DYNAMICS" };

    juce::Slider compThresholdSlider;
    juce::ComboBox compRatioCombo;
    juce::ComboBox compAttackCombo;
    juce::ComboBox compReleaseCombo;
    juce::Slider compMakeupSlider;
    juce::ToggleButton compSCHPFButton { "SC HPF" };
    juce::ToggleButton compLinkButton { "LINK" };
    juce::ToggleButton compBypassButton { "COMP" };

    juce::Label compThreshLabel { {}, "THRESH" };
    juce::Label compRatioLabel { {}, "RATIO" };
    juce::Label compAttackLabel { {}, "ATK" };
    juce::Label compReleaseLabel { {}, "REL" };
    juce::Label compMakeupLabel { {}, "GAIN" };

    // Limiter
    juce::Slider limThresholdSlider;
    juce::ToggleButton limBypassButton { "LIMIT" };
    juce::Label limThreshLabel { {}, "LIMIT" };

    // === OUTPUT SECTION (Bottom) ===
    SectionHeader outputHeader { "OUTPUT" };
    juce::Slider outputLevelSlider;
    juce::ToggleButton masterBypassButton { "BYPASS" };
    juce::Label outputLabel { {}, "OUTPUT" };

    // VU Meters (Side)
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
    void drawRackScrew(juce::Graphics& g, int x, int y);

    // Classic Neve Colors
    static constexpr uint32_t neveBlueGrey = 0xFF5B6B7A;      // Classic blue-grey metal
    static constexpr uint32_t neveDarkBlue = 0xFF3D4A56;      // Darker accent
    static constexpr uint32_t neveMetalLight = 0xFF7A8A99;    // Light metal highlight
    static constexpr uint32_t neveOrange = 0xFFE67E22;        // Classic orange
    static constexpr uint32_t neveRed = 0xFFCC3333;           // Classic red
    static constexpr uint32_t neveCream = 0xFFF5F0E6;         // Vintage cream
    static constexpr uint32_t neveGold = 0xFFD4A84B;          // Gold lettering
    static constexpr uint32_t neveScrewSilver = 0xFFA0A8B0;   // Screw color

    // Layout constants for vertical strip
    static constexpr int stripWidth = 340;
    static constexpr int stripHeight = 780;
    static constexpr int knobSize = 50;
    static constexpr int smallKnobSize = 42;
    static constexpr int labelHeight = 14;
    static constexpr int sectionPadding = 6;
    static constexpr int headerHeight = 20;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeveStripAudioProcessorEditor)
};
