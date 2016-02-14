#ifndef SRC_GRAPHICS_POINTCHART_H_
#define SRC_GRAPHICS_POINTCHART_H_

#include <xcb/xcb.h>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "ChartColormap.h"
#include "ChartData.h"
#include "Chart.h"

namespace graphics {

    class PointChart : public Chart {
    public:
        PointChart(uint16_t _width = 400, uint16_t _height = 260) :
                Chart(_width, _height) {
            // points context
            pointsContext = xcb_generate_id(connection);
            uint32_t mask = XCB_GC_FOREGROUND;
            uint32_t values2[] = {colormap->getGreen()->pixel};
            xcb_create_gc(connection, pointsContext, screen->root,
                          mask, values2);

            // clean points context
            cleanPointsContext = xcb_generate_id(connection);
            uint32_t values22[] = {screen->black_pixel};
            xcb_create_gc(connection, cleanPointsContext,
                          screen->root, mask,
                          values22);
        }

        ~PointChart() {
            pointsContext = 0;
            cleanPointsContext = 0;
        }

        void drawPoints() const {
            xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN,
                           window, pointsContext, data->size(),
                           data->getOutpoints()->getArray());
        }

        void drawCleanPoints() const {
            xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN,
                           window, cleanPointsContext,
                           data->size(),
                           data->getOutpoints()->getArray());
        }

        /**
         * 1. draw clean current points
         * 2. add new points and update all points position
         * 3. draw updated current points
         */
        void redrawNewPoints(double x, double y) const {
            drawCleanPoints();
            data->addPoint(x, y);
            drawPoints();

            if (data->size() % 36 == 0) { // оптимизация :)
                drawAxes();
                drawAxesLabels();
            }

            flush();
        }

        void draw() const {
            drawBackground();
            drawAxes();
            drawAxesLabels();
            drawPoints();
        }


    private:

        xcb_gcontext_t pointsContext;
        xcb_gcontext_t cleanPointsContext;


    };

}

#endif /* SRC_GRAPHICS_CHART_H_ */
