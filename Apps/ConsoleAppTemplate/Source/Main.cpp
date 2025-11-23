#include <juce_core/juce_core.h>

int main()
{
    auto v = std::vector<int> {1, 2, 3, 4, 5};

    for (auto& e: v)
    {
        if (e == 2)
            v.push_back(50);

        std::cout << e << std::endl;
    }

    return 0;
}
