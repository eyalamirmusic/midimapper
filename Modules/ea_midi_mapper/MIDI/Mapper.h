#pragma once

#include "../Common/Common.h"

namespace EA::MIDI
{
constexpr int getNumMIDINotes()
{ return 128; }

using MappedNotes = Vector<int>;

struct NoteState
{
    NoteState() { mappedNotes.reserve(getNumMIDINotes()); }

    MappedNotes mappedNotes;
};

struct HeldNotes
{
    void map(int source, int dest) noexcept { heldNotes[source].mappedNotes.add(dest); }

    const MappedNotes& getHeldNotes(int source) noexcept
    { return heldNotes[source].mappedNotes; }

    int getNumTimesNoteIsHeld(int note) const noexcept;

    void clear(int source) noexcept { heldNotes[source].mappedNotes.clear(); }

    Array<NoteState, getNumMIDINotes()> heldNotes;
};

struct NoteMapper
{
    virtual ~NoteMapper() = default;

    virtual void map(int source, HeldNotes& dest) noexcept = 0;
};

struct Options
{
    bool sendNoteOffBeforeEachNoteOn = true;
};

class Mapper
{
public:
    void process(MidiBuffer& midiMessages, NoteMapper& mapper) noexcept;

    MidiBuffer& getProcessed(const MidiBuffer& midiMessages, NoteMapper& mapper) noexcept;

    void setOptions(const Options& optionsToUse) noexcept { options = optionsToUse; }

private:
    void processNoteOff(const MidiMessage& message, int samplePos) noexcept;

    void setNoteNumAndAddToOutput(MidiMessage m, int note, int samplePos) noexcept;

    void processNoteOn(NoteMapper& noteMapper,
                       const MidiMessage& message,
                       int samplePos) noexcept;

    Options options;
    MidiBuffer output;
    HeldNotes heldNotes;
};
} // namespace EA::MIDI