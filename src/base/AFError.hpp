/*
 * This source file is part of ARK
 * For the latest info, see https://github.com/ArkNX
 *
 * Copyright (c) 2013-2019 ArkNX authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"),
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#pragma once

#include "base/AFPlatform.hpp"
#include "base/AFSingleton.hpp"

namespace ark {

// will be used for internal errors
class AFErrorCategory : public std::error_category, public AFSingleton<AFErrorCategory>
{
public:
    const char* name() const noexcept override
    {
        return "ark_error";
    }

    std::string message(int err_val) const override
    {
        switch (err_val)
        {
            case -1:
                return "first error";
            case -2:
                return "second error";
            default:
                return "unknown error";
                break;
        }

        return "";
    }

    static std::error_code MakeErrorCode(int code)
    {
        return std::error_code(code, AFErrorCategory::get_instance());
    }
};

} // namespace ark
