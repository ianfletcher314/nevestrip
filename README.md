# NeveStrip - Neve-Style Channel Strip Plugin

A channel strip plugin inspired by classic Neve console circuits, featuring preamp, EQ, and dynamics modeled on legendary Neve hardware.

## Overview

NeveStrip captures the essence of Neve console channels - the transformer warmth, the musical inductor-based EQ, and the punchy dynamics. These circuits defined the sound of countless hit records.

## The Neve Sound

### What Makes Neve Special

**Transformer Coloration**
- Input and output transformers add subtle harmonic richness
- Soft saturation at high levels
- Low-frequency "weight" and high-frequency "silk"

**Class A Circuitry**
- Rich, warm harmonic content
- Musical distortion characteristics
- Punchy transient response

**Inductor-Based EQ**
- Smooth, musical curves
- Resonant characteristics
- "Expensive" sound

## Modules

### 1. Preamp Section (1073-inspired)

| Control | Range | Description |
|---------|-------|-------------|
| Input Gain | 0dB to +60dB | Mic/line input level |
| Output Trim | -20dB to +10dB | Output level control |
| Phase | 0/180 | Polarity flip |
| HPF | Off/50/80/160/300Hz | High pass filter |
| Transformer Drive | 0-100% | Amount of transformer saturation |

### 2. EQ Section (1073/1084-inspired)

#### High Frequency
- **Frequency**: 10kHz, 12kHz, 16kHz (shelf)
- **Gain**: +/-16dB
- **Character**: Silky Neve HF, never harsh

#### High-Mid Frequency (1084-style addition)
- **Frequency**: 1.5kHz, 2.4kHz, 3.2kHz, 4.8kHz, 7.2kHz
- **Gain**: +/-12dB
- **Q**: Proportional (tightens as you boost)

#### Low-Mid Frequency
- **Frequency**: 220Hz, 360Hz, 700Hz, 1.6kHz, 3.2kHz
- **Gain**: +/-16dB
- **Character**: Musical midrange control

#### Low Frequency
- **Frequency**: 35Hz, 60Hz, 110Hz, 220Hz (shelf)
- **Gain**: +/-16dB
- **Character**: Solid low end, never muddy

#### EQ Features
- **EQ In/Out**: Bypass the EQ section
- **Pre/Post**: EQ before or after dynamics
- **Proportional Q**: Q narrows with boost (Neve characteristic)

### 3. Dynamics Section (2254/33609-inspired)

#### Compressor
| Control | Description |
|---------|-------------|
| Threshold | -40dB to +10dB |
| Ratio | 1.5:1, 2:1, 3:1, 4:1, 6:1 (classic Neve ratios) |
| Attack | Fast (2ms), Medium (8ms), Slow (20ms) |
| Release | Fast (100ms), Medium (400ms), Slow (1.2s), Auto |
| Makeup | 0 to +20dB |

#### Limiter
- **Threshold**: Sets limiting point
- **Classic diode bridge limiting**

#### Dynamics Features
- **Link**: Stereo linking for bus use
- **Sidechain HPF**: Reduces pumping from bass
- **Dynamics In/Out**: Bypass dynamics

### 4. Output Section

- **Output Level**: Final output gain
- **VU Meter**: Classic Neve-style meter
- **Peak LED**: Clip indicator

## Signal Flow

```
Input -> Transformer (input) -> HPF -> Gain Stage
      -> EQ (pre-dynamics option) -> Compressor -> Limiter
      -> EQ (post-dynamics option) -> Output Gain
      -> Transformer (output) -> Output
```

## UI Design

Classic Neve aesthetic with:
- Blue-grey metal faceplate
- Chicken head knobs
- Stepped rotary switches
- Red/orange gain knobs
- VU meter with Neve styling

