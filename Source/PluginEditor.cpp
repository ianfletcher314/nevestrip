#include "PluginEditor.h"

//==============================================================================
// NeveLookAndFeel
//==============================================================================
NeveLookAndFeel::NeveLookAndFeel()
{
    knobColour = juce::Colour(0xFFF7FAFC);  // Cream by default

    setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xFF2D3748));
    setColour(juce::ComboBox::textColourId, juce::Colour(0xFFF7FAFC));
    setColour(juce::ComboBox::outlineColourId, juce::Colour(0xFF4A5568));
    setColour(juce::ComboBox::arrowColourId, juce::Colour(0xFFF7FAFC));

    setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0xFF2D3748));
    setColour(juce::PopupMenu::textColourId, juce::Colour(0xFFF7FAFC));
    setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xFF4A5568));
    setColour(juce::PopupMenu::highlightedTextColourId, juce::Colour(0xFFFFFFFF));

    setColour(juce::Label::textColourId, juce::Colour(0xFFF7FAFC));
}

void NeveLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                        float sliderPosProportional, float /*rotaryStartAngle*/,
                                        float /*rotaryEndAngle*/, juce::Slider& /*slider*/)
{
    auto bounds = juce::Rectangle<float>((float)x, (float)y, (float)width, (float)height).reduced(4.0f);
    float cx = bounds.getCentreX();
    float cy = bounds.getCentreY();
    float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 2.0f;

    // Outer ring (knurled edge)
    g.setColour(juce::Colour(0xFF1A202C));
    g.fillEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);

    // Knurl pattern
    g.setColour(juce::Colour(0xFF2D3748));
    int numKnurls = 20;
    for (int i = 0; i < numKnurls; ++i)
    {
        float angle = i * juce::MathConstants<float>::twoPi / numKnurls;
        float x1 = cx + (radius - 1.0f) * std::cos(angle);
        float y1 = cy + (radius - 1.0f) * std::sin(angle);
        float x2 = cx + (radius - 4.0f) * std::cos(angle);
        float y2 = cy + (radius - 4.0f) * std::sin(angle);
        g.drawLine(x1, y1, x2, y2, 1.5f);
    }

    // Main knob body with gradient
    float innerRadius = radius * 0.75f;
    juce::ColourGradient knobGradient(
        juce::Colour(0xFF4A5568), cx - innerRadius * 0.3f, cy - innerRadius * 0.3f,
        juce::Colour(0xFF2D3748), cx + innerRadius * 0.3f, cy + innerRadius * 0.3f, true);
    g.setGradientFill(knobGradient);
    g.fillEllipse(cx - innerRadius, cy - innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);

    // Inner ring
    g.setColour(juce::Colour(0xFF718096));
    g.drawEllipse(cx - innerRadius, cy - innerRadius, innerRadius * 2.0f, innerRadius * 2.0f, 1.0f);

    // Pointer (chicken head style)
    float pointerAngle = juce::jmap(sliderPosProportional, 0.0f, 1.0f, -2.356f, 2.356f) + juce::MathConstants<float>::pi;
    float pointerLength = innerRadius * 0.7f;

    // Draw pointer triangle
    juce::Path pointer;
    float tipX = cx + pointerLength * std::cos(pointerAngle);
    float tipY = cy + pointerLength * std::sin(pointerAngle);
    float baseWidth = innerRadius * 0.25f;
    float perpAngle = pointerAngle + juce::MathConstants<float>::halfPi;
    float base1X = cx + baseWidth * std::cos(perpAngle);
    float base1Y = cy + baseWidth * std::sin(perpAngle);
    float base2X = cx - baseWidth * std::cos(perpAngle);
    float base2Y = cy - baseWidth * std::sin(perpAngle);

    pointer.startNewSubPath(tipX, tipY);
    pointer.lineTo(base1X, base1Y);
    pointer.lineTo(base2X, base2Y);
    pointer.closeSubPath();

    g.setColour(knobColour);
    g.fillPath(pointer);

    // Center dot
    float dotRadius = innerRadius * 0.15f;
    g.setColour(juce::Colour(0xFF1A202C));
    g.fillEllipse(cx - dotRadius, cy - dotRadius, dotRadius * 2.0f, dotRadius * 2.0f);
}

void NeveLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                        bool /*shouldDrawButtonAsHighlighted*/,
                                        bool /*shouldDrawButtonAsDown*/)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(2.0f);

    // Button background
    g.setColour(button.getToggleState() ? juce::Colour(0xFFED8936) : juce::Colour(0xFF2D3748));
    g.fillRoundedRectangle(bounds, 4.0f);

    // Border
    g.setColour(juce::Colour(0xFF4A5568));
    g.drawRoundedRectangle(bounds, 4.0f, 1.0f);

    // Text
    g.setColour(button.getToggleState() ? juce::Colour(0xFF1A202C) : juce::Colour(0xFFF7FAFC));
    g.setFont(juce::Font(11.0f, juce::Font::bold));
    g.drawText(button.getButtonText(), bounds, juce::Justification::centred);
}

void NeveLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool /*isButtonDown*/,
                                    int /*buttonX*/, int /*buttonY*/, int /*buttonW*/, int /*buttonH*/,
                                    juce::ComboBox& /*box*/)
{
    auto bounds = juce::Rectangle<float>(0, 0, (float)width, (float)height);

    g.setColour(juce::Colour(0xFF2D3748));
    g.fillRoundedRectangle(bounds, 4.0f);

    g.setColour(juce::Colour(0xFF4A5568));
    g.drawRoundedRectangle(bounds, 4.0f, 1.0f);

    // Arrow
    juce::Path arrow;
    float arrowX = width - 15.0f;
    float arrowY = height * 0.5f;
    arrow.addTriangle(arrowX - 4.0f, arrowY - 2.0f, arrowX + 4.0f, arrowY - 2.0f, arrowX, arrowY + 3.0f);
    g.setColour(juce::Colour(0xFFF7FAFC));
    g.fillPath(arrow);
}

juce::Font NeveLookAndFeel::getComboBoxFont(juce::ComboBox& /*box*/)
{
    return juce::Font(12.0f);
}

juce::Font NeveLookAndFeel::getLabelFont(juce::Label& /*label*/)
{
    return juce::Font(11.0f, juce::Font::bold);
}

//==============================================================================
// VUMeter
//==============================================================================
VUMeter::VUMeter()
{
}

void VUMeter::setLevel(float newLevel)
{
    level = newLevel;
    repaint();
}

void VUMeter::setGainReduction(float gr)
{
    gainReduction = gr;
    repaint();
}

