#include "PluginEditor.h"

//==============================================================================
// NeveLookAndFeel - Chicken Head Knob Style
//==============================================================================
NeveLookAndFeel::NeveLookAndFeel()
{
    knobColour = juce::Colour(0xFFE8E0D0);  // Warmer vintage cream
    isChickenHead = true;

    // More realistic steel-blue colors
    setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xFF4A5A68));
    setColour(juce::ComboBox::textColourId, juce::Colour(0xFFE0D8C8));
    setColour(juce::ComboBox::outlineColourId, juce::Colour(0xFF3A4A58));
    setColour(juce::ComboBox::arrowColourId, juce::Colour(0xFFD0C8B8));

    setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0xFF4A5A68));
    setColour(juce::PopupMenu::textColourId, juce::Colour(0xFFE0D8C8));
    setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xFF6A7A88));
    setColour(juce::PopupMenu::highlightedTextColourId, juce::Colour(0xFFFFFFFF));

    setColour(juce::Label::textColourId, juce::Colour(0xFFE0D8C8));
}

void NeveLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                        float sliderPosProportional, float /*rotaryStartAngle*/,
                                        float /*rotaryEndAngle*/, juce::Slider& /*slider*/)
{
    auto bounds = juce::Rectangle<float>((float)x, (float)y, (float)width, (float)height).reduced(2.0f);
    float cx = bounds.getCentreX();
    float cy = bounds.getCentreY();
    float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 1.0f;

    // Drop shadow underneath
    juce::ColourGradient shadowGrad(
        juce::Colour(0x50000000), cx, cy,
        juce::Colours::transparentBlack, cx, cy + radius * 1.3f, true);
    g.setGradientFill(shadowGrad);
    g.fillEllipse(cx - radius + 3, cy - radius + 3, radius * 2.0f, radius * 2.0f);

    // Outer metal ring (bezel) - more realistic brushed metal
    juce::ColourGradient bezelGradient(
        juce::Colour(0xFF909898), cx - radius * 0.7f, cy - radius * 0.7f,
        juce::Colour(0xFF505860), cx + radius * 0.7f, cy + radius * 0.7f, true);
    bezelGradient.addColour(0.3, juce::Colour(0xFF707880));
    bezelGradient.addColour(0.6, juce::Colour(0xFF808890));
    g.setGradientFill(bezelGradient);
    g.fillEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);

    // Bezel edge highlight
    g.setColour(juce::Colour(0x30FFFFFF));
    g.drawEllipse(cx - radius + 1, cy - radius + 1, (radius - 1) * 2.0f, (radius - 1) * 2.0f, 1.0f);

    // Knurled edge pattern - more subtle and realistic
    float knurlRadius = radius - 1.5f;
    int numKnurls = 32;
    for (int i = 0; i < numKnurls; ++i)
    {
        float angle = i * juce::MathConstants<float>::twoPi / numKnurls;
        float x1 = cx + knurlRadius * std::cos(angle);
        float y1 = cy + knurlRadius * std::sin(angle);
        float x2 = cx + (knurlRadius - 2.5f) * std::cos(angle);
        float y2 = cy + (knurlRadius - 2.5f) * std::sin(angle);

        // Alternate light and dark for 3D knurl effect
        if (i % 2 == 0)
            g.setColour(juce::Colour(0xFF404850));
        else
            g.setColour(juce::Colour(0xFF606870));
        g.drawLine(x1, y1, x2, y2, 0.8f);
    }

    // Main knob body with improved 3D gradient
    float innerRadius = radius * 0.70f;
    juce::ColourGradient knobGradient(
        knobColour.brighter(0.25f), cx - innerRadius * 0.4f, cy - innerRadius * 0.4f,
        knobColour.darker(0.15f), cx + innerRadius * 0.4f, cy + innerRadius * 0.4f, true);
    knobGradient.addColour(0.4, knobColour.brighter(0.08f));
    knobGradient.addColour(0.6, knobColour);
    g.setGradientFill(knobGradient);
    g.fillEllipse(cx - innerRadius, cy - innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);

    // Subtle highlight ring on knob
    g.setColour(knobColour.brighter(0.3f).withAlpha(0.4f));
    g.drawEllipse(cx - innerRadius + 1.5f, cy - innerRadius + 1.5f, (innerRadius - 1.5f) * 2.0f, (innerRadius - 1.5f) * 2.0f, 1.0f);

    // Pointer angle calculation
    float pointerAngle = juce::jmap(sliderPosProportional, 0.0f, 1.0f, -2.4f, 2.4f) + juce::MathConstants<float>::pi;

    if (isChickenHead)
    {
        // CHICKEN HEAD pointer style - more refined
        float pointerLength = innerRadius * 0.82f;
        float baseWidth = innerRadius * 0.32f;
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
        g.setColour(juce::Colour(0x35000000));
        g.fillPath(pointer, juce::AffineTransform::translation(1.5f, 1.5f));

        // Main pointer with gradient
        juce::ColourGradient ptrGrad(
            knobColour.darker(0.05f), tipX, tipY,
            knobColour.darker(0.2f), cx, cy, false);
        g.setGradientFill(ptrGrad);
        g.fillPath(pointer);

        // Pointer highlight edge
        g.setColour(knobColour.brighter(0.15f));
        g.strokePath(pointer, juce::PathStrokeType(0.7f));
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

    // Center cap screw - more realistic
    float capRadius = innerRadius * 0.24f;
    juce::ColourGradient capGradient(
        juce::Colour(0xFF707880), cx - capRadius * 0.6f, cy - capRadius * 0.6f,
        juce::Colour(0xFF404850), cx + capRadius * 0.6f, cy + capRadius * 0.6f, true);
    g.setGradientFill(capGradient);
    g.fillEllipse(cx - capRadius, cy - capRadius, capRadius * 2.0f, capRadius * 2.0f);

    // Cap edge
    g.setColour(juce::Colour(0xFF303840));
    g.drawEllipse(cx - capRadius, cy - capRadius, capRadius * 2.0f, capRadius * 2.0f, 0.5f);

    // Screw slot with depth
    g.setColour(juce::Colour(0xFF1A2028));
    g.drawLine(cx - capRadius * 0.55f, cy, cx + capRadius * 0.55f, cy, 1.8f);
    g.setColour(juce::Colour(0x30FFFFFF));
    g.drawLine(cx - capRadius * 0.5f, cy + 0.8f, cx + capRadius * 0.5f, cy + 0.8f, 0.5f);
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
// VUMeter - Classic Analog Style with Scale Markings
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

void VUMeter::drawScaleMarkings(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    g.setFont(juce::Font(8.0f));

    if (showGR)
    {
        // GR scale: 0 to -20 dB
        const char* labels[] = { "0", "-5", "-10", "-15", "-20" };
        for (int i = 0; i < 5; ++i)
        {
            float y = bounds.getY() + (i * bounds.getHeight() / 4.0f);
            g.setColour(juce::Colour(0xFF909090));
            g.drawText(labels[i], bounds.getRight() + 2, (int)y - 5, 20, 10, juce::Justification::left);
            g.setColour(juce::Colour(0xFF505860));
            g.drawHorizontalLine((int)y, bounds.getRight() - 4, bounds.getRight());
        }
    }
    else
    {
        // Level scale: +6 to -48 dB
        const char* labels[] = { "+6", "0", "-6", "-12", "-24", "-48" };
        float positions[] = { 0.0f, 0.1f, 0.2f, 0.3f, 0.5f, 0.9f };
        for (int i = 0; i < 6; ++i)
        {
            float y = bounds.getY() + (positions[i] * bounds.getHeight());
            g.setColour(juce::Colour(0xFF909090));
            g.drawText(labels[i], bounds.getRight() + 2, (int)y - 5, 22, 10, juce::Justification::left);
            g.setColour(juce::Colour(0xFF505860));
            g.drawHorizontalLine((int)y, bounds.getRight() - 4, bounds.getRight());
        }
    }
}

void VUMeter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Leave space for scale markings on the right
    auto meterBounds = bounds.removeFromLeft(bounds.getWidth() * 0.6f).reduced(1.0f);

    // Outer bezel with metallic gradient
    juce::ColourGradient bezelGrad(
        juce::Colour(0xFF404850), meterBounds.getX(), meterBounds.getY(),
        juce::Colour(0xFF252A30), meterBounds.getRight(), meterBounds.getBottom(), false);
    g.setGradientFill(bezelGrad);
    g.fillRoundedRectangle(meterBounds, 4.0f);

    // Inner recessed area
    auto innerBounds = meterBounds.reduced(3.0f);
    g.setColour(juce::Colour(0xFF101418));
    g.fillRoundedRectangle(innerBounds, 2.0f);

    // Meter value
    float displayValue = showGR ? gainReduction : level;

    // Smooth the display (non-const for updating)
    float targetSmoothed = showGR ? smoothedGR : smoothedLevel;
    targetSmoothed += 0.25f * (displayValue - targetSmoothed);
    if (showGR)
        smoothedGR = targetSmoothed;
    else
        smoothedLevel = targetSmoothed;

    // Convert to display height
    float meterFill;
    if (showGR)
    {
        meterFill = std::min(1.0f, smoothedGR / 20.0f);
    }
    else
    {
        float dbLevel = 20.0f * std::log10(std::max(0.0001f, smoothedLevel));
        meterFill = std::max(0.0f, (dbLevel + 60.0f) / 66.0f);  // -60 to +6 dB range
    }

    // Draw meter bar with segments
    auto barBounds = innerBounds.reduced(2.0f);
    int numSegments = 20;  // More segments for finer resolution
    float segmentHeight = barBounds.getHeight() / numSegments;
    float segmentGap = 1.0f;

    for (int i = 0; i < numSegments; ++i)
    {
        float segmentY = barBounds.getBottom() - (i + 1) * segmentHeight;
        float segmentThreshold = (float)(i + 1) / numSegments;

        if (meterFill >= segmentThreshold - 0.04f)
        {
            juce::Colour segColour;
            if (showGR)
            {
                // GR meter: gradient from green to yellow to red
                float ratio = (float)i / numSegments;
                if (ratio < 0.4f)
                    segColour = juce::Colour(0xFF44AA44);
                else if (ratio < 0.7f)
                    segColour = juce::Colour(0xFFD4A020);
                else
                    segColour = juce::Colour(0xFFB03030);
            }
            else if (i >= 18)  // Top 2 segments (> 0 dB)
            {
                segColour = juce::Colour(0xFFB03030);  // Red (hot)
            }
            else if (i >= 15)  // -6 to 0 dB
            {
                segColour = juce::Colour(0xFFD4A020);  // Amber
            }
            else
            {
                segColour = juce::Colour(0xFF44AA44);  // Green
            }

            // Add slight gradient to each segment for 3D effect
            juce::ColourGradient segGrad(
                segColour.brighter(0.1f), barBounds.getX(), segmentY,
                segColour.darker(0.1f), barBounds.getRight(), segmentY, false);
            g.setGradientFill(segGrad);
            g.fillRect(barBounds.getX(), segmentY + segmentGap,
                       barBounds.getWidth(), segmentHeight - segmentGap * 2);
        }
        else
        {
            // Dim segment with slight 3D effect
            g.setColour(juce::Colour(0xFF1A1E22));
            g.fillRect(barBounds.getX(), segmentY + segmentGap,
                       barBounds.getWidth(), segmentHeight - segmentGap * 2);
        }
    }

    // Draw scale markings
    drawScaleMarkings(g, innerBounds);

    // Glass reflection effect
    juce::ColourGradient glassReflection(
        juce::Colours::white.withAlpha(0.08f), innerBounds.getX(), innerBounds.getY(),
        juce::Colours::transparentWhite, innerBounds.getX(), innerBounds.getY() + 20, false);
    g.setGradientFill(glassReflection);
    g.fillRoundedRectangle(innerBounds.getX(), innerBounds.getY(), innerBounds.getWidth(), 20, 2.0f);
}

