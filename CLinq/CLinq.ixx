/// @file CLinq.ixx
/// Defines the CLinq module interface.

module;
#include <cstdint>
#include <string>
#include <vector>
#include <concepts>
#include <type_traits>
#include <stdexcept>
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
        using size_type = int32_t;
        using value_type = TElement;

        using iterator = std::vector<TElement>::iterator;
        using const_iterator = std::vector<TElement>::const_iterator;
        using reverse_iterator = std::vector<TElement>::reverse_iterator;

        /// Initializes a new instance of the CLinqCollection class.
        /// @param elements The initial elements.
        explicit CLinqCollection(std::vector<TElement> const& elements) noexcept
            : _elements(elements)
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

    private:
        std::vector<TElement> _elements;
};