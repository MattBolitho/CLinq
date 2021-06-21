/// @file CLinq.hpp
/// Defines the CLinq module interface.

#ifndef CLINQ_HPP
#define CLINQ_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <initializer_list>
#include <concepts>
#include <type_traits>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <type_traits>

/// Checks if the given type is iterable. By default, this will be false.
/// @tparam T The type to check.
template <typename T, typename = void>
struct IsCLinqIterable : std::false_type {};

/// The type is iterable if it has 'begin' and 'end'.
/// @tparam T The type to check.
template <typename T>
struct IsCLinqIterable<T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>> : std::true_type {};

/// Concept for checking if a type is iterable.
/// @tparam T The type to check.
template <typename T>
concept CLinqIterable = IsCLinqIterable<T>::value;

/// Thrown when an error occurs in the CLinq library.
class CLinqException final : public std::runtime_error
{
    public:
        /// Initializes a new instance of the CLinqException class.
        explicit CLinqException() noexcept
            : std::runtime_error("")
        {
        }

        /// Initializes a new instance of the CLinqException class.
        /// @param message The message with the exception.
        explicit CLinqException(char const* const message) noexcept
            : std::runtime_error(message)
        {
        }

        /// Initializes a new instance of the CLinqException class.
        /// @param message The message with the exception.
        explicit CLinqException(std::string const& message) noexcept
            : std::runtime_error(message)
        {
        }
};

/// A collection of elements supporting CLinq methods.
/// @tparam TElement The type of elements in the collection.
template <typename TElement>
class CLinqCollection
{
    public:
        using value_type = TElement;
        using size_type = typename std::vector<TElement>::size_type;

        using iterator = typename std::vector<TElement>::iterator;
        using const_iterator = typename std::vector<TElement>::const_iterator;
        using reverse_iterator = typename std::vector<TElement>::reverse_iterator;

        /// Type alias for a function that maps elements to true or false.
        using MatchFunction = std::function<bool(TElement)>;

        /// Type alias for a function that projects elements to some other type.
        /// @tparam TProjection The type to project the elements to.
        template <typename TProjection>
        using ProjectionFunction = std::function<TProjection(TElement)>;

        /// Initializes a new instance of the CLinqCollection class.
        CLinqCollection() noexcept
            : _elements(std::vector<TElement>())
        {
        }

        /// Initializes a new instance of the CLinqCollection class.
        CLinqCollection(std::initializer_list<TElement> const& elements) noexcept
            : _elements(std::vector<TElement>(elements.begin(), elements.end()))
        {
        }

        /// Initializes a new instance of the CLinqCollection class.
        /// @param elements The initial elements.
        CLinqCollection(std::vector<TElement> const& elements) noexcept
            : _elements(elements)
        {
        }

        /// Initializes a new instance of the CLinqCollection class.
        /// @param memory A block of memory to copy values from.
        /// @param numberOfElements The number of elements to copy.
        explicit CLinqCollection(TElement* const memory, size_type const numberOfElements) noexcept
            : _elements(std::vector<TElement>(memory, memory + numberOfElements))
        {
        }

        /// Initializes a new instance of the CLinqCollection class.
        /// @tparam TIterable An iterable type.
        /// @param iterable The iterable instance.
        template <CLinqIterable TIterable>
        explicit CLinqCollection(TIterable& iterable)
            : _elements(std::vector<TElement>())
        {
            for (auto& element : iterable)
            {
                _elements.emplace_back(element);
            }
        }

        /// Gets a reference to the element at a given index of the collection.
        /// @param i The index.
        /// @returns A reference to the element at a given index of the collection.
        TElement& operator[](size_type const i)
        {
            return _elements[i];
        }

        /// Gets a const reference to the element at a given index of the collection.
        /// @param i The index.
        /// @returns A const reference to the element at a given index of the collection.
        TElement const& operator[](size_type const i) const
        {
            return _elements[i];
        }