void VUMeter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(2.0f);

    // Background
    g.setColour(juce::Colour(0xFF1A202C));
    g.fillRoundedRectangle(bounds, 3.0f);

    // Meter value
    float displayValue = showGR ? gainReduction : level;

    // Smooth the display
    float targetSmoothed = showGR ? smoothedGR : smoothedLevel;
    targetSmoothed += 0.3f * (displayValue - targetSmoothed);
    if (showGR)
        smoothedGR = targetSmoothed;
    else
        smoothedLevel = targetSmoothed;

    // Convert to display height
    float meterHeight;
    if (showGR)
    {
        // GR meter: 0-20dB range
        meterHeight = std::min(1.0f, smoothedGR / 20.0f);
    }
    else
    {
        // Level meter: -60 to 0dB range, displayed as linear
        float dbLevel = 20.0f * std::log10(std::max(0.0001f, smoothedLevel));
        meterHeight = std::max(0.0f, (dbLevel + 60.0f) / 60.0f);
    }

    // Draw meter bar
    auto meterBounds = bounds.reduced(3.0f);
    float barHeight = meterBounds.getHeight() * meterHeight;

    if (showGR)
    {
        // GR meter - draw from top
        g.setColour(juce::Colour(0xFFE53E3E));
        g.fillRect(meterBounds.getX(), meterBounds.getY(),
                   meterBounds.getWidth(), barHeight);
    }
    else
    {
        // Level meter - draw from bottom
        juce::ColourGradient gradient(
            juce::Colour(0xFF48BB78), meterBounds.getX(), meterBounds.getBottom(),
            juce::Colour(0xFFE53E3E), meterBounds.getX(), meterBounds.getY(), false);
        gradient.addColour(0.7, juce::Colour(0xFFECC94B));
        g.setGradientFill(gradient);
        g.fillRect(meterBounds.getX(), meterBounds.getBottom() - barHeight,
                   meterBounds.getWidth(), barHeight);
    }

    // Scale marks
    g.setColour(juce::Colour(0xFF4A5568));
    for (int i = 0; i <= 4; ++i)
    {
        float y = meterBounds.getY() + (meterBounds.getHeight() * i / 4.0f);
        g.drawHorizontalLine((int)y, meterBounds.getX(), meterBounds.getRight());
    }
}

//==============================================================================
// SectionHeader
//==============================================================================
SectionHeader::SectionHeader(const juce::String& t) : title(t)
{
}

void SectionHeader::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Background stripe
    g.setColour(juce::Colour(0xFF2D3748));
    g.fillRect(bounds);

    // Text
    g.setColour(juce::Colour(0xFFED8936));
    g.setFont(juce::Font(13.0f, juce::Font::bold));
    g.drawText(title, bounds.reduced(8.0f, 0.0f), juce::Justification::centredLeft);
}

