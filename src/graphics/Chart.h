#ifndef SRC_GRAPHICS_CHART_H_
#define SRC_GRAPHICS_CHART_H_

#include <xcb/xcb.h>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <mutex>

#include "ChartColormap.h"
#include "ChartData.h"
#include "StoreImage.h"

namespace graphics {

    template<typename R>
    class Chart {
    public:

        typedef typename ChartData<R>::Axis ChartAxis;

        Chart(uint16_t _width = 400, uint16_t _height = 260) :
                connection(xcb_connect(NULL, NULL)), width(_width), height(_height),
                changeDataMutex(new std::mutex()) {
            std::cout << "Create Chart" << std::endl;
            if (xcb_connection_has_error(connection)) {
                throw std::runtime_error("Cannot open display");
            }

            int16_t xr = 20;
            int16_t yr = 20;
            uint16_t wr = xr + width;
            uint16_t hr = yr + height;
            rectangle = {xr, yr, wr, hr};

            // get the first screen
            screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

            colormap = new ChartColormap(connection, screen, window);

            int16_t xr2 = xr + 10;
            int16_t yr2 = yr + 10;
            uint16_t wr2 = wr - 20;
            uint16_t hr2 = hr - 20;
            xcb_rectangle_t rectanglePoints = {xr2, yr2, wr2, hr2};
            data = new ChartData<R>(rectanglePoints);

            createContexts();

            // create window
            window = xcb_generate_id(connection);
            uint32_t values[2];
            values[0] = screen->white_pixel;
            values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;
            xcb_create_window(connection, XCB_COPY_FROM_PARENT, window,
                              screen->root, 10, 10,
                              2 * rectangle.x + rectangle.width,
                              2 * rectangle.y + rectangle.height, 1,
                              XCB_WINDOW_CLASS_INPUT_OUTPUT,
                              screen->root_visual,
                              XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK, values);

            // set title
            setWindowTitle("Chart");
        }

        ~Chart() {
            std::cout << "Destroy Chart" << std::endl;
            backgroundContext = 0;
            screen = nullptr;
            delete colormap;
            delete data;
            delete changeDataMutex;
            xcb_disconnect(connection);
        }

        ChartData<R> *getData() const {
            return data;
        }

        /**
         * Безопасноя отрисовка - перед функцией рисования производится блокировка мьютекса.
         */
        void drawSafely() const {
//            std::cout<< "lock before draw"<< std::endl;
            std::lock_guard<std::mutex> guard(*changeDataMutex);
            draw();
        }

        void removeDataSafely() {
//            std::cout<< "lock before remove data"<< std::endl;
            std::lock_guard<std::mutex> guard(*changeDataMutex);
            data->removeData();
        }

        /**
         * Show chart and run chart event loop in other thread
         */
        void runChart() const {
            // show window
            xcb_map_window(connection, window);
            xcb_flush(connection);

            int done = 0;
            xcb_generic_event_t *e;
            /* event loop */
            while (!done && (e = xcb_wait_for_event(connection))) {
                switch (e->response_type & ~0x80) {
                    case XCB_EXPOSE: /* отрисовать или перерисовать окно */
//			std::cout << "XCB_EXPOSE event" << std::endl;

//                        draw();
                        drawSafely();

                        xcb_flush(connection);
                        break;
//		case XCB_KEY_PRESS: /* выйти по нажатию клавиши */
//			done = 1;
//			break;
                }
                free(e);
            }
            std::cout << "chart closed" << std::endl;
        }

        virtual void draw() const = 0;

        void drawAxes() const {
            // vertical lines
            for (ChartAxis &axis : (*data->getYAxes())) {
                drawAxis(axis);
            }

            // horizontal lines
            for (ChartAxis &axis : (*data->getXAxes())) {
                drawAxis(axis);
            }
        }

        void drawAxesLabels() const {
            // vertical labels
            for (ChartAxis &axis : (*data->getYAxes())) {
                drawAxisLabel(axis);
            }

            // horizontal labels
            for (ChartAxis &axis : (*data->getXAxes())) {
                drawAxisLabel(axis);
            }
        }

        void drawAxis(const ChartAxis &axis) const {
            // 1. clean text labels - draw black rectangle
            xcb_poly_line(connection, XCB_COORD_MODE_ORIGIN, window, axisContext, 2,
                          axis.line);
        }

