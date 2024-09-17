#pragma once

#include "repr/repr.hpp"

#include <stdint.h>
#include <cmath>
#include <cstdint>
#include <format>

#define VECTOR_FOLD(...) /*************************/ \
    [&]<size_t... I>(std::index_sequence<I...>)      \
    {                                                \
        __VA_ARGS__                                  \
    }(std::make_index_sequence<Size>{});

namespace rptr::math
{

    namespace impl
    {
        template <typename Element, size_t Size>
        struct VectorMembers
        {
        };

        template <typename Element, size_t Size>
            requires(Size == 1)
        struct VectorMembers<Element, Size>
        {
            union
            {
                struct
                {
                    Element x;
                };
                Element v[Size]{};
            };
        };

        template <typename Element, size_t Size>
            requires(Size == 2)
        struct VectorMembers<Element, Size>
        {
            union
            {
                struct
                {
                    Element x;
                    Element y;
                };
                Element v[Size]{};
            };
        };

        template <typename Element, size_t Size>
            requires(Size == 3)
        struct VectorMembers<Element, Size>
        {
            union
            {
                struct
                {
                    Element x;
                    Element y;
                    Element z;
                };
                Element v[Size]{};
            };
        };

        template <typename Element, size_t Size>
            requires(Size >= 4)
        struct VectorMembers<Element, Size>
        {
            union
            {
                struct
                {
                    Element x;
                    Element y;
                    Element z;
                    Element w;
                };
                Element v[Size]{};
            };
        };

    } // namespace impl

    template <typename Element, size_t Size>
    struct Vector : public impl::VectorMembers<Element, Size>
    {
        using This = Vector<Element, Size>;

        constexpr Vector() = default;

        constexpr Vector(const Vector &) = default;
        This &operator=(const This &) = default;

        constexpr Vector(Vector &&) = default;
        This &operator=(This &&) = default;

        template <typename OtherElement>
        // cppcheck-suppress[noExplicitConstructor]; implicit conversion
        constexpr Vector(const Vector<OtherElement, Size> &p_other)
        {
            VECTOR_FOLD(
                ((this->v[I] = p_other.v[I]), ...); //
            );
        }

        template <typename... Elements>
        constexpr Vector(Elements... p_v)
            requires(sizeof...(Elements) == Size)
        {
            VECTOR_FOLD(
                ((this->v[I] = p_v), ...); //
            );
        }

        constexpr explicit Vector(Element p_all)
        {
            VECTOR_FOLD(
                ((this->v[I] = p_all), ...); //
            );
        }

        constexpr const Element &operator[](size_t p_index) const
        {
            return this->v[p_index];
        }

        constexpr Element &operator[](size_t p_index)
        {
            return this->v[p_index];
        }

        constexpr bool operator==(const This &p_other) const
        {
            return VECTOR_FOLD(
                return ((this->v[I] == p_other.v[I]) && ...); //
            );
        }

        constexpr bool operator!=(const This &p_other) const
        {
            return !(*this == p_other);
        }

        constexpr This operator*(const This &p_other) const
        {
            return VECTOR_FOLD(
                return This{ this->v[I] * p_other.v[I]... }; //
            );
        }

        constexpr This operator*(const Element &p_other) const
        {
            return VECTOR_FOLD(
                return This{ this->v[I] * p_other... }; //
            );
        }

        constexpr This operator/(const This &p_other) const
        {
            return VECTOR_FOLD(
                return This{ this->v[I] / p_other.v[I]... }; //
            );
        }

        constexpr This operator/(const Element &p_other) const
        {
            return VECTOR_FOLD(
                return This{ this->v[I] / p_other... }; //
            );
        }

        constexpr This operator%(const This &p_other) const
        {
            return VECTOR_FOLD(
                return This{ this->v[I] % p_other.v[I]... }; //
            );
        }

        constexpr This operator%(const Element &p_other) const
        {
            return VECTOR_FOLD(
                return This{ this->v[I] % p_other... }; //
            );
        }

        constexpr This operator+(const This &p_other) const
        {
            return VECTOR_FOLD(
                return This{ this->v[I] + p_other.v[I]... }; //
            );
        }

        constexpr This operator+(const Element &p_other) const
        {
            return VECTOR_FOLD(
                return This{ this->v[I] + p_other... }; //
            );
        }

        constexpr This operator-(const This &p_other) const
        {
            return VECTOR_FOLD(
                return This{ this->v[I] - p_other.v[I]... }; //
            );
        }

        constexpr This operator-(const Element &p_other) const
        {
            return VECTOR_FOLD(
                return This{ this->v[I] - p_other... }; //
            );
        }

        constexpr This operator-() const
        {
            return VECTOR_FOLD(
                return This{ -this->v[I]... }; //
            );
        }

        constexpr Element product() const
        {
            return VECTOR_FOLD(
                return ((this->v[I]) * ...); //
            );
        }

        struct Hash
        {
            size_t operator()(const Vector<Element, Size> &p_vec) const
            {
                return VECTOR_FOLD(
                    return ((std::hash<Element>{}(p_vec.v[I]) << I) ^ ...); //
                );
            }
        };
    };

    template <typename Element, size_t Size>
    inline constexpr Vector<Element, Size> element_min(const Vector<Element, Size> &p_a, const Vector<Element, Size> &p_b)
    {
        return VECTOR_FOLD(
            return Vector<Element, Size>{ std::min(p_a.v[I], p_b.v[I])... }; //
        );
    }

