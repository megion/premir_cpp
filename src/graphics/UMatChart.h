#ifndef SRC_GRAPHICS_UMATCHART_H_
#define SRC_GRAPHICS_UMATCHART_H_

#include <xcb/xcb.h>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <cmath>

#include "ChartColormap.h"
#include "ChartData.h"
#include "Chart.h"
#include "models/DataSample.h"
#include "utils/RMatrix.h"

namespace graphics {

    struct UMatCell {
        double colorValue;
    };

    template<typename Out>
    class UMatChart : public Chart<bool> {
    public:

        typedef utils::RMatrix<models::NeuronInfo, Out> UMatCodes;

        UMatChart(uint16_t _width, uint16_t _height) :
                Chart(_width, _height) {
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

        ~UMatChart() {
            arcsContext = 0;
            cleanArcsContext = 0;
        }

        void draw() const {
            drawBackground();
//            drawAxes();
//            drawAxesLabels();
            drawUMat();
        }

        void addHexaUMatPoints(UMatCodes* uMatrix) {
            UMatCodes &uvalue = (*uMatrix);
            size_t uxdim = uvalue.getRowSize();
            size_t uydim = uvalue.getColSize();

            double sin30 = 0.5;
            double cos30 = std::pow(3, 0.5)/2.0;

            //double m = 1; // сторона многоугольника
            size_t index = 0;
            size_t k = 4;

            for (size_t i = 0; i < uxdim; i++) {
                double x0 = 0;
                double y0 = i*2*cos30;
                for (size_t j = 0, k = 4; j < uydim; j++) {
                    ChartData<bool>::Point points[7];

                    points[0].x = x0;
                    points[0].y = y0;

                    points[1].x = x0 + sin30;
                    points[1].y = y0 - cos30;

                    points[2].x = points[1].x + 1.0;
                    points[2].y = points[1].y;

                    points[3].x = points[0].x + 2.0;
                    points[3].y = points[0].y;

                    points[4].x = points[2].x;
                    points[4].y = points[0].y + cos30;

                    points[5].x = points[1].x;
                    points[5].y = points[4].y;

                    points[6].x = points[0].x;
                    points[6].y = points[0].y;

                    data->addPoints(index, points, 7);


                    // k = 2,2,4,4,2,2,4,4...
                    if (j%2==0) {
                         if (k==4) {
                             k = 2;
                         } else {
                             k = 4;
                         }
                    }

                    x0 = points[k].x;
                    y0 = points[k].y;

                    ++index;
                }

            }
        }

        void drawUMat() const {
            for (size_t r = 0; r < data->getOutpoints()->getRowSize(); ++r) {
                utils::RDMatrix<bool, xcb_point_t>::Row &outRow =
                        data->getOutpoints()->getRow(r);
                xcb_poly_line(connection, XCB_COORD_MODE_ORIGIN,
                              window, arcsContext, outRow.pointSize,
                              outRow.points);
            }
        }

    private:

        xcb_gcontext_t arcsContext;
        xcb_gcontext_t cleanArcsContext;


    };

}

#endif
