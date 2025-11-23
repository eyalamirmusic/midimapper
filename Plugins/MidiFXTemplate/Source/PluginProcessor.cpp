#include "PluginProcessor.h"

void MidiFXProcessor::processBlock(juce::AudioBuffer<float>& /*buffer*/,
                                   juce::MidiBuffer& midiMessages)

{
    auto transposedMidi = transposer.process(midiMessages, transpose->get());

    EA::MIDI::log(transposedMidi);
    EA::MIDI::replaceContentsWith(transposedMidi, midiMessages);
}

juce::AudioProcessorEditor* MidiFXProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiFXProcessor();
}