        /// Spaceship operator.
        /// @returns An ordering comparing this instance and the given collection.
        auto operator<=>(CLinqCollection<TElement> const&) const = default;

        /// Concatenates this collection with the given collection and returns the result.
        /// @param collection The collection.
        /// @returns This instance concatenated with the given collection.
        CLinqCollection<TElement> operator+(CLinqCollection<TElement> const& collection) const noexcept
        {
            auto newSize = _elements.size() + collection._elements.size();
            auto newElements = std::vector<TElement>(newSize);

            for (auto i = 0; i < _elements.size(); ++i)
            {
                newElements[i] = _elements[i];
            }

            size_type index = 0;
            for (auto i = _elements.size(); i < newSize; ++i)
            {
                newElements[i] = collection._elements[index];
                ++index;
            }

            return CLinqCollection<TElement>(newElements);
        }

        /// Gets an empty collection.
        /// @return An empty collection.
        static CLinqCollection<TElement> Empty()
        {
            return CLinqCollection<TElement>();
        }

        /// Gets a collection containing the same element n times.
        /// @param element The element.
        /// @param count The number of elements.
        /// @returns A collection containing the same element n times.
        static CLinqCollection<TElement> Repeat(TElement const& element, size_type const count)
        {
            return CLinqCollection<TElement>(std::vector<TElement>(count, element));
        }

        /// Gets a collection starting from the initial element and prefix incrementing it n times.
        /// @param initial The initial element.
        /// @param count The number of elements.
        /// @returns A collection starting from the initial element and prefix incrementing it n times.
        static CLinqCollection<TElement> Range(TElement const& initial, size_type const count)
        {
            auto element = initial;
            auto elements = std::vector<TElement>(count);

            for (size_type i = 0; i < count; ++i)
            {
                elements[i] = element;
                ++element;
            }

            return CLinqCollection<TElement>(elements);
        }

        /// Gets the iterator at the start of the collection.
        iterator begin() noexcept
        {
            return _elements.begin();
        }

        /// Gets the iterator at the end of the collection.
        iterator end() noexcept
        {
            return _elements.end();
        }

        /// Gets the const iterator at the start of the collection.
        const_iterator cbegin() const noexcept
        {
            return _elements.cbegin();
        }

        /// Gets the const iterator at the end of the collection.
        const_iterator cend() const noexcept
        {
            return _elements.cend();
        }

        /// Checks that every element in the collection matches the given match function.
        /// @param matchFunction The match function.
        /// @returns True if every element in the collection matches the given match
        /// function, false otherwise.
        bool All(MatchFunction const& matchFunction) const noexcept
        {
            for (auto& element : _elements)
            {
                if (!matchFunction(element))
                {
                    return false;
                }
            }

            return true;
        }

        /// Checks if the collection contains any element.
        /// @returns True if the collection contains any element, false otherwise.
        bool Any() const noexcept
        {
            return !_elements.empty();
        }

        /// Checks if the collection contains any element that matches the
        /// given function.
        /// @param matchFunction The match function.
        /// @returns True if the collection contains any element that matches
        /// the given function, false otherwise.
        bool Any(MatchFunction const& matchFunction) const noexcept
        {
            for (auto& element : _elements)
            {
                if (matchFunction(element))
                {
                    return true;
                }
            }

            return false;
        }

        /// Appends the element to the collection.
        /// @param element The element.
        /// @returns A new collection with the element appended.
        CLinqCollection<TElement> Append(TElement const& element) const
        {
            auto newElements = _elements;
            newElements.emplace_back(element);

            return CLinqCollection<TElement>(newElements);
        }

        /// Gets a const reference to the element at given index in the collection.
        /// @param index The index.
        /// @returns A const reference to the element at given index in the collection.
        /// @throws CLinqException When the index is out of range.
        TElement const& At(size_type const index) const
        {
            ThrowIfEmpty();
            ThrowIfOutOfRange(index);

            return _elements[index];
        }

