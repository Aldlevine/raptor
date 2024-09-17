#pragma once

#include "grid_defs.hpp"

namespace rptr::grid
{
    template <typename Type_>
    class GridChunk
    {
    public:
        using Type = Type_;
        using Data = std::array<Type, CHUNK_SIZE.product()>;

        const Data &data() const { return m_data; }
        Data &data() { return m_data; }

    private:
        Data m_data{};
    };
} // namespace rptr::grid
