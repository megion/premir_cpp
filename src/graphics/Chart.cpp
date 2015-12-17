#include "Chart.h"

namespace graphics {

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
//			std::cout << "XCB_EXPOSE event" << std::endl;

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
	drawPoints();

	if (data->size() % 36 == 0) { // каждую третью
		drawAxes();
		drawAxesLabels();
//			drawPoints();
	}

	xcb_flush(connection);

//	drawCleanPoints();

}

void Chart::setWindowTitle(const char* title) {
	/* set the title of the window */
	xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window,
			XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(title), title);

	/* set the title of the window icon */
	xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window,
			XCB_ATOM_WM_ICON_NAME, XCB_ATOM_STRING, 8, strlen(title), title);
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
	for (ChartData::Axis& axis : (*data->getYAxes())) {
		drawAxis(axis);
	}

	// horizontal lines
	for (ChartData::Axis& axis : (*data->getXAxes())) {
		drawAxis(axis);
	}
}

void Chart::drawAxesLabels() const {
	// vertical labels
	for (ChartData::Axis& axis : (*data->getYAxes())) {
		drawAxisLabel(axis);
	}

	// horizontal labels
	for (ChartData::Axis& axis : (*data->getXAxes())) {
		drawAxisLabel(axis);
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

