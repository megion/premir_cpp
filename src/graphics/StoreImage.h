#ifndef SRC_GRAPHICS_STOREIMAGE_H_
#define SRC_GRAPHICS_STOREIMAGE_H_

#include <xcb/xcb.h>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <cstdio>
#include <cstring>

#include <png.h>
#include <pngconf.h>
#include <pnglibconf.h>

namespace graphics {

    class StoreImage {
    public:

        StoreImage(const char *filename, uint16_t _width, uint16_t _height, xcb_connection_t *_con,
                   xcb_screen_t *_screen, xcb_drawable_t  pixmap) :
                connection(_con), screen(_screen), width(_width), height(_height) {
            fp = fopen(filename, "wb");
            if (fp == NULL) {
                std::string errMsg(std::strerror(errno));
                errMsg = errMsg + ": " + filename;
                throw std::runtime_error(errMsg);
            }

            xcb_get_image_cookie_t image_cookie = xcb_get_image(connection, XCB_IMAGE_FORMAT_Z_PIXMAP,
                                                                pixmap, 0, 0, width, height, ~0);
            image = xcb_get_image_reply(connection, image_cookie, 0);
            if (!image) {
                throw std::runtime_error("Cannot get image");
            }
            std::cout << "create image: " << filename << std::endl;
        }

        ~StoreImage() {
            std::free(image);

            if (fclose(fp) < 0) {
                throw std::runtime_error(std::strerror(errno));
            }
            fp = nullptr;
        }


        void saveImage() {
            png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if (png_ptr == NULL) {
                fprintf(stderr, "Could not allocate write struct\n");
                return;
            }

            png_infop png_info_ptr = png_create_info_struct(png_ptr);
            if (png_info_ptr == NULL) {
                fprintf(stderr, "Could not allocate info struct\n");
                return;
            }

            // setup exception handling
            if (setjmp(png_jmpbuf(png_ptr))) {
                fprintf(stderr, "Error during png creation\n");
                return;
            }

            png_init_io(png_ptr, fp);

            // write header (8 bit colour depth)
            png_set_IHDR(png_ptr, png_info_ptr, width, height, 8,
                         PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                         PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

            // Set title
            char title[] = "Screenshot";
            char key[] = "Title";
            png_text title_text;
            title_text.compression = PNG_TEXT_COMPRESSION_NONE;
            title_text.key = key;
            title_text.text = title;
            png_set_text(png_ptr, png_info_ptr, &title_text, 1);
            png_write_info(png_ptr, png_info_ptr);


            uint32_t imageDateSize = xcb_get_image_data_length(image);
            uint8_t *imageData = xcb_get_image_data(image);
            std::cout << "image date size: " << imageDateSize << std::endl;


            // allocate memory for one row (3 bytes per pixel - RGB)
            png_bytep png_row = (png_bytep) std::malloc(3 * width * sizeof(png_byte));

            if (png_row == NULL) {
                throw std::runtime_error("Cannot get image data");
            }

            // start write image data
            size_t index = 0;
            for (uint32_t y = 0; y < height; y++) {
                for (uint32_t x = 0; x < width; x++) {
                    uint8_t& red = imageData[index];
                    ++index;
                    uint8_t& green = imageData[index];
                    ++index;
                    uint8_t& blue = imageData[index];
                    ++index;
                    ++index; // пропускаем четвертый байт т.е. 3 байта на пиксель

                    png_byte *ptr = &(png_row[x * 3]);
                    ptr[0] = blue;
                    ptr[1] = green;
                    ptr[2] = red;
                }
                png_write_row(png_ptr, png_row);
            }

            // End write

            // End write
            png_write_end(png_ptr, NULL);

            // Free
            if (png_info_ptr != NULL) {
                png_free_data(png_ptr, png_info_ptr, PNG_FREE_ALL, -1);
            }
            if (png_ptr != NULL) {
                png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
            }
            if (png_row != NULL) {
                std::free(png_row);
            }
        }

    protected:
        FILE *fp;

        xcb_connection_t *connection;
        xcb_screen_t *screen;
        xcb_get_image_reply_t *image;

        uint16_t width;
        uint16_t height;

    };

}

#endif
