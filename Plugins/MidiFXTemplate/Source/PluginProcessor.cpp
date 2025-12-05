#include "PluginProcessor.h"

MidiFXProcessor::MidiFXProcessor()
{
    addParameter(transpose);
    addParameter(mode);
    addParameter(fixedNote);
}

void MidiFXProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto params = PluginHelpers::saveParamsTree(*this);

    auto pluginPreset = juce::ValueTree(getName());
    pluginPreset.appendChild(params, nullptr);

    copyXmlToBinary(*pluginPreset.createXml(), destData);
}

void MidiFXProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
    {
        auto preset = juce::ValueTree::fromXml(*xml);
        auto params = preset.getChildWithName("Params");

        PluginHelpers::loadParamsTree(*this, params);
    }
}

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

        case 4:
            fixed.noteNum = fixedNote->get();
            mapper.process(midiMessages, fixed);
        break;

        default:
            jassertfalse;
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiFXProcessor();
}