        /// Concatenates the two collections and returns the result as a new instance.
        /// @param collection The collection.
        /// @returns The two collections concatenated.
        CLinqCollection<TElement> Concat(CLinqCollection<TElement> collection) const
        {
            return *this + collection;
        }

        /// Checks whether or not the collection contains the given element.
        /// @param element The element to find.
        /// @returns True if the set contains the given element, false otherwise.
        bool Contains(TElement const& element) const
        {
            return VectorContains(_elements, element);
        }

        /// Gets the number of elements in the collection.
        /// @returns The number of elements in the collection.
        size_type Count() const noexcept
        {
            return _elements.size();
        }

        /// Gets the number of elements in the collection that match the given match function.
        /// @param matchFunction The match function.
        /// @returns The number of elements that match the given match function.
        size_type Count(MatchFunction const& matchFunction) const
        {
            size_type count = 0;

            for (auto& element : _elements)
            {
                if (matchFunction(element))
                {
                    ++count;
                }
            }

            return count;
        }

        /// Gets the distinct elements of the collection.
        /// @returns Gets the distinct elements of the collection.
        CLinqCollection<TElement> Distinct() const
        {
            auto newElements = std::vector<TElement>();

            for (auto& element : _elements)
            {
                if (!VectorContains(newElements, element))
                {
                    newElements.emplace_back(element);
                }
            }

            return CLinqCollection<TElement>(newElements);
        }

        /// Gets the elements in this collection with elements in the given collection omitted.
        /// @param collection The collection.
        /// @returns The elements in this collection with elements in the given collection omitted.
        CLinqCollection<TElement> Except(CLinqCollection<TElement> const& collection) const
        {
            auto newElements = std::vector<TElement>();

            for (auto& element : _elements)
            {
                if (!VectorContains(collection._elements, element))
                {
                    newElements.emplace_back(element);
                }
            }

            return CLinqCollection<TElement>(newElements);
        }

        /// Gets a const reference to the first element in the collection.
        /// @returns A const reference to the first element in the collection.
        TElement const& First() const
        {
            ThrowIfEmpty();

            return _elements.front();
        }

        /// Computes the set intersection of this collection and the given collection.
        /// @param collection The collection.
        /// @returns The set intersection of this collection and the given collection.
        CLinqCollection<TElement> Intersection(CLinqCollection<TElement> const& collection) const
        {
            auto newElements = std::vector<TElement>();

            for (auto& element : _elements)
            {
                if (VectorContains(_elements, element) && VectorContains(collection._elements, element))
                {
                    newElements.emplace_back(element);
                }
            }

            return CLinqCollection<TElement>(newElements);
        }

        /// Gets a const reference to the last element in the collection.
        /// @returns A const reference to the last element in the collection.
        TElement const& Last() const
        {
            ThrowIfEmpty();

            return _elements.back();
        }

        /// Prepends the element to the collection.
        /// @param element The element.
        /// @returns A new collection with the element prepended.
        CLinqCollection<TElement> Prepend(TElement const& element) const
        {
            auto newElements = _elements;
            newElements.emplace(newElements.begin(), element);

            return CLinqCollection<TElement>(newElements);
        }

        /// Gets the collection in reverse order.
        /// @returns The collection in reverse order.
        CLinqCollection<TElement> Reverse() const
        {
            auto newElements = _elements;
            std::reverse(newElements.begin(), newElements.end());
            return CLinqCollection<TElement>(newElements);
        }

        /// Projects each element to a new sequence using the projection function.
        /// @tparam TProjection The type to project the elements to.
        /// @param projectionFunction The projection function/
        /// @returns Each element projected into a new sequence.
        template <typename TProjection>
        CLinqCollection<TProjection> Select(ProjectionFunction<TProjection> const& projectionFunction) const
        {
            auto newElements = std::vector<TProjection>(_elements.size());

            for (size_type i = 0; i < _elements.size(); ++i)
            {
                newElements[i] = projectionFunction(_elements[i]);
            }

            return CLinqCollection<TProjection>(newElements);
        }

