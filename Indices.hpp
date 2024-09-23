#ifndef INDICES
#define INDICES
#include <cstdint>


namespace Indices{
    uint8_t index(uint64_t pos);
    uint8_t index_msb(uint64_t board);
    uint8_t index_lsb(uint64_t board);
    uint64_t msb(uint64_t board);
    uint64_t lsb(uint64_t board);
}


#endif
