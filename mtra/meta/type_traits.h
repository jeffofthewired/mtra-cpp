#pragma once

#include <cstddef>
#include <utility>

namespace mtra {

struct false_type {
    static constexpr bool value = false;
};

struct true_type {
    static constexpr bool value = true;
};

/// PRIMARY TYPE CATEGORIES

// mtra::is_void
// checks if a type is void

template <typename T> struct is_void : false_type {};
template <> struct is_void<void> : true_type {};

// mtra::is_integral
// checks if a type is an integral type

template <typename T> struct is_integral : false_type {};
template <> struct is_integral<bool> : true_type {};
template <> struct is_integral<char> : true_type {};
template <> struct is_integral<signed char> : true_type {};
template <> struct is_integral<short> : true_type {};
template <> struct is_integral<int> : true_type {};
template <> struct is_integral<long> : true_type {};
template <> struct is_integral<long long> : true_type {};
template <> struct is_integral<unsigned char> : true_type {};
template <> struct is_integral<unsigned short> : true_type {};
template <> struct is_integral<unsigned int> : true_type {};
template <> struct is_integral<unsigned long> : true_type {};
template <> struct is_integral<unsigned long long> : true_type {};

// mtra::is_floating_point
// checks if a type is a floating-point type

template <typename T> struct is_floating_point : false_type {};
template <> struct is_floating_point<float> : true_type {};
template <> struct is_floating_point<double> : true_type {};
template <> struct is_floating_point<long double> : true_type {};

// mtra::is_array
// checks if a type is an array type
template <typename T> struct is_array : false_type {};
template <typename T> struct is_array<T[]> : true_type {};
template <typename T, std::size_t N> struct is_array<T[N]> : true_type {};

// mtra::is_enum
// checks of a type is a enumeration type
// MISSING

// mtra::is_union
// checks of a type is a union type
// MISSING

// mtra::is_class
// checks of a type is a non-union class type
// MISSING

// mtra::is_function
// checks of a type is a function type
// MISSING

// mtra::is_pointer
// checks of a type is a pointer type
// MISSING

// mtra::is_lvalue_reference
// checks if a type is an lvalue reference
template <typename T> struct is_lvalue_reference : false_type {};
template <typename T> struct is_lvalue_reference<T&> : true_type {};

// mtra::is_rvalue_reference
// checks if a type is an rvalue reference
template <typename T> struct is_rvalue_reference : false_type {};
template <typename T> struct is_rvalue_reference<T&&> : true_type {};

// mtra::is_member_object_pointer
// checks of a type is a non-static member object pointer
// MISSING

// mtra::is_member_function_pointer
// checks of a type is a non-static member function pointer
// MISSING

/// COMPOSITE TYPE CATEGORIES
// all missing ...

/// TYPE PROPERTIES

// mtra::is_const
// checks if a type is const-qualified
template <typename T> struct is_const : false_type {};
template <typename T> struct is_const<const T> : true_type {};

// mtra::is_volatile
// checks if a type is volatile-qualified
template <typename T> struct is_volatile : false_type {};
template <typename T> struct is_volatile<const T> : true_type {};

// mtra::is_trivial
// checks of a type is trivial
// MISSING

// mtra::is_trivially_copyable
// checks of a type is trivially copyable
// MISSING

// mtra::is_standard_layout
// checks of a type is a standard-layout type
// MISSING

// mtra::is_pod
// checks of a type is a plain-old data (POD) type
// MISSING

// the rest in this section are also missing...

/// SUPPORTED OPERATIONS

// mtra::is_constructible
// checks if a type has a constructor for specific arguments
template <typename T, typename... Args>
struct is_constructible {
private:
    template <typename U, typename... A, typename = decltype(U(std::declval<A>()...))>
    static auto test(int) -> true_type;

    template <typename, typename...>
    static auto test(...) -> false_type;

public:
    static constexpr bool value = decltype(test<T, Args...>(0))::value;
};
// trivially & nothrow are missing...

// mtra::is_default_constructible
// checks if a type has a default constructor
template <typename T>
struct is_default_constructible : is_constructible<T> {};
// trivially & nothrow are missing...

// mtra::is_copy_constructible
// checks if a type has a copy constructor
template <typename T>
struct is_copy_constructible : is_constructible<T, const T&> {};
// trivially & nothrow are missing...

// mtra::is_move_constructible
// checks if a type has a move constructor
template <typename T>
struct is_move_constructible : is_constructible<T, T&&> {}; 
// trivially & nothrow are missing...

// mtra::is_assignable
// checks if a type has an assignment operator for a specific argument
template <typename T, typename Arg>
struct is_assignable {
private:
    template <
        typename U,
        typename A,
        typename = decltype(std::declval<U>().operator=(std::declval<A>()))
    >
    static auto test(int) -> true_type;

