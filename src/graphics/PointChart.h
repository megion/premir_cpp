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
                Chart(_width, _height), currentBufSize(0) {
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

            // оптимизация выделения памяти
            getData()->getOutpoints()->setPointCapacityIncrease(40);
        }

        ~PointChart() {
            pointsContext = 0;
            cleanPointsContext = 0;
        }

        void drawPoints() const {
            for (size_t r = 0; r < data->getOutpoints()->getRowSize(); ++r) {
                utils::RDMatrix<bool, xcb_point_t>::Row &outPoint =
                        data->getOutpoints()->getRow(r);
                xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, window, pointsContext, outPoint.pointSize,
                               outPoint.points);
            }

        }

        void drawCleanPoints() const {
            for (size_t r = 0; r < data->getOutpoints()->getRowSize(); ++r) {
                utils::RDMatrix<bool, xcb_point_t>::Row &outPoint = data->getOutpoints()->getRow(r);
                xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, window, cleanPointsContext, outPoint.pointSize,
                               outPoint.points);
            }
        }

        /**
         * 1. draw clean current points
         * 2. add new points and update all points position
         * 3. draw updated current points
         */
        void redrawNewPoints(ChartData::Point *points, size_t len) const {
            drawCleanPoints();
            data->addPoints(0, points, len);
            drawPoints();

            if (data->size() % 36 == 0) { // оптимизация :)
                drawAxes();
                drawAxesLabels();
            }

            flush();
        }

        void redrawNewPoint(double x, double y) {
            drawCleanPoints();

            ChartData::Point points[1];
            points[0].x = x;
            points[0].y = y;
            data->addPoints(0, points, 1);
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

        const static size_t MAX_BUFF_SIZE = 36;
        size_t currentBufSize;
        ChartData::Point pointsBuff[MAX_BUFF_SIZE];

    };

}

#endif /* SRC_GRAPHICS_CHART_H_ */
