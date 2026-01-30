cmake_minimum_required(VERSION 3.22)

project(NeveStrip VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find JUCE - adjust path as needed
set(JUCE_DIR "/Users/ianfletcher/JUCE")

# Add JUCE as subdirectory
add_subdirectory(${JUCE_DIR} ${CMAKE_BINARY_DIR}/JUCE)

# Define the plugin
juce_add_plugin(NeveStrip
    COMPANY_NAME "Compstortion"
    BUNDLE_ID "com.Compstortion.NeveStrip"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Cmps
    PLUGIN_CODE Neve
    FORMATS AU VST3
    PRODUCT_NAME "NeveStrip"
    VST3_CATEGORIES "Fx"
)

# Source files
target_sources(NeveStrip
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/DSP/Transformer.cpp
        Source/DSP/HighPassFilter.cpp
        Source/DSP/NeveEQ.cpp
        Source/DSP/NeveCompressor.cpp
        Source/DSP/NeveLimiter.cpp
)

# Include directories
target_include_directories(NeveStrip
    PRIVATE
        Source
        Source/DSP
)

# JUCE modules
target_link_libraries(NeveStrip
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Compiler definitions
target_compile_definitions(NeveStrip
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)
