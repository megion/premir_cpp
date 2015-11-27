#ifndef SRC_GRAPHICS_CHART_H_
#define SRC_GRAPHICS_CHART_H_

#include <xcb/xcb.h>
#include <exception>
#include <stdexcept>

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

		createBackgroundContext();
		createPointsContext();

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
		setWindowTitle("Chart", "Chart");
	}

	~Chart() {
		std::cout << "call disconnect: " << std::endl;
		backgroundContext = 0;
		screen = nullptr;
		xcb_disconnect(connection);
	}

	/**
	 * Show chart and run chart event loop in other thread
	 */
	void runChart();

private:

	void createBackgroundContext() {
		// create black graphics context
		backgroundContext = xcb_generate_id(connection);
		uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
		uint32_t values[] = { screen->black_pixel, 0 };
		xcb_create_gc(connection, backgroundContext, screen->root, mask,
				values);
	}

	void createPointsContext() {
		pointsContext = xcb_generate_id(connection);
		uint32_t mask = XCB_GC_FOREGROUND;
		uint32_t values[] = { screen->white_pixel };
		xcb_create_gc(connection, pointsContext, screen->root, mask, values);
	}

	void drawPoints(const xcb_point_t *points, uint32_t points_len);

	void setWindowTitle(const char* title, const char* iconTitle);

	xcb_connection_t* connection;
	xcb_screen_t* screen;
	xcb_window_t window;

	xcb_gcontext_t backgroundContext;
	xcb_gcontext_t pointsContext;

	uint16_t width;
	uint16_t height;
	xcb_rectangle_t rectangle;
};

void Chart::runChart() {
	// show window
	xcb_map_window(connection, window);
	xcb_flush(connection);

	xcb_point_t points[] = { { 100, 100 }, { 101, 100 }, { 102, 100 } };

	// TODO: should be run in other thread
	int done = 0;
	xcb_generic_event_t *e;
	/* event loop */
	while (!done && (e = xcb_wait_for_event(connection))) {
		switch (e->response_type & ~0x80) {
		case XCB_EXPOSE: /* отрисовать или перерисовать окно */
			std::cout << "XCB_EXPOSE event" << std::endl;

			xcb_poly_fill_rectangle(connection, window, backgroundContext, 1,
					&rectangle);
			drawPoints(points, 3);

			xcb_flush(connection);
			break;
//		case XCB_KEY_PRESS: /* выйти по нажатию клавиши */
//			done = 1;
//			break;
		}
		free(e);
	}
}

void Chart::drawPoints(const xcb_point_t *points, uint32_t points_len) {
	xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, window, pointsContext,
			points_len, points);
}

void Chart::setWindowTitle(const char* title, const char* iconTitle) {
	/* set the title of the window */
	xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window,
			XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(title), title);

	/* set the title of the window icon */
	xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window,
			XCB_ATOM_WM_ICON_NAME, XCB_ATOM_STRING, 8, strlen(iconTitle),
			iconTitle);
}

}

#endif /* SRC_GRAPHICS_CHART_H_ */
