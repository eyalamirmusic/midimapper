#pragma once

#include <shared_plugin_helpers/shared_plugin_helpers.h>
#include <ea_midi_mapper/ea_midi_mapper.h>

class MidiFXProcessor : public PluginHelpers::ProcessorBase
{
    using ParamBool = juce::AudioParameterBool;
    using ParamInt = juce::AudioParameterInt;

public:
    MidiFXProcessor()
    {
        addParameter(transpose);
        addParameter(harmonize);
    }

private:
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;

    EA::MIDI::Mapper mapper;
    EA::MIDI::Transposer transposer;
    EA::MIDI::HarmonizedTransposer harmonizedTransposer;

    ParamBool* harmonize = new ParamBool({"Harmonize", 1}, "Harmonize", false);
    ParamInt* transpose = new ParamInt({"Transpose", 1}, "Transpose", -12, 12, 0);
};
