#pragma once

#include <shared_plugin_helpers/shared_plugin_helpers.h>
#include <ea_data_structures/ea_data_structures.h>

constexpr int getNumMIDINotes()
{
    return 128;
}

using MappedNotes = EA::Vector<int>;

struct NoteState
{
    NoteState() { mappedNotes.reserve(128); }

    MappedNotes mappedNotes;
};

struct HeldNotes
{
    void map(int source, int dest) noexcept { heldNotes[source].mappedNotes.add(dest); }

    const MappedNotes& getHeldNotes(int source) noexcept
    {
        return heldNotes[source].mappedNotes;
    }

    void clear(int source) noexcept { heldNotes[source].mappedNotes.clear(); }

    EA::Array<NoteState, getNumMIDINotes()> heldNotes;
};

struct MidiTransposer
{
    void mapNotes(int source, HeldNotes& dest, int transpose)
    {
        if (transpose != 0)
        {
            dest.map(source, source);
        }
        auto transposedNote = source + transpose;
        transposedNote = juce::jlimit(0, 127, transposedNote);
        dest.map(source, transposedNote);
    }

    juce::MidiBuffer& process(const juce::MidiBuffer& midiMessages, int transpose)
    {
        output.clear();

        for (auto m: midiMessages)
        {
            auto message = m.getMessage();

            if (message.isNoteOn())
            {
                auto originalNote = message.getNoteNumber();

                mapNotes(originalNote, heldNotes, transpose);

                for (auto& note: heldNotes.getHeldNotes(originalNote))
                {
                    message.setNoteNumber(note);
                    output.addEvent(message, m.samplePosition);
                }
            }
            else if (message.isNoteOff())
            {
                auto originalNote = message.getNoteNumber();

                for (auto& note: heldNotes.getHeldNotes(originalNote))
                {
                    message.setNoteNumber(note);
                    output.addEvent(message, m.samplePosition);
                }

                heldNotes.clear(originalNote);
            }
            else
            {
                output.addEvent(message, m.samplePosition);
            }
        }

        return output;
    }

    juce::MidiBuffer output;
    HeldNotes heldNotes;
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
