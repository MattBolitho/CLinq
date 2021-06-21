# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [🔖 [0.2.1]](https://github.com/MattBolitho/CLinq/releases/tag/CLinq-0.2) - 21/06/2021
### ✨ Added
- 

### 🙌 Improvements
- Improves Visual Studio solution format.


## [🔖 [0.2.0]](https://github.com/MattBolitho/CLinq/releases/tag/CLinq-0.2) - 17/06/2021
### ✨ Added
- Adds casting of element types.
- `Contains`, `Distinct`, `Except`, `Single` and `Reverse` methods.
- `Skip`, `SkipLast`, `SkipWhile`, `Take`, `TakeLast`, `TakeWhile` methods.
- Set operations `Union` and `Intersection`.

### 🙌 Improvements
- `CLinqCollection<TElement>` can now be constructed with initializer list.

## [🔖 [0.1.0]](https://github.com/MattBolitho/CLinq/releases/tag/CLinq-0.1) - 17/06/2021
### ✨ Added
- Unit test project using Catch2.
- Adds initial `CLinqCollection<T>` containing:
    - `const` and non `const` iterators.
    - Operators for comparisons, indexing and concatenation.
    - Positional access methods `At`, `First`, `Last`.
    - Predicate methods `Any` and `All`.
    - Collection expansion methods `Concat`, `Append` and `Prepend`.
    - Generator static methods `Range` and `Repeat`.
    - STL container methods `ToVector`, `ToList`, `ToSet`, `ToMap`, `ToUnorderedMap`.
    - Query methods `Select` and `Where`.
