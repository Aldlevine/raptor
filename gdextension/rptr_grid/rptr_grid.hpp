#pragma once

#include "rptr_grid_types.hpp"

#include "rptr_convert/rptr_convert.hpp"
#include "rptr_voxel/rptr_voxel.hpp"

#include "grid/grid.hpp"
#include "math/math.hpp"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <variant>

#define MAKE_GRID_TYPE_CASE(p_enum, p_type) \
    case RptrGridType::p_enum:              \
    {                                       \
        m_grid = grid::Grid<p_type>{};      \
        break;                              \
    }

#define REQUIRE_VALID_GRID(p_type, p_return)                                                                                    \
    if constexpr (std::same_as<p_type, std::monostate>)                                                                         \
    {                                                                                                                           \
        ::godot::_err_print_error(                                                                                              \
            FUNCTION_STR, __FILE__, __LINE__, "Method/function failed. Returning: " _STR(m_retval), "m_grid is uninitialized"); \
        return p_return;                                                                                                        \
    }                                                                                                                           \
    else

#define BIND_GRID_TYPE_ENUM(p_enum) \
    godot::ClassDB::bind_integer_constant(get_class_static(), "", #p_enum, static_cast<uint8_t>(RptrGridType::p_enum))

namespace rptr::gdextension
{
    class RptrGrid : public godot::RefCounted
    {
        GDCLASS(RptrGrid, godot::RefCounted);

    public:
        uint8_t get_type()
        {
            return static_cast<uint8_t>(m_grid_type);
        }

        void set_type(const uint8_t &p_grid_type)
        {
            m_grid_type = static_cast<RptrGridType>(p_grid_type);

            switch (m_grid_type)
            {
                MAKE_GRID_TYPE_CASE(UINT8, uint8_t);
                MAKE_GRID_TYPE_CASE(UINT16, uint16_t);
                MAKE_GRID_TYPE_CASE(UINT32, uint32_t);
                MAKE_GRID_TYPE_CASE(UINT64, uint64_t);
                MAKE_GRID_TYPE_CASE(INT8, int8_t);
                MAKE_GRID_TYPE_CASE(INT16, int16_t);
                MAKE_GRID_TYPE_CASE(INT32, int32_t);
                MAKE_GRID_TYPE_CASE(INT64, int64_t);
                MAKE_GRID_TYPE_CASE(FLOAT32, float_t);
                MAKE_GRID_TYPE_CASE(FLOAT64, double_t);
                MAKE_GRID_TYPE_CASE(VECTOR2I, math::Vector2i);
                MAKE_GRID_TYPE_CASE(VECTOR2F, math::Vector2f);
                MAKE_GRID_TYPE_CASE(VECTOR3I, math::Vector3i);
                MAKE_GRID_TYPE_CASE(VECTOR3F, math::Vector3f);
                MAKE_GRID_TYPE_CASE(VECTOR4I, math::Vector4i);
                MAKE_GRID_TYPE_CASE(VECTOR4F, math::Vector4f);
                MAKE_GRID_TYPE_CASE(UINT8_VOXEL, RptrVoxel<uint8_t>);
                MAKE_GRID_TYPE_CASE(UINT16_VOXEL, RptrVoxel<uint16_t>);
                MAKE_GRID_TYPE_CASE(UINT32_VOXEL, RptrVoxel<uint32_t>);
                MAKE_GRID_TYPE_CASE(UINT64_VOXEL, RptrVoxel<uint64_t>);
                MAKE_GRID_TYPE_CASE(INT8_VOXEL, RptrVoxel<int8_t>);
                MAKE_GRID_TYPE_CASE(INT16_VOXEL, RptrVoxel<int16_t>);
                MAKE_GRID_TYPE_CASE(INT32_VOXEL, RptrVoxel<int32_t>);
                MAKE_GRID_TYPE_CASE(INT64_VOXEL, RptrVoxel<int64_t>);
                MAKE_GRID_TYPE_CASE(FLOAT32_VOXEL, RptrVoxel<float_t>);
                MAKE_GRID_TYPE_CASE(FLOAT64_VOXEL, RptrVoxel<double_t>);
                MAKE_GRID_TYPE_CASE(VECTOR2I_VOXEL, RptrVoxel<math::Vector2i>);
                MAKE_GRID_TYPE_CASE(VECTOR2F_VOXEL, RptrVoxel<math::Vector2f>);
                MAKE_GRID_TYPE_CASE(VECTOR3I_VOXEL, RptrVoxel<math::Vector3i>);
                MAKE_GRID_TYPE_CASE(VECTOR3F_VOXEL, RptrVoxel<math::Vector3f>);
                MAKE_GRID_TYPE_CASE(VECTOR4I_VOXEL, RptrVoxel<math::Vector4i>);
                MAKE_GRID_TYPE_CASE(VECTOR4F_VOXEL, RptrVoxel<math::Vector4f>);
                case RptrGridType::NONE:
                    m_grid = std::monostate{};
                    break;
            }
        }

        godot::Variant getv(const godot::Vector3i &p_index)
        {
            return std::visit(
                [&](auto &p_grid) -> godot::Variant
                {
                    using GridType = std::remove_cvref_t<decltype(p_grid)>;
                    REQUIRE_VALID_GRID(GridType, godot::Variant{})
                    {
                        const auto &index = convert<math::Vector3i>(p_index);
                        const auto &value = convert<RptrTypeToGodotType<typename GridType::Type>>(p_grid.getv(index));
                        return value;
                    }
                },
                m_grid);
        }

        void setv(const godot::Vector3i &p_index, godot::Variant p_value)
        {
            std::visit(
                [&](auto &p_grid) -> void
                {
                    using GridType = std::remove_cvref_t<decltype(p_grid)>;
                    REQUIRE_VALID_GRID(GridType, )
                    {
                        const auto &index = convert<math::Vector3i>(p_index);
                        const auto &value = convert<typename GridType::Type>(p_value);
                        p_grid.setv(index, value);
                    }
                },
                m_grid);
        }

        void fill(const godot::AABB &p_bounds, godot::Variant p_value)
        {
            std::visit(
                [this, &p_bounds, &p_value](auto &p_grid) -> void
                {
                    using GridType = std::remove_cvref_t<decltype(p_grid)>;
                    REQUIRE_VALID_GRID(GridType, )
                    {
                        const auto &bounds = convert<math::AABBf>(p_bounds);
                        const auto &value = convert<typename GridType::Type>(p_value);
                        p_grid.fill(bounds, value);
                    }
                },
                m_grid);
        }

    protected:
        static void _bind_methods()
        {
            BIND_GRID_TYPE_ENUM(UINT8);
            BIND_GRID_TYPE_ENUM(UINT16);
            BIND_GRID_TYPE_ENUM(UINT32);
            BIND_GRID_TYPE_ENUM(UINT64);
            BIND_GRID_TYPE_ENUM(INT8);
            BIND_GRID_TYPE_ENUM(INT16);
            BIND_GRID_TYPE_ENUM(INT32);
            BIND_GRID_TYPE_ENUM(INT64);
            BIND_GRID_TYPE_ENUM(FLOAT32);
            BIND_GRID_TYPE_ENUM(FLOAT64);
            BIND_GRID_TYPE_ENUM(VECTOR2I);
            BIND_GRID_TYPE_ENUM(VECTOR2F);
            BIND_GRID_TYPE_ENUM(VECTOR3I);
            BIND_GRID_TYPE_ENUM(VECTOR3F);
            BIND_GRID_TYPE_ENUM(VECTOR4I);
            BIND_GRID_TYPE_ENUM(VECTOR4F);
            BIND_GRID_TYPE_ENUM(UINT8_VOXEL);
            BIND_GRID_TYPE_ENUM(UINT16_VOXEL);
            BIND_GRID_TYPE_ENUM(UINT32_VOXEL);
            BIND_GRID_TYPE_ENUM(UINT64_VOXEL);
            BIND_GRID_TYPE_ENUM(INT8_VOXEL);
            BIND_GRID_TYPE_ENUM(INT16_VOXEL);
            BIND_GRID_TYPE_ENUM(INT32_VOXEL);
            BIND_GRID_TYPE_ENUM(INT64_VOXEL);
            BIND_GRID_TYPE_ENUM(FLOAT32_VOXEL);
            BIND_GRID_TYPE_ENUM(FLOAT64_VOXEL);
            BIND_GRID_TYPE_ENUM(VECTOR2I_VOXEL);
            BIND_GRID_TYPE_ENUM(VECTOR2F_VOXEL);
            BIND_GRID_TYPE_ENUM(VECTOR3I_VOXEL);
            BIND_GRID_TYPE_ENUM(VECTOR3F_VOXEL);
            BIND_GRID_TYPE_ENUM(VECTOR4I_VOXEL);
            BIND_GRID_TYPE_ENUM(VECTOR4F_VOXEL);

            godot::ClassDB::bind_method(godot::D_METHOD("get_type"), &RptrGrid::get_type);
            godot::ClassDB::bind_method(godot::D_METHOD("set_type", "p_type"), &RptrGrid::set_type);

            godot::ClassDB::bind_method(godot::D_METHOD("getv", "p_index"), &RptrGrid::getv);
            godot::ClassDB::bind_method(godot::D_METHOD("setv", "p_index", "p_value"), &RptrGrid::setv);
            godot::ClassDB::bind_method(godot::D_METHOD("fill", "p_bounds", "p_value"), &RptrGrid::fill);
        }

    private:
        RptrGridType m_grid_type{};
        RptrGridVariant m_grid{ std::monostate{} };
    };
} //namespace rptr::gdextension

#undef MAKE_GRID_TYPE_CASE
#undef REQUIRE_VALID_GRID
#undef BIND_GRID_TYPE_ENUM