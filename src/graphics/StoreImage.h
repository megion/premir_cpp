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
                   xcb_screen_t *_screen, xcb_pixmap_t _pixmap) :
                connection(_con), screen(_screen), width(_width), height(_height), pixmap(_pixmap) {
            fp = fopen(filename, "wb");
            if (fp == NULL) {
                std::string errMsg(std::strerror(errno));
                errMsg = errMsg + ": " + filename;
                throw std::runtime_error(errMsg);
            }

//            pixmap = xcb_generate_id(connection);
//            xcb_create_pixmap(connection, screen->root_depth, pixmap, window, width, height );
//            // возможно сначала нужно скопировать window -> to pixmap
//            uint32_t values2[] = {screen->white_pixel};
//            xcb_gcontext_t gc = xcb_generate_id(connection);
//            uint32_t mask = XCB_GC_FOREGROUND;
//            xcb_create_gc(connection, gc, screen->root, mask, values2);
//            xcb_copy_area(connection, window, pixmap, gc, 0, 0, 0, 0, width, height);
//
//            const xcb_get_geometry_cookie_t geoCookie = xcb_get_geometry_unchecked(connection,  pixmap);
//            geo = xcb_get_geometry_reply(connection, geoCookie, 0);
//            if (!geo) {
//                throw std::runtime_error("Cannot get geo");
//            }
//            xcb_create_pixmap(connection,screen->root_depth, pixmap, window,this->width,this->height);


            xcb_get_image_cookie_t image_cookie = xcb_get_image(connection, XCB_IMAGE_FORMAT_Z_PIXMAP,
                                                                pixmap, 0, 0, width, height, ~0);
            imrep = xcb_get_image_reply(connection, image_cookie, 0);
            if (!imrep) {
                throw std::runtime_error("Cannot get image");
            }

//            image = xcb_image_get(connection, window, 0, 0, width, height, XCB_GC_FOREGROUND, XCB_IMAGE_FORMAT_XY_PIXMAP);
//            if (!image) {
//                throw std::runtime_error("Cannot get image");
//            }
        }

        ~StoreImage() {
//            std::free(image);

//            std::free(geo);
            std::free(imrep);

            if (fclose(fp) < 0) {
                throw std::runtime_error(std::strerror(errno));
            }
            fp = nullptr;
        }


        void saveImage() {
            png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if (png_ptr == NULL) {
                fprintf(stderr, "Could not allocate write struct\n");
            }

            png_infop png_info_ptr = png_create_info_struct(png_ptr);
            if (png_info_ptr == NULL) {
                fprintf(stderr, "Could not allocate info struct\n");
            }

            // Setup Exception handling
            if (setjmp(png_jmpbuf(png_ptr))) {
                fprintf(stderr, "Error during png creation\n");
            }

            png_init_io(png_ptr, fp);

            // Write header (8 bit colour depth)
//            png_uint_32 w;
            png_set_IHDR(png_ptr, png_info_ptr, width, height,
                         8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                         PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

            // Set title
            char title[] = "Screenshot";
            char key[] = "Title";
//            if (title != NULL) {
            png_text title_text;
            title_text.compression = PNG_TEXT_COMPRESSION_NONE;
            title_text.key = key;
            title_text.text = title;
            png_set_text(png_ptr, png_info_ptr, &title_text, 1);
//            }

            png_write_info(png_ptr, png_info_ptr);


            uint32_t imageDateSize = xcb_get_image_data_length(imrep);
            uint8_t *imageData = xcb_get_image_data(imrep);
//            std::cout << "imageDateSize: " << image->size << std::endl;
            std::cout << "imageDateSize2: " << imageDateSize << std::endl;

//            if (imageData == NULL) {
//                throw std::runtime_error("Cannot get image data");
//            }

            // Allocate memory for one row (3 bytes per pixel - RGB)
            png_bytep png_row = (png_bytep) std::malloc(3 * width * sizeof(png_byte));

            if (png_row == NULL) {
                throw std::runtime_error("Cannot get image data");
            }


            //////////// start write
            // Write image data
            size_t index = 0;
            for (uint32_t y = 0; y < height; y++) {
                for (uint32_t x = 0; x < width; x++) {
                    uint8_t& red = imageData[index];
                    ++index;
                    uint8_t& green = imageData[index];
                    ++index;
                    uint8_t& blue = imageData[index];
                    ++index;
                    ++index;
//                    uint32_t pixel = xcb_image_get_pixel(image, x, y);
//
//                    unsigned long red_mask = 0xff0000;
//                    unsigned long green_mask = 0x00ff00;
//                    unsigned long blue_mask = 0x0000ff;
//
//                    unsigned char blue = pixel & blue_mask;
//                    unsigned char green = (pixel & green_mask) >> 8;
//                    unsigned char red = (pixel & red_mask) >> 16;

//                    std::cout << "x: " << x << " y: " << y << " pixel: " << pixel << std::endl;



//                    unsigned char blue = pixel & blue_mask;
//                    unsigned char green = (pixel & green_mask) >> 8;
//                    unsigned char red = (pixel & red_mask) >> 16;

//                    buffer[y*img->width*3+x*3+0] = (char)(pixel>>16);
//                    buffer[y*img->width*3+x*3+1] = (char)((pixel&0x00ff00)>>8);
//                    buffer[y*img->width*3+x*3+2] = (char)(pixel&0x0000ff);

                    png_byte *ptr = &(png_row[x * 3]);
                    ptr[0] = blue;
                    ptr[1] = green;
                    ptr[2] = red;
                }
                png_write_row(png_ptr, png_row);
            }

            // End write
            //////////// end write

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


//            xcb_image_t* image = xcb_image_create_native(connection, width, height, XCB_IMAGE_FORMAT_Z_PIXMAP,
//                                                XCB_COPY_FROM_PARENT, NULL, 0L, NULL);











        }

    protected:
        FILE *fp;

        xcb_connection_t *connection;
        xcb_screen_t *screen;
//        xcb_window_t window;

        xcb_get_image_reply_t *imrep;
//        xcb_image_t *image;

        xcb_pixmap_t pixmap;
//        xcb_get_geometry_reply_t* geo;

        uint16_t width;
        uint16_t height;

    };

}

#endif
