#include "etesian/ScanChain.h"
#include <cstdlib>
#include <algorithm>


namespace Etesian {

int64_t chainLengthFromOrder(
    const std::vector<int>& order,
    const std::vector<int64_t>& xs,
    const std::vector<int64_t>& ys,
    int64_t start_x,
    int64_t start_y,
    int64_t end_x,
    int64_t end_y
) {
    int64_t total = 0;

    if (order.empty()) return 0;

    // -------- start → first --------
    int first = order[0];
    total += std::llabs(start_x - xs[first])
           + std::llabs(start_y - ys[first]);

    // -------- internal chain --------
    for (size_t i = 0; i + 1 < order.size(); ++i) {
        int i1 = order[i];
        int i2 = order[i + 1];

        total += std::llabs(xs[i1] - xs[i2])
               + std::llabs(ys[i1] - ys[i2]);
    }

    // -------- last → end --------
    int last = order.back();
    total += std::llabs(xs[last] - end_x)
           + std::llabs(ys[last] - end_y);

    return total;
}
std::vector<int> orderScanChain(
    const std::vector<int64_t>& xs,
    const std::vector<int64_t>& ys,
    int64_t start_x,
    int64_t start_y,
    int64_t end_x,
    int64_t end_y
) {
    int n = (int)xs.size();

    std::vector<int> used(n, 0);
    std::vector<int> order;

    int64_t cx = start_x;
    int64_t cy = start_y;

    // -------- nearest neighbor --------
    for (int k = 0; k < n; ++k) {

        int best = -1;
        int64_t best_d = -1;

        for (int i = 0; i < n; ++i) {
            if (used[i] == 0) {
                int64_t d = std::llabs(cx - xs[i])
                          + std::llabs(cy - ys[i]);

                if (best == -1 || d < best_d) {
                    best = i;
                    best_d = d;
                }
            }
        }

        order.push_back(best);
        used[best] = 1;

        cx = xs[best];
        cy = ys[best];
    }

    // -------- 2-opt --------
    bool improved = true;

    while (improved) {
        improved = false;

        for (int i = 0; i < (int)order.size() - 2; ++i) {
            for (int j = i + 1; j < (int)order.size(); ++j) {

                std::vector<int> new_order = order;
                std::reverse(new_order.begin() + i, new_order.begin() + j);

                if (chainLengthFromOrder(new_order, xs, ys,
                                         start_x, start_y,
                                         end_x, end_y)
                  < chainLengthFromOrder(order, xs, ys,
                                         start_x, start_y,
                                         end_x, end_y)) {
                    order = new_order;
                    improved = true;
                }
            }
        }
    }

    return order;
}

}
