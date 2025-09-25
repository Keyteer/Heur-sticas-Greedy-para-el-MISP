// Simple progress bar helper
#pragma once
#include <cstdio>
#include <cmath>

// Prints a simple progress bar to stdout.
// percent: value in [0,1]
// width: character width of the bar (default 40)
// label: optional label shown before the bar
static inline void print_progress_bar(double percent, int width = 40, const char *label = "") {
    if (percent < 0) percent = 0;
    if (percent > 1) percent = 1;
    int filled = (int)round(percent * width);
    int i;
    if (label && label[0]) {
        std::printf("%s ", label);
    }
    std::printf("[");
    for (i = 0; i < filled; ++i) std::putchar('#');
    for (; i < width; ++i) std::putchar('-');
    std::printf("]%.1f%%\r", percent * 100.0);
    std::fflush(stdout);
}

// Overload: provide current and total; completion percentage is calculated internally.
// current: progress done (can be 0..total)
// total: total amount (if <= 0, percent is treated as 0)
static inline void print_progress_bar(long long current, long long total, int width = 40, const char *label = "") {
    double percent = 0.0;
    if (total > 0) {
        percent = (double)current / (double)total;
    }
    // clamp percent to [0,1]
    if (percent < 0.0) percent = 0.0;
    if (percent > 1.0) percent = 1.0;
    print_progress_bar(percent, width, label);
}