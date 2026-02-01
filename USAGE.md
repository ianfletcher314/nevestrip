# NeveStrip - Usage Guide

**Neve-Style Channel Strip Plugin**

NeveStrip is a classic British console channel strip emulation featuring transformer-colored preamp, musical 4-band EQ with fixed frequency selections, smooth opto-style compressor, and brick-wall limiter. Designed to add that legendary warmth and character to any source.

---

## Use Cases in Modern Rock Production

### Drum Bus Processing

The Neve sound is legendary on drums - punchy, warm, and musical.

**Drum Bus - Neve Character:**
- Input Gain: +3 to +6 dB
- HPF: 50 Hz
- Transformer Drive: 30-50%
- EQ:
  - LF: 60 Hz, +2-3 dB (weight)
  - LM: 360 Hz, -2 dB (reduce mud)
  - HM: 3.2 kHz, +1-2 dB (attack)
  - HF: 12 kHz, +2 dB (air)
- Compressor: Threshold -15 dB, Ratio 3:1, Attack Med, Release Med, SC HPF On
- Limiter: Bypass (use for peaks if needed)
- Output: Match input level

**Aggressive Drum Bus:**
- Input Gain: +8 to +12 dB
- HPF: 80 Hz
- Transformer Drive: 60-80%
- EQ:
  - LF: 110 Hz, +3 dB
  - LM: 700 Hz, -3 dB
  - HM: 4.8 kHz, +3 dB
  - HF: 10 kHz, +2 dB
- Compressor: Threshold -20 dB, Ratio 4:1, Attack Fast, Release Med
- Output Trim: -4 to -6 dB (compensate for drive)

### Guitar Bus / Individual Tracks

Neve EQ shapes guitar tone beautifully without harshness.

**Electric Guitar - Warmth:**
- Input Gain: 0 to +3 dB
- HPF: 80 Hz
- Transformer Drive: 20-40%
- EQ:
  - LF: 110 Hz, +1 dB (body)
  - LM: 700 Hz, -1 dB (reduce honk)
  - HM: 3.2 kHz, +1-2 dB (presence)
  - HF: 12 kHz, +1 dB (sparkle)
- Compressor: Bypass or subtle
- EQ Pre/Post: Pre (EQ before compression)

**Heavy Rhythm Guitars:**
- Input Gain: +3 dB
- HPF: 100 Hz
- Transformer Drive: 40-60%
- EQ:
  - LF: 110 Hz, -1 dB (tighten)
  - LM: 360 Hz, -2 dB
  - HM: 2.4 kHz, +2 dB (aggression)
  - HF: 10 kHz, +1 dB
- Compressor: Bypass (use for dynamics control if needed)

**Acoustic Guitar:**
- Input Gain: 0 dB
- HPF: 80 Hz
- Transformer Drive: 10-20%
- EQ:
  - LF: 110 Hz, -1 dB (reduce boom)
  - LM: 360 Hz, -2 dB (reduce boxiness)
  - HM: 3.2 kHz, +1 dB (clarity)
  - HF: 12 kHz, +2-3 dB (air)
- Compressor: Threshold -12 dB, Ratio 1.5:1, Attack Med, Release Med

### Bass Guitar

The Neve transformer sound is magic on bass.

**Rock Bass - Punchy:**
- Input Gain: +3 to +6 dB
- HPF: 50 Hz
- Transformer Drive: 40-60%
- EQ:
  - LF: 60 Hz, +2-3 dB (sub weight)
  - LM: 220 Hz, -2 dB (reduce mud)
  - HM: 1.6 kHz, +2 dB (growl)
  - HF: 10 kHz, +1 dB (string definition)
- Compressor: Threshold -18 dB, Ratio 4:1, Attack Med, Release Med
- Limiter: Threshold -6 dB (catch peaks)

**Vintage Bass Tone:**
- Input Gain: +6 to +10 dB
- HPF: Off
- Transformer Drive: 60-80%
- EQ:
  - LF: 60 Hz, +3 dB
  - LM: 360 Hz, -1 dB
  - HM: 1.6 kHz, +1 dB
  - HF: 10 kHz, 0 dB
