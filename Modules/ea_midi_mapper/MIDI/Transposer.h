#pragma once

#include "Mapper.h"

namespace EA::MIDI
{
inline int transposeNote(int note, int offset) noexcept
{
    auto transposedNote = note + offset;
    return juce::jlimit(0, 127, transposedNote);
}

struct Transposer : NoteMapper
{
    void map(int source, HeldNotes& dest) noexcept override
    {
        dest.map(source, transposeNote(source, transpose));
    }

    int transpose = 0;
};

struct HarmonizedTransposer : NoteMapper
{
    void map(int source, HeldNotes& dest) noexcept override
    {
        if (transpose != 0)
            dest.map(source, source);

        dest.map(source, transposeNote(source, transpose));
    }

    int transpose = 0;
};

struct StatefulTransposer : NoteMapper
{
    void map(int source, HeldNotes& dest) noexcept override
    {
        if (transpose != 0)
            dest.map(source, source);

        dest.map(source, transposeNote(source, transpose));

        if (direction)
        {
            ++transpose;

            if (transpose == 12)
                direction = false;
        }
        else
        {
            --transpose;

            if (transpose == -12)
                direction = true;
        }
    }

    int transpose = 0;
    bool direction = true;
};

struct RandomTransposer : NoteMapper
{
    void map(int source, HeldNotes& dest) noexcept override
    {
        auto transpose = random.nextInt({-24, 24});

        if (transpose != 0)
            dest.map(source, source);

        dest.map(source, transposeNote(source, transpose));
    }

    juce::Random random;
};
} // namespace EA::MIDI
