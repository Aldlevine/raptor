#pragma once

#include "vector.hpp"

#include "repr/repr.hpp"

namespace rptr::math
{
    template <typename Element>
    struct AABB
    {
        using This = AABB<Element>;

        Element position;
        Element size;

        constexpr AABB() = default;

        constexpr AABB(const Element &p_position, const Element &p_size) :
            position(p_position), size(p_size)
        {
        }

        template <typename OtherElement>
        constexpr AABB(const AABB<OtherElement> &p_other) : // NOLINT(*-explicit-constructor)
            position(p_other.position), size(p_other.size)
        {
        }

        constexpr bool operator==(const This &p_other) const
        {
            return position == p_other.position && size == p_other.size;
        }

        constexpr bool operator!=(const This &p_other) const
        {
            return !(*this == p_other);
        }

        constexpr Element get_end() const
        {
            return position + size;
        }

        constexpr This intersection(const This &p_aabb) const
        {
            Element src_min = position;
            Element src_max = position + size;
            Element dst_min = p_aabb.position;
            Element dst_max = p_aabb.position + p_aabb.size;

            Element min, max;

            if (src_min.x > dst_max.x || src_max.x < dst_min.x)
            {
                return This{};
            }
            else
            {
                min.x = (src_min.x > dst_min.x) ? src_min.x : dst_min.x;
                max.x = (src_max.x < dst_max.x) ? src_max.x : dst_max.x;
            }

            if (src_min.y > dst_max.y || src_max.y < dst_min.y)
            {
                return This{};
            }
            else
            {
                min.y = (src_min.y > dst_min.y) ? src_min.y : dst_min.y;
                max.y = (src_max.y < dst_max.y) ? src_max.y : dst_max.y;
            }

            if (src_min.z > dst_max.z || src_max.z < dst_min.z)
            {
                return This{};
            }
            else
            {
                min.z = (src_min.z > dst_min.z) ? src_min.z : dst_min.z;
                max.z = (src_max.z < dst_max.z) ? src_max.z : dst_max.z;
            }

            return This{ min, max - min };
        }

        constexpr bool encloses(const This &p_aabb) const
        {
            const Element &src_min = position;
            const Element &src_max = position + size;
            const Element &dst_min = p_aabb.position;
            const Element &dst_max = p_aabb.position + p_aabb.size;

            return ((src_min.x <= dst_min.x) && (src_max.x >= dst_max.x) &&
                    (src_min.y <= dst_min.y) && (src_max.y >= dst_max.y) &&
                    (src_min.z <= dst_min.z) && (src_max.z >= dst_max.z));
        }

        constexpr bool has_point(const Element &p_point) const
        {
            const Element &begin = position;
            const Element &end = position + size;
            return ((p_point.x >= begin.x && p_point.x < end.x) &&
                    (p_point.y >= begin.y && p_point.y < end.y) &&
                    (p_point.z >= begin.z && p_point.z < end.z));
        }
    };

    using AABBi = AABB<Vector3i>;
    using AABBf = AABB<Vector3f>;

} // namespace rptr::math

namespace rptr::repr
{
    template <typename Element>
    inline Repr &to_repr(const math::AABB<Element> &p_aabb, Repr &p_repr)
    {
        return p_repr
            .push_scope("AABB")
            .push_field("position", p_aabb.position)
            .push_field("size", p_aabb.size)
            .push_field("end", p_aabb.get_end())
            .pop_scope();
    }
} // namespace rptr::repr
