#include "PluginProcessor.h"

DelayProcessor::DelayProcessor()
{
    addParameter(timeChoice);
    addParameter(delayParam);
    addParameter(musicalTimeDuration);
    addParameter(velReduction);
    addParameter(feedbackParam);
}

void DelayProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                  juce::MidiBuffer& midiMessages)

{
    velProcessor.reduction = velReduction->get();

    auto numFeedbacks = feedbackParam->get();
    auto delayTime = getDelayTime();

    for (int index = 0; index < numFeedbacks; ++index)
    {
        feedbackLines[index].process(
            midiMessages, buffer.getNumSamples(), delayTime, velProcessor);
    }
}

void DelayProcessor::prepareToPlay(double sampleRate, int)
{
    for (auto& delay: feedbackLines)
        delay.delay.prepare(sampleRate);
}

float DelayProcessor::getDelayTime() const noexcept
{
    if (timeChoice->getIndex() == 0)
        return delayParam->get();

    auto time = getMusicalDelayTime();

    return (float) EA::MIDI::getDelayTimeFor(*this, time);
}

float DelayProcessor::getMusicalDelayTime() const noexcept
{
    switch (musicalTimeDuration->getIndex())
    {
        case 0:
            return 2.f;
        case 1:
            return 1.f;
        case 2:
            return 0.5;
        case 3:
            return 0.25;
        case 4:
            return 1.f / 3.f;
        case 5:
            return 0.75;
        default:
            return 1.f;
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayProcessor();
}