    template <typename, typename>
    static auto test(...) -> false_type;

public:
    static constexpr bool value = decltype(test<T, Arg>(0))::value;
};
// trivially & nothrow are missing

// mtra::is_copy_assignable
// checks if a type has a copy assignment operator
template <typename T>
struct is_copy_assignable : is_assignable<T, const T&> {};

// mtra::is_move_assignable
// checks if a type has a move assignment operator
template <typename T>
struct is_move_assignable : is_assignable<T, T&&> {};

// mtra::is_destructible
// checks if a type has a non-deleted destructor
template <typename T>
struct is_destructible {
private:
    template <typename U, typename = decltype(std::declval<U&>().~U())>
    static auto test(int) -> true_type;

    template <typename>
    static auto test(...) -> false_type;

public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

// mtra::has_virtual_destructor
// checks if a type has a virtual destructor
// MISSING
//
// the remaining traits are also missing...
//
/// PROPERTY QUERIES
// all missing...

/// TYPE RELATIONSHIPS
template <typename A, typename B>
struct is_same : false_type {};
template <typename A>
struct is_same<A, A> : true_type {};

// MISSING: to be continued...

/// TYPE TRANSFORMATIONS

// mtra::remove_cv
// mtra::remove_const
// mtra::remove_volatile
// removes const and/or volatile specifiers from the given type

template <typename T>
struct remove_cv {
    using type = T;
};

template <typename T>
struct remove_cv<const T> {
    using type = T;
};

template <typename T>
struct remove_cv<volatile T> {
    using type = T;
};

template <typename T>
struct remove_cv<const volatile T> {
    using type = T;
};

template <typename T>
struct remove_const {
    using type = T;
};

template <typename T>
struct remove_const<const T> {
    using type = T;
};

template <typename T>
struct remove_volatile {
    using type = T;
};

template <typename T>
struct remove_volatile<volatile T> {
    using type = T;
};

// mtra::add_cv
// mtra::add_const
// mtra::add_volatile
// adds const and/or volatile specifiers to a given type
template <typename T>
struct add_cv {
    using type = const volatile T;
};

template <typename T>
struct add_const {
    using type = const T;
};

template <typename T>
struct add_volatile {
    using type = volatile T;
};

// mtra::remove_reference
// removes a reference from the given type
template <typename T>
struct remove_reference {
    using type = T;
};

template <typename T>
struct remove_reference<T&> {
    using type = T;
};

template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

// mtra::add_lvalue_reference
// just naively chucks an extra & on the end
// may result in reference collapsing?
template <typename T>
struct add_lvalue_reference {
    using type = T&;
};

// mtra::add_rvalue_reference
template <typename T>
struct add_rvalue_reference {
    using type = T&&;
};

// MISSING
// skipping all the sign modifiers...

// mtra::remove_pointer
template <typename T>
struct remove_pointer {
    using type = T;
};

template <typename T>
struct remove_pointer<T *> {
    using type = T;
};

// mtra::add_pointer
template <typename T>
struct add_pointer {
    using type = T*;
};

/// OTHER TRANSFORMATIONS
// skipped the first few

// mtra::decay
// applies type transformations as when passing a function argument by value
// BLOCKER: have to implement is_array and is_function first

// mtra::enable_if
// conditionally removes a function overload or template specialization from overload resolution
template <bool Cond, typename T = void>
struct enable_if { };

template <typename T>
struct enable_if<true, T> {
    using type = T;
};

// mtra::conditional
// chooses one type or another based on compile-time boolean
template <bool Cond, typename T, typename F>
struct conditional {
    using type = F;
};

template <typename T, typename F>
struct conditional<true, T, F> {
    using type = T;
};

// MISSING skipping a few...
// ...

// mtra::result_of
// invoke_result
// deduces the result type of invoking a callable object with a set of arguments
template <typename F, typename... Args>
struct result_of {
    using type = decltype(std::declval<F>()(std::declval<Args>()...));
};

// mtra::void_t

}

