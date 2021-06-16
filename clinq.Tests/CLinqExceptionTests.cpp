/// @file CLinqExceptionTests.cpp
/// Unit tests for the CLinqException type.

import CLinq;

#include "catch.hpp"

SCENARIO("CLinqException can be initialized with a message")
{
    GIVEN("An exception message")
    {
        constexpr auto* message = "Test message";

        WHEN("CLinqException is constructed with message")
        {
            auto exception = CLinqException(message);

            THEN("CLinqException contains expected message")
            {
                REQUIRE(strcmp(message, exception.what()) == 0);
            }
        }
    }
}