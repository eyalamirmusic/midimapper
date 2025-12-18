#pragma once

#include "Helpers.h"

namespace EA::MIDI
{
struct HeldNoteState
{
    int count = 0;
};

class DuplicateNoteHandler
{
public:
    void process(MidiBuffer& input) noexcept;

private:
    void handleNoteOn(const MidiMessage& message, int samplePos) noexcept;
    void handleNoteOff(const MidiMessage& message, int samplePos) noexcept;
    void handleNote(const MidiMessage& message, int samplePos) noexcept;

    MidiBuffer output;

    Array<Array<HeldNoteState, 128>, 16> heldNotes;
};

} // namespace EA::MIDI