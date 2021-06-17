/// @file CLinq.ixx
/// Defines the CLinq module interface.

module;
#include <cstdint>
#include <string>
#include <vector>
#include <concepts>
#include <type_traits>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
export module CLinq;

/// Checks if the given type is iterable. By default, this will be false.
/// @tparam T The type to check.
export template <typename T, typename = void>
struct IsCLinqIterable : std::false_type {};

/// The type is iterable if it has 'begin' and 'end'.
/// @tparam T The type to check.
export template <typename T>
struct IsCLinqIterable<T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>> : std::true_type {};

/// Concept for checking if a type is iterable.
/// @tparam T The type to check.
export template <typename T>
concept CLinqIterable = IsCLinqIterable<T>::value;

/// Thrown when an error occurs in the CLinq library.
export class CLinqException final : public std::runtime_error
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
export template <typename TElement>
class CLinqCollection
{
    public:
        using value_type = TElement;
        using size_type = std::vector<TElement>::size_type;

        using iterator = std::vector<TElement>::iterator;
        using const_iterator = std::vector<TElement>::const_iterator;
        using reverse_iterator = std::vector<TElement>::reverse_iterator;

        /// Type alias for a function that maps elements to true or false.
        using MatchFunction = std::function<bool(TElement)>;

        /// Type alias for a function that projects elements to some other type.
        /// @tparam TProjection The type to project the elements to.
        template <typename TProjection>
        using ProjectionFunction = std::function<TProjection(TElement)>;

        /// Initializes a new instance of the CLinqCollection class.
        explicit CLinqCollection() noexcept
            : _elements(std::vector<TElement>())
        {
        }

        /// Initializes a new instance of the CLinqCollection class.
        /// @param elements The initial elements.
        explicit CLinqCollection(std::vector<TElement> const& elements) noexcept
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
        /// @param collection The collection to compare to.
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

        /// Gets a const reference to the first element in the collection.
        /// @returns A const reference to the first element in the collection.
        TElement const& First() const
        {
            ThrowIfEmpty();

            return _elements.front();
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

        void ThrowIfOutOfRange(size_type const index) const
        {
            if (index >= _elements.size())
            {
                throw CLinqException("Index was out of range."
                    "Attemped to access index " + std::to_string(index) +
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