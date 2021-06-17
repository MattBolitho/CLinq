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
        explicit CLinqCollection(TElement* const memory, size_type numberOfElements) noexcept
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
};