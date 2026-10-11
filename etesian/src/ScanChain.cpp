#include "etesian/ScanChain.h"
#include <cstdlib>
#include <algorithm>
#include <iostream>


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
    int64_t end_y,
    int fast
) {
    int n = (int)xs.size();

    std::vector<int> order;


    // FAST MODE : SERPENTINE
    if (fast == 1) {
        std::cout << "  o  Scan chain ordering: FAST serpentine algorithm." << std::endl;
        // Find the FF selected as the start candidate.
        // Its coordinates are start_x and start_y.
        int start_index = -1;
    
        for (int i = 0; i < n; ++i) {
    
            if (xs[i] == start_x && ys[i] == start_y) {
                start_index = i;
                break;
            }
        }
    
    
        // Safety check.
        // Normally this should never happen because the start point
        // corresponds to one of the FFs selected in Python.
        if (start_index == -1) {
            return order;
        }
    
    
        // Create a list containing all FF indexes.
        for (int i = 0; i < n; ++i) {
            order.push_back(i);
        }
    
    
        // Decide if rows are visited from bottom to top
        // or from top to bottom.
        int64_t min_y = ys[0];
        int64_t max_y = ys[0];
    
        for (int i = 1; i < n; ++i) {
    
            if (ys[i] < min_y) {
                min_y = ys[i];
            }
    
            if (ys[i] > max_y) {
                max_y = ys[i];
            }
        }
    
    
        int64_t distance_bottom = std::llabs(start_y - min_y);
        int64_t distance_top    = std::llabs(start_y - max_y);
    
        bool bottom_to_top = true;
    
        if (distance_top < distance_bottom) {
            bottom_to_top = false;
        }
    
    
        // Decide the horizontal direction of the first row.
        int64_t min_x = xs[0];
        int64_t max_x = xs[0];
    
        for (int i = 1; i < n; ++i) {
    
            if (xs[i] < min_x) {
                min_x = xs[i];
            }
    
            if (xs[i] > max_x) {
                max_x = xs[i];
            }
        }
    
    
        int64_t distance_left  = std::llabs(start_x - min_x);
        int64_t distance_right = std::llabs(start_x - max_x);
    
        bool left_to_right = true;
    
        if (distance_right < distance_left) {
            left_to_right = false;
        }
    
    
        // Sort FFs by row.
        // Inside each row, FFs are initially sorted from left to right.
        std::sort(
            order.begin(),
            order.end(),
            [&](int a, int b) {
    
                if (ys[a] != ys[b]) {
    
                    if (bottom_to_top) {
                        return ys[a] < ys[b];
                    }
                    else {
                        return ys[a] > ys[b];
                    }
                }
    
                return xs[a] < xs[b];
            }
        );
    
    
        // Apply the serpentine direction row by row.
        int row_start = 0;
        bool current_left_to_right = left_to_right;
    
        while (row_start < n) {
    
            int row_end = row_start + 1;
    
            // Find the end of the current row.
            while (row_end < n &&
                   ys[order[row_end]] == ys[order[row_start]]) {
    
                row_end++;
            }
    
    
            // Reverse this row if the direction
            // is from right to left.
            if (current_left_to_right == false) {
    
                std::reverse(
                    order.begin() + row_start,
                    order.begin() + row_end
                );
            }
    
    
            // Change direction for the next row.
            current_left_to_right = !current_left_to_right;
    
            row_start = row_end;
        }
    
    
        // Find the start candidate in the serpentine order.
        int start_position = -1;
    
        for (int i = 0; i < n; ++i) {
    
            if (order[i] == start_index) {
                start_position = i;
                break;
            }
        }
    
    
        // Rotate the order so that the selected candidate
        // becomes the first FF of the scan chain.
        if (start_position > 0) {
    
            std::rotate(
                order.begin(),
                order.begin() + start_position,
                order.end()
            );
        }
    
    
        return order;
    }


    // NORMAL MODE : NEAREST NEIGHBOR + 2-OPT
    std::cout << "  o  Scan chain ordering: OPTIMIZED nearest-neighbor + 2-opt algorithm." << std::endl;
    std::vector<int> used(n, 0);

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

                std::reverse(
                    new_order.begin() + i,
                    new_order.begin() + j
                );

                if (chainLengthFromOrder(
                        new_order,
                        xs,
                        ys,
                        start_x,
                        start_y,
                        end_x,
                        end_y
                    )
                    <
                    chainLengthFromOrder(
                        order,
                        xs,
                        ys,
                        start_x,
                        start_y,
                        end_x,
                        end_y
                    )) {

                    order = new_order;
                    improved = true;
                }
            }
        }
    }


    return order;
}

}
