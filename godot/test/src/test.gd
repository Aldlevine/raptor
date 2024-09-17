extends "tester.gd"


func define_typed_grid_tests(
    type: int,
    type_name: String,
    clear_value,
    test_value) -> void:

    (valid("RptrGrid<" + type_name + ">:: getv (clear)")
        .setup(func():
            var grid := RptrGrid.new()
            grid.set_type(type)
            return [grid])
        .test(func(v: ValidTest, grid):
            v.assert_eq(grid.getv(Vector3i(0, 0, 0)), clear_value)
            ))

    (valid("RptrGrid<" + type_name + ">:: setv, getv")
        .setup(func():
            var grid := RptrGrid.new()
            grid.set_type(type)
            return [grid])
        .test(func(v: ValidTest, grid):
            grid.setv(Vector3i(0, 0, 0), test_value)
            v.assert_eq(grid.getv(Vector3i(0, 0, 0)), test_value)
            ))

    (valid("RptrGrid<" + type_name + ">:: fill, getv")
        .setup(func():
            var grid := RptrGrid.new()
            grid.set_type(type)
            return [grid])
        .test(func(v: ValidTest, grid):
            grid.fill(AABB(Vector3i(0, 0, 0), Vector3i(16, 16, 16)), test_value)
            for x in 16:
                for y in 16:
                    for z in 16:
                        v.assert_eq(grid.getv(Vector3i(x, y, z)), test_value)
            ))

    var rptr_grid_fill_no_alloc = RptrGrid.new()
    rptr_grid_fill_no_alloc.set_type(type)
    (perf("RptrGrid<" + type_name + ">:: fill 128^3 - no-alloc", 500)
        .setup(func():
            return [rptr_grid_fill_no_alloc])
        .test(func(grid):
            grid.fill(AABB(Vector3i(0, 0, 0), Vector3i(128, 128, 128)), test_value)
            ))

