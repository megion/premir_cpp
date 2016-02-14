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
        SammonMapChart(uint16_t _width = 400, uint16_t _height = 260) :
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

        ~SammonMapChart() {
            arcsContext = 0;
            cleanArcsContext = 0;
        }

        void draw() const {
            drawBackground();
//            drawAxes();
            drawAxesLabels();
//            drawPoints();
            drawArcs();
        }

        void drawArcs() const {
            // TODO: необходимо заблокировать изменение данных перед рисованием
            // и разблокировать после или создать локальную копию данных (на чтение)
            // для которых идет рисование

            size_t len = data->getOutpoints()->size();
            xcb_arc_t arcs[len];
            xcb_point_t *outPoints = data->getOutpoints()->getArray();
            for (size_t i = 0; i < len; ++i) {
                xcb_point_t &point = outPoints[i];
                arcs[i].x = point.x - 2;
                arcs[i].y = point.y - 2;
                arcs[i].width = 4;
                arcs[i].height = 4;
                arcs[i].angle1 = 0;
                arcs[i].angle2 = 360 << 6;
            }
            xcb_poly_arc(connection, window,
                         arcsContext,
                         len, arcs);
            flush();
        }

    private:

        xcb_gcontext_t arcsContext;
        xcb_gcontext_t cleanArcsContext;


    };

}

#endif /* SRC_GRAPHICS_CHART_H_ */
