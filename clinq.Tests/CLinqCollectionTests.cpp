/// @file CLinqCollectionTests.cpp
/// Unit tests for the CLinqCollection type.

import CLinq;

#include "catch.hpp"

TEST_CASE("CLinqCollection iterators")
{
    auto collection = CLinqCollection<int>({ 1,2,3,4,5 });

    SECTION("Can be iterated")
    {
        for (auto& element : collection)
        {
        }
    }

    SECTION("Can be const iterated")
    {
        for (auto const& element : collection)
        {
        }
    }
}