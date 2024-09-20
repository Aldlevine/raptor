// NOLINTBEGIN(*-avoid-do-while)
// NOLINTBEGIN(*-avoid-magic-numbers)
// NOLINTBEGIN(*-macro-usage)

#include "grid/grid.hpp"
#include "grid/grid_cell.hpp"

#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace rptr;

template <typename t_Type>
constexpr const char *type_name{ "" };
template <>
constexpr auto type_name<uint8_t>{ "uint8_t" };
template <>
constexpr auto type_name<uint16_t>{ "uint16_t" };
template <>
constexpr auto type_name<uint32_t>{ "uint32_t" };
template <>
constexpr auto type_name<uint64_t>{ "uint64_t" };
template <>
constexpr auto type_name<int8_t>{ "uint8_t" };
template <>
constexpr auto type_name<int16_t>{ "uint16_t" };
template <>
constexpr auto type_name<int32_t>{ "uint32_t" };
template <>
constexpr auto type_name<int64_t>{ "uint64_t" };
template <>
constexpr auto type_name<float_t>{ "float_t" };
template <>
constexpr auto type_name<double_t>{ "double_t" };
template <grid::UnitType t_Unit, size_t t_channels>
const auto type_name<grid::Grid<t_Unit, t_channels>> =
    (std::string{ "Grid<" } + type_name<t_Unit> + ", " + std::to_string(t_channels) + ">");

template <typename t_Unit, size_t t_channels>
constexpr grid::GridCell<t_Unit, t_channels> clear_value{};

template <typename t_Unit, size_t t_channels>
constexpr auto test_value = []<size_t... I>(std::index_sequence<I...>)
{
    return grid::GridCell<t_Unit, t_channels>{ std::array<t_Unit, t_channels>{ (I + 1)... } };
}(std::make_index_sequence<t_channels>());

static_assert(clear_value<uint8_t, 4> == std::array<uint8_t, 4>{ 0, 0, 0, 0 });
static_assert(test_value<uint8_t, 4> == std::array<uint8_t, 4>{ 1, 2, 3, 4 });

template <typename t_Type, size_t t_channels>
void test_getv_setv()
{
    using Grid = grid::Grid<t_Type, t_channels>;

    SECTION(type_name<Grid>)
    {
        Grid g{};

        REQUIRE((g.get({ 16, 16, 16 }) == clear_value<t_Type, t_channels>));

        g.set({ 16, 16, 16 }, test_value<t_Type, t_channels>);

        REQUIRE((g.get({ 16, 16, 16 }) == test_value<t_Type, t_channels>));
        REQUIRE((g.get({ 0, 0, 0 }) == clear_value<t_Type, t_channels>));
    }
}

TEST_CASE("Grid::getv, Grid::setv", "[grid]")
{
    test_getv_setv<uint8_t, 1>();
    test_getv_setv<uint8_t, 4>();
    test_getv_setv<uint8_t, 4 * 6>();

    test_getv_setv<uint16_t, 1>();
    test_getv_setv<uint16_t, 4>();
    test_getv_setv<uint16_t, 4 * 6>();

    test_getv_setv<uint32_t, 1>();
    test_getv_setv<uint32_t, 4>();
    test_getv_setv<uint32_t, 4 * 6>();

    test_getv_setv<uint64_t, 1>();
    test_getv_setv<uint64_t, 4>();
    test_getv_setv<uint64_t, 4 * 6>();

    test_getv_setv<float_t, 1>();
    test_getv_setv<float_t, 4>();
    test_getv_setv<float_t, 4 * 6>();

    test_getv_setv<double_t, 1>();
    test_getv_setv<double_t, 4>();
    test_getv_setv<double_t, 4 * 6>();
}

template <typename t_Type, size_t t_channels, bool t_benchmarks = true>
void test_fill()
{
    using Grid = grid::Grid<t_Type, t_channels>;

    Grid g{};

    SECTION(type_name<Grid>)
    {
        g.fill({ { 0, 0, 0 }, { 127, 127, 127 } }, test_value<t_Type, t_channels>);

        REQUIRE((g.get({ 126, 126, 126 }) == test_value<t_Type, t_channels>));
        REQUIRE((g.get({ 127, 127, 127 }) == clear_value<t_Type, t_channels>));
    }
}