- Compressor: Ratio 3:1, slow attack for note articulation
- Output Trim: -4 to -8 dB

### Vocals

Neve preamps and EQs are studio standards for vocals.

**Lead Vocal - Classic:**
- Input Gain: +3 to +6 dB
- HPF: 80 Hz
- Transformer Drive: 15-30%
- EQ:
  - LF: 110 Hz, -1 dB (reduce proximity)
  - LM: 360 Hz, -2 dB (reduce mud)
  - HM: 3.2 kHz, +2-3 dB (presence)
  - HF: 12 kHz, +2 dB (air)
- Compressor: Threshold -15 dB, Ratio 3:1, Attack Med, Release Med, Makeup +4 dB
- Limiter: Threshold -3 dB (safety)
- EQ Pre/Post: Post (EQ after compression for this vocal)

**Aggressive Rock Vocal:**
- Input Gain: +6 to +10 dB
- HPF: 120 Hz
- Transformer Drive: 40-60%
- EQ:
  - LF: 220 Hz, -2 dB
  - LM: 700 Hz, -1 dB
  - HM: 2.4 kHz, +3 dB
  - HF: 10 kHz, +1 dB
- Compressor: Threshold -20 dB, Ratio 4:1, Attack Fast

**Backing Vocals:**
- Input Gain: +3 dB
- HPF: 160 Hz
- Transformer Drive: 20%
- EQ:
  - LF: 220 Hz, -2 dB
  - HM: 3.2 kHz, -1 dB (tuck behind lead)
  - HF: 12 kHz, +1 dB
- Compressor: Ratio 4:1, more aggressive

### Mix Bus / Mastering

Neve character on the mix bus adds cohesion and warmth.

**Mix Bus - Subtle Warmth:**
- Input Gain: 0 dB
- HPF: 35 Hz
- Transformer Drive: 10-20%
- EQ:
  - LF: 60 Hz, +1 dB
  - LM: 360 Hz, -0.5 dB
  - HM: 3.2 kHz, +0.5 dB
  - HF: 12 kHz, +1 dB
- Compressor: Threshold -15 dB, Ratio 2:1, Attack Slow, Release Auto, Stereo Link On
- Limiter: Bypass
- Output Level: 0 dB

---

## Recommended Settings

### Quick Reference Table

| Application | Input Gain | Drive | HPF | Comp Ratio | Key EQ |
|------------|-----------|-------|-----|------------|--------|
| Drum Bus | +6 dB | 40% | 50 Hz | 3:1 | +LF, +HM |
| Guitars | +3 dB | 30% | 80 Hz | - | -LM, +HM |
| Bass | +6 dB | 50% | 50 Hz | 4:1 | +LF, +HM |
| Lead Vocal | +4 dB | 25% | 80 Hz | 3:1 | -LM, +HM, +HF |
| Mix Bus | 0 dB | 15% | 35 Hz | 2:1 | Subtle all |

### EQ Frequency Reference

**LF (Low Frequency) Options:**
- 35 Hz: Sub-bass, feel
- 60 Hz: Weight, power
- 110 Hz: Punch, body
- 220 Hz: Warmth, thickness

**LM (Low-Mid) Options:**
- 220 Hz: Thickness, mud
- 360 Hz: Body, boxiness
- 700 Hz: Warmth, nasality
- 1.6 kHz: Honk, presence
- 3.2 kHz: Attack (overlaps with HM)

**HM (High-Mid) Options:**
- 1.5 kHz: Presence
- 2.4 kHz: Bite, aggression
- 3.2 kHz: Clarity, presence
- 4.8 kHz: Edge, definition
- 7.2 kHz: Brilliance, air

**HF (High Frequency) Options:**
- 10 kHz: Air, presence
- 12 kHz: Shimmer, sparkle
- 16 kHz: Ultra-highs, air

### Compressor Quick Settings

**Conservative (Vocals, Acoustic):**
- Ratio: 1.5:1 or 2:1
- Attack: Med (8ms)
- Release: Med (400ms) or Auto

**Standard (Drums, Bass):**
- Ratio: 3:1 or 4:1
- Attack: Fast (2ms) or Med (8ms)
- Release: Med (400ms)

