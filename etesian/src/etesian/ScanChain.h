#pragma once

#include <vector>
#include <cstdint>

namespace Etesian {

    // -------- Compute Chain Length --------
    int64_t chainLengthFromOrder( const std::vector<int>& order,
                                 const std::vector<int64_t>& xs,
                                 const std::vector<int64_t>& ys,
                                 int64_t start_x,
                                 int64_t start_y,
                                 int64_t end_x,
                                 int64_t end_y );

    // ------- Ordering Chain Elements -------
    std::vector<int> orderScanChain( const std::vector<int64_t>& xs,
                                    const std::vector<int64_t>& ys,
                                    int64_t start_x,
                                    int64_t start_y,
                                    int64_t end_x,
                                    int64_t end_y );

}
