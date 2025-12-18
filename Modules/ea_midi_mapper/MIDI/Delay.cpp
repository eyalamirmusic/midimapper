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
                    float delaySeconds,
                    int numSamples,
                    DelayProcessor& processor) noexcept
{
    //You have to call prepare() before processing!
    jassert(timePerSample > 0.0);

    for (auto m: input)
    {
        auto message = m.getMessage();

        auto delayed = DelayedMessage();
        delayed.message = message;
        delayed.triggerTime = time + (m.samplePosition * timePerSample);

        if (processor.shouldDelay(message))
            delayed.triggerTime += (double) delaySeconds;

        messages.push_back(delayed);
    }

    input.clear();

    for (int sample = 0; sample < numSamples; ++sample)
    {
        for (auto& message: messages)
        {
            if (!message.triggered && message.triggerTime <= time)
            {
                processor.processDelayed(message.message);
                input.addEvent(message.message, sample);
                message.triggered = true;
            }
        }

        auto toRemove = [](const DelayedMessage& m) { return m.triggered; };
        messages.eraseIf(toRemove);

        time += timePerSample;
    }

    duplicates.process(input);
}

double getBPM(const juce::AudioProcessor& processor, double fallbackBPM = 120.0) noexcept
{
    if (auto ph = processor.getPlayHead())
    {
        if (auto pos = ph->getPosition())
        {
            if (auto bpm = pos->getBpm())
            {
                return *bpm;
            }
        }
    }

    return fallbackBPM;
}

double getDelayTimeFor(double musicalTime, double bpm) noexcept
{
    return 60.0 / bpm * musicalTime;
}

double getDelayTimeFor(const juce::AudioProcessor& processor,
                       double musicalTime,
                       double fallbackBPM) noexcept
{
    return getDelayTimeFor(musicalTime, getBPM(processor, fallbackBPM));
}
} // namespace EA::MIDI