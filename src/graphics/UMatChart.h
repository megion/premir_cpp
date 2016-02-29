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
    class UMatChart : public Chart<Out> {
    public:

        typedef utils::RMatrix<models::NeuronInfo, Out> UMatCodes;
        typedef typename ChartData<Out>::Point OutPoint;
        typedef typename utils::RDMatrix<Out, xcb_point_t> OutMatrix;
        typedef typename OutMatrix::Row OutRow;

        UMatChart(uint16_t _width, uint16_t _height) : Chart<Out>(_width, _height) {
            // arcs context
            uint32_t values2[] = {Chart<Out>::colormap->getGreen()->pixel};
            cellBorderContext = xcb_generate_id(Chart<Out>::connection);
            uint32_t mask = XCB_GC_FOREGROUND;
            xcb_create_gc(Chart<Out>::connection, cellBorderContext, Chart<Out>::screen->root,
                          mask, values2);

            uint32_t values22[] = {Chart<Out>::colormap->getGray()->pixel};
            uint32_t mask2 = XCB_GC_BACKGROUND;
            cellBackgroundContext = xcb_generate_id(Chart<Out>::connection);
            xcb_create_gc(Chart<Out>::connection, cellBackgroundContext, Chart<Out>::screen->root,
                          mask, values22);

            // создадим 1000 оттенков серого
//            Chart<Out>::colormap->createGrayColors(1000);
            Chart<Out>::colormap->createWavelengthColors(1000);
        }

        ~UMatChart() {
            cellBorderContext = 0;
            cellBackgroundContext = 0;
        }

        void draw(const xcb_pixmap_t& pixmap) const {
//            Chart<Out>::drawBackground();
//            drawAxes();
//            drawAxesLabels();
            drawUMat(pixmap);
            Chart<Out>::flush();
        }
        
        void addHexaUMatPoints(UMatCodes *uMatrix) {
            // TODO: необходимл заблокировать мьютекс перед измененем точек графика
            // иначе поток (событийный цикл draw event loop) иногда может читать изменяемые данные что может привести
            // к ошибкам
            std::lock_guard<std::mutex> guard(*Chart<Out>::changeDataMutex);

            UMatCodes &uvalue = (*uMatrix);
            size_t uxdim = uvalue.getRowSize();
            size_t uydim = uvalue.getColSize();

            double sin30 = 0.5;
            double cos30 = std::pow(3, 0.5) / 2.0;

            //double m = 1; // сторона многоугольника
            size_t index = 0;
            size_t k = 4;

            for (size_t i = 0; i < uxdim; i++) {
                double x0 = 0;
                double y0 = i * 2 * cos30;
                for (size_t j = 0, k = 4; j < uydim; j++) {
                    OutPoint points[7];

                    points[0].x = x0;
                    points[0].y = y0;

                    points[1].x = x0 + sin30;
                    points[1].y = y0 + cos30;

                    points[2].x = points[1].x + 1.0;
                    points[2].y = points[1].y;

                    points[3].x = points[0].x + 2.0;
                    points[3].y = points[0].y;

                    points[4].x = points[2].x;
                    points[4].y = points[0].y - cos30;

                    points[5].x = points[1].x;
                    points[5].y = points[4].y;

                    points[6].x = points[0].x;
                    points[6].y = points[0].y;

                    Chart<Out>::data->addPoints(index, points, 7);
                    (*Chart<Out>::data->getOutpoints())[index].data = uvalue[i][j];

                    // k = 2,2,4,4,2,2,4,4...
                    if (j % 2 == 0) {
                        if (k == 4) {
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

        void drawUMat(const xcb_pixmap_t& pixmap) const {
            OutMatrix* outMatrix = Chart<Out>::data->getOutpoints();
            size_t rowSize = outMatrix->getRowSize();
            for (size_t r = 0; r < rowSize; ++r) {
                OutRow &outRow = (*outMatrix)[r];
//                xcb_poly_line(Chart<Out>::connection, XCB_COORD_MODE_ORIGIN,
//                              Chart<Out>::window, cellBorderContext, outRow.pointSize,
//                              outRow.points);
//                if (r % 2 == 0) {
//                    uint32_t values[] = {colormap->getGray()->pixel};
//                    uint32_t mask = XCB_GC_FOREGROUND;
//                    xcb_change_gc(connection, cellBackgroundContext, mask, values);
//                } else {
//
//                }
//                std::cout<<"outRow.pointSize: "<<outRow.pointSize<<std::endl;
//                uint32_t values[] = {Chart<Out>::colormap->getScaledGrayColor(outRow.data)->pixel};
                uint32_t values[] = {Chart<Out>::colormap->getWavelengthColor(outRow.data)->pixel};
                uint32_t mask = XCB_GC_FOREGROUND;
                xcb_change_gc(Chart<Out>::connection, cellBackgroundContext, mask, values);
                xcb_fill_poly(Chart<Out>::connection, pixmap, cellBackgroundContext,
                              XCB_POLY_SHAPE_CONVEX, XCB_COORD_MODE_ORIGIN, outRow.pointSize, outRow.points);

            }
        }

    private:

        xcb_gcontext_t cellBorderContext;
        xcb_gcontext_t cellBackgroundContext;


    };

}

#endif