```
+------------------------------------------------------------------+
|  NEVESTRIP                                              [BYPASS] |
+------------------------------------------------------------------+
|                                                                   |
|  +--- PREAMP ---+  +----------- EQ -----------+  +-- DYNAMICS --+ |
|  |              |  |                          |  |              | |
|  |   [INPUT]    |  |  HF: [FREQ] [GAIN]      |  |  [THRESH]    | |
|  |     ()       |  |        ()     ()         |  |     ()       | |
|  |              |  |                          |  |              | |
|  |   [DRIVE]    |  |  HM: [FREQ] [GAIN]      |  |  [RATIO]     | |
|  |     ()       |  |        ()     ()         |  |    (sw)      | |
|  |              |  |                          |  |              | |
|  |   HPF        |  |  LM: [FREQ] [GAIN]      |  |  [ATTACK]    | |
|  |   [OFF]      |  |        ()     ()         |  |    (sw)      | |
|  |  50 80       |  |                          |  |              | |
|  | 160 300      |  |  LF: [FREQ] [GAIN]      |  |  [RELEASE]   | |
|  |              |  |        ()     ()         |  |    (sw)      | |
|  |   [PHASE]    |  |                          |  |              | |
|  +--- [OUT] ----+  +---- [EQ IN] [PRE/POST]--+  +-- [MAKEUP]---+ |
|                                                                   |
|  +---------------------- VU METER --------------------------------+|
|  |        -20  -10  -7  -5  -3  -1   0  +1  +2  +3              | |
|  |   =================================|====                 [GR]  |
|  +--------------------------------------------------------------------+
+------------------------------------------------------------------+
```

## Implementation Plan

### Phase 1: Project Setup
- [ ] Create JUCE project
- [ ] Set up AU/VST3 targets
- [ ] Define all parameters
- [ ] Basic plugin structure

### Phase 2: Preamp/Transformer DSP
- [ ] Implement transformer saturation model
  - [ ] Asymmetric soft clipping
  - [ ] Harmonic generation (even harmonics)
  - [ ] Low frequency response shaping
  - [ ] High frequency "silk"
- [ ] Input gain staging
- [ ] High-pass filter (stepped frequencies)
- [ ] Phase inversion

### Phase 3: EQ DSP
- [ ] Implement inductor-based EQ models
  - [ ] Research Neve EQ curves
  - [ ] Proportional Q behavior
  - [ ] Shelf filters with Neve character
  - [ ] Parametric bands with musical curves
- [ ] Stepped frequency selection
- [ ] Pre/post dynamics routing

### Phase 4: Dynamics DSP
- [ ] Implement Neve-style compressor
  - [ ] Diode bridge detection modeling
  - [ ] Classic attack/release times
  - [ ] Ratio characteristics
  - [ ] Auto-release algorithm
- [ ] Implement limiter
- [ ] Sidechain filtering
- [ ] Stereo linking

### Phase 5: UI Development
- [ ] Design Neve-inspired aesthetic
  - [ ] Metal faceplate texture
  - [ ] Authentic color scheme (blue-grey)
- [ ] Create custom controls
  - [ ] Chicken head knobs
  - [ ] Stepped rotary switches
  - [ ] Vintage toggle switches
- [ ] Implement VU meter
  - [ ] Classic ballistics
  - [ ] Gain reduction mode
- [ ] Peak LED indicator

### Phase 6: Refinement
- [ ] A/B comparison with real Neve
- [ ] Tweak EQ curves for authenticity
- [ ] Optimize transformer model
- [ ] CPU optimization
- [ ] Preset system

### Phase 7: Testing & Release
- [ ] Test on various sources
- [ ] Compare to Neve hardware (if possible)
- [ ] Create factory presets
  - [ ] "Vocal Chain"
  - [ ] "Drum Bus"
  - [ ] "Bass Guitar"
  - [ ] "Mix Bus"
- [ ] Documentation

## Technical Details

### Transformer Modeling

Options for implementation:
1. **Waveshaper + Filtering**: Simple, CPU-efficient
2. **Magnetic Hysteresis**: More accurate, CPU-heavy
3. **Harmonic Injection**: Add specific harmonics based on measurements

### EQ Curve Characteristics

Neve EQs have distinctive curves:
- Asymmetric boost/cut shapes
- Proportional Q (wider Q at low gain, narrower at high gain)
- Gentle slopes on shelves
- Musical resonance on bells

### Compressor Characteristics

Neve compressors (2254, 33609) features:
- Diode bridge detection (soft knee)
- Fast attack, musical release
- Punchy, aggressive character
- Classic "snap" on drums

## Reference Hardware

- **Neve 1073**: Classic preamp + EQ
- **Neve 1084**: Added parametric mid band
- **Neve 2254**: Channel compressor
- **Neve 33609**: Stereo bus compressor

## Dependencies

- JUCE Framework 7.x
- C++17 or later

## Building

```bash
# macOS
cd Builds/MacOSX
xcodebuild -project NeveStrip.xcodeproj -configuration Release
```

## Legal Note

This is an independent project inspired by classic Neve circuits. "Neve" is a trademark of AMS Neve Ltd. This plugin is not affiliated with or endorsed by AMS Neve.

## License

MIT License
