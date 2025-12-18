#include "PluginProcessor.h"

DelayProcessor::DelayProcessor()
{
    addParameter(delayParam);
    addParameter(velReduction);
    addParameter(feedbackParam);
}

void DelayProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                  juce::MidiBuffer& midiMessages)

{
    velProcessor.reduction = velReduction->get();

    auto numFeedbacks = feedbackParam->get();
    auto delayTime = delayParam->get();

    for (int index = 0; index < numFeedbacks; ++index)
    {
        feedbackLines[index].process(
            midiMessages, buffer.getNumSamples(), delayTime, velProcessor);
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayProcessor();
}
