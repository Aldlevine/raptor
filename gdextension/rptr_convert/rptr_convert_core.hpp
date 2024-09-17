#pragma once

#include "math/aabb.hpp"
#include "math/vector.hpp"

#include <godot_cpp/variant/variant.hpp>

#include <stddef.h>
#include <string>

#define CHECK_MEMBER_TRIVIAL(TYPE_A, MEMBER_A, TYPE_B, MEMBER_B)               \
    (std::is_same_v<decltype(TYPE_A::MEMBER_A), decltype(TYPE_B::MEMBER_B)> && \
     offsetof(TYPE_A, MEMBER_A) == offsetof(TYPE_B, MEMBER_B))

#define CHECK_MEMBER_STRUCT(TYPE_A, MEMBER_A, TYPE_B, MEMBER_B)                  \
    (types_compatible<decltype(TYPE_A::MEMBER_A), decltype(TYPE_B::MEMBER_B)> && \
     offsetof(TYPE_A, MEMBER_A) == offsetof(TYPE_B, MEMBER_B))

namespace rptr::gdextension
{
    template <typename FromType, typename ToType>
    inline constexpr static bool types_compatible = false;

    template <typename ToType>
    ToType convert(const auto &p_from);

    template <typename ToType>
    inline constexpr const ToType &convert(const auto &p_from)
        requires(types_compatible<std::remove_cvref_t<decltype(p_from)>, ToType>)
    {
        using FromType = std::decay_t<decltype(p_from)>;
        static_assert(sizeof(FromType) == sizeof(ToType));
        static_assert(alignof(FromType) == alignof(ToType));
        static_assert(std::is_trivially_copyable_v<FromType>);
        static_assert(std::is_trivially_copyable_v<ToType>);
        return *reinterpret_cast<const ToType *>(&p_from);
    }

    template <typename ToType>
    inline ToType convert(const auto &p_from)
        requires(
            std::same_as<std::remove_cvref_t<decltype(p_from)>, ToType> ||
            std::convertible_to<std::remove_cvref_t<decltype(p_from)>, ToType>)

    {
        return static_cast<ToType>(p_from);
    }

    // Vector2i

    template <>
    inline constexpr bool types_compatible<rptr::math::Vector2i, godot::Vector2i> = []()
    {
        using A = rptr::math::Vector2i;
        using B = godot::Vector2i;
        return (std::is_trivially_copyable_v<A> &&
                std::is_trivially_copyable_v<B> &&
                sizeof(A) == sizeof(B) &&
                alignof(A) == alignof(B) &&
                CHECK_MEMBER_TRIVIAL(A, x, B, x) &&
                CHECK_MEMBER_TRIVIAL(A, y, B, y));
    }();

    template <>
    inline constexpr bool types_compatible<godot::Vector2i, rptr::math::Vector2i> = types_compatible<rptr::math::Vector2i, godot::Vector2i>;

    // Vector2f

    template <>
    inline constexpr bool types_compatible<rptr::math::Vector2f, godot::Vector2> = []()
    {
        using A = rptr::math::Vector2f;
        using B = godot::Vector2;
        return (CHECK_MEMBER_TRIVIAL(A, x, B, x) &&
                CHECK_MEMBER_TRIVIAL(A, y, B, y));
    }();

    template <>
    inline constexpr bool types_compatible<godot::Vector2, rptr::math::Vector2f> = types_compatible<rptr::math::Vector2f, godot::Vector2>;

    // Vector3i

    template <>
    inline constexpr bool types_compatible<rptr::math::Vector3i, godot::Vector3i> = []()
    {
        using A = rptr::math::Vector3i;
        using B = godot::Vector3i;
        return (CHECK_MEMBER_TRIVIAL(A, x, B, x) &&
                CHECK_MEMBER_TRIVIAL(A, y, B, y) &&
                CHECK_MEMBER_TRIVIAL(A, z, B, z));
    }();

    template <>
    inline constexpr bool types_compatible<godot::Vector3i, rptr::math::Vector3i> = types_compatible<rptr::math::Vector3i, godot::Vector3i>;

    // Vector3f

    template <>
    inline constexpr bool types_compatible<rptr::math::Vector3f, godot::Vector3> = []()
    {
        using A = rptr::math::Vector3f;
        using B = godot::Vector3;
        return (CHECK_MEMBER_TRIVIAL(A, x, B, x) &&
                CHECK_MEMBER_TRIVIAL(A, y, B, y) &&
                CHECK_MEMBER_TRIVIAL(A, z, B, z));
    }();

    template <>
    inline constexpr bool types_compatible<godot::Vector3, rptr::math::Vector3f> = types_compatible<rptr::math::Vector3f, godot::Vector3>;

    // Vector4i

    template <>
    inline constexpr bool types_compatible<rptr::math::Vector4i, godot::Vector4i> = []()
    {
        using A = rptr::math::Vector4i;
        using B = godot::Vector4i;
        return (CHECK_MEMBER_TRIVIAL(A, x, B, x) &&
                CHECK_MEMBER_TRIVIAL(A, y, B, y) &&
                CHECK_MEMBER_TRIVIAL(A, z, B, z) &&
                CHECK_MEMBER_TRIVIAL(A, w, B, w));
    }();

