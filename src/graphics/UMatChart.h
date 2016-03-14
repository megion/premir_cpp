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
#include "models/models.h"
#include "utils/RMatrix.h"
#include "utils/HashMapArray.h"

namespace graphics {

    template<typename Out>
    struct UMatCell {
        Out uValue;
        double labelColor;
        bool useLabelColor;
    };

    template<typename Out, typename Label>
    class UMatChart : public Chart<UMatCell<Out>> {
    public:

        typedef utils::RMatrix<models::NeuronInfo, Out> UMatCodes;
        typedef typename ChartData<UMatCell<Out>>::Point OutPoint;
        typedef utils::RDMatrix<UMatCell<Out>, xcb_point_t> OutMatrix;
        typedef typename OutMatrix::Row OutRow;

        typedef utils::HashMapArray<Label, models::LabelInfo> LabelsMatrix;
        typedef typename utils::Entry<Label, models::LabelInfo> LabelsEntry;
        typedef typename utils::R3DMatrix<bool, bool, LabelsEntry>::Row LabelsRow;
        typedef typename utils::R3DMatrix<bool, bool, LabelsEntry>::Cell LabelsCell;

        UMatChart(uint16_t _width, uint16_t _height) : Chart<UMatCell<Out>>(_width, _height) {
            // arcs context
            uint32_t values2[] = {Chart<UMatCell<Out>>::colormap->getGreen()->pixel};
            cellBorderContext = xcb_generate_id(Chart<UMatCell<Out>>::connection);
            uint32_t mask = XCB_GC_FOREGROUND;
            xcb_create_gc(Chart<UMatCell<Out>>::connection, cellBorderContext, Chart<UMatCell<Out>>::screen->root,
                          mask, values2);

            uint32_t values22[] = {Chart<UMatCell<Out>>::colormap->getGray()->pixel};
            uint32_t mask2 = XCB_GC_BACKGROUND;
            cellBackgroundContext = xcb_generate_id(Chart<UMatCell<Out>>::connection);
            xcb_create_gc(Chart<UMatCell<Out>>::connection, cellBackgroundContext, Chart<UMatCell<Out>>::screen->root,
                          mask, values22);

            // создадим 1000 оттенков серого
            grayColors = Chart<UMatCell<Out>>::colormap->createGrayColors(1000);
            labelsColors = Chart<UMatCell<Out>>::colormap->createCubehelixColors(1000, 2.0, 0.5, 2.0, 1.0);
        }

        ~UMatChart() {
            cellBorderContext = 0;
            cellBackgroundContext = 0;

            if(grayColors) {
                Chart<UMatCell<Out>>::colormap->freeColors(grayColors);
                grayColors = nullptr;
            }

            if(labelsColors) {
                Chart<UMatCell<Out>>::colormap->freeColors(labelsColors);
                labelsColors = nullptr;
            }
        }

        void draw(const xcb_drawable_t& pixmap) const {
            Chart<UMatCell<Out>>::drawBackground(pixmap);
//            drawAxes();
//            drawAxesLabels();
            drawUMat(pixmap);
            Chart<UMatCell<Out>>::flush();
        }

        void setUMatWinnerLabelsForKey(LabelsMatrix* winnerLabels, const Label& key, size_t xdim) {
//            (*Chart<Out>::data->getOutpoints())[index].data = uvalue[i][j];


            for (size_t r = 0; r < winnerLabels->getMatrix()->getRowSize(); ++r) {

                // перевод winnerIndex (r) -> umatIndex (data->getOutpoints)
                size_t xw = (r) % (xdim);
                size_t yw = (r) / (xdim);
                size_t umatIndex = xw + yw*(xdim);
//                std::cout << "umatIndex: " << umatIndex << std::endl;

//                models::LabelInfo* lInfo = winnerLabels->getValue(r, key);
//                if (lInfo) {
//                    UMatCell<Out>& cell = (*Chart<UMatCell<Out>>::data->getOutpoints())[2*r].data;
//
//                    if (lInfo->scaledCount > 0.9) {
//                        cell.labelColor = lInfo->scaledCount;
//                        cell.useLabelColor = true;
//                        std::cout << "cell.labelColor: " << cell.labelColor << std::endl;
//                    }
//
////                    if (cell.useLabelColor) {
////                        // TODO: назначаем самый
////                        if (cell.labelColor < lInfo->scaledCount) {
////                            cell.labelColor = lInfo->scaledCount;
////                        }
////                    }
//                }
                LabelsRow &row = (*winnerLabels->getMatrix())[r];
                for (size_t c = 0; c < row.cellSize; ++c) {
                    LabelsCell &cell = row[c];
                    for (size_t p = 0; p < cell.pointSize; ++p) {
                        LabelsEntry &e = cell[p];



                        UMatCell<Out>& ucell = (*Chart<UMatCell<Out>>::data->getOutpoints())[2*umatIndex].data;

//                        if (e.value.scaledCount > 0.9) {
                            ucell.labelColor = e.value.scaledCount;
                            ucell.useLabelColor = true;
//                            std::cout << "ucell.labelColor: " << ucell.labelColor << std::endl;
//                        }
                    }
                }
            }

        }
        
        void addHexaUMatPoints(UMatCodes *uMatrix) {
            // TODO: необходимл заблокировать мьютекс перед измененем точек графика
            // иначе поток (событийный цикл draw event loop) иногда может читать изменяемые данные что может привести
            // к ошибкам
            std::lock_guard<std::mutex> guard(*Chart<UMatCell<Out>>::changeDataMutex);

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

                    Chart<UMatCell<Out>>::data->addPoints(index, points, 7);
                    UMatCell<Out>& cell = (*Chart<UMatCell<Out>>::data->getOutpoints())[index].data;
                    cell.uValue = uvalue[i][j];
                    cell.useLabelColor = false;

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

        void drawUMat(const xcb_drawable_t& pixmap) const {
            OutMatrix* outMatrix = Chart<UMatCell<Out>>::data->getOutpoints();
            size_t rowSize = outMatrix->getRowSize();
            for (size_t r = 0; r < rowSize; ++r) {
                OutRow &outRow = (*outMatrix)[r];

                xcb_alloc_color_reply_t* cellColor;
                if (outRow.data.useLabelColor) {
//                    cellColor = Chart<UMatCell<Out>>::colormap->getScaledColor(labelsColors, outRow.data.labelColor);
                    cellColor = Chart<UMatCell<Out>>::colormap->getGreen();
                } else {
                    cellColor = Chart<UMatCell<Out>>::colormap->getScaledColor(grayColors, outRow.data.uValue);
                }

                uint32_t values[] = {cellColor->pixel};
//                uint32_t values[] = {Chart<Out>::colormap->getWavelengthColor(outRow.data)->pixel};
                uint32_t mask = XCB_GC_FOREGROUND;
                xcb_change_gc(Chart<UMatCell<Out>>::connection, cellBackgroundContext, mask, values);
                xcb_fill_poly(Chart<UMatCell<Out>>::connection, pixmap, cellBackgroundContext,
                              XCB_POLY_SHAPE_CONVEX, XCB_COORD_MODE_ORIGIN, outRow.pointSize, outRow.points);

            }
        }

    private:

        xcb_gcontext_t cellBorderContext;
        xcb_gcontext_t cellBackgroundContext;

        // коллекция серых цветов: от белого до черного
        utils::CArrayList<xcb_alloc_color_reply_t *> *grayColors;
        utils::CArrayList<xcb_alloc_color_reply_t *> *labelsColors;

//        // цвета радуги :)
//        utils::CArrayList<xcb_alloc_color_reply_t *> *waveColors;

    };

}

#endif
