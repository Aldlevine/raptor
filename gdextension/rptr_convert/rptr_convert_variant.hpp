#pragma once

#include "rptr_convert_core.hpp"

#include "rptr_voxel/rptr_voxel.hpp"

namespace rptr::gdextension
{
    template <typename RptrType>
    struct RptrTypeToGodotTypeImpl
    {
        using GodotType = RptrType;
    };

#define MAKE_RPTR_TYPE_TO_GODOT_TYPE(p_rptr_type, p_gd_type) \
    template <>                                              \
    struct RptrTypeToGodotTypeImpl<p_rptr_type>              \
    {                                                        \
        using GodotType = p_gd_type;                         \
    }

    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector2u8, godot::Vector2i);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector3u8, godot::Vector3i);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector4u8, godot::Vector4i);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector2u16, godot::Vector2i);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector3u16, godot::Vector3i);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector4u16, godot::Vector4i);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector2u32, godot::Vector2i);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector3u32, godot::Vector3i);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector4u32, godot::Vector4i);

    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector2i8, godot::Vector2i);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector3i8, godot::Vector3i);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector4i8, godot::Vector4i);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector2i16, godot::Vector2i);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector3i16, godot::Vector3i);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector4i16, godot::Vector4i);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector2i32, godot::Vector2i);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector3i32, godot::Vector3i);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector4i32, godot::Vector4i);

    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector2f32, godot::Vector2);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector3f32, godot::Vector3);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(math::Vector4f32, godot::Vector4);

    MAKE_RPTR_TYPE_TO_GODOT_TYPE(RptrVoxel<uint8_t>, godot::PackedByteArray);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(RptrVoxel<uint16_t>, godot::PackedInt32Array);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(RptrVoxel<uint32_t>, godot::PackedInt64Array);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(RptrVoxel<uint64_t>, godot::PackedInt64Array);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(RptrVoxel<int8_t>, godot::PackedInt32Array);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(RptrVoxel<int16_t>, godot::PackedInt32Array);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(RptrVoxel<int32_t>, godot::PackedInt32Array);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(RptrVoxel<int64_t>, godot::PackedInt64Array);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(RptrVoxel<float_t>, godot::PackedFloat32Array);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(RptrVoxel<double_t>, godot::PackedFloat64Array);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(RptrVoxel<math::Vector2i>, godot::PackedVector2Array);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(RptrVoxel<math::Vector2f>, godot::PackedVector2Array);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(RptrVoxel<math::Vector3i>, godot::PackedVector3Array);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(RptrVoxel<math::Vector3f>, godot::PackedVector3Array);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(RptrVoxel<math::Vector4i>, godot::PackedVector4Array);
    MAKE_RPTR_TYPE_TO_GODOT_TYPE(RptrVoxel<math::Vector4f>, godot::PackedVector4Array);

#undef MAKE_RPTR_TYPE_TO_GODOT_TYPE

    template <typename RptrType>
    using RptrTypeToGodotType = RptrTypeToGodotTypeImpl<RptrType>::GodotType;

    template <typename FromType, typename ToType>
    constexpr bool is_voxel_direct_convert_to_gd = false;
    template <typename FromType, typename ToType>
    constexpr bool is_voxel_direct_convert_to_rptr = false;

    template <typename ToType>
    ToType convert(const auto &p_from)
        requires is_voxel_direct_convert_to_gd<std::remove_cvref_t<decltype(p_from)>, ToType>
    {
        /// TODO: Need to verify the validity of this operation
        using RptrType = std::remove_cvref_t<decltype(p_from)>;
        ToType result{};
        result.resize(6);
        memcpy(result.ptrw(), &p_from, sizeof(RptrType));
        return result;
    }

    template <typename ToType>
    ToType convert(const auto &p_from)
        requires is_voxel_direct_convert_to_rptr<std::remove_cvref_t<decltype(p_from)>, ToType>
    {
        /// TODO: Need to verify the validity of this operation

        ToType result{};

        ERR_FAIL_COND_V_MSG(p_from.size() != 6, result, "Array must have 6 elements to convert to Voxel");

        memcpy(&result, p_from.ptr(), sizeof(ToType));
        return result;
    }

#define MAKE_VOXEL_DIRECT_CONVERTERS(p_rptr_type, p_gd_type)                                  \
    template <>                                                                               \
    constexpr bool is_voxel_direct_convert_to_rptr<p_gd_type, RptrVoxel<p_rptr_type>> = true; \
    template <>                                                                               \
    constexpr bool is_voxel_direct_convert_to_gd<RptrVoxel<p_rptr_type>, p_gd_type> = true;

    MAKE_VOXEL_DIRECT_CONVERTERS(uint8_t, godot::PackedByteArray);
    MAKE_VOXEL_DIRECT_CONVERTERS(int32_t, godot::PackedInt32Array);
    MAKE_VOXEL_DIRECT_CONVERTERS(int64_t, godot::PackedInt64Array);
    MAKE_VOXEL_DIRECT_CONVERTERS(float_t, godot::PackedFloat32Array);
    MAKE_VOXEL_DIRECT_CONVERTERS(double_t, godot::PackedFloat64Array);
    MAKE_VOXEL_DIRECT_CONVERTERS(math::Vector2f, godot::PackedVector2Array);
    MAKE_VOXEL_DIRECT_CONVERTERS(math::Vector3f, godot::PackedVector3Array);
    MAKE_VOXEL_DIRECT_CONVERTERS(math::Vector4f, godot::PackedVector4Array);
    MAKE_VOXEL_DIRECT_CONVERTERS(math::Vector4f, godot::PackedColorArray);

