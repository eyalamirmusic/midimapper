#include "DuplicateNoteHandler.h"

namespace EA::MIDI
{

void DuplicateNoteHandler::process(MidiBuffer& input) noexcept
{
    output.clear();

    for (auto m: input)
    {
        auto message = m.getMessage();

        if (message.isNoteOnOrOff())
        {
           handleNote(message, m.samplePosition);
        }
        else
        {
            output.addEvent(message, m.samplePosition);
        }
    }

    copyBuffers(output, input);
}
void DuplicateNoteHandler::handleNoteOn(const MidiMessage& message,
                                        int samplePos) noexcept
{
    auto noteNum = message.getNoteNumber();
    auto channel = message.getChannel() - 1;

    auto& count = heldNotes[channel][noteNum].count;

    if (message.isNoteOn())
    {
        if (count > 0)
            output.addEvent(toNoteOff(message), samplePos);

        output.addEvent(message, samplePos);

        ++count;
    }
}
void DuplicateNoteHandler::handleNoteOff(const MidiMessage& message,
                                         int samplePos) noexcept
{
    auto noteNum = message.getNoteNumber();
    auto channel = message.getChannel() - 1;

    auto& count = heldNotes[channel][noteNum].count;

    if (count > 0)
    {
        --count;

        if (count == 0)
        {
            output.addEvent(message, samplePos);
        }
    }
    else if (count == 0)
    {
        jassertfalse;
    }
}

void DuplicateNoteHandler::handleNote(const MidiMessage& message, int samplePos) noexcept
{
    if (message.isNoteOn())
        handleNoteOn(message, samplePos);
    else
        handleNoteOff(message, samplePos);
}
} // namespace EA::MIDI