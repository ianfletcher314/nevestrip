#include "PluginEditor.h"

//==============================================================================
// NeveLookAndFeel - Chicken Head Knob Style
//==============================================================================
NeveLookAndFeel::NeveLookAndFeel()
{
    knobColour = juce::Colour(0xFFF5F0E6);  // Cream by default
    isChickenHead = true;

    setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xFF3D4A56));
    setColour(juce::ComboBox::textColourId, juce::Colour(0xFFF5F0E6));
    setColour(juce::ComboBox::outlineColourId, juce::Colour(0xFF2A3440));
    setColour(juce::ComboBox::arrowColourId, juce::Colour(0xFFF5F0E6));

    setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0xFF3D4A56));
    setColour(juce::PopupMenu::textColourId, juce::Colour(0xFFF5F0E6));
    setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xFF5B6B7A));
    setColour(juce::PopupMenu::highlightedTextColourId, juce::Colour(0xFFFFFFFF));

    setColour(juce::Label::textColourId, juce::Colour(0xFFF5F0E6));
}

void NeveLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                        float sliderPosProportional, float /*rotaryStartAngle*/,
                                        float /*rotaryEndAngle*/, juce::Slider& /*slider*/)
{
    auto bounds = juce::Rectangle<float>((float)x, (float)y, (float)width, (float)height).reduced(2.0f);
    float cx = bounds.getCentreX();
    float cy = bounds.getCentreY();
    float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 1.0f;

    // Shadow underneath
    g.setColour(juce::Colour(0x40000000));
    g.fillEllipse(cx - radius + 2, cy - radius + 2, radius * 2.0f, radius * 2.0f);

    // Outer metal ring (bezel)
    juce::ColourGradient bezelGradient(
        juce::Colour(0xFF8090A0), cx - radius, cy - radius,
        juce::Colour(0xFF404850), cx + radius, cy + radius, false);
    g.setGradientFill(bezelGradient);
    g.fillEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);

    // Knurled edge pattern
    float knurlRadius = radius - 1.0f;
    g.setColour(juce::Colour(0xFF303840));
    int numKnurls = 24;
    for (int i = 0; i < numKnurls; ++i)
    {
        float angle = i * juce::MathConstants<float>::twoPi / numKnurls;
        float x1 = cx + knurlRadius * std::cos(angle);
        float y1 = cy + knurlRadius * std::sin(angle);
        float x2 = cx + (knurlRadius - 3.0f) * std::cos(angle);
        float y2 = cy + (knurlRadius - 3.0f) * std::sin(angle);
        g.drawLine(x1, y1, x2, y2, 1.0f);
    }

    // Main knob body with gradient for 3D effect
    float innerRadius = radius * 0.72f;
    juce::ColourGradient knobGradient(
        knobColour.brighter(0.3f), cx - innerRadius * 0.5f, cy - innerRadius * 0.5f,
        knobColour.darker(0.2f), cx + innerRadius * 0.5f, cy + innerRadius * 0.5f, true);
    g.setGradientFill(knobGradient);
    g.fillEllipse(cx - innerRadius, cy - innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);

    // Inner ring highlight
    g.setColour(knobColour.brighter(0.4f).withAlpha(0.5f));
    g.drawEllipse(cx - innerRadius + 1, cy - innerRadius + 1, (innerRadius - 1) * 2.0f, (innerRadius - 1) * 2.0f, 1.0f);

    // Pointer angle calculation
    float pointerAngle = juce::jmap(sliderPosProportional, 0.0f, 1.0f, -2.4f, 2.4f) + juce::MathConstants<float>::pi;

    if (isChickenHead)
    {
        // CHICKEN HEAD pointer style
        float pointerLength = innerRadius * 0.85f;
        float baseWidth = innerRadius * 0.35f;
        float perpAngle = pointerAngle + juce::MathConstants<float>::halfPi;

        juce::Path pointer;
        float tipX = cx + pointerLength * std::cos(pointerAngle);
        float tipY = cy + pointerLength * std::sin(pointerAngle);
        float base1X = cx + baseWidth * std::cos(perpAngle);
        float base1Y = cy + baseWidth * std::sin(perpAngle);
        float base2X = cx - baseWidth * std::cos(perpAngle);
        float base2Y = cy - baseWidth * std::sin(perpAngle);

        pointer.startNewSubPath(tipX, tipY);
        pointer.lineTo(base1X, base1Y);
        pointer.lineTo(base2X, base2Y);
        pointer.closeSubPath();

        // Pointer shadow
        g.setColour(juce::Colour(0x40000000));
        g.fillPath(pointer, juce::AffineTransform::translation(1.0f, 1.0f));

        // Main pointer
        g.setColour(knobColour.darker(0.1f));
        g.fillPath(pointer);

        // Pointer highlight edge
        g.setColour(knobColour.brighter(0.2f));
        g.strokePath(pointer, juce::PathStrokeType(0.5f));
    }
    else
    {
        // Simple line indicator
        float lineLength = innerRadius * 0.7f;
        float lineStartRadius = innerRadius * 0.2f;
        g.setColour(juce::Colour(0xFF202020));
        g.drawLine(
            cx + lineStartRadius * std::cos(pointerAngle),
            cy + lineStartRadius * std::sin(pointerAngle),
            cx + lineLength * std::cos(pointerAngle),
            cy + lineLength * std::sin(pointerAngle),
            2.5f);
    }

    // Center cap screw
    float capRadius = innerRadius * 0.22f;
    juce::ColourGradient capGradient(
        juce::Colour(0xFF606870), cx - capRadius, cy - capRadius,
        juce::Colour(0xFF303840), cx + capRadius, cy + capRadius, true);
    g.setGradientFill(capGradient);
    g.fillEllipse(cx - capRadius, cy - capRadius, capRadius * 2.0f, capRadius * 2.0f);

    // Screw slot
    g.setColour(juce::Colour(0xFF202020));
    g.drawLine(cx - capRadius * 0.6f, cy, cx + capRadius * 0.6f, cy, 1.5f);
}

void NeveLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                        bool shouldDrawButtonAsHighlighted,
                                        bool /*shouldDrawButtonAsDown*/)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(1.0f);

    // Button shadow
    g.setColour(juce::Colour(0x40000000));
    g.fillRoundedRectangle(bounds.translated(1, 1), 3.0f);

    // Button background with beveled look
    juce::Colour bgColour = button.getToggleState()
        ? juce::Colour(0xFFE67E22)  // Orange when on
        : juce::Colour(0xFF3D4A56); // Dark blue-grey when off

    if (shouldDrawButtonAsHighlighted)
        bgColour = bgColour.brighter(0.1f);

    juce::ColourGradient btnGradient(
        bgColour.brighter(0.15f), bounds.getX(), bounds.getY(),
        bgColour.darker(0.1f), bounds.getX(), bounds.getBottom(), false);
    g.setGradientFill(btnGradient);
    g.fillRoundedRectangle(bounds, 3.0f);

    // Border
    g.setColour(juce::Colour(0xFF2A3440));
    g.drawRoundedRectangle(bounds, 3.0f, 1.0f);

    // Top highlight
    g.setColour(juce::Colour(0x30FFFFFF));
    g.drawLine(bounds.getX() + 3, bounds.getY() + 1, bounds.getRight() - 3, bounds.getY() + 1, 1.0f);

    // Text
    g.setColour(button.getToggleState() ? juce::Colour(0xFF1A1A1A) : juce::Colour(0xFFF5F0E6));
    g.setFont(juce::Font(10.0f, juce::Font::bold));
    g.drawText(button.getButtonText(), bounds, juce::Justification::centred);
}

void NeveLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool /*isButtonDown*/,
                                    int /*buttonX*/, int /*buttonY*/, int /*buttonW*/, int /*buttonH*/,
                                    juce::ComboBox& /*box*/)
{
    auto bounds = juce::Rectangle<float>(0, 0, (float)width, (float)height);

    // Shadow
    g.setColour(juce::Colour(0x40000000));
    g.fillRoundedRectangle(bounds.translated(1, 1), 3.0f);

    // Background gradient
    juce::ColourGradient bgGradient(
        juce::Colour(0xFF4A5666), 0, 0,
        juce::Colour(0xFF3D4A56), 0, (float)height, false);
    g.setGradientFill(bgGradient);
    g.fillRoundedRectangle(bounds, 3.0f);

    // Border
    g.setColour(juce::Colour(0xFF2A3440));
    g.drawRoundedRectangle(bounds, 3.0f, 1.0f);

    // Arrow
    juce::Path arrow;
    float arrowX = width - 12.0f;
    float arrowY = height * 0.5f;
    arrow.addTriangle(arrowX - 4.0f, arrowY - 2.0f, arrowX + 4.0f, arrowY - 2.0f, arrowX, arrowY + 3.0f);
    g.setColour(juce::Colour(0xFFF5F0E6));
    g.fillPath(arrow);
}

juce::Font NeveLookAndFeel::getComboBoxFont(juce::ComboBox& /*box*/)
{
    return juce::Font(10.0f);
}

juce::Font NeveLookAndFeel::getLabelFont(juce::Label& /*label*/)
{
    return juce::Font(10.0f, juce::Font::bold);
}

//==============================================================================
// VUMeter - Classic Analog Style
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
    auto bounds = getLocalBounds().toFloat().reduced(1.0f);

    // Outer bezel
    g.setColour(juce::Colour(0xFF2A3440));
    g.fillRoundedRectangle(bounds, 4.0f);

    // Inner meter area
    auto meterBounds = bounds.reduced(3.0f);
    g.setColour(juce::Colour(0xFF1A1F26));
    g.fillRoundedRectangle(meterBounds, 2.0f);

    // Meter value
    float displayValue = showGR ? gainReduction : level;

    // Smooth the display
    float targetSmoothed = showGR ? smoothedGR : smoothedLevel;
    targetSmoothed += 0.25f * (displayValue - targetSmoothed);
    if (showGR)
        smoothedGR = targetSmoothed;
    else
        smoothedLevel = targetSmoothed;

    // Convert to display height
    float meterHeight;
    if (showGR)
    {
        meterHeight = std::min(1.0f, smoothedGR / 20.0f);
    }
    else
    {
        float dbLevel = 20.0f * std::log10(std::max(0.0001f, smoothedLevel));
        meterHeight = std::max(0.0f, (dbLevel + 60.0f) / 60.0f);
    }

    // Draw meter bar with segments
    auto barBounds = meterBounds.reduced(2.0f);
    int numSegments = 12;
    float segmentHeight = barBounds.getHeight() / numSegments;
    float segmentGap = 1.0f;

    for (int i = 0; i < numSegments; ++i)
    {
        float segmentY = barBounds.getBottom() - (i + 1) * segmentHeight;
        float segmentThreshold = (float)(i + 1) / numSegments;

        if (meterHeight >= segmentThreshold - 0.08f)
        {
            juce::Colour segColour;
            if (showGR)
            {
                segColour = juce::Colour(0xFFCC3333);  // Red for GR
            }
            else if (i >= 10)
            {
                segColour = juce::Colour(0xFFCC3333);  // Red (hot)
            }
            else if (i >= 8)
            {
                segColour = juce::Colour(0xFFE6A822);  // Amber
            }
            else
            {
                segColour = juce::Colour(0xFF44AA44);  // Green
            }

            g.setColour(segColour);
            g.fillRect(barBounds.getX(), segmentY + segmentGap,
                       barBounds.getWidth(), segmentHeight - segmentGap * 2);
        }
        else
        {
            // Dim segment
            g.setColour(juce::Colour(0xFF252A30));
            g.fillRect(barBounds.getX(), segmentY + segmentGap,
                       barBounds.getWidth(), segmentHeight - segmentGap * 2);
        }
    }

    // Scale marks on right side
    g.setColour(juce::Colour(0xFF606870));
    for (int i = 0; i <= numSegments; i += 3)
    {
        float y = barBounds.getBottom() - i * segmentHeight;
        g.drawHorizontalLine((int)y, barBounds.getRight() - 3, barBounds.getRight());
    }
}

//==============================================================================
// SectionHeader - Metal Strip Style
//==============================================================================
SectionHeader::SectionHeader(const juce::String& t) : title(t)
{
}

