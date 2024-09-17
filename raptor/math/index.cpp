#include "index.hpp"

namespace rptr::math
{
    size_t linearize_index(const Vector3i &p_size, const Vector3i &p_index)
    {
        return ((p_index.x) + (p_index.y * p_size.x) +
                (p_index.z * p_size.x * p_size.y));
    }

    int linearize_index(const AABBi &p_bounds, const Vector3i &p_index)
    {
        return linearize_index(p_bounds.size, p_index - p_bounds.position);
    }

    Vector3i vectorize_index(const Vector3i &p_size, size_t p_index)
    {
        Vector3i offset;
        offset.x = p_index % p_size.x;
        offset.y = (p_index / p_size.x) % p_size.y;
        offset.z = p_index / (p_size.x * p_size.y);
        return offset;
    }

    Vector3i vectorize_index(const AABBi &p_bounds, int p_index)
    {
        return p_bounds.position + vectorize_index(p_bounds.size, p_index);
    }

} // namespace rptr::math
