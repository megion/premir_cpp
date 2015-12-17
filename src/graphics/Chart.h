#ifndef SRC_GRAPHICS_CHART_H_
#define SRC_GRAPHICS_CHART_H_

#include <xcb/xcb.h>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "ChartColormap.h"
#include "ChartData.h"

namespace graphics {

class Chart {
public:
	Chart(uint16_t _width = 400, uint16_t _height = 260) :
			connection(xcb_connect(NULL, NULL)), width(_width), //
			height(_height) {
		std::cout << "call connect: " << std::endl;
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
		xcb_rectangle_t rectanglePoints = { xr2, yr2, wr2, hr2 };
		data = new ChartData(rectanglePoints);

		createContexts();

		// create window
		window = xcb_generate_id(connection);
		uint32_t values[2];
		values[0] = screen->white_pixel;
		values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;
		xcb_create_window(connection, XCB_COPY_FROM_PARENT, window,
				screen->root, 10, 10, 2 * rectangle.x + rectangle.width,
				2 * rectangle.y + rectangle.height, 1,
				XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual,
				XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK, values);

		// set title
		setWindowTitle("Chart");
	}

	~Chart() {
		std::cout << "call disconnect: " << std::endl;
		backgroundContext = 0;
		screen = nullptr;
		delete colormap;
		delete data;
		xcb_disconnect(connection);
	}

	ChartData* getData() {
		return data;
	}

	/**
	 * Show chart and run chart event loop in other thread
	 */
	void runChart() const;
	void draw() const;
	void drawAxes() const;
	void drawAxesLabels() const;
	void drawAxis(const ChartData::Axis& axis) const;
	void drawAxisLabel(const ChartData::Axis& axis) const;

	/**
	 * 1. draw clean current points
	 * 2. add new points and update all points position
	 * 3. draw updated current points
	 */
	void redrawNewPoints(double x, double y) const;

	void flush() const {
		xcb_flush(connection);
	}

	void drawPoints() const {
		xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, window, pointsContext,
				data->size(), data->getOutpoints()->getArray());
	}

	void drawCleanPoints() const {
		xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, window,
				cleanPointsContext, data->size(),
				data->getOutpoints()->getArray());
	}

	void drawBackground() const {
		xcb_poly_fill_rectangle(connection, window, backgroundContext, 1,
				&rectangle);
	}

	void setWindowTitle(const char* title);

private:

	void createContexts();
	void testCookie(xcb_void_cookie_t cookie, xcb_connection_t* connection,
			const char* errMessage);

	xcb_connection_t* connection;
	xcb_screen_t* screen;
	xcb_window_t window;

	ChartColormap* colormap;
	ChartData* data;

	xcb_gcontext_t backgroundContext;
	xcb_gcontext_t pointsContext;
	xcb_gcontext_t cleanPointsContext;
	xcb_gcontext_t axisContext;
	xcb_gcontext_t axisFontContext;
	xcb_gcontext_t cleanAxesLabelContext;

	uint16_t width;
	uint16_t height;
	xcb_rectangle_t rectangle;
};

}

#endif /* SRC_GRAPHICS_CHART_H_ */