//==============================================================================
// SectionHeader - Realistic Metal Strip Style
//==============================================================================
SectionHeader::SectionHeader(const juce::String& t) : title(t)
{
}

void SectionHeader::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Realistic brushed metal gradient with multiple color stops
    juce::ColourGradient stripGradient(
        juce::Colour(0xFF6A7A88), 0, bounds.getY(),
        juce::Colour(0xFF4A5A68), 0, bounds.getBottom(), false);
    stripGradient.addColour(0.3, juce::Colour(0xFF5A6A78));
    stripGradient.addColour(0.5, juce::Colour(0xFF7A8A98));  // Highlight in middle
    stripGradient.addColour(0.7, juce::Colour(0xFF5A6A78));
    g.setGradientFill(stripGradient);
    g.fillRect(bounds);

    // Subtle horizontal brush lines
    g.setColour(juce::Colours::white.withAlpha(0.03f));
    for (int i = 0; i < (int)bounds.getHeight(); i += 2)
    {
        g.drawHorizontalLine((int)bounds.getY() + i, bounds.getX(), bounds.getRight());
    }

    // Top bright edge (light source from above)
    g.setColour(juce::Colour(0xFF9AAAB8));
    g.drawHorizontalLine((int)bounds.getY(), bounds.getX(), bounds.getRight());
    g.setColour(juce::Colour(0x40FFFFFF));
    g.drawHorizontalLine((int)bounds.getY() + 1, bounds.getX(), bounds.getRight());

    // Bottom shadow edge
    g.setColour(juce::Colour(0xFF2A3A48));
    g.drawHorizontalLine((int)bounds.getBottom() - 1, bounds.getX(), bounds.getRight());
    g.setColour(juce::Colour(0xFF1A2A38));
    g.drawHorizontalLine((int)bounds.getBottom() - 2, bounds.getX(), bounds.getRight());

    // Engraved text with proper emboss effect
    g.setFont(juce::Font(11.0f, juce::Font::bold));

    // Light edge below text (as if light catches the engraving)
    g.setColour(juce::Colour(0x60FFFFFF));
    g.drawText(title, bounds.translated(0.5f, 1.0f).reduced(8.0f, 0.0f), juce::Justification::centred);

    // Dark shadow inside the engraving
    g.setColour(juce::Colour(0xFF1A2A38));
    g.drawText(title, bounds.translated(-0.5f, -0.5f).reduced(8.0f, 0.0f), juce::Justification::centred);

    // Main gold text
    g.setColour(juce::Colour(0xFFC4983C));  // More realistic gold
    g.drawText(title, bounds.reduced(8.0f, 0.0f), juce::Justification::centred);
}