**Aggressive (Rock Vocals, Heavy Drums):**
- Ratio: 4:1 or 6:1
- Attack: Fast (2ms)
- Release: Fast (100ms) or Auto

---

## Signal Flow Tips

### Signal Flow Order

NeveStrip processes in this order:
1. Input Gain
2. Phase (inversion if enabled)
3. HPF
4. Transformer Drive
5. Output Trim
6. EQ (Pre) OR Compressor (Post)
7. Compressor OR EQ
8. Limiter
9. Output Level

### EQ Pre vs Post Dynamics

- **Pre (EQ before Compression)**: EQ shapes the signal before compression - compression responds to EQ'd signal
- **Post (EQ after Compression)**: Compression first, then EQ shapes the compressed signal - more natural dynamics

**When to use Pre:** Bass, guitars, when you want EQ to affect compression behavior
**When to use Post:** Vocals, mix bus, when you want final tonal shaping after dynamics

### Using Transformer Drive

- **0-20%**: Subtle warmth, slight harmonic enhancement
- **30-50%**: Noticeable color, classic Neve sound
- **60-80%**: Significant saturation, grit and edge
- **80-100%**: Heavy distortion, lo-fi character

---

## Combining with Other Plugins

### Complete Vocal Chain
1. **NeveStrip** - preamp, EQ, compression
2. **VoxProc** - de-essing only
3. Send to **Dynoverb** - reverb

### Bass Processing Chain
1. **NeveStrip** - the complete bass tone
2. **Ducker** - sidechain from kick
3. **TapeWarm** (optional) - additional warmth

### Drum Processing
1. **NeveStrip** on individual drums and/or bus
2. **Bus Glue** - additional glue compression
3. **TapeWarm** - analog character
4. **StereoImager** - width control

### Guitar Processing
1. **PDLBRD** - amp and effects
2. **NeveStrip** - console character and EQ
3. Or just **NeveStrip** for recording direct

---

## Quick Start Guide

**Get a great vocal sound in 60 seconds:**

1. Insert NeveStrip on your vocal track
2. Set **Input Gain** to +4 dB
3. Set **HPF** to 80 Hz
4. Set **Transformer Drive** to 25%
5. Set **EQ**:
   - LF: 110 Hz, -1 dB
   - LM: 360 Hz, -2 dB
   - HM: 3.2 kHz, +2 dB
   - HF: 12 kHz, +2 dB
6. Enable **Compressor**:
   - Threshold: -15 dB
   - Ratio: 3:1
   - Attack: Med
   - Release: Med
   - Makeup: +4 dB
7. Set **EQ Pre/Post** to Post
8. Set **Limiter Threshold** to -3 dB
9. Adjust **Output Level** to match bypass
10. A/B with bypass

**Get punchy bass in 60 seconds:**

1. Insert NeveStrip on your bass track
2. Set **Input Gain** to +6 dB
3. Set **HPF** to 50 Hz
4. Set **Transformer Drive** to 50%
5. Set **Output Trim** to -3 dB
6. Set **EQ**:
   - LF: 60 Hz, +2 dB
   - LM: 220 Hz, -2 dB
   - HM: 1.6 kHz, +2 dB
   - HF: 10 kHz, +1 dB
7. Enable **Compressor**:
   - Threshold: -18 dB
   - Ratio: 4:1
   - Attack: Med
   - Release: Med
8. Set **EQ Pre/Post** to Pre
9. Set **Limiter Threshold** to -6 dB
10. Adjust **Output Level** to match bypass

**Add Neve warmth to mix bus in 30 seconds:**

1. Insert NeveStrip on your mix bus
2. Set **HPF** to 35 Hz
3. Set **Transformer Drive** to 15%
4. Set **EQ**:
   - LF: 60 Hz, +1 dB
   - HF: 12 kHz, +1 dB
5. Enable **Compressor**:
   - Threshold: -15 dB (1-2 dB GR)
   - Ratio: 2:1
   - Attack: Slow
   - Release: Auto
   - Stereo Link: On
6. Match **Output Level** with Input
7. A/B with bypass - should sound "better" without being obvious
