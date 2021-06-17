/// @file CLinqCollectionTests.cpp
/// Unit tests for the CLinqCollection type.

import CLinq;

#include <map>
#include <unordered_map>
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

SCENARIO("CLinqCollections can check if a given value matches a function")
{
    GIVEN("A nonempty collection")
    {
        auto const elements = std::vector<int>{ 1,2,3 };
        auto collection = CLinqCollection<int>(elements);

        WHEN("Matching value exists")
        {
            auto matchFunction = [](int const i) { return i == 1; };

            THEN("true is returned")
            {
                REQUIRE(collection.Any(matchFunction));
            }
        }

        WHEN("Matching value exists")
        {
            auto matchFunction = [](int const i) { return i == INT64_MIN; };

            THEN("false is returned")
            {
                REQUIRE_FALSE(collection.Any(matchFunction));
            }
        }
    }

    GIVEN("An empty collection")
    {
        auto collection = CLinqCollection<int>::Empty();

        THEN("false is returned")
        {
            REQUIRE_FALSE(collection.Any([](int const i) { return i == 1; }));
        }
    }
}

SCENARIO("CLinqCollections can check if any values exist")
{
    GIVEN("A nonempty collection")
    {
        auto collection = CLinqCollection<int>({ 1, 2, 3 });

        THEN("true is returned")
        {
            REQUIRE(collection.Any());
        }
    }

    GIVEN("An empty collection")
    {
        auto collection = CLinqCollection<int>::Empty();

        THEN("false is returned")
        {
            REQUIRE_FALSE(collection.Any());
        }
    }
}

SCENARIO("CLinqCollections can check if all values match a function")
{
    GIVEN("A nonempty collection")
    {
        auto const elements = std::vector<int>{ 1,2,3 };
        auto collection = CLinqCollection<int>(elements);

        WHEN("All values match")
        {
            auto matchFunction = [](int const i) { return i > 0; };

            THEN("true is returned")
            {
                REQUIRE(collection.All(matchFunction));
            }
        }

        WHEN("Some values don't match")
        {
            auto matchFunction = [](int const i) { return i % 2 == 1; };

            THEN("false is returned")
            {
                REQUIRE_FALSE(collection.All(matchFunction));
            }
        }
    }

    GIVEN("An empty collection")
    {
        auto collection = CLinqCollection<int>::Empty();

        THEN("false is returned")
        {
            REQUIRE(collection.All([](int const i) { return i == 1; }));
        }
    }
}

SCENARIO("CLinqCollections can have values appended")
{
    GIVEN("A collection and a value")
    {
        auto collection = CLinqCollection<int>({1, 2});
        auto constexpr value = 3;
        auto expectedCollection = CLinqCollection<int>({ 1, 2, 3 });

        WHEN("One collection is appended to the other")
        {
            auto actualCollection = collection.Append(value);

            THEN("The correct collection is returned")
            {
                REQUIRE(expectedCollection == actualCollection);
            }
        }
    }
}

SCENARIO("CLinqCollections can have values prepended")
{
    GIVEN("A collection and a value")
    {
        auto collection = CLinqCollection<int>({1, 2});
        auto constexpr value = 3;
        auto expectedCollection = CLinqCollection<int>({ 3, 1, 2 });

        WHEN("One collection is prepended to the other")
        {
            auto actualCollection = collection.Prepend(value);

            THEN("The correct collection is returned")
            {
                REQUIRE(expectedCollection == actualCollection);
            }
        }
    }
}

SCENARIO("CLinqCollections can be concatenated")
{
    GIVEN("Two collections")
    {
        auto collection1 = CLinqCollection<int>({1, 2});
        auto collection2 = CLinqCollection<int>({3, 4});
        auto expectedCollection = CLinqCollection<int>({1, 2, 3, 4});

        WHEN("One collection is appended to the other")
        {
            auto actualCollection = collection1.Concat(collection2);

            THEN("The correct collection is returned")
            {
                REQUIRE(expectedCollection == actualCollection);
            }
        }
    }
}

SCENARIO("CLinqCollections can be generated from repeat value")
{
    GIVEN("A value and a count")
    {
        auto constexpr count = 5;
        auto constexpr value = 1;
        auto expected = CLinqCollection<int>({1,1,1,1,1});

        WHEN("Value is repeated")
        {
            auto actual = CLinqCollection<int>::Repeat(value, count);

            THEN("The correct collection is returned")
            {
                REQUIRE(expected == actual);
            }
        }
    }
}

SCENARIO("CLinqCollections can be generated from sequential values")
{
    GIVEN("An initial value and a count")
    {
        auto constexpr count = 5;
        auto constexpr initialValue = 1;
        auto expected = CLinqCollection<int>({1, 2, 3, 4, 5});

        WHEN("Value is repeated")
        {
            auto actual = CLinqCollection<int>::Range(initialValue, count);

            THEN("The correct collection is returned")
            {
                REQUIRE(expected == actual);
            }
        }
    }
}