//==============================================================================
// NeveStripContent - Scrollable content area
//==============================================================================
void NeveStripContent::paint(juce::Graphics& g)
{
    // Content area is painted by the parent editor
    juce::ignoreUnused(g);
}

//==============================================================================
// NeveStripAudioProcessorEditor - Vertical Channel Strip Layout
//==============================================================================
NeveStripAudioProcessorEditor::NeveStripAudioProcessorEditor(NeveStripAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(stripWidth, stripHeight);

    // Setup viewport for scrolling
    viewport.setViewedComponent(&contentComponent, false);
    viewport.setScrollBarsShown(true, false);  // Vertical scrollbar only
    viewport.getVerticalScrollBar().setColour(juce::ScrollBar::thumbColourId, juce::Colour(0xFF5A6A78));
    viewport.getVerticalScrollBar().setColour(juce::ScrollBar::trackColourId, juce::Colour(0xFF2A3A48));
    addAndMakeVisible(viewport);

    orangeKnobLookAndFeel.setKnobColour(juce::Colour(neveOrange));
    redKnobLookAndFeel.setKnobColour(juce::Colour(neveRed));

    // Setup all components - add to content component for scrolling
    contentComponent.addAndMakeVisible(preampHeader);
    contentComponent.addAndMakeVisible(eqHeader);
    contentComponent.addAndMakeVisible(dynamicsHeader);
    contentComponent.addAndMakeVisible(outputHeader);

    // === PREAMP/INPUT SECTION ===
    setupSlider(inputGainSlider, inputGainLabel);
    inputGainSlider.setLookAndFeel(&orangeKnobLookAndFeel);
    setupSlider(outputTrimSlider, outputTrimLabel);
    setupSlider(transformerDriveSlider, transformerLabel);

    setupComboBox(hpfCombo);
    contentComponent.addAndMakeVisible(hpfLabel);
    hpfLabel.setJustificationType(juce::Justification::centred);

    phaseButton.setLookAndFeel(&neveLookAndFeel);
    contentComponent.addAndMakeVisible(phaseButton);

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
    contentComponent.addAndMakeVisible(eqBypassButton);
    contentComponent.addAndMakeVisible(eqPrePostButton);

    // === DYNAMICS SECTION ===
    setupSlider(compThresholdSlider, compThreshLabel);
    setupComboBox(compRatioCombo);
    contentComponent.addAndMakeVisible(compRatioLabel);
    compRatioLabel.setJustificationType(juce::Justification::centred);
    setupComboBox(compAttackCombo);
    contentComponent.addAndMakeVisible(compAttackLabel);
    compAttackLabel.setJustificationType(juce::Justification::centred);
    setupComboBox(compReleaseCombo);
    contentComponent.addAndMakeVisible(compReleaseLabel);
    compReleaseLabel.setJustificationType(juce::Justification::centred);
    setupSlider(compMakeupSlider, compMakeupLabel);
    compMakeupSlider.setLookAndFeel(&orangeKnobLookAndFeel);

    compSCHPFButton.setLookAndFeel(&neveLookAndFeel);
    compLinkButton.setLookAndFeel(&neveLookAndFeel);
    compBypassButton.setLookAndFeel(&neveLookAndFeel);
    contentComponent.addAndMakeVisible(compSCHPFButton);
    contentComponent.addAndMakeVisible(compLinkButton);
    contentComponent.addAndMakeVisible(compBypassButton);

    setupSlider(limThresholdSlider, limThreshLabel);
    limBypassButton.setLookAndFeel(&neveLookAndFeel);
    contentComponent.addAndMakeVisible(limBypassButton);

    // === OUTPUT SECTION ===
    setupSlider(outputLevelSlider, outputLabel);
    outputLevelSlider.setLookAndFeel(&orangeKnobLookAndFeel);
    masterBypassButton.setLookAndFeel(&neveLookAndFeel);
    contentComponent.addAndMakeVisible(masterBypassButton);

    // Meters - these stay in the main editor (fixed position)
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
    contentComponent.addAndMakeVisible(slider);

    label.setJustificationType(juce::Justification::centred);
    label.setFont(juce::Font(10.0f, juce::Font::bold));
    contentComponent.addAndMakeVisible(label);
}

