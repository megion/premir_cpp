/*
 * ChartThread.h
 *
 *  Created on: Dec 5, 2015
 *      Author: ilya
 */

#ifndef SRC_GRAPHICS_CHARTTHREAD_H_
#define SRC_GRAPHICS_CHARTTHREAD_H_

#include <thread>
#include <iostream>

#include "graphics/Chart.h"

namespace graphics {

class ChartThread {
public:
	ChartThread(uint16_t _width = 400, uint16_t _height = 260) :
			chart(Chart(_width, _height)), chartThread(
					std::thread(&Chart::runChart, &chart)) {
	}

	~ChartThread() {
		std::cout << "destroy ChartThread - call join" << chartThread.joinable() << std::endl;
		chartThread.join();
		std::cout << "destroy ChartThread - call join after" << std::endl;
	}

	Chart& getChart() {
		return chart;
	}

private:
	Chart chart;
	std::thread chartThread;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_CHARTTHREAD_H_ */