void SectionHeader::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Metal strip gradient
    juce::ColourGradient stripGradient(
        juce::Colour(0xFF5B6B7A), 0, bounds.getY(),
        juce::Colour(0xFF3D4A56), 0, bounds.getBottom(), false);
    g.setGradientFill(stripGradient);
    g.fillRect(bounds);

    // Top highlight
    g.setColour(juce::Colour(0xFF7A8A99));
    g.drawHorizontalLine((int)bounds.getY(), bounds.getX(), bounds.getRight());

    // Bottom shadow
    g.setColour(juce::Colour(0xFF2A3440));
    g.drawHorizontalLine((int)bounds.getBottom() - 1, bounds.getX(), bounds.getRight());

    // Text with embossed look
    g.setFont(juce::Font(11.0f, juce::Font::bold));

    // Shadow
    g.setColour(juce::Colour(0xFF2A3440));
    g.drawText(title, bounds.translated(1, 1).reduced(8.0f, 0.0f), juce::Justification::centred);

    // Main text
    g.setColour(juce::Colour(0xFFD4A84B));  // Gold lettering
    g.drawText(title, bounds.reduced(8.0f, 0.0f), juce::Justification::centred);
}

//==============================================================================
// NeveStripAudioProcessorEditor - Vertical Channel Strip Layout
//==============================================================================
NeveStripAudioProcessorEditor::NeveStripAudioProcessorEditor(NeveStripAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(stripWidth, stripHeight);

    orangeKnobLookAndFeel.setKnobColour(juce::Colour(neveOrange));
    redKnobLookAndFeel.setKnobColour(juce::Colour(neveRed));

    // Setup all components
    addAndMakeVisible(preampHeader);
    addAndMakeVisible(eqHeader);
    addAndMakeVisible(dynamicsHeader);
    addAndMakeVisible(outputHeader);

    // === PREAMP/INPUT SECTION ===
    setupSlider(inputGainSlider, inputGainLabel);
    inputGainSlider.setLookAndFeel(&orangeKnobLookAndFeel);
    setupSlider(outputTrimSlider, outputTrimLabel);
    setupSlider(transformerDriveSlider, transformerLabel);

    setupComboBox(hpfCombo);
    addAndMakeVisible(hpfLabel);
    hpfLabel.setJustificationType(juce::Justification::centred);

    phaseButton.setLookAndFeel(&neveLookAndFeel);
    addAndMakeVisible(phaseButton);

    // === EQ SECTION ===
    setupComboBox(hfFreqCombo);
    setupSlider(hfGainSlider, hfLabel);
    hfGainSlider.setLookAndFeel(&redKnobLookAndFeel);

    setupComboBox(hmFreqCombo);
    setupSlider(hmGainSlider, hmLabel);

    setupComboBox(lmFreqCombo);
    setupSlider(lmGainSlider, lmLabel);

    setupComboBox(lfFreqCombo);
    setupSlider(lfGainSlider, lfLabel);
    lfGainSlider.setLookAndFeel(&redKnobLookAndFeel);

    eqBypassButton.setLookAndFeel(&neveLookAndFeel);
    eqPrePostButton.setLookAndFeel(&neveLookAndFeel);
    addAndMakeVisible(eqBypassButton);
    addAndMakeVisible(eqPrePostButton);

    // === DYNAMICS SECTION ===
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

    // === OUTPUT SECTION ===
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
    label.setFont(juce::Font(10.0f, juce::Font::bold));
    addAndMakeVisible(label);
}

void NeveStripAudioProcessorEditor::setupComboBox(juce::ComboBox& combo)
{
    combo.setLookAndFeel(&neveLookAndFeel);
    addAndMakeVisible(combo);
}

