#ifndef SRC_GRAPHICS_COLOR_UTILS_H
#define SRC_GRAPHICS_COLOR_UTILS_H

#include <xcb/xcb.h>
#include <cstdlib>
#include <cmath>

namespace graphics {

    /**
     * Calculate Cubehelix color
     * http://www.mrao.cam.ac.uk/~dag/CUBEHELIX/cubetry.html
     *
     * start = 0.5; // start color 0.5 is purple
     * double rots = -1.5; // rotations in colour (typically -1.5 to 1.5, e.g. -1.0 is one blue->green->red cycle)
     * double hue = 1.0; // intensity scaling (in the range 0.0 (B+W) to 1.0 to be strictly correct,
     * larger values may be OK with particular start/end colours) default 1
     * double gamma = 1.0; // gamma correction for intensit (default 1)
     */
    void calculateCubehelixColor(double val, double minVal, double maxVal, uint16_t &r, uint16_t &g, uint16_t &b,
                                 double start = 0.5, double rots = -1.5, double hue = 1.0, double gamma = 1.0);

    void calculateWavelengthColor(double val, double minVal, double maxVal, uint16_t &r, uint16_t &g, uint16_t &b);

}

#endif
