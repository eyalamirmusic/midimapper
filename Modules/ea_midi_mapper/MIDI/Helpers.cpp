#include "Helpers.h"

namespace EA::MIDI
{
void replaceContentsWith(const MidiBuffer& source, MidiBuffer& dest) noexcept
{
    dest.clear();

    for (auto m: source)
        dest.addEvent(m.getMessage(), m.samplePosition);
}

void copyBuffers(const MidiBuffer& source, MidiBuffer& dest) noexcept
{
    dest.clear();
    mixBuffers(source, dest);
}

void mixBuffers(const MidiBuffer& source, MidiBuffer& dest) noexcept
{
    for (auto m: source)
        dest.addEvent(m.getMessage(), m.samplePosition);
}

void log(const MidiBuffer& midiMessages) noexcept
{
    for (auto m: midiMessages)
    {
        auto message = m.getMessage();
        auto samplePos = m.samplePosition;

        std::cout << message.getDescription() << " SamplePos:" << String(samplePos)
                  << std::endl;
    }
}

MidiMessage toNoteOff(const MidiMessage& m) noexcept
{
    return MidiMessage::noteOff(m.getChannel(), m.getNoteNumber(), m.getVelocity());
}
} // namespace EA::MIDI