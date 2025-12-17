#pragma once

#include <shared_plugin_helpers/shared_plugin_helpers.h>
#include <ea_midi_mapper/ea_midi_mapper.h>

class MidiFXProcessor : public PluginHelpers::ProcessorBase
{
    using ParamBool = juce::AudioParameterBool;
    using ParamInt = juce::AudioParameterInt;
    using ParamChoice = juce::AudioParameterChoice;

public:
    MidiFXProcessor();

private:
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
};