TEST_CASE("Grid::fill", "[grid]")
{
    test_fill<uint8_t, 1>();
    test_fill<uint8_t, 4>();
    test_fill<uint8_t, 4 * 6>();

    test_fill<uint16_t, 1>();
    test_fill<uint16_t, 4>();
    test_fill<uint16_t, 4 * 6>();

    test_fill<uint32_t, 1>();
    test_fill<uint32_t, 4>();
    test_fill<uint32_t, 4 * 6>();

    test_fill<uint64_t, 1>();
    test_fill<uint64_t, 4>();
    test_fill<uint64_t, 4 * 6>();

    test_fill<float_t, 1>();
    test_fill<float_t, 4>();
    test_fill<float_t, 4 * 6>();

    test_fill<double_t, 1>();
    test_fill<double_t, 4>();
    test_fill<double_t, 4 * 6>();
}

#define BENCHMARK_GRID_FILL(p_Unit, p_channels)                                                  \
    {                                                                                            \
        using Grid = ::rptr::grid::Grid<p_Unit, p_channels>;                                     \
        Grid g{};                                                                                \
        constexpr size_t sz_B = ((128ul * 128 * 128) * sizeof(typename Grid::m_Chunk::m_Cell));  \
        constexpr size_t sz_KiB = sz_B / 1024;                                                   \
        constexpr size_t sz_MiB = sz_KiB / 1024;                                                 \
        BENCHMARK(type_name<Grid> + "::fill(128^3 " + std::to_string(sz_MiB) + "MiB)")           \
        {                                                                                        \
            g.fill({ { 0, 0, 0 }, { 128, 128, 128 } }, test_value<p_Unit, p_channels>);          \
        };                                                                                       \
        BENCHMARK(type_name<Grid> + "::fill(128^3 off " + std::to_string(sz_MiB) + "MiB)")       \
        {                                                                                        \
            g.fill({ grid::CHUNK_SIZE / 2, { 128, 128, 128 } }, test_value<p_Unit, p_channels>); \
        };                                                                                       \
    }

TEST_CASE("Grid::fill benchmark", "[grid]")
{
    {
        using Grid = ::rptr ::grid ::Grid<uint8_t, 1>;
        Grid g{};
        constexpr size_t sz_B = ((128ul * 128 * 128) * sizeof(typename Grid ::m_Chunk ::m_Cell));
        constexpr size_t sz_KiB = sz_B / 1024;
        constexpr size_t sz_MiB = sz_KiB / 1024;
        if (Catch ::Benchmark ::Benchmark CATCH2_INTERNAL_BENCHMARK_8{ type_name<Grid> + "::fill(128^3 " + std ::to_string(sz_MiB) + "MiB)" })
            CATCH2_INTERNAL_BENCHMARK_8 = [&](int)
            { g.fill({ { 0, 0, 0 }, { 128, 128, 128 } }, test_value<uint8_t, 1>); };
        if (Catch ::Benchmark ::Benchmark CATCH2_INTERNAL_BENCHMARK_9{ type_name<Grid> + "::fill(128^3 off " + std ::to_string(sz_MiB) + "MiB)" })
            CATCH2_INTERNAL_BENCHMARK_9 = [&](int)
            { g.fill({ grid ::CHUNK_SIZE / 2, { 128, 128, 128 } }, test_value<uint8_t, 1>); };
    };
    BENCHMARK_GRID_FILL(uint8_t, 4);
    BENCHMARK_GRID_FILL(uint8_t, 4ul * 6);

    BENCHMARK_GRID_FILL(uint16_t, 1);
    BENCHMARK_GRID_FILL(uint16_t, 4);
    BENCHMARK_GRID_FILL(uint16_t, 4ul * 6);

    BENCHMARK_GRID_FILL(uint32_t, 1);
    BENCHMARK_GRID_FILL(uint32_t, 4);
    BENCHMARK_GRID_FILL(uint32_t, 4ul * 6);

    BENCHMARK_GRID_FILL(uint64_t, 1);
    BENCHMARK_GRID_FILL(uint64_t, 4);
    BENCHMARK_GRID_FILL(uint64_t, 4ul * 6);

    BENCHMARK_GRID_FILL(float_t, 1);
    BENCHMARK_GRID_FILL(float_t, 4);
    BENCHMARK_GRID_FILL(float_t, 4ul * 6);

    BENCHMARK_GRID_FILL(double_t, 1);
    BENCHMARK_GRID_FILL(double_t, 4);
    BENCHMARK_GRID_FILL(double_t, 4ul * 6);
}

#undef BENCHMARK_GRID_FILL

// NOLINTEND(*-macro-usage)
// NOLINTEND(*-avoid-magic-numbers)
// NOLINTEND(*-avoid-do-while)