//==============================================================================
// NeveStripAudioProcessorEditor
//==============================================================================
NeveStripAudioProcessorEditor::NeveStripAudioProcessorEditor(NeveStripAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(800, 500);

    orangeKnobLookAndFeel.setKnobColour(juce::Colour(neveOrange));

    // Setup all components
    addAndMakeVisible(preampHeader);
    addAndMakeVisible(eqHeader);
    addAndMakeVisible(dynamicsHeader);
    addAndMakeVisible(outputHeader);

    // Preamp section
    setupSlider(inputGainSlider, inputGainLabel);
    inputGainSlider.setLookAndFeel(&orangeKnobLookAndFeel);
    setupSlider(outputTrimSlider, outputTrimLabel);
    setupSlider(transformerDriveSlider, transformerLabel);

    setupComboBox(hpfCombo);
    addAndMakeVisible(hpfLabel);
    hpfLabel.setJustificationType(juce::Justification::centred);

    phaseButton.setLookAndFeel(&neveLookAndFeel);
    addAndMakeVisible(phaseButton);

    // EQ section
    setupComboBox(hfFreqCombo);
    setupSlider(hfGainSlider, hfLabel);
    setupComboBox(hmFreqCombo);
    setupSlider(hmGainSlider, hmLabel);
    setupComboBox(lmFreqCombo);
    setupSlider(lmGainSlider, lmLabel);
    setupComboBox(lfFreqCombo);
    setupSlider(lfGainSlider, lfLabel);

    eqBypassButton.setLookAndFeel(&neveLookAndFeel);
    eqPrePostButton.setLookAndFeel(&neveLookAndFeel);
    addAndMakeVisible(eqBypassButton);
    addAndMakeVisible(eqPrePostButton);

    // Dynamics section
    setupSlider(compThresholdSlider, compThreshLabel);
    setupComboBox(compRatioCombo);
    addAndMakeVisible(compRatioLabel);
    compRatioLabel.setJustificationType(juce::Justification::centred);
    setupComboBox(compAttackCombo);
    addAndMakeVisible(compAttackLabel);
    compAttackLabel.setJustificationType(juce::Justification::centred);
    setupComboBox(compReleaseCombo);
    addAndMakeVisible(compReleaseLabel);
    compReleaseLabel.setJustificationType(juce::Justification::centred);
    setupSlider(compMakeupSlider, compMakeupLabel);
    compMakeupSlider.setLookAndFeel(&orangeKnobLookAndFeel);

    compSCHPFButton.setLookAndFeel(&neveLookAndFeel);
    compLinkButton.setLookAndFeel(&neveLookAndFeel);
    compBypassButton.setLookAndFeel(&neveLookAndFeel);
    addAndMakeVisible(compSCHPFButton);
    addAndMakeVisible(compLinkButton);
    addAndMakeVisible(compBypassButton);

    setupSlider(limThresholdSlider, limThreshLabel);
    limBypassButton.setLookAndFeel(&neveLookAndFeel);
    addAndMakeVisible(limBypassButton);

    // Output section
    setupSlider(outputLevelSlider, outputLabel);
    outputLevelSlider.setLookAndFeel(&orangeKnobLookAndFeel);
    masterBypassButton.setLookAndFeel(&neveLookAndFeel);
    addAndMakeVisible(masterBypassButton);

    // Meters
    addAndMakeVisible(inputMeter);
    addAndMakeVisible(outputMeter);
    addAndMakeVisible(grMeter);
    grMeter.setShowGR(true);
    addAndMakeVisible(inputMeterLabel);
    addAndMakeVisible(outputMeterLabel);
    addAndMakeVisible(grMeterLabel);
    inputMeterLabel.setJustificationType(juce::Justification::centred);
    outputMeterLabel.setJustificationType(juce::Justification::centred);
    grMeterLabel.setJustificationType(juce::Justification::centred);

    // Create attachments
    auto& apvts = audioProcessor.getAPVTS();

    inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "inputGain", inputGainSlider);
    outputTrimAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "outputTrim", outputTrimSlider);
    transformerDriveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "transformerDrive", transformerDriveSlider);
    hpfAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, "hpfFreq", hpfCombo);
    phaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, "phase", phaseButton);

    hfFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, "hfFreq", hfFreqCombo);
    hfGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "hfGain", hfGainSlider);
    hmFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, "hmFreq", hmFreqCombo);
    hmGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "hmGain", hmGainSlider);
    lmFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, "lmFreq", lmFreqCombo);
    lmGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "lmGain", lmGainSlider);
    lfFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, "lfFreq", lfFreqCombo);
    lfGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "lfGain", lfGainSlider);
    eqBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, "eqBypass", eqBypassButton);
    eqPrePostAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, "eqPrePost", eqPrePostButton);

    compThresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "compThreshold", compThresholdSlider);
    compRatioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, "compRatio", compRatioCombo);
    compAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, "compAttack", compAttackCombo);
    compReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, "compRelease", compReleaseCombo);
    compMakeupAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "compMakeup", compMakeupSlider);
    compSCHPFAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, "compSCHPF", compSCHPFButton);
    compLinkAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, "compLink", compLinkButton);
    compBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, "compBypass", compBypassButton);

    limThresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "limThreshold", limThresholdSlider);
    limBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, "limBypass", limBypassButton);

    outputLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "outputLevel", outputLevelSlider);
    masterBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, "masterBypass", masterBypassButton);

    startTimerHz(30);
}

