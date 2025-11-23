#pragma once

#include "Mapper.h"

namespace EA::MIDI
{
struct Transposer : NoteMapper
{
    void map(int source, HeldNotes& dest) noexcept override
    {
        auto transposedNote = source + transpose;
        transposedNote = juce::jlimit(0, 127, transposedNote);
        dest.map(source, transposedNote);
    }

    int transpose = 0;
};

struct HarmonizedTransposer: NoteMapper
{
    void map(int source, HeldNotes& dest) noexcept override
    {
        if (transpose != 0)
            dest.map(source, source);

        auto transposedNote = source + transpose;
        transposedNote = juce::jlimit(0, 127, transposedNote);
        dest.map(source, transposedNote);
    }

    int transpose = 0;
};
}