void NeveStripAudioProcessorEditor::setupComboBox(juce::ComboBox& combo)
{
    combo.setLookAndFeel(&neveLookAndFeel);
    contentComponent.addAndMakeVisible(combo);
}

void NeveStripAudioProcessorEditor::drawBrushedMetalTexture(juce::Graphics& g, juce::Rectangle<float> bounds, juce::Colour baseColour)
{
    // Base metal gradient
    juce::ColourGradient metalGrad(
        baseColour.brighter(0.08f), bounds.getX(), bounds.getY(),
        baseColour.darker(0.05f), bounds.getX(), bounds.getBottom(), false);
    metalGrad.addColour(0.3, baseColour.brighter(0.03f));
    metalGrad.addColour(0.5, baseColour);
    metalGrad.addColour(0.7, baseColour.darker(0.02f));
    g.setGradientFill(metalGrad);
    g.fillRect(bounds);

    // Subtle diagonal brush strokes (like real brushed aluminum)
    g.setColour(juce::Colours::white.withAlpha(0.02f));
    for (float i = -bounds.getHeight(); i < bounds.getWidth() + bounds.getHeight(); i += 2.0f)
    {
        g.drawLine(bounds.getX() + i, bounds.getY(),
                   bounds.getX() + i + bounds.getHeight() * 0.3f, bounds.getBottom(), 0.5f);
    }

    // Additional horizontal brush lines
    g.setColour(juce::Colours::white.withAlpha(0.015f));
    for (int y = 0; y < (int)bounds.getHeight(); y += 2)
    {
        g.drawHorizontalLine((int)bounds.getY() + y, bounds.getX(), bounds.getRight());
    }
}

