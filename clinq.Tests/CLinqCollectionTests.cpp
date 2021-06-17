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

SCENARIO("CLinqCollections can be constructed")
{
    GIVEN("No parameters")
    {
        THEN("An empty collection is created")
        {
            REQUIRE(CLinqCollection<int>::Empty() == CLinqCollection<int>());
        }
    }

    GIVEN("A vector")
    {
        std::vector<int> values{ 1, 2 };
        auto collection = CLinqCollection<int>(values);

        THEN("The elements are copied from the vector")
        {
            REQUIRE(collection[0] == values[0]);
            REQUIRE(collection[1] == values[1]);
        }
    }

    GIVEN("A block of memory and a number of elements")
    {
        std::vector<int> values{ 1, 2 };
        auto collection = CLinqCollection<int>(values.data(), values.size());

        THEN("The elements are copied from the vector")
        {
            REQUIRE(collection[0] == values[0]);
            REQUIRE(collection[1] == values[1]);
        }
    }
}

SCENARIO("CLinqCollections can be tested for element-wise equality")
{
    GIVEN("A collection")
    {
        auto collection = CLinqCollection<std::string>(std::vector<std::string>{ "hello", "world" });

        WHEN("Compared with a collection with fewer elements")
        {
            auto compareCollection = CLinqCollection<std::string>(std::vector<std::string>{ "hello" });

            THEN("Comparison is false")
            {
                REQUIRE_FALSE(collection == compareCollection);
            }
        }

        WHEN("Compared with a collection with more elements")
        {
            auto compareCollection = CLinqCollection<std::string>(std::vector<std::string>{ "hello", "world", "!" });

            THEN("Comparison is false")
            {
                REQUIRE_FALSE(collection == compareCollection);
            }
        }

        WHEN("Compared with a collection containing the same number of differing elements")
        {
            auto compareCollection = CLinqCollection<std::string>(std::vector<std::string>{ "world", "hello" });

            THEN("Comparison is false")
            {
                REQUIRE_FALSE(collection == compareCollection);
            }
        }

        WHEN("Compared with a collection containing the same number of equal elements")
        {
            auto compareCollection = CLinqCollection<std::string>(std::vector<std::string>{ "hello", "world" });

            THEN("Comparison is true")
            {
                REQUIRE(collection == compareCollection);
            }
        }
    }
}

SCENARIO("Elements in CLinqCollections can be accessed by index")
{
    constexpr auto* string = "world";
    constexpr auto* newString = "World";
    auto elements = std::vector<std::string>{ string, "hello" };

    GIVEN("Non-const collection")
    {
        auto collection = CLinqCollection<std::string>(elements);

        WHEN("Accessed by index")
        {
            THEN("Correct element value is read")
            {
                REQUIRE(string == collection[0]);
            }

            THEN("Element can be set to new value")
            {
                collection[0] = newString;

                REQUIRE(newString == collection[0]);
            }
        }
    }

    GIVEN("Const collection")
    {
        auto const collection = CLinqCollection<std::string>(elements);

        WHEN("Accessed by index")
        {
            THEN("Correct element value is read")
            {
                REQUIRE(string == collection[0]);
            }
        }
    }
}

SCENARIO("CLinqCollections can be concatenated with addition operator")
{
    GIVEN("Two CLinqCollection instances")
    {
        auto collection1 = CLinqCollection<int>({ 1,2,3,4 });
        auto collection2 = CLinqCollection<int>({ 5,6,7,8,9 });
        auto expected = CLinqCollection<int>({ 1,2,3,4,5,6,7,8,9 });

        WHEN("Collections are concatenated using the addition operator")
        {
            auto concatenatedCollection = collection1 + collection2;

            THEN("The expected collection is returned")
            {
                REQUIRE(expected == concatenatedCollection);
            }
        }
    }
}

SCENARIO("CLinqCollections can have the first element accessed if the collection is not empty")
{
    GIVEN("A nonempty collection")
    {
        auto constexpr expected = 1;
        auto collection = CLinqCollection<int>({ expected,2,3,4 });

        WHEN("The first element is accessed")
        {
            auto firstElement = collection.First();

            THEN("The first element is returned")
            {
                REQUIRE(expected == firstElement);
            }
        }
    }

    GIVEN("An emtpy collection")
    {
        auto collection = CLinqCollection<int>::Empty();

        WHEN("The first element is accessed")
        {
            THEN("An exception is thrown")
            {
                REQUIRE_THROWS(collection.First());
            }
        }
    }
}

SCENARIO("CLinqCollections can have the last element accessed if the collection is not empty")
{
    GIVEN("A nonempty collection")
    {
        auto constexpr expected = 4;
        auto collection = CLinqCollection<int>({ 1,2,3,expected });

        WHEN("The last element is accessed")
        {
            auto lastElement = collection.Last();

            THEN("The last element is returned")
            {
                REQUIRE(expected == lastElement);
            }
        }
    }

    GIVEN("An emtpy collection")
    {
        auto collection = CLinqCollection<int>::Empty();

        WHEN("The last element is accessed")
        {
            THEN("An exception is thrown")
            {
                REQUIRE_THROWS(collection.Last());
            }
        }
    }
}

SCENARIO("CLinqCollections can be accessed at a given index with appropriate exceptions")
{
    GIVEN("A nonempty collection")
    {
        auto constexpr expected = 3;
        auto collection = CLinqCollection<int>({ 1,2,expected,4 });

        WHEN("Given index is out of range")
        {
            THEN("An exception is thrown")
            {
                REQUIRE_THROWS(collection.At(INT64_MAX));
            }
        }

        WHEN("Given index is in range")
        {
            THEN("The correct element is returned")
            {
                REQUIRE(expected == collection.At(2));
            }
        }
    }

    GIVEN("An emtpy collection")
    {
        auto collection = CLinqCollection<int>::Empty();

        WHEN("At is called")
        {
            THEN("An exception is thrown")
            {
                REQUIRE_THROWS(collection.At(1));
            }
        }
    }
}

SCENARIO("CLinqCollections can be get the number of elements contained within")
{
    GIVEN("A collection")
    {
        auto const elements = std::vector<int>{ 1,2,3 };
        auto collection = CLinqCollection<int>(elements);

        WHEN("Elements are counted with no match function")
        {
            auto const expectedSize = elements.size();

            THEN("Count of all elements is returned")
            {
                REQUIRE(expectedSize == collection.Count());
            }
        }

        WHEN("Elements are counted with match function")
        {
            auto const expectedSize = 1;

            THEN("Count of all elements is returned")
            {
                auto actualSize = collection.Count([](int const i) { return i == 1; });
                REQUIRE(expectedSize == actualSize);
            }
        }
    }
}