func define_tests() -> void:
    define_typed_grid_tests(RptrGrid.UINT8, "UINT8", 0, 10)
    define_typed_grid_tests(RptrGrid.UINT16, "UINT16", 0, 10)
    define_typed_grid_tests(RptrGrid.UINT32, "UINT32", 0, 10)
    define_typed_grid_tests(RptrGrid.UINT64, "UINT64", 0, 10)
    define_typed_grid_tests(RptrGrid.INT8, "INT8", 0, -10)
    define_typed_grid_tests(RptrGrid.INT16, "INT16", 0, -10)
    define_typed_grid_tests(RptrGrid.INT32, "INT32", 0, -10)
    define_typed_grid_tests(RptrGrid.INT64, "INT64", 0, -10)
    define_typed_grid_tests(RptrGrid.FLOAT32, "FLOAT32", 0, PackedFloat32Array([-10.1])[0])
    define_typed_grid_tests(RptrGrid.FLOAT64, "FLOAT64", 0, -10.1)
    define_typed_grid_tests(RptrGrid.VECTOR2I, "VECTOR2I", Vector2i.ZERO, Vector2i(1, 2))
    define_typed_grid_tests(RptrGrid.VECTOR2F, "VECTOR2F", Vector2.ZERO, Vector2(1.1, 2.1))
    define_typed_grid_tests(RptrGrid.VECTOR3I, "VECTOR3I", Vector3i.ZERO, Vector3i(1, 2, 3))
    define_typed_grid_tests(RptrGrid.VECTOR3F, "VECTOR3F", Vector3.ZERO, Vector3(1.1, 2.1, 3.1))
    define_typed_grid_tests(RptrGrid.VECTOR4I, "VECTOR4I", Vector4i.ZERO, Vector4i(1, 2, 3, 4))
    define_typed_grid_tests(RptrGrid.VECTOR4F, "VECTOR4F", Vector4.ZERO, Vector4(1.1, 2.1, 3.1, 4.1))

    define_typed_grid_tests(RptrGrid.UINT8_VOXEL, "UINT8_VOXEL",
        PackedByteArray([0, 0, 0, 0, 0, 0]),
        PackedByteArray([1, 2, 3, 4, 5, 6]))
    define_typed_grid_tests(RptrGrid.UINT16_VOXEL, "UINT16_VOXEL",
        PackedInt32Array([0, 0, 0, 0, 0, 0]),
        PackedInt32Array([1, 2, 3, 4, 5, 6]))
    define_typed_grid_tests(RptrGrid.UINT32_VOXEL, "UINT32_VOXEL",
        PackedInt64Array([0, 0, 0, 0, 0, 0]),
        PackedInt64Array([1, 2, 3, 4, 5, 6]))
    define_typed_grid_tests(RptrGrid.UINT64_VOXEL, "UINT64_VOXEL",
        PackedInt64Array([0, 0, 0, 0, 0, 0]),
        PackedInt64Array([1, 2, 3, 4, 5, 6]))
    define_typed_grid_tests(RptrGrid.INT8_VOXEL, "INT8_VOXEL",
        PackedInt32Array([0, 0, 0, 0, 0, 0]),
        PackedInt32Array([1, 2, 3, 4, 5, 6]))
    define_typed_grid_tests(RptrGrid.INT16_VOXEL, "INT16_VOXEL",
        PackedInt32Array([0, 0, 0, 0, 0, 0]),
        PackedInt32Array([1, 2, 3, 4, 5, 6]))
    define_typed_grid_tests(RptrGrid.INT32_VOXEL, "INT32_VOXEL",
        PackedInt32Array([0, 0, 0, 0, 0, 0]),
        PackedInt32Array([1, 2, 3, 4, 5, 6]))
    define_typed_grid_tests(RptrGrid.INT64_VOXEL, "INT64_VOXEL",
        PackedInt64Array([0, 0, 0, 0, 0, 0]),
        PackedInt64Array([1, 2, 3, 4, 5, 6]))
    define_typed_grid_tests(RptrGrid.FLOAT32_VOXEL, "FLOAT32_VOXEL",
        PackedFloat32Array([0, 0, 0, 0, 0, 0]),
        PackedFloat32Array([1.1, 2.1, 3.1, 4.1, 5.1, 6.1]))
    define_typed_grid_tests(RptrGrid.FLOAT64_VOXEL, "FLOAT64_VOXEL",
        PackedFloat64Array([0, 0, 0, 0, 0, 0]),
        PackedFloat64Array([1.1, 2.1, 3.1, 4.1, 5.1, 6.1]))

    define_typed_grid_tests(RptrGrid.VECTOR2I_VOXEL, "VECTOR2I_VOXEL",
        PackedVector2Array([
            Vector2i.ZERO, Vector2i.ZERO,
            Vector2i.ZERO, Vector2i.ZERO,
            Vector2i.ZERO, Vector2i.ZERO]),
        PackedVector2Array([
            Vector2i(1, 1), Vector2i(2, 2),
            Vector2i(3, 3), Vector2i(4, 4),
            Vector2i(5, 5), Vector2i(6, 6),
            ]))

    define_typed_grid_tests(RptrGrid.VECTOR2F_VOXEL, "VECTOR2F_VOXEL",
        PackedVector2Array([
            Vector2.ZERO, Vector2.ZERO,
            Vector2.ZERO, Vector2.ZERO,
            Vector2.ZERO, Vector2.ZERO]),
        PackedVector2Array([
            Vector2(1.1, 1.1), Vector2(2.1, 2.1),
            Vector2(3.1, 3.1), Vector2(4.1, 4.1),
            Vector2(5.1, 5.1), Vector2(6.1, 6.1),
            ]))

    define_typed_grid_tests(RptrGrid.VECTOR3I_VOXEL, "VECTOR3I_VOXEL",
        PackedVector3Array([
            Vector3i.ZERO, Vector3i.ZERO,
            Vector3i.ZERO, Vector3i.ZERO,
            Vector3i.ZERO, Vector3i.ZERO]),
        PackedVector3Array([
            Vector3i(1, 1, 1), Vector3i(2, 2, 2),
            Vector3i(3, 3, 3), Vector3i(4, 4, 4),
            Vector3i(5, 5, 5), Vector3i(6, 6, 6),
            ]))

    define_typed_grid_tests(RptrGrid.VECTOR3F_VOXEL, "VECTOR3F_VOXEL",
        PackedVector3Array([
            Vector3.ZERO, Vector3.ZERO,
            Vector3.ZERO, Vector3.ZERO,
            Vector3.ZERO, Vector3.ZERO]),
        PackedVector3Array([
            Vector3(1.1, 1.1, 1.1), Vector3(2.1, 2.1, 2.1),
            Vector3(3.1, 3.1, 3.1), Vector3(4.1, 4.1, 4.1),
            Vector3(5.1, 5.1, 5.1), Vector3(6.1, 6.1, 6.1),
            ]))

    define_typed_grid_tests(RptrGrid.VECTOR4I_VOXEL, "VECTOR4I_VOXEL",
        PackedVector4Array([
            Vector4i.ZERO, Vector4i.ZERO,
            Vector4i.ZERO, Vector4i.ZERO,
            Vector4i.ZERO, Vector4i.ZERO]),
        PackedVector4Array([
            Vector4i(1, 1, 1, 1), Vector4i(2, 2, 2, 2),
            Vector4i(3, 3, 3, 3), Vector4i(4, 4, 4, 4),
            Vector4i(5, 5, 5, 5), Vector4i(6, 6, 6, 6),
            ]))

    define_typed_grid_tests(RptrGrid.VECTOR4F_VOXEL, "VECTOR4F_VOXEL",
        PackedVector4Array([
            Vector4.ZERO, Vector4.ZERO,
            Vector4.ZERO, Vector4.ZERO,
            Vector4.ZERO, Vector4.ZERO]),
        PackedVector4Array([
            Vector4(1.1, 1.1, 1.1, 1.1), Vector4(2.1, 2.1, 2.1, 2.1),
            Vector4(3.1, 3.1, 3.1, 3.1), Vector4(4.1, 4.1, 4.1, 4.1),
            Vector4(5.1, 5.1, 5.1, 5.1), Vector4(6.1, 6.1, 6.1, 6.1),
            ]))


func _ready() -> void:
    get_tree().quit()