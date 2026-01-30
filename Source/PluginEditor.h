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
// Classic VU Meter Component (Analog Style) - Wider with Scale Markings
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

    void drawScaleMarkings(juce::Graphics& g, juce::Rectangle<float> bounds);
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
// Scrollable Content Component
//==============================================================================
class NeveStripContent : public juce::Component
{
public:
    void paint(juce::Graphics& g) override;
};

//==============================================================================
// Main Editor - Vertical Channel Strip Layout with Viewport
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

    // Viewport for scrolling
    juce::Viewport viewport;
    NeveStripContent contentComponent;

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
    void drawBrushedMetalTexture(juce::Graphics& g, juce::Rectangle<float> bounds, juce::Colour baseColour);
    void layoutContent();

    // Realistic Neve Colors - based on actual hardware
    static constexpr uint32_t neveBlueGrey = 0xFF6B7B8A;      // Actual brushed steel blue-grey
    static constexpr uint32_t neveDarkBlue = 0xFF4A5A68;      // Darker steel accent
    static constexpr uint32_t neveMetalLight = 0xFF8A9AA8;    // Light brushed highlight
    static constexpr uint32_t neveMetalDark = 0xFF3A4A58;     // Dark shadow
    static constexpr uint32_t neveOrange = 0xFFD97030;        // Classic Neve orange (more muted)
    static constexpr uint32_t neveRed = 0xFFA83030;           // Classic red (less saturated)
    static constexpr uint32_t neveCream = 0xFFE8E0D0;         // Vintage cream (warmer)
    static constexpr uint32_t neveGold = 0xFFC4983C;          // Engraved gold lettering
    static constexpr uint32_t neveScrewSilver = 0xFF909898;   // Realistic screw color

    // Layout constants for vertical strip
    static constexpr int stripWidth = 380;
    static constexpr int stripHeight = 700;  // Visible height (content can scroll)
    static constexpr int contentHeight = 820; // Total content height
    static constexpr int knobSize = 54;
    static constexpr int smallKnobSize = 44;
    static constexpr int labelHeight = 14;
    static constexpr int sectionPadding = 8;
    static constexpr int headerHeight = 22;
    static constexpr int meterWidth = 32;     // Wider meters
    static constexpr int meterHeight = 120;   // Taller meters
    static constexpr int meterAreaWidth = 90; // More space for meters + labels

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeveStripAudioProcessorEditor)
};