void NeveStripAudioProcessorEditor::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    // Main faceplate - classic Neve blue-grey metal
    juce::ColourGradient bgGradient(
        juce::Colour(neveMetalLight), 0, 0,
        juce::Colour(neveBlueGrey), 0, (float)getHeight(), false);
    g.setGradientFill(bgGradient);
    g.fillRect(bounds);

    // Brushed metal texture (horizontal lines)
    g.setColour(juce::Colour(neveBlueGrey).brighter(0.03f));
    for (int i = 0; i < getHeight(); i += 2)
    {
        g.drawHorizontalLine(i, 0.0f, (float)getWidth());
    }

    // Left edge shadow
    g.setColour(juce::Colour(0x30000000));
    g.fillRect(0, 0, 3, getHeight());

    // Right edge highlight
    g.setColour(juce::Colour(0x20FFFFFF));
    g.fillRect(getWidth() - 3, 0, 3, getHeight());

    // Rack mount holes (top)
    drawRackScrew(g, 12, 8);
    drawRackScrew(g, getWidth() - 12, 8);

    // Rack mount holes (bottom)
    drawRackScrew(g, 12, getHeight() - 8);
    drawRackScrew(g, getWidth() - 12, getHeight() - 8);

    // Top banner area
    g.setColour(juce::Colour(neveDarkBlue));
    g.fillRect(0, 18, getWidth(), 32);

    // Banner bevels
    g.setColour(juce::Colour(neveMetalLight));
    g.drawHorizontalLine(18, 0, (float)getWidth());
    g.setColour(juce::Colour(0xFF2A3440));
    g.drawHorizontalLine(49, 0, (float)getWidth());

    // Logo/Title
    g.setFont(juce::Font(20.0f, juce::Font::bold));
    g.setColour(juce::Colour(0xFF2A3440));  // Shadow
    g.drawText("NEVESTRIP", 0, 22, getWidth(), 24, juce::Justification::centred);
    g.setColour(juce::Colour(neveGold));
    g.drawText("NEVESTRIP", 0, 21, getWidth(), 24, juce::Justification::centred);

    // Meters area background on right side
    int meterAreaX = getWidth() - 70;
    g.setColour(juce::Colour(neveDarkBlue).withAlpha(0.5f));
    g.fillRect(meterAreaX - 5, 55, 75, getHeight() - 70);
}

void NeveStripAudioProcessorEditor::drawRackScrew(juce::Graphics& g, int x, int y)
{
    float radius = 5.0f;

    // Screw hole shadow
    g.setColour(juce::Colour(0xFF2A3440));
    g.fillEllipse((float)x - radius - 1, (float)y - radius - 1, radius * 2 + 2, radius * 2 + 2);

    // Screw head
    juce::ColourGradient screwGradient(
        juce::Colour(0xFFB0B8C0), (float)x - radius, (float)y - radius,
        juce::Colour(0xFF707880), (float)x + radius, (float)y + radius, true);
    g.setGradientFill(screwGradient);
    g.fillEllipse((float)x - radius, (float)y - radius, radius * 2, radius * 2);

    // Phillips head slot
    g.setColour(juce::Colour(0xFF404850));
    g.drawLine((float)x - 3, (float)y, (float)x + 3, (float)y, 1.5f);
    g.drawLine((float)x, (float)y - 3, (float)x, (float)y + 3, 1.5f);
}

void NeveStripAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    int margin = 8;
    int smallMargin = 4;
    int comboHeight = 20;
    int buttonHeight = 22;
    int currentY = 55;

    // Meter area on right side
    int meterAreaWidth = 65;
    int meterWidth = 18;
    int meterHeight = 100;
    int controlAreaWidth = getWidth() - meterAreaWidth - margin;

    // === INPUT/PREAMP SECTION ===
    preampHeader.setBounds(0, currentY, controlAreaWidth, headerHeight);
    currentY += headerHeight + smallMargin;

    // Row 1: Input Gain (large, centered)
    auto inputArea = juce::Rectangle<int>(margin, currentY, controlAreaWidth - margin * 2, knobSize + labelHeight);
    inputGainLabel.setBounds(inputArea.removeFromTop(labelHeight));
    inputGainSlider.setBounds(inputArea.withSizeKeepingCentre(knobSize, knobSize));
    currentY += knobSize + labelHeight + smallMargin;

    // Row 2: Trim and Drive side by side
    int halfWidth = (controlAreaWidth - margin * 3) / 2;
    auto trimArea = juce::Rectangle<int>(margin, currentY, halfWidth, smallKnobSize + labelHeight);
    outputTrimLabel.setBounds(trimArea.removeFromTop(labelHeight));
    outputTrimSlider.setBounds(trimArea.withSizeKeepingCentre(smallKnobSize, smallKnobSize));

    auto driveArea = juce::Rectangle<int>(margin * 2 + halfWidth, currentY, halfWidth, smallKnobSize + labelHeight);
    transformerLabel.setBounds(driveArea.removeFromTop(labelHeight));
    transformerDriveSlider.setBounds(driveArea.withSizeKeepingCentre(smallKnobSize, smallKnobSize));
    currentY += smallKnobSize + labelHeight + smallMargin;

    // Row 3: HPF and Phase
    hpfLabel.setBounds(margin, currentY, halfWidth, labelHeight);
    phaseButton.setBounds(margin * 2 + halfWidth, currentY, halfWidth, buttonHeight);
    currentY += labelHeight;
    hpfCombo.setBounds(margin, currentY, halfWidth, comboHeight);
    currentY += comboHeight + sectionPadding;

    // === EQ SECTION ===
    eqHeader.setBounds(0, currentY, controlAreaWidth, headerHeight);
    currentY += headerHeight + smallMargin;

    // EQ bands in 2x2 grid
    int eqKnobSize = 44;
    int eqColWidth = (controlAreaWidth - margin * 3) / 2;
    int eqRowHeight = eqKnobSize + labelHeight + comboHeight + 2;

    // HF (top left)
    auto hfArea = juce::Rectangle<int>(margin, currentY, eqColWidth, eqRowHeight);
    hfLabel.setBounds(hfArea.removeFromTop(labelHeight));
    hfFreqCombo.setBounds(hfArea.removeFromTop(comboHeight).reduced(2, 0));
    hfGainSlider.setBounds(hfArea.withSizeKeepingCentre(eqKnobSize, eqKnobSize));

    // HM (top right)
    auto hmArea = juce::Rectangle<int>(margin * 2 + eqColWidth, currentY, eqColWidth, eqRowHeight);
    hmLabel.setBounds(hmArea.removeFromTop(labelHeight));
    hmFreqCombo.setBounds(hmArea.removeFromTop(comboHeight).reduced(2, 0));
    hmGainSlider.setBounds(hmArea.withSizeKeepingCentre(eqKnobSize, eqKnobSize));

    currentY += eqRowHeight + 2;

    // LM (bottom left)
    auto lmArea = juce::Rectangle<int>(margin, currentY, eqColWidth, eqRowHeight);
    lmLabel.setBounds(lmArea.removeFromTop(labelHeight));
    lmFreqCombo.setBounds(lmArea.removeFromTop(comboHeight).reduced(2, 0));
    lmGainSlider.setBounds(lmArea.withSizeKeepingCentre(eqKnobSize, eqKnobSize));

    // LF (bottom right)
    auto lfArea = juce::Rectangle<int>(margin * 2 + eqColWidth, currentY, eqColWidth, eqRowHeight);
    lfLabel.setBounds(lfArea.removeFromTop(labelHeight));
    lfFreqCombo.setBounds(lfArea.removeFromTop(comboHeight).reduced(2, 0));
    lfGainSlider.setBounds(lfArea.withSizeKeepingCentre(eqKnobSize, eqKnobSize));

    currentY += eqRowHeight + 2;

    // EQ buttons
    eqBypassButton.setBounds(margin, currentY, eqColWidth, buttonHeight);
    eqPrePostButton.setBounds(margin * 2 + eqColWidth, currentY, eqColWidth, buttonHeight);
    currentY += buttonHeight + sectionPadding;

    // === DYNAMICS SECTION ===
    dynamicsHeader.setBounds(0, currentY, controlAreaWidth, headerHeight);
    currentY += headerHeight + smallMargin;

    // Compressor: Threshold and Makeup knobs
    int dynKnobSize = 44;
    auto threshArea = juce::Rectangle<int>(margin, currentY, halfWidth, dynKnobSize + labelHeight);
    compThreshLabel.setBounds(threshArea.removeFromTop(labelHeight));
    compThresholdSlider.setBounds(threshArea.withSizeKeepingCentre(dynKnobSize, dynKnobSize));

    auto makeupArea = juce::Rectangle<int>(margin * 2 + halfWidth, currentY, halfWidth, dynKnobSize + labelHeight);
    compMakeupLabel.setBounds(makeupArea.removeFromTop(labelHeight));
    compMakeupSlider.setBounds(makeupArea.withSizeKeepingCentre(dynKnobSize, dynKnobSize));
    currentY += dynKnobSize + labelHeight + 2;

    // Ratio, Attack, Release in a row
    int thirdWidth = (controlAreaWidth - margin * 4) / 3;
    compRatioLabel.setBounds(margin, currentY, thirdWidth, labelHeight);
    compAttackLabel.setBounds(margin * 2 + thirdWidth, currentY, thirdWidth, labelHeight);
    compReleaseLabel.setBounds(margin * 3 + thirdWidth * 2, currentY, thirdWidth, labelHeight);
    currentY += labelHeight;

    compRatioCombo.setBounds(margin, currentY, thirdWidth, comboHeight);
    compAttackCombo.setBounds(margin * 2 + thirdWidth, currentY, thirdWidth, comboHeight);
    compReleaseCombo.setBounds(margin * 3 + thirdWidth * 2, currentY, thirdWidth, comboHeight);
    currentY += comboHeight + 2;

    // Comp buttons
    compBypassButton.setBounds(margin, currentY, thirdWidth, buttonHeight);
    compLinkButton.setBounds(margin * 2 + thirdWidth, currentY, thirdWidth, buttonHeight);
    compSCHPFButton.setBounds(margin * 3 + thirdWidth * 2, currentY, thirdWidth, buttonHeight);
    currentY += buttonHeight + smallMargin;

    // Limiter row
    limThreshLabel.setBounds(margin, currentY, halfWidth, labelHeight);
    currentY += labelHeight;
    limThresholdSlider.setBounds(margin + (halfWidth - smallKnobSize) / 2, currentY, smallKnobSize, smallKnobSize);
    limBypassButton.setBounds(margin * 2 + halfWidth, currentY + (smallKnobSize - buttonHeight) / 2, halfWidth, buttonHeight);
    currentY += smallKnobSize + sectionPadding;

    // === OUTPUT SECTION ===
    outputHeader.setBounds(0, currentY, controlAreaWidth, headerHeight);
    currentY += headerHeight + smallMargin;

    // Output level knob (centered)
    auto outputArea = juce::Rectangle<int>(margin, currentY, controlAreaWidth - margin * 2, knobSize + labelHeight);
    outputLabel.setBounds(outputArea.removeFromTop(labelHeight));
    outputLevelSlider.setBounds(outputArea.withSizeKeepingCentre(knobSize, knobSize));
    currentY += knobSize + labelHeight + smallMargin;

    // Master bypass
    masterBypassButton.setBounds(margin + (controlAreaWidth - margin * 2 - 100) / 2, currentY, 100, buttonHeight);

    // === METERS (Right side) ===
    int meterStartY = 60;
    int meterX = getWidth() - meterAreaWidth + 5;
    int meterSpacing = (getHeight() - meterStartY - 30) / 3;

    // Input meter
    inputMeterLabel.setBounds(meterX, meterStartY, meterAreaWidth - 10, labelHeight);
    inputMeter.setBounds(meterX + (meterAreaWidth - 10 - meterWidth) / 2, meterStartY + labelHeight, meterWidth, meterHeight);

    // Output meter
    outputMeterLabel.setBounds(meterX, meterStartY + meterSpacing, meterAreaWidth - 10, labelHeight);
    outputMeter.setBounds(meterX + (meterAreaWidth - 10 - meterWidth) / 2, meterStartY + meterSpacing + labelHeight, meterWidth, meterHeight);

    // GR meter
    grMeterLabel.setBounds(meterX, meterStartY + meterSpacing * 2, meterAreaWidth - 10, labelHeight);
    grMeter.setBounds(meterX + (meterAreaWidth - 10 - meterWidth) / 2, meterStartY + meterSpacing * 2 + labelHeight, meterWidth, meterHeight);
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