void NeveStripAudioProcessorEditor::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Main faceplate - realistic brushed steel blue-grey
    drawBrushedMetalTexture(g, bounds, juce::Colour(neveBlueGrey));

    // Left edge shadow (3D depth)
    juce::ColourGradient leftShadow(
        juce::Colour(0x50000000), 0, 0,
        juce::Colours::transparentBlack, 4, 0, false);
    g.setGradientFill(leftShadow);
    g.fillRect(0, 0, 5, getHeight());

    // Right edge highlight
    juce::ColourGradient rightHighlight(
        juce::Colours::transparentWhite, (float)getWidth() - 5, 0,
        juce::Colour(0x18FFFFFF), (float)getWidth(), 0, false);
    g.setGradientFill(rightHighlight);
    g.fillRect(getWidth() - 5, 0, 5, getHeight());

    // Top edge highlight
    g.setColour(juce::Colour(0x25FFFFFF));
    g.drawHorizontalLine(0, 0, (float)getWidth());

    // Bottom edge shadow
    g.setColour(juce::Colour(0x40000000));
    g.drawHorizontalLine(getHeight() - 1, 0, (float)getWidth());

    // Rack mount holes (top)
    drawRackScrew(g, 14, 10);
    drawRackScrew(g, getWidth() - 14, 10);

    // Rack mount holes (bottom)
    drawRackScrew(g, 14, getHeight() - 10);
    drawRackScrew(g, getWidth() - 14, getHeight() - 10);

    // Top banner area with darker steel
    auto bannerBounds = juce::Rectangle<float>(0, 22, (float)getWidth(), 34);
    juce::ColourGradient bannerGrad(
        juce::Colour(neveDarkBlue).brighter(0.05f), 0, bannerBounds.getY(),
        juce::Colour(neveDarkBlue).darker(0.1f), 0, bannerBounds.getBottom(), false);
    g.setGradientFill(bannerGrad);
    g.fillRect(bannerBounds);

    // Banner bevels
    g.setColour(juce::Colour(neveMetalLight).withAlpha(0.4f));
    g.drawHorizontalLine(22, 0, (float)getWidth());
    g.setColour(juce::Colour(neveMetalDark));
    g.drawHorizontalLine(55, 0, (float)getWidth());

    // Logo/Title with engraved look
    g.setFont(juce::Font(18.0f, juce::Font::bold));

    // Light catch on the engraving
    g.setColour(juce::Colour(0x50FFFFFF));
    g.drawText("NEVESTRIP", 0, 30, getWidth(), 24, juce::Justification::centred);

    // Shadow in the engraving
    g.setColour(juce::Colour(neveMetalDark));
    g.drawText("NEVESTRIP", 0, 28, getWidth(), 24, juce::Justification::centred);

    // Main gold text
    g.setColour(juce::Colour(neveGold));
    g.drawText("NEVESTRIP", 0, 29, getWidth(), 24, juce::Justification::centred);

    // Meters area background on right side (recessed panel look)
    int meterAreaX = getWidth() - meterAreaWidth;
    auto meterPanel = juce::Rectangle<float>((float)meterAreaX, 60, (float)meterAreaWidth, (float)getHeight() - 75);

    // Recessed area
    juce::ColourGradient recessGrad(
        juce::Colour(neveMetalDark).darker(0.2f), meterPanel.getX(), meterPanel.getY(),
        juce::Colour(neveMetalDark), meterPanel.getX(), meterPanel.getBottom(), false);
    g.setGradientFill(recessGrad);
    g.fillRect(meterPanel);

    // Inner shadow on top/left of recess
    g.setColour(juce::Colour(0x40000000));
    g.drawHorizontalLine((int)meterPanel.getY(), meterPanel.getX(), meterPanel.getRight());
    g.drawVerticalLine((int)meterPanel.getX(), meterPanel.getY(), meterPanel.getBottom());

    // Bottom/right highlight of recess
    g.setColour(juce::Colour(0x20FFFFFF));
    g.drawHorizontalLine((int)meterPanel.getBottom() - 1, meterPanel.getX(), meterPanel.getRight());
    g.drawVerticalLine((int)meterPanel.getRight() - 1, meterPanel.getY(), meterPanel.getBottom());
}