        void drawAxisLabel(const ChartAxis &axis) const {
            // 1. clean text labels - draw black rectangle
            if (!axis.hideLabel) {
                uint16_t labelHeight = 16;
                uint16_t labelWidth = 80;
                int16_t labelRectTop = axis.labelPoint.y - labelHeight;
                xcb_rectangle_t labelRect = {axis.labelPoint.x, labelRectTop,
                                             labelWidth, labelHeight};
                xcb_poly_fill_rectangle(connection, window, cleanAxesLabelContext,
                                        1,
                                        &labelRect);
                char buffer[32];
                snprintf(buffer, sizeof(buffer), "%g", axis.labelValue);
                xcb_image_text_8(connection, strlen(buffer), window,
                                 axisFontContext,
                                 axis.labelPoint.x, axis.labelPoint.y, buffer);
            }
        }

        void flush() const {
            xcb_flush(connection);
        }

        void drawBackground() const {
            xcb_poly_fill_rectangle(connection, window, backgroundContext, 1,
                                    &rectangle);
        }

        void setWindowTitle(const char *title) {
            /* set the title of the window */
            xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window,
                                XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(title),
                                title);

            /* set the title of the window icon */
            xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window,
                                XCB_ATOM_WM_ICON_NAME, XCB_ATOM_STRING, 8,
                                strlen(title), title);
        }

        void saveImage() {
            StoreImage store("test.png", width, height, connection, screen);
            store.saveImage();
        }

    protected:

        xcb_connection_t *connection;
        xcb_screen_t *screen;
        xcb_window_t window;

        ChartColormap *colormap;
        ChartData<R> *data;

        xcb_gcontext_t backgroundContext;

        xcb_gcontext_t axisContext;
        xcb_gcontext_t axisFontContext;
        xcb_gcontext_t cleanAxesLabelContext;

        uint16_t width;
        uint16_t height;
        xcb_rectangle_t rectangle;

        std::mutex* changeDataMutex;

        void createContexts() {
            // black background context
            backgroundContext = xcb_generate_id(connection);
            uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
            uint32_t values[] = {screen->black_pixel, 0};
            xcb_create_gc(connection, backgroundContext, screen->root, mask,
                          values);

            // axis context
            mask = XCB_GC_FOREGROUND;
            axisContext = xcb_generate_id(connection);
            uint32_t values3[] = {colormap->getGray()->pixel};
            xcb_create_gc(connection, axisContext, screen->root, mask, values3);

            // axis labels font context
            const char *fontName = "fixed";
            xcb_font_t axisFont = xcb_generate_id(connection);
            xcb_void_cookie_t fontCookie = xcb_open_font_checked(connection,
                                                                 axisFont,
                                                                 strlen(fontName),
                                                                 fontName);
            testCookie(fontCookie, connection, "can't open font");
            /* create graphics context */
            axisFontContext = xcb_generate_id(connection);
            mask = XCB_GC_FOREGROUND | XCB_GC_FONT;
            uint32_t values4[3] = {screen->white_pixel, axisFont};
            xcb_void_cookie_t gcCookie = xcb_create_gc_checked(connection,
                                                               axisFontContext,
                                                               screen->root, mask,
                                                               values4);
            testCookie(gcCookie, connection, "can't create gc");
            /* close font */
            fontCookie = xcb_close_font_checked(connection, axisFont);
            testCookie(fontCookie, connection, "can't close font");

            // clean axes label context
            cleanAxesLabelContext = xcb_generate_id(connection);
            mask = XCB_GC_FOREGROUND;
            uint32_t values5[] = {screen->black_pixel};
            xcb_create_gc(connection, cleanAxesLabelContext, screen->root, mask,
                          values5);
        }

        void testCookie(xcb_void_cookie_t cookie, xcb_connection_t *connection, const char *errMessage) {
            xcb_generic_error_t *error = xcb_request_check(connection, cookie);
            if (error) {
                std::cout << "ERROR: " << errMessage << " code : " <<
                error->error_code << std::endl;
                free(error);
                throw std::runtime_error("XCB cookie error");
            }
        }
    };

}

#endif
