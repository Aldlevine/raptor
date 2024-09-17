#pragma once

#include "math/math.hpp"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace rptr::gdextension
{
    template <typename RptrType>
    struct RptrVoxel
    {
        std::array<RptrType, 6> faces;
    };

    using RptrByteVoxel = RptrVoxel<uint8_t>;
    using RptrInt32Voxel = RptrVoxel<int32_t>;
    using RptrInt64Voxel = RptrVoxel<int64_t>;
    using RptrFloat32Voxel = RptrVoxel<float_t>;
    using RptrFloat64Voxel = RptrVoxel<double_t>;
    using RptrVector2fVoxel = RptrVoxel<math::Vector2f>;
    using RptrVector3fVoxel = RptrVoxel<math::Vector3f>;
    using RptrVector4fVoxel = RptrVoxel<math::Vector4f>;
    using RptrColorVoxel = RptrVoxel<math::Vector4f>;

} // namespace rptr::gdextension
