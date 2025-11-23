#include "PluginProcessor.h"

static void replaceContentsWith(const juce::MidiBuffer& source,
                                juce::MidiBuffer& dest) noexcept
{
    dest.clear();

    for (auto m: source)
        dest.addEvent(m.getMessage(), m.samplePosition);
}

static void logMIDI(const juce::MidiBuffer& midiMessages)
{
    for (auto m: midiMessages)
    {
        auto message = m.getMessage();
        auto samplePos = m.samplePosition;

        std::cout << message.getDescription() << " SamplePos:" << juce::String(samplePos)
                  << std::endl;
    }
}



void MidiFXProcessor::processBlock(juce::AudioBuffer<float>& /*buffer*/,
                                   juce::MidiBuffer& midiMessages)

{
    auto transposedMidi = transposer.process(midiMessages, transpose->get());

    logMIDI(transposedMidi);
    replaceContentsWith(transposedMidi, midiMessages);
}

juce::AudioProcessorEditor* MidiFXProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiFXProcessor();
}
