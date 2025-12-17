#include "PluginProcessor.h"

MidiFXProcessor::MidiFXProcessor()
{

}

void MidiFXProcessor::processBlock(juce::AudioBuffer<float>& /*buffer*/,
                                   juce::MidiBuffer& midiMessages)

{
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiFXProcessor();
}
