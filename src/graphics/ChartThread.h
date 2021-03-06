#ifndef SRC_GRAPHICS_CHART_THREAD_H_
#define SRC_GRAPHICS_CHART_THREAD_H_

#include <thread>
#include <iostream>

#include "graphics/Chart.h"

namespace graphics {

    template<typename R>
    class ChartThread {
    public:
        ChartThread(Chart<R> *chart) :
                chartThread(std::thread(&Chart<R>::runChart, chart)) {
        }

        ~ChartThread() {
            std::cout << "destroy ChartThread - call join" <<
            chartThread.joinable() << std::endl;
            chartThread.join();
//            delete chart;
            std::cout << "destroy ChartThread - call join after" << std::endl;
        }

//        Chart *getChart() {
//            return chart;
//        }

    private:
//        Chart *chart;
        std::thread chartThread;

    };

}

#endif
