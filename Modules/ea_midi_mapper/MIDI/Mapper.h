#pragma once

#include "../Common/Common.h"

namespace EA::MIDI
{
constexpr int getNumMIDINotes()
{
    return 128;
}

using MappedNotes = Vector<int>;

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

    Array<NoteState, getNumMIDINotes()> heldNotes;
};

struct Transposer
{
    void mapNotes(int source, HeldNotes& dest, int transpose);

    MidiBuffer& process(const MidiBuffer& midiMessages, int transpose);

    MidiBuffer output;
    HeldNotes heldNotes;
};
} // namespace EA::MIDI