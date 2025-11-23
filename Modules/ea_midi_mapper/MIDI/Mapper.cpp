#include "Mapper.h"

namespace EA::MIDI
{

void Transposer::mapNotes(int source, HeldNotes& dest, int transpose)
{
    if (transpose != 0)
    {
        dest.map(source, source);
    }
    auto transposedNote = source + transpose;
    transposedNote = juce::jlimit(0, 127, transposedNote);
    dest.map(source, transposedNote);
}

MidiBuffer& Transposer::process(const MidiBuffer& midiMessages, int transpose)
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
} // namespace EA::MIDI