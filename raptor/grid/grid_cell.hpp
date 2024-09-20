#pragma once

#include "repr/repr.hpp"
#include "utils/utils.hpp"

#include <array>
#include <tuple>
#include <type_traits>
#include <utility>

namespace rptr::grid
{
    /// @brief Unit type compatible with a grid.
    /// @tparam t_Type: The type under question
    template <typename t_Type>
    concept UnitType = std::is_fundamental_v<t_Type>;

    /// @brief Type that can be copied to a GridCell.
    /// @tparam t_Type: The type under question
    /// @tparam t_Unit: The unit type (value type of each channel)
    /// @tparam t_channels: The number of channels
    template <typename t_Type, typename t_Unit, size_t t_channels>
    concept CopiesToGridCell =
        UnitType<t_Unit> &&
        (std::tuple_size_v<t_Type> == t_channels) &&
        utils::is_homogenous_tuple_of_type<t_Type, t_Unit>;

    /// @brief A single cell in a grid.
    /// @tparam t_Unit: The unit type (value type of each channel)
    /// @tparam t_channels: The number of channels
    template <UnitType t_Unit, size_t t_channels>
    class GridCell;
} // namespace rptr::grid

namespace std
{
    /// @brief Overload std::tuple_size for GridCell.
    template <rptr::grid::UnitType t_Unit, size_t t_channels>
    struct tuple_size<rptr::grid::GridCell<t_Unit, t_channels>>
        : public std::integral_constant<size_t, t_channels>
    {
    };

    /// @brief Overload std::tuple_element for GridCell.
    template <size_t t_I, rptr::grid::UnitType t_Unit, size_t t_channels>
    struct tuple_element<t_I, rptr::grid::GridCell<t_Unit, t_channels>>
    {
        using type = t_Unit;
    };

    /// @brief Overload std::get for GridCell.
    template <size_t t_I, rptr::grid::UnitType t_Unit, size_t t_channels>
    constexpr t_Unit &get(rptr::grid::GridCell<t_Unit, t_channels> &p_grid_cell)
    {
        return p_grid_cell[t_I];
    }

    /// @brief Overload std::get for GridCell - const.
    template <size_t t_I, rptr::grid::UnitType t_Unit, size_t t_channels>
    constexpr const t_Unit &get(const rptr::grid::GridCell<t_Unit, t_channels> &p_grid_cell)
    {
        return p_grid_cell[t_I];
    }

} //namespace std

namespace rptr::grid
{
    template <UnitType t_Unit, size_t t_channels>
    class GridCell
    {
    public:
        /// @brief The unit type (value type of each channel).
        using m_Unit = t_Unit;
        /// @brief The type of the underlying data.
        using m_Data = std::array<t_Unit, t_channels>;

        /// @brief The number of channels.
        constexpr static auto m_channels = t_channels;

        /// @brief Default constructor.
        constexpr GridCell() = default;

        /// @brief Copy construct from underlying data type.
        constexpr explicit GridCell(const m_Data &p_data) :
            m_data{ p_data } {}

        /// @brief Move construct from underlying data type.
        constexpr explicit GridCell(m_Data &&p_data) :
            m_data{ std::move(p_data) } {}

        /// @brief Copy construct from any compatible type.
        explicit GridCell(const CopiesToGridCell<m_Unit, m_channels> auto &p_other)
        {
            do_copy(p_other);
        }

        /// @brief Copy assign from any compatible type.
        GridCell &operator=(const CopiesToGridCell<m_Unit, m_channels> auto &p_other)
        {
            do_copy(p_other);
            return *this;
        }

        /// @brief Get channel ref from index.
        /// @param index: The channel index to get
        constexpr auto &&operator[](this auto &&self, size_t index) { return self.m_data[index]; }

        /// @brief Get a pointer to the underlying data.
        constexpr auto data(this auto &&self) { return self.m_data.data(); }

        /// @brief Get an iterator to the first channel.
        constexpr auto begin(this auto &&self) { return self.m_data.begin(); }

        /// @brief Get an iterator to 1 past the last channel.
        constexpr auto end(this auto &&self) { return self.m_data.end(); }

        /// @brief Compare GridCell for equality with any compatible type.
        /// @param p_this: The GridCell to compare
        /// @param p_other: The other object to compare
        friend constexpr bool operator==(const GridCell &p_this, const CopiesToGridCell<m_Unit, m_channels> auto &p_other)
        {
            return [&]<size_t... I>(std::index_sequence<I...>)
            {
                return ((std::get<I>(p_this) == std::get<I>(p_other)) && ... && true);
            }(std::make_index_sequence<m_channels>());
        }

        /// @brief Compare GridCell for inequality with any compatible type.
        /// @param p_this: The grid cell to compare
        /// @param p_other: The other object to compare
        friend constexpr bool operator!=(const GridCell &p_this, const CopiesToGridCell<m_Unit, m_channels> auto &p_other)
        {
            return !(p_this == p_other);
        }

        /// @brief Write representation to ostream.
        /// @param p_os: The ostream to write to
        /// @param p_this: The cell to repr/write
        friend std::ostream &operator<<(std::ostream &p_os, const GridCell &p_this)
        {
            return p_os << repr::to_repr(p_this);
        }

    private:
        m_Data m_data{};

        void do_copy(const CopiesToGridCell<m_Unit, m_channels> auto &p_other)
        {
            [&]<size_t... I>()
            {
                ((this->m_data[I] = std::get<I>(p_other)), ...);
            }(std::make_index_sequence<m_channels>());
        }
    };
} // namespace rptr::grid

namespace rptr::repr
{
    /// Overload to_repr for GridCell
    /// @tparam t_Unit: The unit type (value type of each channel)
    /// @tparam t_channels: The number of channels
    /// @param p_grid_cell: The cell to repr
    /// @param p_repr: The repr context object
    template <grid::UnitType t_Unit, size_t t_channels>
    inline Repr &to_repr(const grid::GridCell<t_Unit, t_channels> &p_grid_cell, Repr &p_repr)
    {
        p_repr.push_scope("GridCell<" + std::to_string(t_channels) + ">");

        [&p_grid_cell, &p_repr]<size_t... I>(std::index_sequence<I...>)
        {
            (p_repr.push_value(std::to_string(p_grid_cell[I])), ...);
        }(std::make_index_sequence<t_channels>());

        return p_repr.pop_scope();
    }

} // namespace rptr::repr
