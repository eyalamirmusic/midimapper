#include "Mapper.h"
#include "Helpers.h"

namespace EA::MIDI
{

int HeldNotes::getNumTimesNoteIsHeld(int note) const noexcept
{
    int held = 0;

    for (auto& notes: heldNotes)
    {
        if (notes.mappedNotes.contains(note))
            ++held;
    }

    return held;
}

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
            processNoteOn(noteMapper, message, m.samplePosition);
        else if (message.isNoteOff())
            processNoteOff(message, m.samplePosition);
        else
            output.addEvent(message, m.samplePosition);
    }

    return output;
}

void Mapper::processNoteOff(const MidiMessage& message, int samplePos) noexcept
{
    auto originalNote = message.getNoteNumber();

    for (auto& note: heldNotes.getHeldNotes(originalNote))
    {
        if (heldNotes.getNumTimesNoteIsHeld(note) == 1)
            setNoteNumAndAddToOutput(message, note, samplePos);
    }

    heldNotes.clear(originalNote);
}

void Mapper::setNoteNumAndAddToOutput(MidiMessage m, int note, int samplePos) noexcept
{
    m.setNoteNumber(note);
    output.addEvent(m, samplePos);
}

void Mapper::processNoteOn(NoteMapper& noteMapper,
                           const MidiMessage& message,
                           int samplePos) noexcept
{
    auto originalNote = message.getNoteNumber();

    noteMapper.map(originalNote, heldNotes);

    for (auto& note: heldNotes.getHeldNotes(originalNote))
    {
        if (options.sendNoteOffBeforeEachNoteOn
            && heldNotes.getNumTimesNoteIsHeld(note) > 1)
        {
            auto m =
                MidiMessage::noteOff(message.getChannel(), note, message.getVelocity());
            output.addEvent(m, samplePos);
        }

        setNoteNumAndAddToOutput(message, note, samplePos);
    }
}
} // namespace EA::MIDI