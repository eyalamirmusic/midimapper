#pragma once

#include <shared_plugin_helpers/shared_plugin_helpers.h>
#include <ea_midi_mapper/ea_midi_mapper.h>

struct VelocityReducerProcessor : EA::MIDI::DelayProcessor
{
    void processDelayed(juce::MidiMessage& m) override
    {
        if (m.isNoteOnOrOff())
        {
            auto newVel = m.getFloatVelocity() * reduction;
            newVel = juce::jlimit(0.f, 1.f, newVel);
            m.setVelocity(newVel);
        }
    }

    float reduction = 0.3f;
};

struct DelayLine
{
    void process(juce::MidiBuffer& midiBuffer,
                 int numSamples,
                 float sampleDelay,
                 EA::MIDI::DelayProcessor& processor)
    {
        duplicator.store(midiBuffer);
        delay.process(midiBuffer, sampleDelay, numSamples, processor);
        duplicator.mix(midiBuffer);
    }

    EA::MIDI::Delay delay;
    EA::MIDI::Duplicator duplicator;
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
    void prepareToPlay(double sampleRate, int) override;

    VelocityReducerProcessor velProcessor;

    EA::Array<DelayLine, 8> feedbackLines;

    ParamFloat* delayParam = new ParamFloat({"Delay", 1}, "Delay", 0.f, 3.f, 0.001f);
    ParamFloat* velReduction =
        new ParamFloat({"VelReduction", 1}, "Velocity Reduction", 0.f, 1.f, 0.1f);
    ParamInt* feedbackParam = new ParamInt({"Feedback", 1}, "Feedback", 1, 8, 1);
};
