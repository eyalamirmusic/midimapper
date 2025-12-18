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

struct DelayProcessor
{
    virtual ~DelayProcessor() = default;

    virtual bool shouldDelay(const MidiMessage&) { return true; }
    virtual void processDelayed(MidiMessage&) {}
};

DelayProcessor& getDefaultDelayProcessor();

struct Delay
{
    Delay();

    void process(MidiBuffer& input,
                 int sampleDelay,
                 int numSamples,
                 DelayProcessor& processor = getDefaultDelayProcessor()) noexcept;

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