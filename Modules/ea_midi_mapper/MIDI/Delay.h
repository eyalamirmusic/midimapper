#pragma once

#include "DuplicateNoteHandler.h"

namespace EA::MIDI
{
struct DelayedMessage
{
    double triggerTime = 0.0;
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

    void prepare(double sr) { timePerSample = 1.0 / sr; }

    void process(MidiBuffer& input,
                 float delaySecond,
                 int numSamples,
                 DelayProcessor& processor = getDefaultDelayProcessor()) noexcept;

    double time = 0.0;
    double timePerSample = 0.0;

    Vector<DelayedMessage> messages;
    DuplicateNoteHandler duplicates;
};

struct Duplicator
{
    void store(const MidiBuffer& input) noexcept { copyBuffers(input, storage); }
    void mix(MidiBuffer& output) const noexcept { mixBuffers(storage, output); }

    MidiBuffer storage;
};

double getDelayTimeFor(double musicalTime, double bpm) noexcept;

double getDelayTimeFor(const juce::AudioProcessor& processor,
                       double musicalTime,
                       double fallbackBPM = 120.0) noexcept;

} // namespace EA::MIDI