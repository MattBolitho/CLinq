# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## 🔖 [0.1.0] - Unreleased
### ✨ Added
- Unit test project using Catch2.
- Adds `CLinqCollection<T>` containing:
    - `const` and non `const` iterators.
    - Operators for comparisons, indexing and concatenation.
    - Positional access methods `At`, `First`, `Last`.
    - Predicate methods `Any` and `All`.
    - Collection expansion methods `Concat`, `Append` and `Prepend`.
    - Generator static methods `Range` and `Repeat`.
    - STL container methods `ToVector`, `ToList`, `ToSet`, `ToMap`, `ToUnorderedMap`.