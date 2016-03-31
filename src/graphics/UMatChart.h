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

    /**
     * Transform label to color
     */
    template<typename Label>
    class LabelColorMapper {
    public:
        virtual Color labelToColor(const Label &label) const = 0;
    };

    class CellColorMapper {
    public:
        virtual Color getColorByCellValue(const double &cellValue) const = 0;
    };

    class CubehelixCellColorMapper : public CellColorMapper {
    public:
        CubehelixCellColorMapper(size_t _colorSize, double _start, double _rots, double _hue, double _gamma) :
                colorSize(_colorSize), start(_start), rots(_rots), hue(_hue), gamma(_gamma) {
        }

        Color getColorByCellValue(const double &cellValue) const {
            size_t index = (colorSize - 1) * cellValue;
            Color c;
            calculateCubehelixColor(index, 0, colorSize - 1, c.r, c.g, c.b, start, rots, hue, gamma);
            return c;
        }

    private:
        size_t colorSize;
        double start, rots, hue, gamma;
    };

    class DefaultCellColorMapper : public CellColorMapper {
    public:
        DefaultCellColorMapper(size_t _colorSize) : colorSize(_colorSize) {
        }

        Color getColorByCellValue(const double &cellValue) const {
            size_t index = (colorSize - 1) * cellValue;
            uint16_t step = 65535 / colorSize;
            uint16_t colComp = index * step; // компонента цвета
            Color c;
            c.r = colComp;
            c.g = colComp;
            c.b = colComp;
            return c;
        }

    private:
        size_t colorSize;
    };

    struct UMatCell {
        double uValue;
        xcb_alloc_color_reply_t *color;
    };

    template<typename Label>
    class UMatChart : public Chart<UMatCell> {
    public:

        typedef utils::RMatrix<models::NeuronInfo, double> UMatCodes;
        typedef typename ChartData<UMatCell>::Point OutPoint;
        typedef utils::RDMatrix<UMatCell, xcb_point_t> OutMatrix;
        typedef typename OutMatrix::Row OutRow;

        typedef utils::HashMapArray<Label, models::LabelInfo> LabelsMatrix;
        typedef typename utils::Entry<Label, models::LabelInfo> LabelsEntry;
        typedef typename utils::R3DMatrix<bool, bool, LabelsEntry>::Row LabelsRow;
        typedef typename utils::R3DMatrix<bool, bool, LabelsEntry>::Cell LabelsCell;

        UMatChart(uint16_t _width, uint16_t _height, CellColorMapper *_cellColorMapper = nullptr)
                : Chart<UMatCell>(_width, _height), cellColorMapper(_cellColorMapper) {
            // arcs context
            uint32_t values2[] = {Chart<UMatCell>::colormap->getGreen()->pixel};
            cellBorderContext = xcb_generate_id(Chart<UMatCell>::connection);
            uint32_t mask = XCB_GC_FOREGROUND;
            xcb_create_gc(Chart<UMatCell>::connection, cellBorderContext, Chart<UMatCell>::screen->root,
                          mask, values2);

            uint32_t values22[] = {Chart<UMatCell>::colormap->getGray()->pixel};
            uint32_t mask2 = XCB_GC_BACKGROUND;
            cellBackgroundContext = xcb_generate_id(Chart<UMatCell>::connection);
            xcb_create_gc(Chart<UMatCell>::connection, cellBackgroundContext, Chart<UMatCell>::screen->root,
                          mask, values22);

            if (!cellColorMapper) {
                size_t colorSize = 1000;
                cellColorMapper = new DefaultCellColorMapper(colorSize);
                useDefaultCellColorMapper = true;
            } else {
                useDefaultCellColorMapper = false;
            }


            // создадим 1000 оттенков серого
//            grayColors = Chart<UMatCell<Out>>::colormap->createGrayColors(1000);
//            grayColors = Chart<UMatCell<Out>>::colormap->createCubehelixColors(1000, 0.5, -1.5, 1.0, 1.0);
            labelsColors = Chart<UMatCell>::colormap->createWavelengthColors(1000);
        }

        ~UMatChart() {
            cellBorderContext = 0;
            cellBackgroundContext = 0;

//            if(grayColors) {
//                Chart<UMatCell<Out>>::colormap->freeColors(grayColors);
//                grayColors = nullptr;
//            }

            if (labelsColors) {
                Chart<UMatCell>::colormap->freeColors(labelsColors);
                labelsColors = nullptr;
            }

            if (useDefaultCellColorMapper) {
                delete cellColorMapper;
            }
        }

        void draw(const xcb_drawable_t &pixmap) const {
            Chart<UMatCell>::drawBackground(pixmap);
//            drawAxes();
//            drawAxesLabels();
            drawUMat(pixmap);
            Chart<UMatCell>::flush();
        }

        /**
         * перевод winnerIndex (r) -> umatIndex (data->getOutpoints)
         */
        size_t toUmatIndexFromWinnerIndex(const size_t &wIndex, const size_t &xdim, const size_t &ydim) {
            size_t uxdim = 2 * xdim - 1;
            size_t uydim = 2 * ydim - 1;

            // TODO: используем ydim т.к. umat при построении была перевернута т.е. построение по колонкам,
            // а не по строкам (так реализован алгоритм U-Matrix в SOM-pak)
            size_t yw = wIndex / xdim;
            size_t xw = wIndex % xdim;
            // ui = 2(i%xdim)uydim + 2(i/xdim)
            size_t uIndex = 2 * xw * uydim + 2 * yw;
            return uIndex;
        }

        void setAllLabels(LabelsMatrix *winnerLabels, size_t xdim, size_t ydim, LabelColorMapper<Label> *mapper,
                          double keyThreshold = -1.0) {
            for (size_t r = 0; r < winnerLabels->getMatrix()->getRowSize(); ++r) {

                size_t ui = toUmatIndexFromWinnerIndex(r, xdim, ydim);

                LabelsRow &row = (*winnerLabels->getMatrix())[r];
                for (size_t c = 0; c < row.cellSize; ++c) {
                    LabelsCell &cell = row[c];
                    for (size_t p = 0; p < cell.pointSize; ++p) {
                        LabelsEntry &e = cell[p];
                        UMatCell &ucell = (*Chart<UMatCell>::data->getOutpoints())[ui].data;
                        if (keyThreshold < 0 || e.value.scaledCount > keyThreshold) {
//                            ucell.labelColor = e.value.scaledCount;
//                            ucell.useLabelColor = true;

                            if (mapper) {
                                ucell.color = Chart<UMatCell>::colormap->getColor(mapper->labelToColor(e.key));
                            } else {
                                ucell.color = Chart<UMatCell>::colormap->getGreen();
                            }
                        } else {
//                            ucell.labelColor = e.value.scaledCount;
                            ucell.color = Chart<UMatCell>::colormap->getColor(cellColorMapper->getColorByCellValue(
                                    ucell.uValue));//nullptr;
//                            ucell.useLabelColor = false;
                        }
                    }
                }
            }

        }

        /**
         * set label
         */
        void setLabelsForKey(LabelsMatrix *winnerLabels, const Label &key, size_t xdim, size_t ydim,
                             const Color &colorLabel, double keyThreshold = -1.0) {
            for (size_t r = 0; r < winnerLabels->getMatrix()->getRowSize(); ++r) {
                size_t ui = toUmatIndexFromWinnerIndex(r, xdim, ydim);
                models::LabelInfo *lInfo = winnerLabels->getValue(r, key);
                UMatCell &cell = (*Chart<UMatCell>::data->getOutpoints())[ui].data;
                if (lInfo) {
                    if (keyThreshold < 0 || lInfo->scaledCount > keyThreshold) {
//                        cell.labelColor = lInfo->scaledCount;
//                        cell.useLabelColor = true;
                        cell.color = Chart<UMatCell>::colormap->getColor(colorLabel);
                    } else {
                        cell.color = Chart<UMatCell>::colormap->getColor(
                                cellColorMapper->getColorByCellValue(cell.uValue));//nullptr;
//                        cell.useLabelColor = false;
                    }
                } else {
//                    cell.useLabelColor = false;
                }
            }

        }

        void addHexaUMatPoints(UMatCodes *uMatrix) {
            // TODO: необходимл заблокировать мьютекс перед измененем точек графика
            // иначе поток (событийный цикл draw event loop) иногда может читать изменяемые данные что может привести
            // к ошибкам
            std::lock_guard<std::mutex> guard(*Chart<UMatCell>::changeDataMutex);

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

                    Chart<UMatCell>::data->addPoints(index, points, 7);
                    UMatCell &cell = (*Chart<UMatCell>::data->getOutpoints())[index].data;
                    cell.uValue = uvalue[i][j];
//                    cell.useLabelColor = false;
                    cell.color = Chart<UMatCell>::colormap->getColor(cellColorMapper->getColorByCellValue(
                            cell.uValue));

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

        void drawUMat(const xcb_drawable_t &pixmap) const {
            OutMatrix *outMatrix = Chart<UMatCell>::data->getOutpoints();
            size_t rowSize = outMatrix->getRowSize();
            for (size_t r = 0; r < rowSize; ++r) {
                OutRow &outRow = (*outMatrix)[r];

                xcb_alloc_color_reply_t *cellColor = outRow.data.color;
//                if (outRow.data.useLabelColor) {
//                    cellColor = Chart<UMatCell<Out>>::colormap->getScaledColor(labelsColors, outRow.data.labelColor);
//                    cellColor = Chart<UMatCell<Out>>::colormap->getGreen();

//                cellColor = outRow.data.color;
//                } else {
//                    cellColor = Chart<UMatCell<Out>>::colormap->getColor(cellColorMapper->getColorByCellValue(
//                            outRow.data.uValue));//Chart<UMatCell<Out>>::colormap->getScaledColor(grayColors, outRow.data.uValue);
//                }

                uint32_t values[] = {cellColor->pixel};
//                uint32_t values[] = {Chart<Out>::colormap->getWavelengthColor(outRow.data)->pixel};
                uint32_t mask = XCB_GC_FOREGROUND;
                xcb_change_gc(Chart<UMatCell>::connection, cellBackgroundContext, mask, values);
                xcb_fill_poly(Chart<UMatCell>::connection, pixmap, cellBackgroundContext,
                              XCB_POLY_SHAPE_CONVEX, XCB_COORD_MODE_ORIGIN, outRow.pointSize, outRow.points);

            }
        }

    private:

        xcb_gcontext_t cellBorderContext;
        xcb_gcontext_t cellBackgroundContext;

        // коллекция серых цветов: от белого до черного
//        utils::CArrayList<xcb_alloc_color_reply_t *> *grayColors;
        utils::CArrayList<xcb_alloc_color_reply_t *> *labelsColors;

        CellColorMapper *cellColorMapper;
        bool useDefaultCellColorMapper;

//        // цвета радуги :)
//        utils::CArrayList<xcb_alloc_color_reply_t *> *waveColors;

    };

}

#endif
