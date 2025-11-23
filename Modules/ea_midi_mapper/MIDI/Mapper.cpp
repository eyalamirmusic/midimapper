#include "Mapper.h"
#include "Helpers.h"

namespace EA::MIDI
{

void Mapper::process(MidiBuffer& midiMessages, NoteMapper& mapper) noexcept
{
    auto& processed = getProcessed(midiMessages, mapper);
    replaceContentsWith(processed, midiMessages);
}

MidiBuffer& Mapper::getProcessed(const MidiBuffer& midiMessages,
                                 NoteMapper& noteMapper) noexcept
{
    output.clear();

    for (auto m: midiMessages)
    {
        auto message = m.getMessage();

        if (message.isNoteOn())
        {
            auto originalNote = message.getNoteNumber();

            noteMapper.map(originalNote, heldNotes);

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