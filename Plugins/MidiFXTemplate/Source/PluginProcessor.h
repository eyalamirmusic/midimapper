#pragma once

#include <shared_plugin_helpers/shared_plugin_helpers.h>
#include <ea_data_structures/ea_data_structures.h>

struct NoteState
{
    int mappedNote = -1;
};

constexpr int getNumMIDINotes()
{
    return 128;
}

struct MidiTransposer
{
    juce::MidiBuffer& process(const juce::MidiBuffer& midiMessages, int transpose)
    {
        output.clear();

        for (auto m: midiMessages)
        {
            auto message = m.getMessage();

            if (message.isNoteOn())
            {
                auto originalNote = message.getNoteNumber();
                auto transposedNote = originalNote + transpose;
                transposedNote = juce::jlimit(0, 127, transposedNote);

                message.setNoteNumber(transposedNote);
                heldNotes[originalNote].mappedNote = transposedNote;
            }
            else if (message.isNoteOff())
            {
                auto originalNote = message.getNoteNumber();
                auto& state = heldNotes[originalNote].mappedNote;

                if (state >= 0)
                {
                    message.setNoteNumber(state);
                    state = -1;
                }
            }

            output.addEvent(message, m.samplePosition);
        }

        return output;
    }

    juce::MidiBuffer output;
    EA::Array<NoteState, getNumMIDINotes()> heldNotes;
};

class MidiFXProcessor : public PluginHelpers::ProcessorBase
{
    using ParamInt = juce::AudioParameterInt;

public:
    MidiFXProcessor() { addParameter(transpose); }

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;

private:
    MidiTransposer transposer;
    ParamInt* transpose = new ParamInt({"Transpose", 1}, "Transpose", -12, 12, 0);
};
