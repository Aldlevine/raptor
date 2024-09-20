#pragma once

#include "grid_cell.hpp"
#include "grid_defs.hpp"

namespace rptr::grid
{
    /// @brief A statically sized chunk.
    /// @tparam t_Unit: The unit type (value type of each channel)
    /// @tparam t_channels: The number of channels
    template <UnitType t_Unit, size_t t_channels>
    class GridChunk
    {
    public:
        using m_Unit = t_Unit;
        constexpr static size_t m_channels = t_channels;

        using m_Cell = GridCell<m_Unit, m_channels>;
        using m_Data = std::array<m_Cell, CHUNK_SIZE.product()>;

        constexpr auto begin(this auto &&self) { return self.m_data.begin(); }
        constexpr auto end(this auto &&self) { return self.m_data.end(); }

        constexpr auto &operator[](this auto &&self, size_t index) { return self.m_data[index]; }

    private:
        m_Data m_data{};
    };
} // namespace rptr::grid
