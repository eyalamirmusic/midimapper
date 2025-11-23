#pragma once

#include <shared_plugin_helpers/shared_plugin_helpers.h>
#include <ea_midi_mapper/ea_midi_mapper.h>

class MidiFXProcessor : public PluginHelpers::ProcessorBase
{
    using ParamBool = juce::AudioParameterBool;
    using ParamInt = juce::AudioParameterInt;
    using ParamChoice = juce::AudioParameterChoice;

public:
    MidiFXProcessor()
    {
        addParameter(transpose);
        addParameter(mode);
    }

private:
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    EA::MIDI::Mapper mapper;
    EA::MIDI::Transposer transposer;
    EA::MIDI::HarmonizedTransposer harmonizedTransposer;
    EA::MIDI::StatefulTransposer statefulTransposer;
    EA::MIDI::RandomTransposer random;

    static juce::StringArray getModes()
    {
        return {"Transpose", "Harmonize", "Sequenced Harmonize", "Random"};
    }

    ParamChoice* mode = new ParamChoice({"Mode", 1}, "Mode", getModes(), 0);
    ParamInt* transpose = new ParamInt({"Transpose", 1}, "Transpose", -12, 12, 0);
};
