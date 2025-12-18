#include "Delay.h"

namespace EA::MIDI
{

DelayProcessor& getDefaultDelayProcessor()
{
    static DelayProcessor delay;
    return delay;
}
Delay::Delay()
{
    messages.reserve(10000);
}

void Delay::process(MidiBuffer& input,
                    int sampleDelay,
                    int numSamples,
                    DelayProcessor& processor) noexcept
{
    for (auto m: input)
    {
        auto message = m.getMessage();

        auto delayed = DelayedMessage();
        delayed.message = message;
        delayed.sampleTime = samplePos + m.samplePosition;

        if (processor.shouldDelay(message))
            delayed.sampleTime += sampleDelay;

        messages.push_back(delayed);
    }

    input.clear();

    for (int sample = 0; sample < numSamples; ++sample)
    {
        for (auto& message: messages)
        {
            if (!message.triggered && message.sampleTime <= samplePos)
            {
                processor.processDelayed(message.message);
                input.addEvent(message.message, sample);
                message.triggered = true;
            }
        }

        auto toRemove = [](const DelayedMessage& m) { return m.triggered; };
        messages.eraseIf(toRemove);

        ++samplePos;
    }
}
} // namespace EA::MIDI