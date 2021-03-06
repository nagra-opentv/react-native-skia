// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdint.h>  // for uintptr_t

#include <tuple>    // for std::tie
#include <utility>  // for std::swap

#include "gen/generated_header.h"

class SomeClass {};
class DerivedClass : public SomeClass {};

struct MyStruct {
  SomeClass* ptr;
  SomeClass* ptr2;
  const SomeClass* const_ptr;
  int (*func_ptr_field)();
};

namespace auto_tests {

MyStruct* GetMyStruct() {
  return nullptr;
}

SomeClass* GetSomeClass() {
  return nullptr;
}

SomeClass* ConvertSomeClassToSomeClass(SomeClass* some_class) {
  return some_class;
}

void foo() {
  MyStruct my_struct;

  // After the rewrite |my_struct.ptr_field| is no longer a pointer,
  // so |auto*| won't work.  We fix this up, by appending |.get()|.
  // Expected rewrite: auto* ptr_var = my_struct.ptr.get();
  auto* ptr_var = my_struct.ptr;

  // Tests for other kinds of initialization.
  // Expected rewrite: |.get()| should be appended in both cases below.
  auto* init_test1(my_struct.ptr);
  auto* init_test2{my_struct.ptr};

  // Test for handling of the |const| qualifier.
  // Expected rewrite: const auto* ptr_var = my_struct.ptr.get();
  const auto* const_ptr_var = my_struct.ptr;

  // More complicated initialization expression, but the |ptr_field| struct
  // member dereference is still the top/last expression here.
  // Expected rewrite: ...->ptr.get()
  auto* complicated_var = GetMyStruct()->ptr;

  // The test below covers:
  // 1. Two variables with single |auto|,
  // 2. Tricky placement of |*| (next to the variable name).
  // Expected rewrite: ...ptr.get()... (twice in the 2nd example).
  auto *ptr_var1 = my_struct.ptr, *ptr_var2 = GetSomeClass();
  auto *ptr_var3 = my_struct.ptr, *ptr_var4 = my_struct.ptr;
  auto *ptr_var5 = GetSomeClass(), *ptr_var6 = my_struct.ptr;

  // Test for the case where
  // 1. The resulting type is the same as in the |ptr_var| and |complicated_var|
  //    examples
  // 2. Deep in the initialization expression there is a member dereference
  //    of |ptr_field|
  // but
  // 3. The final/top-level initialization expression doesn't dereference
  //    |ptr_field|.
  // No rewrite expected.
  auto* not_affected_field_var = ConvertSomeClassToSomeClass(my_struct.ptr);

  // Test for pointer |auto| assigned from non-CheckedPtr-elligible field.
  // No rewrite expected.
  auto* func_ptr_var = my_struct.func_ptr_field;

  // Test for non-pointer |auto| assigned from CheckedPtr-elligible field.
  // No rewrite expected.
  auto non_pointer_auto_var = my_struct.ptr;

  // Test for non-auto pointer.
  // No rewrite expected.
  SomeClass* non_auto_ptr_var = my_struct.ptr;
}

}  // namespace auto_tests

namespace printf_tests {

int ConvertSomeClassToInt(SomeClass* some_class) {
  return 123;
}

void MyPrintf(const char* fmt, ...) {}

void foo() {
  MyStruct s;

  // Expected rewrite: MyPrintf("%p", s.ptr.get());
  MyPrintf("%p", s.ptr);

  // Test - all arguments are rewritten.
  // Expected rewrite: MyPrintf("%p, %p", s.ptr.get(), s.ptr2.get());
  MyPrintf("%p, %p", s.ptr, s.ptr2);

  // Test - only |s.ptr|-style arguments are rewritten.
  // Expected rewrite: MyPrintf("%d, %p", 123, s.ptr.get());
  MyPrintf("%d, %p", 123, s.ptr);

  // Test - |s.ptr| is deeply nested.
  // No rewrite expected.
  MyPrintf("%d", ConvertSomeClassToInt(s.ptr));
}

}  // namespace printf_tests

