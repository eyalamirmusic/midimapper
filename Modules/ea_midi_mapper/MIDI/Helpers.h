#pragma once

#include "../Common/Common.h"

namespace EA::MIDI
{
void replaceContentsWith(const MidiBuffer& source, MidiBuffer& dest) noexcept;
void copyBuffers(const MidiBuffer& source, MidiBuffer& dest) noexcept;
void mixBuffers(const MidiBuffer& source, MidiBuffer& dest) noexcept;

void log(const MidiBuffer& midiMessages) noexcept;

MidiMessage toNoteOff(const MidiMessage& m) noexcept;

} // namespace EA::MIDI