        /// Returns the only element of the sequence.
        /// @returns A reference to the single element of the sequence.
        /// @throws CLinqException If 0 or many elements are contained within the collection.
        TElement const& Single() const
        {
            ThrowIfEmpty();
            if (_elements.size() > 1)
            {
                throw CLinqException("Collection contains more than 1 element.");
            }

            return _elements[0];
        }

        /// Skips a given number of elements and returns the rest.
        /// @param numberOfElements The number of elements to skip.
        /// @returns The remainder of the elements after skipping n.
        CLinqCollection<TElement> Skip(size_type const numberOfElements) const
        {
            if (numberOfElements > _elements.size())
            {
                throw CLinqException("Cannot skip more elements than exist in collection.");
            }

            return CLinqCollection<TElement>(std::vector<TElement>(_elements.begin() + numberOfElements, _elements.end()));
        }

        /// Skips a given number of elements from the back of the collection and returns the rest.
        /// @param numberOfElements The number of elements to skip from the back.
        /// @returns The remainder of the elements after skipping n from the back of the collection.
        CLinqCollection<TElement> SkipLast(size_type const numberOfElements) const
        {
            if (numberOfElements > _elements.size())
            {
                throw CLinqException("Cannot skip more elements than exist in collection.");
            }

            return CLinqCollection<TElement>(std::vector<TElement>(_elements.begin(), _elements.end() - numberOfElements));
        }

        /// Skips elements while the match function returns true and returns the remainder of the collection.
        /// @param matchFunction The match function.
        /// @returns A new collection with the elements skipped until the match function returns false.
        CLinqCollection<TElement> SkipWhile(MatchFunction const& matchFunction) const
        {
            size_type skipNumber = 0;
            for (auto& element : _elements)
            {
                if (matchFunction(element))
                {
                    ++skipNumber;
                }
                else
                {
                    break;
                }
            }

            return Skip(skipNumber);
        }

        /// Static casts each element of the collection to a new collection.
        /// @tparam TCast The type to cast to.
        /// @returns The cast collection.
        template <typename TCast>
        CLinqCollection<TCast> StaticCast() const
        {
            static_assert(
                std::is_convertible<TElement, TCast>::value,
                "Cannot cast StaticCast CLinqCollection.");

            auto newElements = std::vector<TCast>(_elements.size());

            for (size_type i = 0; i < _elements.size(); ++i)
            {
                newElements[i] = static_cast<TCast>(_elements[i]);
            }

            return CLinqCollection<TCast>(newElements);
        }

        /// Takes a specific number of elements from the start of the collection.
        /// @param numberOfElements The number of elements to take.
        /// @returns A new collection with the first n elements from the collection.
        CLinqCollection<TElement> Take(size_type const numberOfElements) const
        {
            if (numberOfElements > _elements.size())
            {
                throw CLinqException("Cannot take more elements than exist in collection.");
            }

            auto newElements = _elements;
            newElements.resize(numberOfElements);

            return CLinqCollection<TElement>(newElements);
        }

        /// Takes a specific number of elements from the end of the collection.
        /// @param numberOfElements The number of elements to take.
        /// @returns A new collection with the last n elements from the collection.
        CLinqCollection<TElement> TakeLast(size_type const numberOfElements) const
        {
            if (numberOfElements > _elements.size())
            {
                throw CLinqException("Cannot take more elements than exist in collection.");
            }

            return CLinqCollection<TElement>(std::vector<TElement>(_elements.end() - numberOfElements, _elements.end()));
        }

        /// Takes elements while the match function returns true.
        /// @param matchFunction The match function.
        /// @returns A new collection with the elements taken until the match function returns false.
        CLinqCollection<TElement> TakeWhile(MatchFunction const& matchFunction) const
        {
            auto newElements = std::vector<TElement>();

            for (auto& element : _elements)
            {
                if (matchFunction(element))
                {
                    newElements.emplace_back(element);
                }
                else
                {
                    break;
                }
            }

            return CLinqCollection<TElement>(newElements);
        }