namespace cast_tests {

void foo() {
  MyStruct my_struct;

  // To get |const_cast<...>(...)| to compile after the rewrite we
  // need to rewrite the casted expression.
  // Expected rewrite: const_cast<SomeClass*>(my_struct.const_ptr.get());
  SomeClass* v = const_cast<SomeClass*>(my_struct.const_ptr);
  // Expected rewrite: const_cast<const SomeClass*>(my_struct.ptr.get());
  const SomeClass* v2 = const_cast<const SomeClass*>(my_struct.ptr);

  // To get |reinterpret_cast<uintptr_t>(...)| to compile after the rewrite we
  // need to rewrite the casted expression.
  // Expected rewrite: reinterpret_cast<uintptr_t>(my_struct.ptr.get());
  uintptr_t u = reinterpret_cast<uintptr_t>(my_struct.ptr);

  // There is no need to append |.get()| inside static_cast - unlike the
  // const_cast and reinterpret_cast examples above, static_cast will compile
  // just fine.
  DerivedClass* d = static_cast<DerivedClass*>(my_struct.ptr);
  void* void_var = static_cast<void*>(my_struct.ptr);
}

}  // namespace cast_tests

namespace ternary_operator_tests {

void foo(int x) {
  MyStruct my_struct;
  SomeClass* other_ptr = nullptr;

  // To avoid the following error type:
  //     conditional expression is ambiguous; 'const CheckedPtr<SomeClass>'
  //     can be converted to 'SomeClass *' and vice versa
  // we need to append |.get()| to |my_struct.ptr| below.
  //
  // Expected rewrite: ... my_struct.ptr.get() ...
  SomeClass* v = (x > 123) ? my_struct.ptr : other_ptr;

  // Rewrite in the other position.
  // Expected rewrite: ... my_struct.ptr.get() ...
  SomeClass* v2 = (x > 456) ? other_ptr : my_struct.ptr;

  // No rewrite is needed for the first, conditional argument.
  // No rewrite expected.
  int v3 = my_struct.ptr ? 123 : 456;

  // Test for 1st and 2nd arg.  Only 2nd arg should be rewritten.
  SomeClass* v4 = my_struct.ptr ? my_struct.ptr : other_ptr;
}

}  // namespace ternary_operator_tests

namespace generated_code_tests {

void MyPrintf(const char* fmt, ...) {}

void foo() {
  GeneratedStruct s;

  // No rewrite expected below (i.e. no |.get()| appended), because the field
  // dereferenced below comes from (simulated) generated code.
  MyPrintf("%p", s.ptr_field);
}

}  // namespace generated_code_tests

namespace templated_functions {

template <typename T>
void AffectedFunction(T* t) {}

template <typename T>
void TemplatedFunction_NonTemplatedParam(SomeClass* arg, T t) {}

template <typename T>
class MyTemplate {
 public:
  template <typename U>
  MyTemplate(U* u) {}

