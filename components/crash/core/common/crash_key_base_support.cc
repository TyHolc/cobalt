// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/crash/core/common/crash_key_base_support.h"

#include <memory>

#include "base/debug/crash_logging.h"
#include "components/crash/core/common/crash_key.h"

namespace crash_reporter {

namespace {

// This stores the value for a crash key allocated through the //base API.
template <uint32_t ValueSize>
struct BaseCrashKeyString : public base::debug::CrashKeyString {
  BaseCrashKeyString(const char name[], base::debug::CrashKeySize size)
      : base::debug::CrashKeyString(name, size), impl(name) {
    DCHECK_EQ(static_cast<uint32_t>(size), ValueSize);
  }
  crash_reporter::CrashKeyString<ValueSize> impl;
};

#define SIZE_CLASS_OPERATION(size_class, operation_prefix, operation_suffix) \
  switch (size_class) {                                                      \
    case base::debug::CrashKeySize::Size32:                                  \
      operation_prefix BaseCrashKeyString<32> operation_suffix;              \
      break;                                                                 \
    case base::debug::CrashKeySize::Size64:                                  \
      operation_prefix BaseCrashKeyString<64> operation_suffix;              \
      break;                                                                 \
    case base::debug::CrashKeySize::Size256:                                 \
      operation_prefix BaseCrashKeyString<256> operation_suffix;             \
      break;                                                                 \
    case base::debug::CrashKeySize::Size1024:                                \
      operation_prefix BaseCrashKeyString<1024> operation_suffix;            \
      break;                                                                       \
  }

class CrashKeyBaseSupport : public base::debug::CrashKeyImplementation {
 public:
  CrashKeyBaseSupport() = default;

  ~CrashKeyBaseSupport() override = default;

  base::debug::CrashKeyString* Allocate(
      const char name[],
      base::debug::CrashKeySize size) override {
    SIZE_CLASS_OPERATION(size, return new, (name, size));
    return nullptr;
  }

  void Set(base::debug::CrashKeyString* crash_key,
           base::StringPiece value) override {
    SIZE_CLASS_OPERATION(crash_key->size,
                         reinterpret_cast<, *>(crash_key)->impl.Set(value));
  }

  void Clear(base::debug::CrashKeyString* crash_key) override {
    SIZE_CLASS_OPERATION(crash_key->size,
                         reinterpret_cast<, *>(crash_key)->impl.Clear());
  }

  void OutputCrashKeysToStream(std::ostream& out) override {
#if !defined(STARBOARD)
    // TODO(lukasza): If phasing out breakpad takes a long time, then consider
    // a better way to abstract away difference between crashpad and breakpad.
    // For example, maybe the code below should be moved into
    // third_party/crashpad/crashpad/client and exposed (in an abstract,
    // implementation-agnostic way) via CrashKeyString.  This would allow
    // avoiding using the BUILDFLAG(...) macros here.

    auto* annotations = crashpad::AnnotationList::Get();
    if (!annotations || annotations->begin() == annotations->end())
      return;

    out << "Crash keys:\n";
    for (const crashpad::Annotation* annotation : *annotations) {
      if (!annotation->is_set())
        continue;

      if (annotation->type() != crashpad::Annotation::Type::kString)
        continue;
      base::StringPiece value(static_cast<const char*>(annotation->value()),
                              annotation->size());

      out << "  \"" << annotation->name() << "\" = \"" << value << "\"\n";
    }
#endif
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CrashKeyBaseSupport);
};

#undef SIZE_CLASS_OPERATION

}  // namespace

void InitializeCrashKeyBaseSupport() {
  base::debug::SetCrashKeyImplementation(
      std::make_unique<CrashKeyBaseSupport>());
}

}  // namespace crash_reporter
