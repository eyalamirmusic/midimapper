#include "PluginProcessor.h"

void MidiFXProcessor::processBlock(juce::AudioBuffer<float>& /*buffer*/,
                                   juce::MidiBuffer& midiMessages)

{
    if (harmonize->get())
    {
        harmonizedTransposer.transpose = transpose->get();
        mapper.process(midiMessages, harmonizedTransposer);
    }
    else
    {
        transposer.transpose = transpose->get();
        mapper.process(midiMessages, transposer);
    }

    EA::MIDI::log(midiMessages);
}

juce::AudioProcessorEditor* MidiFXProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiFXProcessor();
}
