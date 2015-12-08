#ifndef SRC_GRAPHICS_CHART_H_
#define SRC_GRAPHICS_CHART_H_

#include <xcb/xcb.h>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <thread>

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
		setWindowTitle("Chart", "Chart");
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
		if (data->getOutpoints()) {
			xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, window,
					pointsContext, data->size(), data->getOutpoints());
		}
	}

	void drawCleanPoints() const {
		if (data->getOutpoints()) {
			xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, window,
					cleanPointsContext, data->size(), data->getOutpoints());
		}
	}

	void drawBackground() const {
		xcb_poly_fill_rectangle(connection, window, backgroundContext, 1,
				&rectangle);
	}

private:

	void createContexts();
	void setWindowTitle(const char* title, const char* iconTitle);
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

void Chart::runChart() const {
	// show window
	xcb_map_window(connection, window);
	xcb_flush(connection);

	int done = 0;
	xcb_generic_event_t *e;
	/* event loop */
	while (!done && (e = xcb_wait_for_event(connection))) {
		switch (e->response_type & ~0x80) {
		case XCB_EXPOSE: /* отрисовать или перерисовать окно */
			std::cout << "XCB_EXPOSE event" << std::endl;

			draw();

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

void Chart::draw() const {
	drawBackground();
	drawAxes();
	drawAxesLabels();
	drawPoints();
}

void Chart::redrawNewPoints(double x, double y) const {
	drawCleanPoints();
	data->addPoint(x, y);
	drawAxes(); // because after clean some points are cleaned
	drawAxesLabels();
	drawPoints();

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

void Chart::createContexts() {
	// black background context
	backgroundContext = xcb_generate_id(connection);
	uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
	uint32_t values[] = { screen->black_pixel, 0 };
	xcb_create_gc(connection, backgroundContext, screen->root, mask, values);

	// points context
	pointsContext = xcb_generate_id(connection);
	mask = XCB_GC_FOREGROUND;
	uint32_t values2[] = { colormap->getGreen()->pixel };
	xcb_create_gc(connection, pointsContext, screen->root, mask, values2);

	// clean points context
	cleanPointsContext = xcb_generate_id(connection);
	uint32_t values22[] = { screen->black_pixel };
	xcb_create_gc(connection, cleanPointsContext, screen->root, mask, values22);

	// axis context
	axisContext = xcb_generate_id(connection);
	uint32_t values3[] = { colormap->getGray()->pixel };
	xcb_create_gc(connection, axisContext, screen->root, mask, values3);

	// axis labels font context
	const char* fontName = "fixed";
	xcb_font_t axisFont = xcb_generate_id(connection);
	xcb_void_cookie_t fontCookie = xcb_open_font_checked(connection, axisFont,
			strlen(fontName), fontName);
	testCookie(fontCookie, connection, "can't open font");
	/* create graphics context */
	axisFontContext = xcb_generate_id(connection);
	mask = XCB_GC_FOREGROUND | XCB_GC_FONT;
	uint32_t values4[3] = { screen->white_pixel, axisFont };
	xcb_void_cookie_t gcCookie = xcb_create_gc_checked(connection,
			axisFontContext, screen->root, mask, values4);
	testCookie(gcCookie, connection, "can't create gc");
	/* close font */
	fontCookie = xcb_close_font_checked(connection, axisFont);
	testCookie(fontCookie, connection, "can't close font");

	// clean axes label context
	cleanAxesLabelContext = xcb_generate_id(connection);
	mask = XCB_GC_FOREGROUND;
	uint32_t values5[] = { screen->black_pixel };
	xcb_create_gc(connection, cleanAxesLabelContext, screen->root, mask,
			values5);
}

void Chart::testCookie(xcb_void_cookie_t cookie, xcb_connection_t* connection,
		const char* errMessage) {
	xcb_generic_error_t* error = xcb_request_check(connection, cookie);
	if (error) {
		std::cout << "ERROR: " << errMessage << " code : " << error->error_code
				<< std::endl;
		free(error);
		throw std::runtime_error("XCB cookie error");
	}
}

void Chart::drawAxes() const {
	// vertical lines
	utils::LinkedList<ChartData::Axis>* axes = data->getYAxes();
	utils::LinkedList<ChartData::Axis>::Iterator iter = axes->iterator();

	while (iter.hasNext()) {
		utils::LinkedList<ChartData::Axis>::Entry* e = iter.next();
		ChartData::Axis& p = e->getValue();
		drawAxis(p);
	}

	// horizontal lines
	axes = data->getXAxes();
	iter = axes->iterator();
	while (iter.hasNext()) {
		utils::LinkedList<ChartData::Axis>::Entry* e = iter.next();
		ChartData::Axis& p = e->getValue();
		drawAxis(p);
	}
}

void Chart::drawAxesLabels() const {
	// vertical labels
	utils::LinkedList<ChartData::Axis>* axes = data->getYAxes();
	utils::LinkedList<ChartData::Axis>::Iterator iter = axes->iterator();

	while (iter.hasNext()) {
		utils::LinkedList<ChartData::Axis>::Entry* e = iter.next();
		ChartData::Axis& p = e->getValue();
		drawAxisLabel(p);
	}

	// horizontal labels
	axes = data->getXAxes();
	iter = axes->iterator();
	while (iter.hasNext()) {
		utils::LinkedList<ChartData::Axis>::Entry* e = iter.next();
		ChartData::Axis& p = e->getValue();
		drawAxisLabel(p);
	}
}

void Chart::drawAxis(const ChartData::Axis& axis) const {
	// 1. clean text labels - draw black rectangle
	xcb_poly_line(connection, XCB_COORD_MODE_ORIGIN, window, axisContext, 2,
			axis.line);
}

void Chart::drawAxisLabel(const ChartData::Axis& axis) const {
	// 1. clean text labels - draw black rectangle
	if (!axis.hideLabel) {
		uint16_t labelHeight = 16;
		uint16_t labelWidth = 80;
		int16_t labelRectTop = axis.labelPoint.y - labelHeight;
		xcb_rectangle_t labelRect = { axis.labelPoint.x, labelRectTop,
				labelWidth, labelHeight };
		xcb_poly_fill_rectangle(connection, window, cleanAxesLabelContext, 1,
				&labelRect);
		char buffer[32];
		snprintf(buffer, sizeof(buffer), "%g", axis.labelValue);
		xcb_image_text_8(connection, strlen(buffer), window, axisFontContext,
				axis.labelPoint.x, axis.labelPoint.y, buffer);
	}
}

}

#endif /* SRC_GRAPHICS_CHART_H_ */
