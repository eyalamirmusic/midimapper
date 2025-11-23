#include <ea_midi_mapper/ea_midi_mapper.h>

int main()
{
    auto buffer = juce::MidiBuffer();

    buffer.addEvent(juce::MidiMessage::noteOn(1, 50, 0.5f), 0);
    buffer.addEvent(juce::MidiMessage::noteOn(1, 100, 0.5f), 0);
    buffer.addEvent(juce::MidiMessage::noteOn(1, 73, 0.5f), 0);

    EA::MIDI::HarmonizedTransposer transposer;
    EA::MIDI::Mapper mapper;

    transposer.transpose = 3;

    mapper.process(buffer, transposer);

    EA::MIDI::log(buffer);
}
