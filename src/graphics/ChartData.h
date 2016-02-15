#ifndef SRC_GRAPHICS_CHART_DATA_H_
#define SRC_GRAPHICS_CHART_DATA_H_

#include <xcb/xcb.h>
#include <cmath>

//#include <cstdio>
//#include <exception>
//#include <stdexcept>
//#include <cerrno>
#include <iostream>
//#include <limits>

#include "utils/CArrayList.h"
#include "utils/RDMatrix.h"

namespace graphics {

/**
 * Data chart container.
 * Transform each input point {double, double} to output ploter coordinates
 */
    class ChartData {
    public:
        ChartData(const xcb_rectangle_t &_boundRect) :
                boundRect(_boundRect), inrange({0, 0, 0, 0}), //
                outpoints(new utils::RDMatrix<bool, xcb_point_t>()), //
                inpoints(new utils::RDMatrix<bool, Point>()) {
            createAxesPoints();
        }

        ~ChartData() {
            delete inpoints;
            delete xAxes;
            delete yAxes;
            delete outpoints;
        }

        struct Point {
            double x;
            double y;
        };

        struct Axis {
            xcb_point_t line[2];
            xcb_point_t labelPoint;
            double labelValue;
            bool hideLabel;
        };

        struct Range {
            double xMax;
            double yMax;
            double xMin;
            double yMin;

            Range(double _xMax, double _yMax, double _xMin, double _yMin) :
                    xMax(_xMax), yMax(_yMax), xMin(_xMin), yMin(_yMin) {
            }
        };

        utils::RDMatrix<bool, xcb_point_t> *getOutpoints() {
            return outpoints;
        }

        utils::CArrayList<Axis> *getXAxes() {
            return xAxes;
        }

        utils::CArrayList<Axis> *getYAxes() {
            return yAxes;
        }

        void addPoints(Point* points, size_t len);

        void updateAxesLabel(double ax, double bx, double ay, double by);

        void removeData() {
            inpoints->removeAll();
            outpoints->removeAll();
        }

    private:
        utils::RDMatrix<bool, Point> *inpoints;
        utils::RDMatrix<bool, xcb_point_t> *outpoints;

        utils::CArrayList<Axis> *yAxes;
        utils::CArrayList<Axis> *xAxes;
        xcb_rectangle_t boundRect;

        Range inrange;

        /**
         * Calculate value by formula (line) out = a*value + b
         */
        int transformToOut(double a, double b, double inValue) {
            return std::round(a * inValue + b);
        }

        double transformToIn(double a, double b, int16_t outValue) {
            return ((double) (outValue) - b) / a;
        }

        void createAxesPoints();

    };

}

#endif
