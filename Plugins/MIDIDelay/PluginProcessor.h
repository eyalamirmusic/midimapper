#pragma once

#include <shared_plugin_helpers/shared_plugin_helpers.h>
#include <ea_midi_mapper/ea_midi_mapper.h>

class DelayProcessor : public PluginHelpers::ProcessorBase
{
    using ParamBool = juce::AudioParameterBool;
    using ParamInt = juce::AudioParameterInt;
    using ParamChoice = juce::AudioParameterChoice;

public:
    DelayProcessor();

private:

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    EA::MIDI::Delay delay;
    EA::MIDI::Duplicator duplicator;

    ParamInt* delayParam = new ParamInt({"Delay", 1}, "Delay", 0, 100000, 0);

};