void NeveStripAudioProcessorEditor::drawRackScrew(juce::Graphics& g, int x, int y)
{
    float radius = 6.0f;

    // Screw hole recess (dark shadow)
    g.setColour(juce::Colour(0xFF1A2430));
    g.fillEllipse((float)x - radius - 2, (float)y - radius - 2, radius * 2 + 4, radius * 2 + 4);

    // Outer rim highlight
    juce::ColourGradient rimGrad(
        juce::Colour(0x30FFFFFF), (float)x - radius, (float)y - radius,
        juce::Colours::transparentBlack, (float)x + radius, (float)y + radius, true);
    g.setGradientFill(rimGrad);
    g.fillEllipse((float)x - radius - 1, (float)y - radius - 1, radius * 2 + 2, radius * 2 + 2);

    // Screw head with realistic metal gradient
    juce::ColourGradient screwGradient(
        juce::Colour(0xFFA8B0B8), (float)x - radius * 0.7f, (float)y - radius * 0.7f,
        juce::Colour(0xFF606870), (float)x + radius * 0.7f, (float)y + radius * 0.7f, true);
    screwGradient.addColour(0.4, juce::Colour(0xFF909898));
    g.setGradientFill(screwGradient);
    g.fillEllipse((float)x - radius, (float)y - radius, radius * 2, radius * 2);

    // Subtle edge ring
    g.setColour(juce::Colour(0xFF505860));
    g.drawEllipse((float)x - radius + 0.5f, (float)y - radius + 0.5f, radius * 2 - 1, radius * 2 - 1, 0.5f);

    // Phillips head slot with depth
    g.setColour(juce::Colour(0xFF303840));
    g.drawLine((float)x - 3.5f, (float)y, (float)x + 3.5f, (float)y, 1.8f);
    g.drawLine((float)x, (float)y - 3.5f, (float)x, (float)y + 3.5f, 1.8f);

    // Highlight on slot edges
    g.setColour(juce::Colour(0x30FFFFFF));
    g.drawLine((float)x - 3, (float)y + 0.5f, (float)x + 3, (float)y + 0.5f, 0.5f);
    g.drawLine((float)x + 0.5f, (float)y - 3, (float)x + 0.5f, (float)y + 3, 0.5f);
}

