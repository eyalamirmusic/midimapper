#pragma once

#include "../Common/Common.h"
#include "Helpers.h"

namespace EA::MIDI
{
struct DelayedMessage
{
    int sampleTime = 0;
    MidiMessage message;
    bool triggered = false;
};

struct Delay
{
    Delay() { messages.reserve(10000); }

    void process(MidiBuffer& input, int sampleDelay, int numSamples) noexcept
    {
        for (auto m: input)
        {
            auto delayed = DelayedMessage();
            delayed.message = m.getMessage();
            delayed.sampleTime = samplePos + m.samplePosition + sampleDelay;
            messages.push_back(delayed);
        }

        input.clear();

        for (int sample = 0; sample < numSamples; ++sample)
        {
            for (auto& message: messages)
            {
                if (!message.triggered && message.sampleTime <= samplePos)
                {
                    input.addEvent(message.message, sample);
                    message.triggered = true;
                }
            }

            auto toRemove = [](const DelayedMessage& m) { return m.triggered; };
            messages.eraseIf(toRemove);

            ++samplePos;
        }
    }

    int samplePos = 0;

    Vector<DelayedMessage> messages;
};

struct Duplicator
{
    void store(const MidiBuffer& input) noexcept { copyBuffers(input, storage); }
    void mix(MidiBuffer& output) noexcept { mixBuffers(storage, output); }

    MidiBuffer storage;
};
} // namespace EA::MIDI