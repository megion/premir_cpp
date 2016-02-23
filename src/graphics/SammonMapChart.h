#ifndef SRC_GRAPHICS_SAMMONMAPCHART_H_
#define SRC_GRAPHICS_SAMMONMAPCHART_H_

#include <xcb/xcb.h>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "ChartColormap.h"
#include "ChartData.h"
#include "Chart.h"

namespace graphics {

    class SammonMapChart : public Chart {
    public:
        SammonMapChart(long _xdim, uint16_t _width, uint16_t _height) :
                Chart(_width, _height), xdim(_xdim) {
            // arcs context
            uint32_t values2[] = {colormap->getGreen()->pixel};
            arcsContext = xcb_generate_id(connection);
            uint32_t mask = XCB_GC_FOREGROUND;
            xcb_create_gc(connection, arcsContext, screen->root,
                          mask, values2);

            uint32_t values22[] = {screen->black_pixel};
            cleanArcsContext = xcb_generate_id(connection);
            xcb_create_gc(connection, cleanArcsContext, screen->root,
                          mask, values22);
        }

        ~SammonMapChart() {
            arcsContext = 0;
            cleanArcsContext = 0;
        }

        void draw() const {
            drawBackground();
//            drawAxes();
            drawAxesLabels();
            drawArcs();
        }

        void drawArcs() const {
            utils::RDMatrix<bool, xcb_point_t> yPolyLines;
            xcb_point_t xPolyLines[xdim];
            size_t xIndex = 0;

            for (size_t r = 0; r < data->getOutpoints()->getRowSize(); ++r) {
                utils::RDMatrix<bool, xcb_point_t>::Row &outRow =
                        data->getOutpoints()->getRow(r);
                xcb_arc_t arcs[outRow.pointSize];

                for (size_t i = 0; i < outRow.pointSize; ++i) {
                    xcb_point_t &point = outRow.points[i];
                    arcs[i].x = point.x - 2;
                    arcs[i].y = point.y - 2;
                    arcs[i].width = 4;
                    arcs[i].height = 4;
                    arcs[i].angle1 = 0;
                    arcs[i].angle2 = 360 << 6;

                    xPolyLines[xIndex].x = point.x;
                    xPolyLines[xIndex].y = point.y;

                    yPolyLines.writeToEndRow(xIndex, point);

                    if (xIndex == xdim - 1) {
                        xcb_poly_line(connection, XCB_COORD_MODE_ORIGIN,
                                      window, arcsContext, xdim,
                                      xPolyLines);
                        xIndex = 0;
                    } else {
                        ++xIndex;
                    }
                }

                xcb_poly_arc(connection, window, arcsContext,
                             outRow.pointSize, arcs);
            }

            for (size_t r = 0; r < yPolyLines.getRowSize(); ++r) {
                utils::RDMatrix<bool, xcb_point_t>::Row &row =
                        yPolyLines.getRow(r);
                xcb_poly_line(connection, XCB_COORD_MODE_ORIGIN,
                              window, arcsContext, row.pointSize,
                              row.points);
            }

            flush();
        }

    private:

        long xdim;
        xcb_gcontext_t arcsContext;
        xcb_gcontext_t cleanArcsContext;


    };

}

#endif /* SRC_GRAPHICS_CHART_H_ */