void NeveStripAudioProcessorEditor::layoutContent()
{
    int margin = 10;
    int smallMargin = 6;
    int comboHeight = 22;
    int buttonHeight = 24;
    int currentY = 0;

    // Control area width (excluding meter area)
    int controlAreaWidth = stripWidth - meterAreaWidth - margin;

    // === INPUT/PREAMP SECTION ===
    preampHeader.setBounds(0, currentY, controlAreaWidth, headerHeight);
    currentY += headerHeight + smallMargin;

    // Row 1: Input Gain (large, centered)
    int inputAreaWidth = controlAreaWidth - margin * 2;
    inputGainLabel.setBounds(margin, currentY, inputAreaWidth, labelHeight);
    currentY += labelHeight;
    inputGainSlider.setBounds(margin + (inputAreaWidth - knobSize) / 2, currentY, knobSize, knobSize);
    currentY += knobSize + smallMargin;

    // Row 2: Trim and Drive side by side (symmetrical)
    int halfWidth = (controlAreaWidth - margin * 2) / 2;
    int knobCenterOffset = (halfWidth - smallKnobSize) / 2;

    outputTrimLabel.setBounds(margin, currentY, halfWidth, labelHeight);
    transformerLabel.setBounds(margin + halfWidth, currentY, halfWidth, labelHeight);
    currentY += labelHeight;
    outputTrimSlider.setBounds(margin + knobCenterOffset, currentY, smallKnobSize, smallKnobSize);
    transformerDriveSlider.setBounds(margin + halfWidth + knobCenterOffset, currentY, smallKnobSize, smallKnobSize);
    currentY += smallKnobSize + smallMargin;

    // Row 3: HPF and Phase (aligned)
    hpfLabel.setBounds(margin, currentY, halfWidth, labelHeight);
    currentY += labelHeight;
    hpfCombo.setBounds(margin, currentY, halfWidth - 4, comboHeight);
    phaseButton.setBounds(margin + halfWidth, currentY, halfWidth - 4, buttonHeight);
    currentY += comboHeight + sectionPadding;

    // === EQ SECTION ===
    eqHeader.setBounds(0, currentY, controlAreaWidth, headerHeight);
    currentY += headerHeight + smallMargin;

    // EQ bands in 2x2 grid (symmetrical)
    int eqKnobSize = 46;
    int eqColWidth = (controlAreaWidth - margin * 2) / 2;
    int eqRowHeight = eqKnobSize + labelHeight + comboHeight + 4;
    int eqKnobOffset = (eqColWidth - eqKnobSize) / 2;

    // HF (top left) and HM (top right)
    hfLabel.setBounds(margin, currentY, eqColWidth, labelHeight);
    hmLabel.setBounds(margin + eqColWidth, currentY, eqColWidth, labelHeight);
    currentY += labelHeight;
    hfFreqCombo.setBounds(margin + 2, currentY, eqColWidth - 4, comboHeight);
    hmFreqCombo.setBounds(margin + eqColWidth + 2, currentY, eqColWidth - 4, comboHeight);
    currentY += comboHeight + 2;
    hfGainSlider.setBounds(margin + eqKnobOffset, currentY, eqKnobSize, eqKnobSize);
    hmGainSlider.setBounds(margin + eqColWidth + eqKnobOffset, currentY, eqKnobSize, eqKnobSize);
    currentY += eqKnobSize + smallMargin;

    // LM (bottom left) and LF (bottom right)
    lmLabel.setBounds(margin, currentY, eqColWidth, labelHeight);
    lfLabel.setBounds(margin + eqColWidth, currentY, eqColWidth, labelHeight);
    currentY += labelHeight;
    lmFreqCombo.setBounds(margin + 2, currentY, eqColWidth - 4, comboHeight);
    lfFreqCombo.setBounds(margin + eqColWidth + 2, currentY, eqColWidth - 4, comboHeight);
    currentY += comboHeight + 2;
    lmGainSlider.setBounds(margin + eqKnobOffset, currentY, eqKnobSize, eqKnobSize);
    lfGainSlider.setBounds(margin + eqColWidth + eqKnobOffset, currentY, eqKnobSize, eqKnobSize);
    currentY += eqKnobSize + smallMargin;

    // EQ buttons (symmetrical)
    eqBypassButton.setBounds(margin, currentY, eqColWidth - 2, buttonHeight);
    eqPrePostButton.setBounds(margin + eqColWidth + 2, currentY, eqColWidth - 2, buttonHeight);
    currentY += buttonHeight + sectionPadding;

    // === DYNAMICS SECTION ===
    dynamicsHeader.setBounds(0, currentY, controlAreaWidth, headerHeight);
    currentY += headerHeight + smallMargin;

    // Compressor: Threshold and Makeup knobs (symmetrical)
    int dynKnobSize = 46;
    int dynKnobOffset = (halfWidth - dynKnobSize) / 2;

    compThreshLabel.setBounds(margin, currentY, halfWidth, labelHeight);
    compMakeupLabel.setBounds(margin + halfWidth, currentY, halfWidth, labelHeight);
    currentY += labelHeight;
    compThresholdSlider.setBounds(margin + dynKnobOffset, currentY, dynKnobSize, dynKnobSize);
    compMakeupSlider.setBounds(margin + halfWidth + dynKnobOffset, currentY, dynKnobSize, dynKnobSize);
    currentY += dynKnobSize + smallMargin;

    // Ratio, Attack, Release in a row (symmetrical thirds)
    int thirdWidth = (controlAreaWidth - margin * 2) / 3;
    compRatioLabel.setBounds(margin, currentY, thirdWidth, labelHeight);
    compAttackLabel.setBounds(margin + thirdWidth, currentY, thirdWidth, labelHeight);
    compReleaseLabel.setBounds(margin + thirdWidth * 2, currentY, thirdWidth, labelHeight);
    currentY += labelHeight;

    compRatioCombo.setBounds(margin, currentY, thirdWidth - 2, comboHeight);
    compAttackCombo.setBounds(margin + thirdWidth + 1, currentY, thirdWidth - 2, comboHeight);
    compReleaseCombo.setBounds(margin + thirdWidth * 2 + 2, currentY, thirdWidth - 2, comboHeight);
    currentY += comboHeight + smallMargin;

    // Comp buttons (symmetrical thirds)
    compBypassButton.setBounds(margin, currentY, thirdWidth - 2, buttonHeight);
    compLinkButton.setBounds(margin + thirdWidth + 1, currentY, thirdWidth - 2, buttonHeight);
    compSCHPFButton.setBounds(margin + thirdWidth * 2 + 2, currentY, thirdWidth - 2, buttonHeight);
    currentY += buttonHeight + smallMargin;

    // Limiter row (symmetrical)
    limThreshLabel.setBounds(margin, currentY, halfWidth, labelHeight);
    currentY += labelHeight;
    limThresholdSlider.setBounds(margin + (halfWidth - smallKnobSize) / 2, currentY, smallKnobSize, smallKnobSize);
    limBypassButton.setBounds(margin + halfWidth + (halfWidth - 80) / 2, currentY + (smallKnobSize - buttonHeight) / 2, 80, buttonHeight);
    currentY += smallKnobSize + sectionPadding;

    // === OUTPUT SECTION ===
    outputHeader.setBounds(0, currentY, controlAreaWidth, headerHeight);
    currentY += headerHeight + smallMargin;

    // Output level knob (centered)
    outputLabel.setBounds(margin, currentY, controlAreaWidth - margin * 2, labelHeight);
    currentY += labelHeight;
    outputLevelSlider.setBounds(margin + (controlAreaWidth - margin * 2 - knobSize) / 2, currentY, knobSize, knobSize);
    currentY += knobSize + smallMargin;

    // Master bypass (centered)
    int bypassWidth = 100;
    masterBypassButton.setBounds(margin + (controlAreaWidth - margin * 2 - bypassWidth) / 2, currentY, bypassWidth, buttonHeight);
    currentY += buttonHeight + margin;

    // Set content component size
    contentComponent.setSize(controlAreaWidth, currentY);
}