    template <>
    inline constexpr bool types_compatible<godot::Vector4i, rptr::math::Vector4i> = types_compatible<rptr::math::Vector4i, godot::Vector4i>;

    // Vector4f

    template <>
    inline constexpr bool types_compatible<rptr::math::Vector4f, godot::Vector4> = []()
    {
        using A = rptr::math::Vector4f;
        using B = godot::Vector4;
        return (
            CHECK_MEMBER_TRIVIAL(A, x, B, x) &&
            CHECK_MEMBER_TRIVIAL(A, y, B, y) &&
            CHECK_MEMBER_TRIVIAL(A, z, B, z) &&
            CHECK_MEMBER_TRIVIAL(A, w, B, w));
    }();

    template <>
    inline constexpr bool types_compatible<godot::Vector4, rptr::math::Vector4f> = types_compatible<rptr::math::Vector4f, godot::Vector4>;

    template <>
    inline const godot::Vector4 &convert<godot::Vector4>(const rptr::math::Vector4f &p_from)
    {
        static_assert(std::is_trivially_copyable_v<rptr::math::Vector4f>);
        static_assert(std::is_trivially_destructible_v<godot::Vector4>);
        return *reinterpret_cast<const godot::Vector4 *>(&p_from);
    }

    template <>
    inline const rptr::math::Vector4f &convert<rptr::math::Vector4f>(const godot::Vector4 &p_from)
    {
        static_assert(std::is_trivially_destructible_v<godot::Vector4>);
        static_assert(std::is_trivially_copyable_v<rptr::math::Vector4f>);
        return *reinterpret_cast<const rptr::math::Vector4f *>(&p_from);
    }

    // Color

    template <>
    inline constexpr bool types_compatible<rptr::math::Vector4f, godot::Color> = []()
    {
        using A = rptr::math::Vector4f;
        using B = godot::Color;
        return (std::is_trivially_copyable_v<A> &&
                std::is_trivially_copyable_v<B> &&
                sizeof(A) == sizeof(B) &&
                alignof(A) == alignof(B) &&
                CHECK_MEMBER_TRIVIAL(A, x, B, r) &&
                CHECK_MEMBER_TRIVIAL(A, y, B, g) &&
                CHECK_MEMBER_TRIVIAL(A, z, B, b) &&
                CHECK_MEMBER_TRIVIAL(A, w, B, a));
    }();

    template <>
    inline constexpr bool types_compatible<godot::Color, rptr::math::Vector4f> = types_compatible<rptr::math::Vector4f, godot::Color>;

    // AABBf

    template <>
    inline constexpr bool types_compatible<rptr::math::AABBf, godot::AABB> = []()
    {
        using A = rptr::math::AABBf;
        using B = godot::AABB;
        return (CHECK_MEMBER_STRUCT(A, position, B, position) &&
                CHECK_MEMBER_STRUCT(A, size, B, size));
    }();

    template <>
    inline constexpr bool types_compatible<godot::AABB, rptr::math::AABBf> = types_compatible<rptr::math::AABBf, godot::AABB>;

    // Static Tests

    template <typename FromType, typename ToType>
    struct ConvertTest
    {
        static ToType func(const ToType &p_value)
        {
            return ToType{};
        }

        static FromType wrapper_func(const ToType &p_value)
        {
            return convert<FromType>(func(p_value));
        }

        static FromType caller_func(const FromType &p_value)
        {
            return wrapper_func(convert<ToType>(p_value));
        }

        static void test()
        {
            //cppcheck-suppress[unreadVariable];
            const auto _ = caller_func(FromType{});
        }
    };

    void test()
    {
        ConvertTest<godot::Vector2i, rptr::math::Vector2i>::test();
        ConvertTest<rptr::math::Vector2i, godot::Vector2i>::test();
        ConvertTest<godot::Vector2, rptr::math::Vector2f>::test();
        ConvertTest<rptr::math::Vector2f, godot::Vector2>::test();

        ConvertTest<godot::Vector3i, rptr::math::Vector3i>::test();
        ConvertTest<rptr::math::Vector3i, godot::Vector3i>::test();
        ConvertTest<godot::Vector3, rptr::math::Vector3f>::test();
        ConvertTest<rptr::math::Vector3f, godot::Vector3>::test();

        ConvertTest<godot::Vector4i, rptr::math::Vector4i>::test();
        ConvertTest<rptr::math::Vector4i, godot::Vector4i>::test();
        ConvertTest<godot::Vector4, rptr::math::Vector4f>::test();
        ConvertTest<rptr::math::Vector4f, godot::Vector4>::test();

        ConvertTest<godot::Color, rptr::math::Vector4f>::test();
        ConvertTest<rptr::math::Vector4f, godot::Color>::test();

        ConvertTest<godot::AABB, rptr::math::AABBf>::test();
        ConvertTest<rptr::math::AABBf, godot::AABB>::test();
    }

} //namespace rptr::gdextension