SCENARIO("CLinqCollections can be projected to maps")
{
    GIVEN("A collection")
    {
        auto collection = CLinqCollection<int>({1, 2});

        WHEN("Projected to map")
        {
            auto keySelector = [](int const i) { return i; };
            auto valueSelector = [](int const i) { return i * i; };
            auto expected = std::map<int, int>{ {1,1}, {2,4} };

            THEN("Expected map is produced")
            {
                REQUIRE(expected == collection.ToMap<int, int>(keySelector, valueSelector));
            }
        }

        WHEN("Projected to unordered map")
        {
            auto keySelector = [](int const i) { return i; };
            auto valueSelector = [](int const i) { return i * i; };
            auto expected = std::unordered_map<int, int>{ {1,1}, {2,4} };

            THEN("Expected map is produced")
            {
                REQUIRE(expected == collection.ToUnorderedMap<int, int>(keySelector, valueSelector));
            }
        }
    }
}

SCENARIO("CLinqCollection elements can be projected to a new sequence")
{
    GIVEN("A collection and projection function")
    {
        auto collection = CLinqCollection<int>({1, 2, 3, 4});
        auto expected = CLinqCollection<int>({2, 4, 6, 8});
        auto projectionFunction = [](int const i){ return i * 2; };

        WHEN("The elements are projected to a new sequence")
        {
            auto projectedCollection = collection.Select<int>(projectionFunction);

            THEN("The expected collection is returned")
            {
                REQUIRE(expected == projectedCollection);
            }
        }
    }
}

SCENARIO("CLinqCollection elements can be filtered to a new sequence")
{
    GIVEN("A collection and filter function")
    {
        auto collection = CLinqCollection<int>({1, 2, 3, 4});
        auto expected = CLinqCollection<int>({1, 2});
        auto filterFunction = [](int const i) { return i < 3; };

        WHEN("The elements are filtered to a new sequence")
        {
            auto filteredCollection = collection.Where(filterFunction);

            THEN("The expected collection is returned")
            {
                REQUIRE(expected == filteredCollection);
            }
        }
    }
}

SCENARIO("CLinqCollections can be cast to other element types")
{
    GIVEN("A collection")
    {
        auto collection = CLinqCollection<int>({1, 2});
        auto expected = CLinqCollection<float>(std::vector<float>{1.f, 2.f});

        WHEN("Collection is cast")
        {
            auto castCollection = collection.StaticCast<float>();

            THEN("Expected collection is returned")
            {
                REQUIRE(expected == castCollection);
            }
        }
    }
}

SCENARIO("CLinqCollections can check for element existence")
{
    GIVEN("A value")
    {
        auto constexpr value = 1;

        WHEN("Collection contains value")
        {
            auto collection = CLinqCollection<int>({1, 2});

            THEN("Contains returns true")
            {
                REQUIRE(collection.Contains(value));
            }
        }

        WHEN("Collection does not contain value")
        {
            auto collection = CLinqCollection<int>({2, 2});

            THEN("Contains returns false")
            {
                REQUIRE_FALSE(collection.Contains(value));
            }
        }
    }
}

SCENARIO("ClinqCollection can generate distinct elements")
{
    GIVEN("A collection")
    {
        auto collection = CLinqCollection<int>({1, 2, 1, 1, 1, 2, 2});
        auto expected = CLinqCollection<int>({1, 2});

        WHEN("Distinct called")
        {
            auto distinct = collection.Distinct();

            THEN("Distinct elements generated")
            {
                REQUIRE(expected == distinct);
            }
        }
    }
}

SCENARIO("CLinqCollections can be reversed")
{
    GIVEN("A collection")
    {
        auto collection = CLinqCollection<int>({1, 2, 3, 4});
        auto expected = CLinqCollection<int>({4, 3, 2, 1});

        WHEN("The collection is reversed")
        {
            auto reversed = collection.Reverse();

            THEN("Expected collection is returned")
            {
                REQUIRE(expected == reversed);
            }
        }
    }
}

SCENARIO("CLinqCollection can have single element returned")
{
    GIVEN("A collection with many elements")
    {
        auto collection = CLinqCollection<int>({ 1, 2, 3, 4 });

        WHEN("Single is called")
        {
            THEN("Exception is thrown")
            {
                REQUIRE_THROWS(collection.Single());
            }
        }
    }

    GIVEN("An empty collection")
    {
        auto collection = CLinqCollection<int>::Empty();

        WHEN("Single is called")
        {
            THEN("Exception is thrown")
            {
                REQUIRE_THROWS(collection.Single());
            }
        }
    }

    GIVEN("A collection with a single element")
    {
        auto collection = CLinqCollection<int>({ 1 });

        WHEN("Single is called")
        {
            auto element = collection.Single();

            THEN("Expected value is returned")
            {
                REQUIRE(1 == element);
            }
        }
    }
}