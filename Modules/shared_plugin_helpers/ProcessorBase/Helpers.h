#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace PluginHelpers
{
juce::ValueTree saveParamsTree(const juce::AudioProcessor& processor);

void loadParamsTree(const juce::AudioProcessor& processor,
                           const juce::ValueTree& tree);

void savePluginParams(const juce::AudioProcessor& processor, juce::MemoryBlock& destData);

void loadPluginParams(const juce::AudioProcessor& processor, const void* data, int sizeInBytes);

} // namespace PluginHelpers