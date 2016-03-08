#ifndef SRC_GRAPHICS_CHART_COLORMAP_H_
#define SRC_GRAPHICS_CHART_COLORMAP_H_

#include <xcb/xcb.h>
#include <iostream>
#include <cstdlib>
#include <cmath>

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

        void createWavelengthColors(size_t colorsSize) {
            if (waveColors) {
                throw std::runtime_error("Wave colors already created");
            }

//            uint16_t maxBand = 65535;
//
//            uint16_t step = 65535 / colorsSize;
            waveColors = new utils::CArrayList<xcb_alloc_color_reply_t *>(colorsSize, 1, colorsSize);
            for (size_t i = 0; i < colorsSize; ++i) {
                uint16_t r, g, b;
//                calculateWavelengthColor(i, 0, colorsSize - 1, r, g, b);
//                calculateBlackbodyColor(i, 0, colorsSize - 1, r, g, b);
                calculateCubehelixColor(i, 0, colorsSize - 1, r, g, b);
                xcb_generic_error_t *error = NULL;
                xcb_alloc_color_reply_t *color = xcb_alloc_color_reply(connection,
                                                                       xcb_alloc_color(connection, colormapId, r,
                                                                                       g, b), &error);
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
            size_t index = (grayColors->size() - 1) * color;
//            std::cout<<"index: " << index << std::endl;
            return (*grayColors)[index];
        }

        xcb_alloc_color_reply_t *getWavelengthColor(double color) {
            size_t index = (waveColors->size() - 1) * (color);
//            std::cout<<"index: " << index << std::endl;
            return (*waveColors)[index];
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


        uint16_t adjustColor(double color, double factor) {
            uint16_t intensityMax = 65535;

            if (color == 0) {
                return 0;
            } else {
                return intensityMax * std::pow(color * factor, 0.8);
            }
        }

        uint16_t rescaleColor(double color) {
            uint16_t intensityMax = 65535;

            if (color < 0) {
                return 0;
            } else if (color > 1) {
                return intensityMax;
            } else {
                return color * intensityMax;
            }
        }

        void calculateWavelengthColor(double val, double minVal, double maxVal, uint16_t &r, uint16_t &g, uint16_t &b) {
            double minVisibleWaveLength = 380.0; // min 380
            double maxVisibleWaveLength = 780.0; // max 780

            double wavelength = (val - minVal) * (maxVisibleWaveLength - minVisibleWaveLength) / (maxVal - minVal) +
                                minVisibleWaveLength;

            double red = 0, green = 0, blue = 0;
            if (wavelength >= 380 && wavelength <= 439) {
                red = -(wavelength - 440.0) / (440.0 - 380.0);
                green = 0;
                blue = 1;
            } else if (wavelength >= 440 && wavelength <= 489) {
                red = 0.0;
                green = (wavelength - 440.0) / (490.0 - 440.0);
                blue = 1;
            } else if (wavelength >= 490 && wavelength <= 509) {
                red = 0.0;
                green = 1;
                blue = -(wavelength - 510.0) / (510.0 - 490.0);
            } else if (wavelength >= 510 && wavelength <= 579) {
                red = (wavelength - 510.0) / (580.0 - 510.0);
                green = 1;
                blue = 0;
            } else if (wavelength >= 580 && wavelength <= 644) {
                red = 1;
                green = -(wavelength - 645.0) / (645.0 - 580.0);
                blue = 0;
            } else if (wavelength >= 645 && wavelength <= 780) {
                red = 1;
                green = 0;
                blue = 0;
            } else {
                red = 0;
                green = 0;
                blue = 0;
            }

            // Let the intensity fall off near the vision limits
            double factor = 0;
            if (wavelength >= 380 && wavelength <= 419) {
                factor = 0.3 + 0.7 * (wavelength - 380.0) / (420.0 - 380.0);
            } else if (wavelength >= 420 && wavelength <= 700) {
                factor = 1;
            } else if (wavelength >= 701 && wavelength <= 780) {
                factor = 0.3 + 0.7 * (780.0 - wavelength) / (780.0 - 700.0);
            } else {
                factor = 0;
            }

            r = adjustColor(red, factor);
            g = adjustColor(green, factor);
            b = adjustColor(blue, factor);
        }


        void calculateBlackbodyColor(double val, double minVal, double maxVal, uint16_t &r, uint16_t &g, uint16_t &b) {
            double minTemperature = 1000.0;
            double maxTemperature = 10000.0;

            double temperature =
                    (val - minVal) * (maxTemperature - minTemperature) / (maxVal - minVal) + minTemperature;

            /*
             * CIE Color Matching Functions (x_bar,y_bar,z_bar)
             * for wavelenghts in 5 nm increments from 380 nm to 780 nm.
             */
            double fColorMatch[][3] = {
                    {0.0014, 0.0000, 0.0065},
                    {0.0022, 0.0001, 0.0105},
                    {0.0042, 0.0001, 0.0201},
                    {0.0076, 0.0002, 0.0362},
                    {0.0143, 0.0004, 0.0679},
                    {0.0232, 0.0006, 0.1102},
                    {0.0435, 0.0012, 0.2074},
                    {0.0776, 0.0022, 0.3713},
                    {0.1344, 0.0040, 0.6456},
                    {0.2148, 0.0073, 1.0391},
                    {0.2839, 0.0116, 1.3856},
                    {0.3285, 0.0168, 1.6230},
                    {0.3483, 0.0230, 1.7471},
                    {0.3481, 0.0298, 1.7826},
                    {0.3362, 0.0380, 1.7721},
                    {0.3187, 0.0480, 1.7441},
                    {0.2908, 0.0600, 1.6692},
                    {0.2511, 0.0739, 1.5281},
                    {0.1954, 0.0910, 1.2876},
                    {0.1421, 0.1126, 1.0419},
                    {0.0956, 0.1390, 0.8130},
                    {0.0580, 0.1693, 0.6162},
                    {0.0320, 0.2080, 0.4652},
                    {0.0147, 0.2586, 0.3533},
                    {0.0049, 0.3230, 0.2720},
                    {0.0024, 0.4073, 0.2123},
                    {0.0093, 0.5030, 0.1582},
                    {0.0291, 0.6082, 0.1117},
                    {0.0633, 0.7100, 0.0782},
                    {0.1096, 0.7932, 0.0573},
                    {0.1655, 0.8620, 0.0422},
                    {0.2257, 0.9149, 0.0298},
                    {0.2904, 0.9540, 0.0203},
                    {0.3597, 0.9803, 0.0134},
                    {0.4334, 0.9950, 0.0087},
                    {0.5121, 1.0000, 0.0057},
                    {0.5945, 0.9950, 0.0039},
                    {0.6784, 0.9786, 0.0027},
                    {0.7621, 0.9520, 0.0021},
                    {0.8425, 0.9154, 0.0018},
                    {0.9163, 0.8700, 0.0017},
                    {0.9786, 0.8163, 0.0014},
                    {1.0263, 0.7570, 0.0011},
                    {1.0567, 0.6949, 0.0010},
                    {1.0622, 0.6310, 0.0008},
                    {1.0456, 0.5668, 0.0006},
                    {1.0026, 0.5030, 0.0003},
                    {0.9384, 0.4412, 0.0002},
                    {0.8544, 0.3810, 0.0002},
                    {0.7514, 0.3210, 0.0001},
                    {0.6424, 0.2650, 0.0000},
                    {0.5419, 0.2170, 0.0000},
                    {0.4479, 0.1750, 0.0000},
                    {0.3608, 0.1382, 0.0000},
                    {0.2835, 0.1070, 0.0000},
                    {0.2187, 0.0816, 0.0000},
                    {0.1649, 0.0610, 0.0000},
                    {0.1212, 0.0446, 0.0000},
                    {0.0874, 0.0320, 0.0000},
                    {0.0636, 0.0232, 0.0000},
                    {0.0468, 0.0170, 0.0000},
                    {0.0329, 0.0119, 0.0000},
                    {0.0227, 0.0082, 0.0000},
                    {0.0158, 0.0057, 0.0000},
                    {0.0114, 0.0041, 0.0000},
                    {0.0081, 0.0029, 0.0000},
                    {0.0058, 0.0021, 0.0000},
                    {0.0041, 0.0015, 0.0000},
                    {0.0029, 0.0010, 0.0000},
                    {0.0020, 0.0007, 0.0000},
                    {0.0014, 0.0005, 0.0000},
                    {0.0010, 0.0004, 0.0000},
                    {0.0007, 0.0002, 0.0000},
                    {0.0005, 0.0002, 0.0000},
                    {0.0003, 0.0001, 0.0000},
                    {0.0002, 0.0001, 0.0000},
                    {0.0002, 0.0001, 0.0000},
                    {0.0001, 0.0000, 0.0000},
                    {0.0001, 0.0000, 0.0000},
                    {0.0001, 0.0000, 0.0000},
                    {0.0000, 0.0000, 0.0000}};

            double XX = 0.0, YY = 0.0, ZZ = 0.0; /* initialize accumulators */
            double con, dis, wavelength, weight;
            short band, nbands = 81;

            /* loop over wavelength bands
             * integration by trapezoid method
             */
            for (band = 0; band < nbands; band++) {
                weight = 1.0;
                if ((band == 0) || (band == nbands - 1)) {
                    weight = 0.5; /* properly weight end points */
                }
                wavelength = 380.0 + (double) band * 5.0;/* wavelength in nm */
                /* generate a black body spectrum */
                con = 1240.0 / 8.617e-5;

                dis = 3.74183e-16 * (1.0 / pow(wavelength, 5)) / (exp(con / (wavelength * temperature)) - 1.);

                // simple integration over bands
                XX = XX + weight * dis * fColorMatch[band][0];
                YY = YY + weight * dis * fColorMatch[band][1];
                ZZ = ZZ + weight * dis * fColorMatch[band][2];
            }

            /* re-normalize the color scale */
            uint16_t intensityMax = 65535;
            r = intensityMax * XX / max3(XX, YY, ZZ);
            g = intensityMax * YY / max3(XX, YY, ZZ);
            b = intensityMax * ZZ / max3(XX, YY, ZZ);

        }

        double max3(double x, double y, double z) {
            double max;
            max = x;
            if (y > max) max = y;
            if (z > max) max = z;
            return max;
        }

        /**
         * Calculate Cubehelix color
         * http://www.mrao.cam.ac.uk/~dag/CUBEHELIX/cubetry.html
         */
        void calculateCubehelixColor(double val, double minVal, double maxVal, uint16_t &r, uint16_t &g, uint16_t &b) {
            double minColor = 0.0;
            double maxColor = 256.0;
            double v = (val - minVal) * (maxColor - minColor) / (maxVal - minVal) + minColor;

            double fract = cubehelixFract(v, maxColor, 0);
            double start = 0.5; // start color 0.5 is purple
            double rots = -1.5; // rotations in colour (typically -1.5 to 1.5, e.g. -1.0 is one blue->green->red cycle)
            double hue = 6.0; // 6 // intensity scaling (in the range 0.0 (B+W) to 1.0 to be strictly correct,
            // larger values may be OK with particular start/end colours) default 1
            double gamma = 10.0; // 10 // gamma correction for intensit (default 1)
            double rd = 0, gd = 0, bd = 0;
            cubehelixRGB(fract, start, rots, hue, gamma, rd, gd, bd);
            r = rescaleColor(rd);
            g = rescaleColor(gd);
            b = rescaleColor(bd);
        }

        double cubehelixFract(double i, double n, double flip) {
            double fraction = i / (n - 1);
            if (flip == 1) {
                fraction = 1.0 - fraction;
            }
            return fraction;
        }

        void cubehelixRGB(double fract, double start, double rots, double hue, double gamma, double &r, double &g,
                          double &b) {
            double pi = 3.14159265358979323846;
            double angle = 2 * pi * (start / 3.0 + 1 + rots * fract);
            double amp = hue * fract * (1.0 - fract) / 2.0;

            double fract2 = std::pow(fract, gamma);

            r = fract2 + amp * (-0.14861 * std::cos(angle) + 1.78277 * std::sin(angle));
            g = fract2 + amp * (-0.29227 * std::cos(angle) - 0.90649 * std::sin(angle));
            b = fract2 + amp * (+1.97294 * std::cos(angle));
        }

    };

}

#endif
