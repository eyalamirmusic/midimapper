#include "PluginProcessor.h"

DelayProcessor::DelayProcessor()
{
    addParameter(delayParam);
    addParameter(velReduction);
}

void DelayProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                  juce::MidiBuffer& midiMessages)

{
    duplicator.store(midiMessages);
    velProcessor.reduction = velReduction->get();
    delay.process(midiMessages, delayParam->get(), buffer.getNumSamples(), velProcessor);
    duplicator.mix(midiMessages);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayProcessor();
}