void NeveStripAudioProcessorEditor::resized()
{
    // Viewport takes the left portion (controls area)
    int viewportWidth = stripWidth - meterAreaWidth;
    viewport.setBounds(0, 60, viewportWidth, getHeight() - 75);

    // Layout the content inside viewport
    layoutContent();

    // === METERS (Right side - fixed position, not scrolled) ===
    int meterStartY = 68;
    int meterX = getWidth() - meterAreaWidth + 8;
    int availableHeight = getHeight() - meterStartY - 20;
    int meterSpacing = availableHeight / 3;

    // Input meter (wider with scale markings)
    inputMeterLabel.setBounds(meterX, meterStartY, meterAreaWidth - 16, labelHeight);
    inputMeter.setBounds(meterX, meterStartY + labelHeight + 2, meterAreaWidth - 16, meterHeight);

    // Output meter
    int outY = meterStartY + meterSpacing;
    outputMeterLabel.setBounds(meterX, outY, meterAreaWidth - 16, labelHeight);
    outputMeter.setBounds(meterX, outY + labelHeight + 2, meterAreaWidth - 16, meterHeight);

    // GR meter
    int grY = meterStartY + meterSpacing * 2;
    grMeterLabel.setBounds(meterX, grY, meterAreaWidth - 16, labelHeight);
    grMeter.setBounds(meterX, grY + labelHeight + 2, meterAreaWidth - 16, meterHeight);
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
