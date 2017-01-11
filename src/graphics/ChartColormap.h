#ifndef SRC_GRAPHICS_CHARTCOLORMAP_H
#define SRC_GRAPHICS_CHARTCOLORMAP_H

#include <xcb/xcb.h>
#include <iostream>
#include <cstdlib>
#include <cmath>

#include "utils/CArrayList.h"
#include "utils/HashMapArray.h"
#include "color_utils.h"

namespace graphics {

    struct Color {
        uint16_t r;
        uint16_t g;
        uint16_t b;

        Color() {
        }

        Color(const uint16_t& _r, const uint16_t& _g, const uint16_t& _b) : r(_r), g(_g), b(_b) {
        }

        // compare function for hash map
        bool operator==(const Color &other) const {
            if (r != other.r || g != other.g || b != other.b) {
                return false;
            }
            return true;
        }
        bool operator!=(const Color &other) const {
            return !((*this) == other);
        }
    };

    class ColorHash : public utils::hash::HashEngine<Color> {
    public:
        ColorHash() {
        }
        size_t hash(const Color &value) const {
            return (size_t)(value.r + value.g + value.b);
        }
    };

    class ChartColormap {
    public:

        typedef typename utils::Entry<Color, xcb_alloc_color_reply_t *> ColorEntry;
        typedef typename utils::R3DMatrix<bool, bool, ColorEntry>::Row ColorRow;
        typedef typename utils::R3DMatrix<bool, bool, ColorEntry>::Cell ColorCell;

        ChartColormap(xcb_connection_t *_connection, xcb_screen_t *_screen, xcb_window_t _window) :
                connection(_connection), screen(_screen), window(_window), colorHash(ColorHash()) {

            colormapId = screen->default_colormap;

            xcb_generic_error_t *error = NULL;
            green = xcb_alloc_color_reply(connection,
                                          xcb_alloc_color(connection, colormapId, 0, 65535, 0), &error);
            gray = xcb_alloc_color_reply(connection,
                                         xcb_alloc_color(connection, colormapId, 32767, 32767, 32767), &error);
            if (error) {
                throw std::runtime_error("Cannot create colormap");
            }

            userColors = new utils::HashMapArray<Color, xcb_alloc_color_reply_t *>(1, &colorHash);
        }

        ~ChartColormap() {
            std::free(green);
            std::free(gray);

            // удалить пользовательские цвета
            for (size_t r = 0; r < userColors->getMatrix()->getRowSize(); ++r) {
                ColorRow &row = (*userColors->getMatrix())[r];
                for (size_t c = 0; c < row.cellSize; ++c) {
                    ColorCell &cell = row[c];
                    for (size_t p = 0; p < cell.pointSize; ++p) {
                        ColorEntry &e = cell[p];
                        std::free(e.value);
                    }
                }
            }
            delete userColors;
        }

        xcb_alloc_color_reply_t *getColor(const Color& ucolor) {
            xcb_alloc_color_reply_t ** ppColor = userColors->getValue(0, ucolor);
            if (ppColor == nullptr) {
                // цвет не найден создаем его
                xcb_generic_error_t *error = NULL;
                xcb_alloc_color_reply_t *color = xcb_alloc_color_reply(connection,
                                                                       xcb_alloc_color(connection, colormapId, ucolor.r,
                                                                                       ucolor.g, ucolor.b), &error);
                if (error) {
                    throw std::runtime_error("Cannot create colormap");
                }
                userColors->pushValue(0, ucolor, color);
                return color;
            } else {
                return *ppColor;
            }
        }

        xcb_alloc_color_reply_t *getGreen() {
            return green;
        }

        xcb_alloc_color_reply_t *getGray() {
            return gray;
        }

//        utils::CArrayList<xcb_alloc_color_reply_t *> *createGrayColors(uint16_t colorsSize) {
//            uint16_t step = 65535 / colorsSize;
//            utils::CArrayList<xcb_alloc_color_reply_t *> *colors = new utils::CArrayList<xcb_alloc_color_reply_t *>(
//                    colorsSize, 1, colorsSize);
//            for (uint16_t i = 0; i < colorsSize; ++i) {
//                uint16_t colComp = i * step; // компонента цвета
//                xcb_generic_error_t *error = NULL;
//                xcb_alloc_color_reply_t *color = xcb_alloc_color_reply(connection,
//                                                                       xcb_alloc_color(connection, colormapId, colComp,
//                                                                                       colComp, colComp), &error);
//                if (error) {
//                    throw std::runtime_error("Cannot create colormap");
//                }
//                (*colors)[i] = color;
//            }
//            return colors;
//        }
//
//        utils::CArrayList<xcb_alloc_color_reply_t *> *createWavelengthColors(size_t colorsSize) {
//            utils::CArrayList<xcb_alloc_color_reply_t *> *colors = new utils::CArrayList<xcb_alloc_color_reply_t *>(
//                    colorsSize, 1, colorsSize);
//            for (size_t i = 0; i < colorsSize; ++i) {
//                uint16_t r, g, b;
//                calculateWavelengthColor(i, 0, colorsSize - 1, r, g, b);
//                xcb_generic_error_t *error = NULL;
//                xcb_alloc_color_reply_t *color = xcb_alloc_color_reply(connection,
//                                                                       xcb_alloc_color(connection, colormapId, r,
//                                                                                       g, b), &error);
//                if (error) {
//                    throw std::runtime_error("Cannot create colormap");
//                }
//                (*colors)[i] = color;
//            }
//
//            return colors;
//        }
//
//        utils::CArrayList<xcb_alloc_color_reply_t *> *createCubehelixColors(size_t colorsSize, double start,
//                                                                            double rots, double hue, double gamma) {
//            utils::CArrayList<xcb_alloc_color_reply_t *> *colors = new utils::CArrayList<xcb_alloc_color_reply_t *>(
//                    colorsSize, 1, colorsSize);
//            for (size_t i = 0; i < colorsSize; ++i) {
//                uint16_t r, g, b;
//                calculateCubehelixColor(i, 0, colorsSize - 1, r, g, b, start, rots, hue, gamma);
//                xcb_generic_error_t *error = NULL;
//                xcb_alloc_color_reply_t *color = xcb_alloc_color_reply(connection,
//                                                                       xcb_alloc_color(connection, colormapId, r,
//                                                                                       g, b), &error);
//                if (error) {
//                    throw std::runtime_error("Cannot create colormap");
//                }
//                (*colors)[i] = color;
//            }
//
//            return colors;
//        }


        /**
         * Параметр color должен принимать значение от 0 ... 1
         */
//        xcb_alloc_color_reply_t *getScaledColor(utils::CArrayList<xcb_alloc_color_reply_t *> *colors,
//                                                const double &color) const {
//            size_t index = (colors->size() - 1) * color;
//            return (*colors)[index];
//        }
//
//        void freeColors(utils::CArrayList<xcb_alloc_color_reply_t *> *colors) {
//            for (size_t i = 0; i < colors->size(); ++i) {
//                std::free((*colors)[i]);
//            }
//            delete colors;
//        }

    private:
        xcb_connection_t *connection;
        xcb_screen_t *screen;
        xcb_window_t window;

        xcb_colormap_t colormapId;

        xcb_alloc_color_reply_t *green;
        xcb_alloc_color_reply_t *gray;

        // пользовательские произвольные цвета,
        // пока будем использовать только первую размерность массива
        utils::HashMapArray<Color, xcb_alloc_color_reply_t *> *userColors;
        ColorHash colorHash;
    };

}

#endif
