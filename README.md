# CLinq
<div style="display: inline;">
<img src="https://img.shields.io/badge/version-0.2.1-blue"/>
<img src="https://img.shields.io/badge/license-MIT-brightgreen"/>
<img src="https://img.shields.io/badge/c%2B%2B-20-orange"/>
</div>

CLinq is a project containing functionality similar to that of .NET's `System.Linq` namespace.

## Getting Started
CLinq is consumable as a single header or a C++20 module.
The header can be found in the `include` directory and the module in the `module` directory.

The API aims to be analagous with Linq, so [the .NET Linq docs](https://docs.microsoft.com/en-us/dotnet/api/system.linq "Linq link") can be used as a quick reference.

## Future Work
- Add remaining Linq methods.
- Consider adding lazy evaluation.

## Acknowledgements
CLinq is based on .NET's [`System.Linq`](https://docs.microsoft.com/en-us/dotnet/api/system.linq "Linq link") functionality.

[Catch2](https://github.com/catchorg/Catch2 "Catch2 repository") is used for testing.