  void AffectedMethod(T* t) {}
};

// We also want to append |.get()| for |T| parameters (i.e. not just for |T*|
// parameters).
//
// One motivating example is ActivityLogDatabasePolicy::ScheduleAndForget which
// passes its argument to base::Unretained.
//
// Another motivating example, is the following pattern from
// //components/variations/service/ui_string_overrider.cc where the type of the
// 2 arguments needs to be kept consistent:
//     const uint32_t* end = ptr_field_ + num_resources_;
//     const uint32_t* element = std::lower_bound(ptr_field_, end, hash);
template <typename T>
void AffectedNonPointerFunction(T t) {}

// AffectedFunctionWithDeepT mimics ConvertPPResourceArrayToObjects from
// //ppapi/cpp/array_output.h
template <typename T>
void AffectedFunctionWithDeepT(MyTemplate<T>* blah) {}

// StructWithPointerToTemplate is used to test AffectedFunctionWithDeepT.
// StructWithPointerToTemplate mimics ResourceArrayOutputAdapter<T>
// (and its |output_| field that will be converted to a CheckedPtr)
// from //ppapi/cpp/array_output.h
template <typename T>
struct StructWithPointerToTemplate {
  MyTemplate<T>* ptr_to_template;
};

void foo() {
  MyStruct my_struct;

  // Expected rewrite - appending: .get()
  AffectedFunction(my_struct.ptr);

  // Expected rewrite - appending: .get()
  MyTemplate<SomeClass> mt(my_struct.ptr);
  // Expected rewrite - appending: .get()
  mt.AffectedMethod(my_struct.ptr);

  // No rewrite expected.
  TemplatedFunction_NonTemplatedParam(my_struct.ptr, 123);

  // Expected rewrite - appending: .get()
  AffectedNonPointerFunction(my_struct.ptr);

  // Expected rewrite - appending: .get()
  StructWithPointerToTemplate<SomeClass> swptt;
  AffectedFunctionWithDeepT(swptt.ptr_to_template);

  // No rewrite expected - T& parameter.
  std::swap(my_struct.ptr, my_struct.ptr2);
  std::tie(my_struct.ptr, my_struct.ptr2) = std::make_pair(nullptr, nullptr);
}

}  // namespace templated_functions

namespace affected_implicit_template_specialization {

template <typename T, typename T2>
struct MyTemplate {
  T* t_ptr;
  T2* t2_ptr;

  struct NestedStruct {
    SomeClass* nested_ptr_field;
    T* nested_t_ptr_field;
  };
  NestedStruct nested_struct_field;
};

template <typename T3>
struct MyTemplate<SomeClass, T3> {
  SomeClass* some_ptr;
  T3* t3_ptr;
};

// The example that forces explicit |isAnonymousStructOrUnion| checks in
// the implementation of GetExplicitDecl.  The example is based on
// buildtools/third_party/libc++/trunk/include/string.
template <typename T>
struct MyStringTemplate {
  struct NestedStruct {
    union {
      long l;
      short s;
      T* t_ptr;
      int* i_ptr;
    };  // Unnamed / anonymous union *field*.

    struct {
      long l2;
      short s2;
      T* t_ptr2;
      int* i_ptr2;
    };  // Unnamed / anonymous struct *field*.
  };
  NestedStruct s;
};

void MyPrintf(const char* fmt, ...) {}

void foo() {
  // |s.t_ptr| comes from implicit template specialization (which needs to be
  // skipped for rewriting, but should be included for appending |.get()|).
  //
  // Expected rewrite: MyPrintf("%p", s.t_ptr.get());
  MyTemplate<int, int> s;
  MyPrintf("%p", s.t_ptr);

  // |s.some_ptr| and |s.t2_ptr| come from implicit template specialization or a
  // partial template specialization.
  //
  // Expected rewrite: MyPrintf("%p", s.some_ptr.get(), s.t3_ptr.get());
  MyTemplate<SomeClass, int> s2;
  MyPrintf("%p %p", s2.some_ptr, s2.t3_ptr);

  // Nested structs require extra care when trying to look up the non-implicit
  // field definition.  Expected rewrite: adding |.get()| suffix.
  MyPrintf("%p", s.nested_struct_field.nested_ptr_field);
  MyPrintf("%p", s.nested_struct_field.nested_t_ptr_field);

  // Lines below are added mainly to Force implicit specialization of
  // MyStringTemplate (to force explicit |isAnonymousStructOrUnion| checks in
  // the rewriter).  Still, the expected rewrite is: appending |.get()| to the
  // printf arg.
  MyStringTemplate<void> mst;
  MyPrintf("%p %p", mst.s.t_ptr, mst.s.t_ptr2);
}

}  // namespace affected_implicit_template_specialization
