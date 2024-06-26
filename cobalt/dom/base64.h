// Copyright 2018 The Cobalt Authors. All Rights Reserved.
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

#ifndef COBALT_DOM_BASE64_H_
#define COBALT_DOM_BASE64_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/optional.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace cobalt {
namespace dom {

// https://infra.spec.whatwg.org/#forgiving-base64-encode
base::Optional<std::string> ForgivingBase64Encode(
    const std::string& string_to_encode);

// https://infra.spec.whatwg.org/#forgiving-base64-decode
absl::optional<std::vector<uint8_t>> ForgivingBase64Decode(
    const std::string& encoded_string);

}  // namespace dom
}  // namespace cobalt

#endif  // COBALT_DOM_BASE64_H_
