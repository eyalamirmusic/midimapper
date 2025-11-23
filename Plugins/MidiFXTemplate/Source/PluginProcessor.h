#pragma once

#include <shared_plugin_helpers/shared_plugin_helpers.h>
#include <ea_midi_mapper/ea_midi_mapper.h>

class MidiFXProcessor : public PluginHelpers::ProcessorBase
{
    using ParamInt = juce::AudioParameterInt;

public:
    MidiFXProcessor() { addParameter(transpose); }

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;

private:
    EA::MIDI::Transposer transposer;
    ParamInt* transpose = new ParamInt({"Transpose", 1}, "Transpose", -12, 12, 0);
};