    template <typename Element, size_t Size>
    inline constexpr Vector<Element, Size> element_max(const Vector<Element, Size> &p_a, const Vector<Element, Size> &p_b)
    {
        return VECTOR_FOLD(
            return Vector<Element, Size>{ std::max(p_a.v[I], p_b.v[I])... }; //
        );
    }

    template <typename Element, size_t Size>
    inline constexpr Element dot(const Vector<Element, Size> &p_a, const Vector<Element, Size> &p_b)
    {
        return VECTOR_FOLD(
            return ((p_a[I] * p_b[I]) + ...); //
        );
    }

    using Vector2u8 = Vector<uint8_t, 2>;
    using Vector3u8 = Vector<uint8_t, 3>;
    using Vector4u8 = Vector<uint8_t, 4>;
    using Vector2i8 = Vector<int8_t, 2>;
    using Vector3i8 = Vector<int8_t, 3>;
    using Vector4i8 = Vector<int8_t, 4>;

    using Vector2u16 = Vector<uint16_t, 2>;
    using Vector3u16 = Vector<uint16_t, 3>;
    using Vector4u16 = Vector<uint16_t, 4>;
    using Vector2i16 = Vector<int16_t, 2>;
    using Vector3i16 = Vector<int16_t, 3>;
    using Vector4i16 = Vector<int16_t, 4>;

    using Vector2u32 = Vector<uint32_t, 2>;
    using Vector3u32 = Vector<uint32_t, 3>;
    using Vector4u32 = Vector<uint32_t, 4>;
    using Vector2i32 = Vector<int32_t, 2>;
    using Vector3i32 = Vector<int32_t, 3>;
    using Vector4i32 = Vector<int32_t, 4>;

    using Vector2u64 = Vector<uint64_t, 2>;
    using Vector3u64 = Vector<uint64_t, 3>;
    using Vector4u64 = Vector<uint64_t, 4>;
    using Vector2i64 = Vector<int64_t, 2>;
    using Vector3i64 = Vector<int64_t, 3>;
    using Vector4i64 = Vector<int64_t, 4>;

    using Vector2f32 = Vector<float_t, 2>;
    using Vector3f32 = Vector<float_t, 3>;
    using Vector4f32 = Vector<float_t, 4>;

    using Vector2f64 = Vector<double_t, 2>;
    using Vector3f64 = Vector<double_t, 3>;
    using Vector4f64 = Vector<double_t, 4>;

    using Vector2i = Vector2i32;
    using Vector2f = Vector2f32;
    using Vector3i = Vector3i32;
    using Vector3f = Vector3f32;
    using Vector4i = Vector4i32;
    using Vector4f = Vector4f32;

} // namespace rptr::math

namespace rptr::repr
{
    template <typename Element, size_t Size>
    inline Repr &to_repr(const math::Vector<Element, Size> &p_vec, Repr &p_repr)
    {
        constexpr auto suffix = std::integral<Element> ? "i" : std::floating_point<Element> ? "f"
                                                                                            : "";
        p_repr.push_scope("Vector" + std::to_string(Size) + suffix);

        // cppcheck-suppress[syntaxError]; False positive
        constexpr auto get_field_name = [&]<size_t I>(std::index_sequence<I> i)
        {
            if constexpr (Size <= 4)
            {
                return std::string(1, "xyzw"[I]);
            }
            return std::to_string(I);
        };

        VECTOR_FOLD(
            (p_repr.push_field(get_field_name(std::index_sequence<I>()), std::to_string(p_vec.v[I])), ...); //
        );

        return p_repr.pop_scope();
    }

} // namespace rptr::repr

namespace std
{
    template <typename Element, size_t Size>
    inline constexpr ::rptr::math::Vector<Element, Size> ceil(const ::rptr::math::Vector<Element, Size> &p_vector)
    {
        return VECTOR_FOLD(
            return ::rptr::math::Vector<Element, Size>{ std::ceil(p_vector.v[I])... }; //
        );
    }

    template <typename Element, size_t Size>
    inline constexpr ::rptr::math::Vector<Element, Size> floor(const ::rptr::math::Vector<Element, Size> &p_vector)
    {
        return VECTOR_FOLD(
            return ::rptr::math::Vector<Element, Size>{ std::floor(p_vector.v[I])... }; //
        );
    }

    template <typename Element, size_t Size>
    inline constexpr ::rptr::math::Vector<Element, Size> round(const ::rptr::math::Vector<Element, Size> &p_vector)
    {
        return VECTOR_FOLD(
            return ::rptr::math::Vector<Element, Size>{ std::round(p_vector.v[I])... }; //
        );
    }

    template <typename Element, size_t Size>
    inline constexpr ::rptr::math::Vector<Element, Size> abs(const ::rptr::math::Vector<Element, Size> &p_vector)
    {
        return VECTOR_FOLD(
            return ::rptr::math::Vector<Element, Size>{ std::abs(p_vector.v[I])... }; //
        );
    }

    template <typename Element>
    inline constexpr Element signum(const Element &p_element)
    {
        if (p_element > 0)
        {
            return Element{ 1 };
        }
        if (p_element < 0)
        {
            return Element{ -1 };
        }
        return p_element;
    }

    template <typename Element, size_t Size>
    inline constexpr ::rptr::math::Vector<Element, Size> signum(const ::rptr::math::Vector<Element, Size> &p_vector)
    {
        return VECTOR_FOLD(
            return ::rptr::math::Vector<Element, Size>{ std::signum(p_vector.v[I])... }; //
        );
    }

} //namespace std

#undef VECTOR_FOLD