        /// Gets a collection of elements from the collection that match the match function.
        /// @param matchFunction The match function/
        /// @returns A collection of elements from the collection that match the match function.
        CLinqCollection<TElement> Where(MatchFunction const& matchFunction) const
        {
            auto newElements = std::vector<TElement>();

            for (auto& element : _elements)
            {
                if (matchFunction(element))
                {
                    newElements.emplace_back(element);
                }
            }

            return CLinqCollection<TElement>(newElements);
        }

        /// Computes the set union of this collection and the given collection.
        /// @param collection The collection.
        /// @returns The set union of this collection and the given collection.
        CLinqCollection<TElement> Union(CLinqCollection<TElement> const& collection) const
        {
            auto newElements = Distinct()._elements;
            auto distinctCollection = collection.Distinct()._elements;

            for (auto& element : distinctCollection)
            {
                if (!VectorContains(newElements, element))
                {
                    newElements.emplace_back(element);
                }
            }

            return CLinqCollection<TElement>(newElements);
        }

        /// Gets the elements in the collection as a vector.
        /// @returns The elements in the collection as a vector.
        std::vector<TElement> ToVector() const noexcept
        {
            return _elements;
        }

        /// Gets the elements in the collection as a list.
        /// @returns The elements in the collection as a list.
        std::list<TElement> ToList() const
        {
            return std::list(_elements.begin(), _elements.end());
        }

        /// Gets the elements in the collection as a set.
        /// @returns The elements in the collection as a set.
        std::list<TElement> ToSet() const
        {
            return std::set(_elements.begin(), _elements.end());
        }

        /// Projects the collection to a map.
        /// @tparam TKey The type of the map's keys.
        /// @tparam TValue The type of the map's values.
        /// @param keySelector A projection function for the keys.
        /// @param valueSelector A projection function for the values.
        /// @returns A map from the elements in the collection.
        template <typename TKey, typename TValue>
        std::map<TKey, TValue> ToMap(
            ProjectionFunction<TKey> const& keySelector,
            ProjectionFunction<TValue> const& valueSelector) const
        {
            return ToMapCore<std::map<TKey, TValue>>(keySelector, valueSelector);
        }

        /// Projects the collection to an unordered map.
        /// @tparam TKey The type of the map's keys.
        /// @tparam TValue The type of the map's values.
        /// @param keySelector A projection function for the keys.
        /// @param valueSelector A projection function for the values.
        /// @returns An unordered map from the elements in the collection.
        template <typename TKey, typename TValue>
        std::unordered_map<TKey, TValue> ToUnorderedMap(
            ProjectionFunction<TKey> const& keySelector,
            ProjectionFunction<TValue> const& valueSelector) const
        {
            return ToMapCore<std::unordered_map<TKey, TValue>>(keySelector, valueSelector);
        }

    private:
        std::vector<TElement> _elements;

        template <typename T>
        static bool VectorContains(std::vector<T> const& vector, T const& value)
        {
            return std::find(vector.begin(), vector.end(), value) != vector.end();
        }

        void ThrowIfOutOfRange(size_type const index) const
        {
            if (index >= _elements.size())
            {
                throw CLinqException("Index was out of range."
                    "Attempted to access index " + std::to_string(index) +
                    "but collection contains " + std::to_string(_elements.size()) + " elements.");
            }
        }

        void ThrowIfEmpty() const
        {
            if (_elements.empty())
            {
                throw CLinqException("Collection is empty.");
            }
        }

        template <typename TMap, typename TKey, typename TValue>
        TMap ToMapCore(
            ProjectionFunction<TKey> const& keySelector,
            ProjectionFunction<TValue> const& valueSelector) const
        {
            TMap map{};

            for (auto& element : _elements)
            {
                map[keySelector(element)] = valueSelector(element);
            }

            return map;
        }
};

#endif // CLINQ_HPP
