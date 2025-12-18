#pragma once

#include <shared_plugin_helpers/shared_plugin_helpers.h>
#include <ea_midi_mapper/ea_midi_mapper.h>

struct VelocityReducerProcessor : EA::MIDI::DelayProcessor
{
    void processDelayed(juce::MidiMessage& m) override
    {
        if (m.isNoteOnOrOff())
        {
            auto newVel = m.getFloatVelocity() - reduction;
            newVel = juce::jlimit(0.f, 1.f, newVel);
            m.setVelocity(newVel);
        }
    }

    float reduction = 0.3f;
};

class DelayProcessor : public PluginHelpers::ProcessorBase
{
    using ParamBool = juce::AudioParameterBool;
    using ParamInt = juce::AudioParameterInt;
    using ParamChoice = juce::AudioParameterChoice;
    using ParamFloat = juce::AudioParameterFloat;

public:
    DelayProcessor();

private:
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    VelocityReducerProcessor velProcessor;
    EA::MIDI::Delay delay;
    EA::MIDI::Duplicator duplicator;

    ParamInt* delayParam = new ParamInt({"Delay", 1}, "Delay", 0, 100000, 0);
    ParamFloat* velReduction =
        new ParamFloat({"VelReduction", 1}, "Velocity Reduction", 0.f, 1.f, 0.1f);
};