#undef MAKE_VOXEL_DIRECT_CONVERTERS

    template <typename FromType, typename ToType>
    constexpr bool is_voxel_indirect_convert_to_gd = false;
    template <typename FromType, typename ToType>
    constexpr bool is_voxel_indirect_convert_to_rptr = false;

    template <typename ToType>
    ToType convert(const auto &p_from)
        requires is_voxel_indirect_convert_to_gd<std::remove_cvref_t<decltype(p_from)>, ToType>
    {
        /// TODO: Need to verify the validity of this operation
        ToType result{};

        using RptrType = std::remove_cvref_t<decltype(p_from)>;
        using RptrFaceType = std::remove_cvref_t<decltype(p_from.faces[0])>;
        using GodotFaceType = RptrTypeToGodotType<RptrFaceType>;

        result.resize(6);
        for (size_t i = 0; i < 6; ++i)
        {
            result[i] = convert<GodotFaceType>(p_from.faces[i]);
        }
        return result;
    }

    template <typename ToType>
    ToType convert(const auto &p_from)
        requires is_voxel_indirect_convert_to_rptr<std::remove_cvref_t<decltype(p_from)>, ToType>
    {
        /// TODO: Need to verify the validity of this operation
        ToType result{};

        ERR_FAIL_COND_V_MSG(p_from.size() != 6, result, "Array must have 6 elements to convert to Voxel");

        using GodotType = std::remove_cvref_t<decltype(p_from)>;
        using RptrFaceType = std::remove_cvref_t<decltype(result.faces[0])>;
        using GodotFaceType = RptrTypeToGodotType<RptrFaceType>;

        for (size_t i = 0; i < 6; ++i)
        {
            result.faces[i] = convert<RptrFaceType>(static_cast<GodotFaceType>(p_from[i]));
        }
        return result;
    }

#define MAKE_VOXEL_INDIRECT_CONVERTERS(p_rptr_type, p_gd_type)                                  \
    template <>                                                                                 \
    constexpr bool is_voxel_indirect_convert_to_rptr<p_gd_type, RptrVoxel<p_rptr_type>> = true; \
    template <>                                                                                 \
    constexpr bool is_voxel_indirect_convert_to_gd<RptrVoxel<p_rptr_type>, p_gd_type> = true;

    MAKE_VOXEL_INDIRECT_CONVERTERS(uint16_t, godot::PackedInt32Array);
    MAKE_VOXEL_INDIRECT_CONVERTERS(uint32_t, godot::PackedInt64Array);
    MAKE_VOXEL_INDIRECT_CONVERTERS(uint64_t, godot::PackedInt64Array);

    MAKE_VOXEL_INDIRECT_CONVERTERS(int8_t, godot::PackedInt32Array);
    MAKE_VOXEL_INDIRECT_CONVERTERS(int16_t, godot::PackedInt32Array);
    MAKE_VOXEL_INDIRECT_CONVERTERS(math::Vector2i, godot::PackedVector2Array);
    MAKE_VOXEL_INDIRECT_CONVERTERS(math::Vector3i, godot::PackedVector3Array);
    MAKE_VOXEL_INDIRECT_CONVERTERS(math::Vector4i, godot::PackedVector4Array);
    MAKE_VOXEL_INDIRECT_CONVERTERS(math::Vector4i, godot::PackedColorArray);

#undef MAKE_VOXEL_INDIRECT_CONVERTERS

    template <typename ToType>
    ToType convert(const godot::Variant &p_from)
    {
        return convert<ToType>(static_cast<RptrTypeToGodotType<ToType>>(p_from));
    }

    // Special case for Vector4f / Color
    template <>
    math::Vector4f convert(const godot::Variant &p_from)
    {
        if (p_from.get_type() == godot::Variant::Type::COLOR)
        {
            return convert<math::Vector4f>(static_cast<godot::Color>(p_from));
        }
        return convert<math::Vector4f>(static_cast<RptrTypeToGodotType<math::Vector4f>>(p_from));
    }

    // Special case for RptrVoxel<Vector4f> / PackedColorArray
    template <>
    RptrVoxel<math::Vector4f> convert(const godot::Variant &p_from)
    {
        using ToType = RptrVoxel<math::Vector4f>;
        if (p_from.get_type() == godot::Variant::Type::PACKED_COLOR_ARRAY)
        {
            return convert<ToType>(static_cast<godot::PackedColorArray>(p_from));
        }
        return convert<ToType>(static_cast<RptrTypeToGodotType<ToType>>(p_from));
    }

    template <typename FromType>
        requires(!std::same_as<RptrTypeToGodotType<FromType>, FromType>)
    godot::Variant convert(const FromType &p_from)
    {
        return convert<RptrTypeToGodotType<FromType>>(p_from);
    }

} // namespace rptr::gdextension