NeveStripAudioProcessorEditor::~NeveStripAudioProcessorEditor()
{
    stopTimer();

    // Clear look and feels
    inputGainSlider.setLookAndFeel(nullptr);
    outputTrimSlider.setLookAndFeel(nullptr);
    transformerDriveSlider.setLookAndFeel(nullptr);
    hfGainSlider.setLookAndFeel(nullptr);
    hmGainSlider.setLookAndFeel(nullptr);
    lmGainSlider.setLookAndFeel(nullptr);
    lfGainSlider.setLookAndFeel(nullptr);
    compThresholdSlider.setLookAndFeel(nullptr);
    compMakeupSlider.setLookAndFeel(nullptr);
    limThresholdSlider.setLookAndFeel(nullptr);
    outputLevelSlider.setLookAndFeel(nullptr);

    phaseButton.setLookAndFeel(nullptr);
    eqBypassButton.setLookAndFeel(nullptr);
    eqPrePostButton.setLookAndFeel(nullptr);
    compSCHPFButton.setLookAndFeel(nullptr);
    compLinkButton.setLookAndFeel(nullptr);
    compBypassButton.setLookAndFeel(nullptr);
    limBypassButton.setLookAndFeel(nullptr);
    masterBypassButton.setLookAndFeel(nullptr);
}

void NeveStripAudioProcessorEditor::setupSlider(juce::Slider& slider, juce::Label& label)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setLookAndFeel(&neveLookAndFeel);
    addAndMakeVisible(slider);

    label.setJustificationType(juce::Justification::centred);
    label.setFont(juce::Font(11.0f, juce::Font::bold));
    addAndMakeVisible(label);
}

void NeveStripAudioProcessorEditor::setupComboBox(juce::ComboBox& combo)
{
    combo.setLookAndFeel(&neveLookAndFeel);
    addAndMakeVisible(combo);
}

void NeveStripAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Main background - Neve blue-grey
    g.fillAll(juce::Colour(neveBlueGrey));

    // Metal texture effect
    for (int i = 0; i < getHeight(); i += 2)
    {
        g.setColour(juce::Colour(neveBlueGrey).brighter(0.02f));
        g.drawHorizontalLine(i, 0.0f, (float)getWidth());
    }

    // Top banner
    g.setColour(juce::Colour(neveDarkGrey));
    g.fillRect(0, 0, getWidth(), 40);

    // Logo/Title
    g.setColour(juce::Colour(neveOrange));
    g.setFont(juce::Font(24.0f, juce::Font::bold));
    g.drawText("NEVESTRIP", 20, 5, 200, 30, juce::Justification::centredLeft);

    g.setColour(juce::Colour(neveCream));
    g.setFont(juce::Font(12.0f));
    g.drawText("Channel Strip", 20, 25, 200, 15, juce::Justification::centredLeft);

    // Section dividers
    g.setColour(juce::Colour(0xFF1A202C));
    g.drawVerticalLine(165, 45.0f, (float)getHeight() - 5);
    g.drawVerticalLine(430, 45.0f, (float)getHeight() - 5);
    g.drawVerticalLine(635, 45.0f, (float)getHeight() - 5);
}

void NeveStripAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    // Top banner
    bounds.removeFromTop(45);

    // Split into sections
    auto preampArea = bounds.removeFromLeft(160);
    auto eqArea = bounds.removeFromLeft(260);
    auto dynamicsArea = bounds.removeFromLeft(200);
    auto outputArea = bounds;

    int headerHeight = 22;
    int knobSize = 55;
    int labelHeight = 16;
    int comboHeight = 22;
    int buttonHeight = 24;
    int margin = 8;

    // === PREAMP SECTION ===
    preampHeader.setBounds(preampArea.removeFromTop(headerHeight));
    preampArea.reduce(margin, margin);

    auto inputRow = preampArea.removeFromTop(knobSize + labelHeight);
    inputGainLabel.setBounds(inputRow.removeFromTop(labelHeight));
    inputGainSlider.setBounds(inputRow.withSizeKeepingCentre(knobSize, knobSize));

    preampArea.removeFromTop(margin);

    auto trimRow = preampArea.removeFromTop(knobSize + labelHeight);
    outputTrimLabel.setBounds(trimRow.removeFromTop(labelHeight));
    outputTrimSlider.setBounds(trimRow.withSizeKeepingCentre(knobSize, knobSize));

    preampArea.removeFromTop(margin);

    auto driveRow = preampArea.removeFromTop(knobSize + labelHeight);
    transformerLabel.setBounds(driveRow.removeFromTop(labelHeight));
    transformerDriveSlider.setBounds(driveRow.withSizeKeepingCentre(knobSize, knobSize));

    preampArea.removeFromTop(margin);

    hpfLabel.setBounds(preampArea.removeFromTop(labelHeight));
    hpfCombo.setBounds(preampArea.removeFromTop(comboHeight).reduced(10, 0));

    preampArea.removeFromTop(margin);
    phaseButton.setBounds(preampArea.removeFromTop(buttonHeight).reduced(10, 0));

    // === EQ SECTION ===
    eqHeader.setBounds(eqArea.removeFromTop(headerHeight));
    eqArea.reduce(margin, margin);

    // Two columns for EQ
    int eqColWidth = eqArea.getWidth() / 2;

    // Left column: HF, HM
    auto eqLeft = eqArea.removeFromLeft(eqColWidth);
    // Right column: LM, LF
    auto eqRight = eqArea;

    // HF
    auto hfRow = eqLeft.removeFromTop(knobSize + labelHeight + comboHeight + 4);
    hfLabel.setBounds(hfRow.removeFromTop(labelHeight));
    hfFreqCombo.setBounds(hfRow.removeFromTop(comboHeight).reduced(5, 0));
    hfGainSlider.setBounds(hfRow.withSizeKeepingCentre(knobSize, knobSize));

    eqLeft.removeFromTop(margin);

    // HM
    auto hmRow = eqLeft.removeFromTop(knobSize + labelHeight + comboHeight + 4);
    hmLabel.setBounds(hmRow.removeFromTop(labelHeight));
    hmFreqCombo.setBounds(hmRow.removeFromTop(comboHeight).reduced(5, 0));
    hmGainSlider.setBounds(hmRow.withSizeKeepingCentre(knobSize, knobSize));

    // LM
    auto lmRow = eqRight.removeFromTop(knobSize + labelHeight + comboHeight + 4);
    lmLabel.setBounds(lmRow.removeFromTop(labelHeight));
    lmFreqCombo.setBounds(lmRow.removeFromTop(comboHeight).reduced(5, 0));
    lmGainSlider.setBounds(lmRow.withSizeKeepingCentre(knobSize, knobSize));

    eqRight.removeFromTop(margin);

    // LF
    auto lfRow = eqRight.removeFromTop(knobSize + labelHeight + comboHeight + 4);
    lfLabel.setBounds(lfRow.removeFromTop(labelHeight));
    lfFreqCombo.setBounds(lfRow.removeFromTop(comboHeight).reduced(5, 0));
    lfGainSlider.setBounds(lfRow.withSizeKeepingCentre(knobSize, knobSize));

    // EQ buttons at bottom of left column
    eqLeft.removeFromTop(margin);
    auto eqButtonRow = eqLeft.removeFromTop(buttonHeight);
    eqBypassButton.setBounds(eqButtonRow.removeFromLeft(eqButtonRow.getWidth() / 2).reduced(2, 0));
    eqPrePostButton.setBounds(eqButtonRow.reduced(2, 0));

    // === DYNAMICS SECTION ===
    dynamicsHeader.setBounds(dynamicsArea.removeFromTop(headerHeight));
    dynamicsArea.reduce(margin, margin);

    // Threshold
    auto threshRow = dynamicsArea.removeFromTop(knobSize + labelHeight);
    compThreshLabel.setBounds(threshRow.removeFromTop(labelHeight));
    compThresholdSlider.setBounds(threshRow.withSizeKeepingCentre(knobSize, knobSize));

    dynamicsArea.removeFromTop(4);

    // Ratio
    compRatioLabel.setBounds(dynamicsArea.removeFromTop(labelHeight));
    compRatioCombo.setBounds(dynamicsArea.removeFromTop(comboHeight).reduced(10, 0));

    dynamicsArea.removeFromTop(4);

    // Attack
    compAttackLabel.setBounds(dynamicsArea.removeFromTop(labelHeight));
    compAttackCombo.setBounds(dynamicsArea.removeFromTop(comboHeight).reduced(10, 0));

    dynamicsArea.removeFromTop(4);

    // Release
    compReleaseLabel.setBounds(dynamicsArea.removeFromTop(labelHeight));
    compReleaseCombo.setBounds(dynamicsArea.removeFromTop(comboHeight).reduced(10, 0));

    dynamicsArea.removeFromTop(4);

    // Makeup
    auto makeupRow = dynamicsArea.removeFromTop(knobSize + labelHeight);
    compMakeupLabel.setBounds(makeupRow.removeFromTop(labelHeight));
    compMakeupSlider.setBounds(makeupRow.withSizeKeepingCentre(knobSize, knobSize));

    dynamicsArea.removeFromTop(4);

    // Comp buttons
    auto compButtonRow = dynamicsArea.removeFromTop(buttonHeight);
    int btnWidth = compButtonRow.getWidth() / 3;
    compBypassButton.setBounds(compButtonRow.removeFromLeft(btnWidth).reduced(2, 0));
    compLinkButton.setBounds(compButtonRow.removeFromLeft(btnWidth).reduced(2, 0));
    compSCHPFButton.setBounds(compButtonRow.reduced(2, 0));

    dynamicsArea.removeFromTop(margin);

    // Limiter
    limThreshLabel.setBounds(dynamicsArea.removeFromTop(labelHeight));
    limThresholdSlider.setBounds(dynamicsArea.removeFromTop(knobSize).withSizeKeepingCentre(knobSize, knobSize));
    limBypassButton.setBounds(dynamicsArea.removeFromTop(buttonHeight).reduced(20, 0));

    // === OUTPUT SECTION ===
    outputHeader.setBounds(outputArea.removeFromTop(headerHeight));
    outputArea.reduce(margin, margin);

    // Output level knob
    auto outLevelRow = outputArea.removeFromTop(knobSize + labelHeight);
    outputLabel.setBounds(outLevelRow.removeFromTop(labelHeight));
    outputLevelSlider.setBounds(outLevelRow.withSizeKeepingCentre(knobSize, knobSize));

    outputArea.removeFromTop(margin);

    // Master bypass
    masterBypassButton.setBounds(outputArea.removeFromTop(buttonHeight).reduced(10, 0));

    outputArea.removeFromTop(margin * 2);

    // Meters
    int meterWidth = 25;
    int meterHeight = 120;
    auto meterArea = outputArea.removeFromTop(meterHeight + labelHeight);

    auto inMeterArea = meterArea.removeFromLeft(meterArea.getWidth() / 3);
    inputMeterLabel.setBounds(inMeterArea.removeFromTop(labelHeight));
    inputMeter.setBounds(inMeterArea.withSizeKeepingCentre(meterWidth, meterHeight));

    auto outMeterArea = meterArea.removeFromLeft(meterArea.getWidth() / 2);
    outputMeterLabel.setBounds(outMeterArea.removeFromTop(labelHeight));
    outputMeter.setBounds(outMeterArea.withSizeKeepingCentre(meterWidth, meterHeight));

    grMeterLabel.setBounds(meterArea.removeFromTop(labelHeight));
    grMeter.setBounds(meterArea.withSizeKeepingCentre(meterWidth, meterHeight));
}

void NeveStripAudioProcessorEditor::timerCallback()
{
    inputMeter.setLevel(audioProcessor.getInputLevel());
    outputMeter.setLevel(audioProcessor.getOutputLevel());
    grMeter.setGainReduction(audioProcessor.getCompressorGR() + audioProcessor.getLimiterGR());

    inputMeter.repaint();
    outputMeter.repaint();
    grMeter.repaint();
}
