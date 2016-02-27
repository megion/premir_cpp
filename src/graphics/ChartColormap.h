#ifndef SRC_GRAPHICS_CHART_COLORMAP_H_
#define SRC_GRAPHICS_CHART_COLORMAP_H_

#include <xcb/xcb.h>
#include <iostream>
#include <stdlib.h>

#include "utils/CArrayList.h"

namespace graphics {

    class ChartColormap {
    public:

        ChartColormap(xcb_connection_t *_connection, xcb_screen_t *_screen, xcb_window_t _window) :
                connection(_connection), screen(_screen), window(_window),
                grayColors(nullptr), waveColors(nullptr) {

            colormapId = screen->default_colormap;

            xcb_generic_error_t *error = NULL;
            green = xcb_alloc_color_reply(connection,
                                          xcb_alloc_color(connection, colormapId, 0, 65535, 0), &error);
            gray = xcb_alloc_color_reply(connection,
                                         xcb_alloc_color(connection, colormapId, 32767, 32767, 32767),
                                         &error);
            if (error) {
                throw std::runtime_error("Cannot create colormap");
            }
        }

        ~ChartColormap() {
            std::free(green);
            std::free(gray);

            if (grayColors) {
                for (size_t i = 0; i < grayColors->size(); ++i) {
                    std::free((*grayColors)[i]);
                }
                delete grayColors;
                grayColors = nullptr;
            }

            if (waveColors) {
                for (size_t i = 0; i < waveColors->size(); ++i) {
                    std::free((*waveColors)[i]);
                }
                delete waveColors;
                waveColors = nullptr;
            }
        }

        xcb_alloc_color_reply_t *getGreen() {
            return green;
        }

        xcb_alloc_color_reply_t *getGray() {
            return gray;
        }

        void createGrayColors(uint16_t colorsSize) {
            if (grayColors) {
                throw std::runtime_error("Gray colors already created");
            }
            uint16_t step = 65535 / colorsSize;
            grayColors = new utils::CArrayList<xcb_alloc_color_reply_t *>(colorsSize, 1, colorsSize);
            for (uint16_t i = 0; i < colorsSize; ++i) {
                uint16_t colComp = i * step; // компонента цвета
                xcb_generic_error_t *error = NULL;
                xcb_alloc_color_reply_t *color = xcb_alloc_color_reply(connection,
                                                                       xcb_alloc_color(connection, colormapId, colComp,
                                                                                       colComp, colComp), &error);
                if (error) {
                    throw std::runtime_error("Cannot create colormap");
                }
                (*grayColors)[i] = color;
            }
        }

        void createWavColors(size_t colorsSize) {
            if (waveColors) {
                throw std::runtime_error("Wave colors already created");
            }
            uint16_t step = 65535 / colorsSize;
            waveColors = new utils::CArrayList<xcb_alloc_color_reply_t *>(colorsSize, 1, colorsSize);
            for (uint16_t i = 0; i < colorsSize; ++i) {
                uint16_t colComp = i * step; // компонента цвета
                xcb_generic_error_t *error = NULL;
                xcb_alloc_color_reply_t *color = xcb_alloc_color_reply(connection,
                                                                       xcb_alloc_color(connection, colormapId, colComp,
                                                                                       colComp, colComp), &error);
                if (error) {
                    throw std::runtime_error("Cannot create colormap");
                }
                (*waveColors)[i] = color;
            }
        }



        /**
         * Параметр color должен принимать значение от 0 ... 1
         */
        xcb_alloc_color_reply_t *getScaledGrayColor(double color) {
            size_t index = (grayColors->size()-1) * color;
//            std::cout<<"index: " << index << std::endl;
            return (*grayColors)[index];
        }


    private:
        xcb_connection_t *connection;
        xcb_screen_t *screen;
        xcb_window_t window;

        xcb_colormap_t colormapId;

        xcb_alloc_color_reply_t *green;
        xcb_alloc_color_reply_t *gray;

        // коллекция серых цветов: от белого до черного
        utils::CArrayList<xcb_alloc_color_reply_t *> *grayColors;
        // цвета радуги :)
        utils::CArrayList<xcb_alloc_color_reply_t *> *waveColors;


    };

}

#endif
