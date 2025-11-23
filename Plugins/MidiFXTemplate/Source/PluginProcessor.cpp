#include "PluginProcessor.h"

void MidiFXProcessor::processBlock(juce::AudioBuffer<float>& /*buffer*/,
                                   juce::MidiBuffer& midiMessages)

{
    switch (mode->getIndex())
    {
        case 0:
            transposer.transpose = transpose->get();
            mapper.process(midiMessages, transposer);
            break;
        case 1:
            harmonizedTransposer.transpose = transpose->get();
            mapper.process(midiMessages, harmonizedTransposer);
            break;
        case 2:
            mapper.process(midiMessages, statefulTransposer);
            break;
        case 3:
            mapper.process(midiMessages, random);
            break;
        default:
            jassertfalse;
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiFXProcessor();
}
