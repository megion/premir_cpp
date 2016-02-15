#include "ChartData.h"

namespace graphics {

    void ChartData::addPoints(Point *points, size_t len) {
        // 1. add point
        inpoints->pushRow(points, len);

        // 2. start coordinate point apply left-down corner = (x0, y0 + h0)
        if (inpoints->getRowSize() == 1 && len == 1) {
            // first point so simple do initialize
            Point &p = points[0];
            inrange.xMax = p.x;
            inrange.xMin = p.x;
            inrange.yMax = p.y;
            inrange.yMin = p.y;

            // start coordinates (x0, y0 + h0)
            xcb_point_t op[1];
            op[0].x = boundRect.x;
            op[0].y = (int16_t) (boundRect.y + boundRect.height);
            outpoints->pushRow(op, 1);
            return;
        }

        // create out points temp array
        utils::CArrayList<xcb_point_t> tempOutPoints(len);

        // if isDirty = true then should recalculate all points
        bool xIsDirty = false;
        bool yIsDirty = false;

        // 3. calculate input range
        for (size_t i = 0; i < len; ++i) {
            Point &p = points[i];
            if (p.x > inrange.xMax) {
                inrange.xMax = p.x;
                xIsDirty = true;
            } else if (p.x < inrange.xMin) {
                inrange.xMin = p.x;
                xIsDirty = true;
            }

            if (p.y > inrange.yMax) {
                inrange.yMax = p.y;
                yIsDirty = true;
            } else if (p.y < inrange.yMin) {
                inrange.yMin = p.y;
                yIsDirty = true;
            }
        }

        // 4. calculate line
        bool xIsConst = false;
        double ax = 0.0, bx = 0.0;
        if (inrange.xMax == inrange.xMin) {
            // x has not ever been change, set to x0
            for (size_t i = 0; i < len; ++i) {
                tempOutPoints[i].x = boundRect.x;
            }
            xIsConst = true;
        } else {
            // x line: xout = ax*xin + bx
            // ax = w0/(xmax-xmin); bx = x0 - ax*xmin
            double xdiff = inrange.xMax - inrange.xMin;
            ax = ((double) boundRect.width) / xdiff;
            bx = ((double) boundRect.x) - ax * inrange.xMin;
        }

        bool yIsConst = false;
        double ay = 0.0, by = 0.0;
        if (inrange.yMax == inrange.yMin) {
            // y has not ever been change, set to y0 + h0
            for (size_t i = 0; i < len; ++i) {
                tempOutPoints[i].y = boundRect.y + boundRect.height;
            }
            yIsConst = true;
        } else {
            // y line: yout = ay*yin + by
            // ay = -h0/(ymax-ymin); by = (y0+h0) - ay*ymin
            double ydiff = inrange.yMax - inrange.yMin;
            ay = -((double) boundRect.height) / ydiff;
            by = ((double) (boundRect.y + boundRect.height)) -
                 ay * inrange.yMin;
        }

        if (xIsConst && yIsConst) {
            outpoints->pushRow(tempOutPoints.getArray(), len);
            return;
        }

        // 5. update outpoints array
        if (!xIsDirty && !yIsDirty) {
            // range input values no changes so calculate only current point
            if (!xIsConst) {
                for (size_t i = 0; i < len; ++i) {
                    tempOutPoints[i].x = transformToOut(ax, bx, points[i].x);
                }
            }
            if (!yIsConst) {
                for (size_t i = 0; i < len; ++i) {
                    tempOutPoints[i].y = transformToOut(ay, by, points[i].y);
                }
            }
            outpoints->pushRow(tempOutPoints.getArray(), len);
        } else {
            outpoints->pushRow(tempOutPoints.getArray(), len);
            for (size_t r = 0; r < inpoints->getRowSize(); ++r) {
                utils::RDMatrix<bool, Point>::Row &inPoints =
                        inpoints->getRow(r);
                utils::RDMatrix<bool, xcb_point_t>::Row &outPoints =
                        outpoints->getRow(r);
                for (size_t c = 0; c < inPoints.colSize; ++c) {
                    // recalculate all if IsDirty else only last is recalculate
                    if (!xIsConst &&
                        (xIsDirty || (r == (inpoints->getRowSize() - 1)))) {
                        outPoints.points[c].x =
                                transformToOut(ax, bx, inPoints.points[c].x);
                    }
                    if (!yIsConst &&
                        (yIsDirty || (r == (inpoints->getRowSize() - 1)))) {
                        outPoints.points[c].y =
                                transformToOut(ay, by, inPoints.points[c].y);
                    }
                }

            }
            updateAxesLabel(ax, bx, ay, by);
        }

    }

    void ChartData::createAxesPoints() {
        // axis step
        int16_t step = 40;

        // vertical
        int16_t verCount = boundRect.width / step;
        xAxes = new utils::CArrayList<Axis>(verCount);
        int16_t ybottom = boundRect.y + boundRect.height;
        int16_t ytop = boundRect.y;
        for (int16_t i = 1; i <= verCount; ++i) {
            int16_t x = i * step + boundRect.x;
            Axis ax = {{{x, ytop}, {x, ybottom}}, {x, ybottom}, 0.0, true};
            xAxes->push(ax);
        }

        // horizontal
        int16_t horCount = boundRect.height / step;
        yAxes = new utils::CArrayList<Axis>(horCount);
        int16_t xleft = boundRect.x;
        int16_t xright = boundRect.x + boundRect.width;
        for (int16_t i = 1; i <= horCount; ++i) {
            int16_t y = boundRect.y + boundRect.height - i * step;
            Axis ax = {{{xleft, y}, {xright, y}}, {xleft, y}, 0.0, true};
            yAxes->push(ax);
        }
    }

    void ChartData::updateAxesLabel(double ax, double bx, double ay,
                                    double by) {
        for (int i = 0; i < xAxes->size(); ++i) {
            Axis &axis = (*xAxes)[i];
            axis.labelValue = transformToIn(ax, bx, axis.line[0].x);
            bool hideLabel = (i % 3 != 0);
            axis.hideLabel = hideLabel;
        }

        for (int i = 0; i < yAxes->size(); ++i) {
            Axis &axis = (*yAxes)[i];
            axis.labelValue = transformToIn(ay, by, axis.line[0].y);
            bool hideLabel = (i + 1) % 2 == 0;
            axis.hideLabel = hideLabel;
        }
    }

}

