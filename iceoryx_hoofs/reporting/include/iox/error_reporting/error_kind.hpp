// Copyright (c) 2023 by Apex.AI Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef IOX_HOOFS_REPORTING_ERROR_REPORTING_ERROR_KIND_HPP
#define IOX_HOOFS_REPORTING_ERROR_REPORTING_ERROR_KIND_HPP

#include <type_traits>

namespace iox
{
namespace er
{

// Tag types for mandatory fatal error categories that always exist.
// They have the suffix "Kind" to allow using the prefix as the actual error type.
// The split between error type and error kind is intentional to allow emitting the same error
// type with a different kind if needed, similar to categorical logging.
//
// In addition, this has the advantage to be more explicit at reporting site instead of hiding
// the information in a function name or the error itself.

struct FatalKind
{
    static constexpr char const* name = "Fatal Error";
};

struct AssertViolationKind
{
    static constexpr char const* name = "Assert Violation";
};

struct EnforceViolationKind
{
    static constexpr char const* name = "EnforceViolation";
};

template <class T>
struct IsFatal : public std::false_type
{
    /// @todo iox-#1032 shouldn't there be a static_assert to prevent using this struct in a generic way without
    /// specialization?
};

// This specialization makes it impossible to specialize them differently elsewhere,
// as this would lead to a compilation error.
// This enforces that these errors are always fatal in the sense that they cause panic and abort.
template <>
struct IsFatal<FatalKind> : public std::true_type
{
};

template <>
struct IsFatal<AssertViolationKind> : public std::true_type
{
};

template <>
struct IsFatal<EnforceViolationKind> : public std::true_type
{
};

// The function syntax is more useful if there is already a value (instead of only a type).
// It must be consistent with the type trait, i.e. yield the same boolean value.
template <class Kind>
bool constexpr isFatal(Kind)
{
    return IsFatal<Kind>::value;
}

template <>
bool constexpr isFatal<FatalKind>(FatalKind)
{
    return IsFatal<FatalKind>::value;
}

template <>
bool constexpr isFatal<AssertViolationKind>(AssertViolationKind)
{
    return IsFatal<AssertViolationKind>::value;
}

template <>
bool constexpr isFatal<EnforceViolationKind>(EnforceViolationKind)
{
    return IsFatal<EnforceViolationKind>::value;
}

// indicates serious condition, unable to continue
constexpr FatalKind FATAL;

// indicates a bug (check only active in debug builds)
constexpr AssertViolationKind ASSERT_VIOLATION;

// indicates a bug (check always active)
constexpr EnforceViolationKind ENFORCE_VIOLATION;

} // namespace er
} // namespace iox

#endif // IOX_HOOFS_REPORTING_ERROR_REPORTING_ERROR_LOGGING_HPP
