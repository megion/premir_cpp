#ifndef SRC_GRAPHICS_CHART_H_
#define SRC_GRAPHICS_CHART_H_

#include <xcb/xcb.h>
#include <exception>
#include <stdexcept>

namespace graphics {

class Chart {
public:
	Chart(uint16_t _width = 100, uint16_t _height = 200) :
		connection(xcb_connect(NULL, NULL)), width(_width), height(_height) {
		std::cout << "call connect: " << std::endl;
		if (xcb_connection_has_error(connection)) {
			throw std::runtime_error("Cannot open display");
		}
	}
	~Chart() {
		std::cout << "call disconnect: " << std::endl;
		xcb_disconnect(connection);
	}

private:
	xcb_connection_t* connection;
	uint16_t width;
	uint16_t height;
};

}

#endif /* SRC_GRAPHICS_CHART_H_ */
