#pragma once

#include "rptr_voxel/rptr_voxel.hpp"

#include "grid/grid.hpp"
#include "math/math.hpp"

#include <variant>

namespace rptr::gdextension
{
    enum class RptrGridType
    {
        NONE,
        UINT8,
        UINT16,
        UINT32,
        UINT64,
        INT8,
        INT16,
        INT32,
        INT64,
        FLOAT32,
        FLOAT64,
        VECTOR2I,
        VECTOR2F,
        VECTOR3I,
        VECTOR3F,
        VECTOR4I,
        VECTOR4F,
        UINT8_VOXEL,
        UINT16_VOXEL,
        UINT32_VOXEL,
        UINT64_VOXEL,
        INT8_VOXEL,
        INT16_VOXEL,
        INT32_VOXEL,
        INT64_VOXEL,
        FLOAT32_VOXEL,
        FLOAT64_VOXEL,
        VECTOR2I_VOXEL,
        VECTOR2F_VOXEL,
        VECTOR3I_VOXEL,
        VECTOR3F_VOXEL,
        VECTOR4I_VOXEL,
        VECTOR4F_VOXEL,
    };

    using RptrGridVariant = std::variant<
        std::monostate,
        grid::Grid<uint8_t>,
        grid::Grid<uint16_t>,
        grid::Grid<uint32_t>,
        grid::Grid<uint64_t>,
        grid::Grid<int8_t>,
        grid::Grid<int16_t>,
        grid::Grid<int32_t>,
        grid::Grid<int64_t>,
        grid::Grid<float_t>,
        grid::Grid<double_t>,
        grid::Grid<math::Vector2i>,
        grid::Grid<math::Vector2f>,
        grid::Grid<math::Vector3i>,
        grid::Grid<math::Vector3f>,
        grid::Grid<math::Vector4i>,
        grid::Grid<math::Vector4f>,
        grid::Grid<RptrVoxel<uint8_t>>,
        grid::Grid<RptrVoxel<uint16_t>>,
        grid::Grid<RptrVoxel<uint32_t>>,
        grid::Grid<RptrVoxel<uint64_t>>,
        grid::Grid<RptrVoxel<int8_t>>,
        grid::Grid<RptrVoxel<int16_t>>,
        grid::Grid<RptrVoxel<int32_t>>,
        grid::Grid<RptrVoxel<int64_t>>,
        grid::Grid<RptrVoxel<float_t>>,
        grid::Grid<RptrVoxel<double_t>>,
        grid::Grid<RptrVoxel<math::Vector2i>>,
        grid::Grid<RptrVoxel<math::Vector2f>>,
        grid::Grid<RptrVoxel<math::Vector3i>>,
        grid::Grid<RptrVoxel<math::Vector3f>>,
        grid::Grid<RptrVoxel<math::Vector4i>>,
        grid::Grid<RptrVoxel<math::Vector4f>>>;
} // namespace rptr::gdextension