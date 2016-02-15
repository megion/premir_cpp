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

        struct NeighborLine {
            xcb_point_t line[2];
        };

        void draw() const {
            drawBackground();
//            drawAxes();
            drawAxesLabels();
            drawArcs();
        }

        void drawArcs() const {
            NeighborLine yNeighborPolyLines[xdim];
            xcb_point_t xPolyLines[xdim];
            size_t xIndex = 0;
            size_t yIndex = 0;
            size_t cnt = 0;

            for (size_t r = 0; r < data->getOutpoints()->getRowSize(); ++r) {
                utils::RDMatrix<bool, xcb_point_t>::Row &outRow =
                        data->getOutpoints()->getRow(r);
                xcb_arc_t arcs[outRow.colSize];

                for (size_t i = 0; i < outRow.colSize; ++i) {
                    xcb_point_t &point = outRow.points[i];
                    arcs[i].x = point.x - 2;
                    arcs[i].y = point.y - 2;
                    arcs[i].width = 4;
                    arcs[i].height = 4;
                    arcs[i].angle1 = 0;
                    arcs[i].angle2 = 360 << 6;

                    xPolyLines[xIndex].x = point.x;
                    xPolyLines[xIndex].y = point.y;

                    yNeighborPolyLines[xIndex].line[yIndex].x = point.x;
                    yNeighborPolyLines[xIndex].line[yIndex].y = point.y;
                    if (cnt >= xdim) {
                        xcb_poly_line(connection, XCB_COORD_MODE_ORIGIN,
                                      window, arcsContext, 2,
                                      yNeighborPolyLines[xIndex].line);
                    }

                    if (xIndex == xdim - 1) {
                        xcb_poly_line(connection, XCB_COORD_MODE_ORIGIN,
                                      window, arcsContext, xdim,
                                      xPolyLines);
                        xIndex = 0;
                        if (yIndex == 1) {
                            yIndex = 0;
                        } else {
                            ++yIndex;
                        }
                    } else {
                        ++xIndex;
                    }
                    ++cnt;
                }

                xcb_poly_arc(connection, window, arcsContext,
                             outRow.colSize, arcs);
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
