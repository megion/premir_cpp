#ifndef SRC_GRAPHICS_POINTCHART_H
#define SRC_GRAPHICS_POINTCHART_H

#include <xcb/xcb.h>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "ChartColormap.h"
#include "ChartData.h"
#include "Chart.h"

namespace graphics {

    class PointChart : public Chart<bool> {
    public:
        PointChart(bool _isFatPoint, uint16_t _width, uint16_t _height) :
                Chart(_width, _height), isFatPoint(_isFatPoint) {
            // points context
            pointsContext = xcb_generate_id(connection);
            uint32_t mask = XCB_GC_FOREGROUND;
            uint32_t values2[] = {colormap->getGreen()->pixel};
            xcb_create_gc(connection, pointsContext, screen->root, mask, values2);

            // clean points context
            cleanPointsContext = xcb_generate_id(connection);
            uint32_t values22[] = {screen->black_pixel};
            xcb_create_gc(connection, cleanPointsContext, screen->root, mask, values22);

            // оптимизация выделения памяти
            getData()->getOutpoints()->setPointCapacityIncrease(40);

            seriesColors = new utils::CArrayList<xcb_alloc_color_reply_t *>();
        }

        ~PointChart() {
            pointsContext = 0;
            cleanPointsContext = 0;
            delete seriesColors;
        }

        void pushSeriesColor(const Color& color) {
            xcb_alloc_color_reply_t * pColor = colormap->getColor(color);
            seriesColors->push(pColor);
        }

        void drawPoints(const xcb_drawable_t &pixmap) const {
            drawPointsOnContext(pixmap, pointsContext);
        }

        void drawCleanPoints(const xcb_drawable_t &pixmap) const {
            drawPointsOnContext(pixmap, cleanPointsContext);
        }

        void drawPointsOnContext(const xcb_drawable_t &pixmap, const xcb_gcontext_t &gContext) const {
            if (isFatPoint) {
                for (size_t r = 0; r < data->getOutpoints()->getRowSize(); ++r) {
                    utils::RDMatrix<bool, xcb_point_t>::Row &outPoint = data->getOutpoints()->getRow(r);
                    utils::CArrayList<xcb_arc_t> arcs(outPoint.pointSize, 1);
					arcs.setSize(outPoint.pointSize);
                    for (size_t i = 0; i < outPoint.pointSize; ++i) {
                        xcb_point_t &point = outPoint.points[i];
                        arcs[i].x = point.x - 2;
                        arcs[i].y = point.y - 2;
                        arcs[i].width = 4;
                        arcs[i].height = 4;
                        arcs[i].angle1 = 0;
                        arcs[i].angle2 = 360 << 6;
                    }

                    if (r<(seriesColors->size())) {
                        xcb_alloc_color_reply_t *cellColor = seriesColors->getArray()[r];
                        uint32_t values[] = {cellColor->pixel};
                        uint32_t mask = XCB_GC_FOREGROUND;
                        xcb_change_gc(connection, pointsContext, mask, values);
                    }

                    xcb_poly_arc(connection, pixmap, gContext, outPoint.pointSize, arcs.getArray());
                }
            } else {
                for (size_t r = 0; r < data->getOutpoints()->getRowSize(); ++r) {

                    utils::RDMatrix<bool, xcb_point_t>::Row &outPoint = data->getOutpoints()->getRow(r);

                    if (r<(seriesColors->size())) {
                        xcb_alloc_color_reply_t *cellColor = seriesColors->getArray()[r];
                        uint32_t values[] = {cellColor->pixel};
                        uint32_t mask = XCB_GC_FOREGROUND;
                        xcb_change_gc(connection, pointsContext, mask, values);
                    }

                    xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, pixmap, gContext, outPoint.pointSize,
                                   outPoint.points);
                }
            }

        }

        /**
         * 1. draw clean current points
         * 2. add new points and update all points position
         * 3. draw updated current points
         */
        void redrawNewPoints(size_t seriesIndex, ChartData<bool>::Point *points, size_t len) const {
            drawCleanPoints(window);
            data->addPoints(seriesIndex, points, len);
            drawPoints(window);

            if (data->size() % 36 == 0) { // оптимизация :)
                drawAxes(window);
                drawAxesLabels(window);
            }

            flush();
        }

        void redrawNewPoint(size_t seriesIndex, double x, double y) const {
            drawCleanPoints(window);

            ChartData<bool>::Point points[1];
            points[0].x = x;
            points[0].y = y;
            data->addPoints(seriesIndex, points, 1);
            drawPoints(window);

            if (data->size() % 36 == 0) { // оптимизация :)
                drawAxes(window);
                drawAxesLabels(window);
            }

            flush();
        }

        void draw(const xcb_drawable_t &pixmap) const {
            drawBackground(pixmap);
            drawAxes(pixmap);
            drawAxesLabels(pixmap);
            drawPoints(pixmap);
        }


    private:
        bool isFatPoint;

        xcb_gcontext_t pointsContext;
        xcb_gcontext_t cleanPointsContext;

        utils::CArrayList<xcb_alloc_color_reply_t *> *seriesColors;

    };

}

#endif
