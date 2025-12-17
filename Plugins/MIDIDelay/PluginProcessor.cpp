#include "PluginProcessor.h"

DelayProcessor::DelayProcessor()
{
    addParameter(delayParam);
}

void DelayProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                   juce::MidiBuffer& midiMessages)

{
    duplicator.store(midiMessages);
    delay.process(midiMessages, delayParam->get(), buffer.getNumSamples());
    duplicator.mix(midiMessages);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayProcessor();
}
