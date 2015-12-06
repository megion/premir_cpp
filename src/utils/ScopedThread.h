#ifndef SRC_UTILS_SCOPEDTHREAD_H_
#define SRC_UTILS_SCOPEDTHREAD_H_

#include <thread>
#include <exception>
#include <stdexcept>

namespace utils {

class ScopedThread {
public:
	/**
	 * Replace specified thread t_ to variable t
	 */
	ScopedThread(std::thread t_) :
			t(std::move(t_)) {
		if (!t.joinable()) {
			throw std::logic_error("No thread");
		}
	}

	~ScopedThread() {
		t.join();
	}

	// disable copy constructor
	ScopedThread(const ScopedThread&) = delete;

	// disable operator =
	ScopedThread& operator=(const ScopedThread&) = delete;

private:
	std::thread t;
};

} /* namespace utils */

#endif /* SRC_UTILS_SCOPEDTHREAD_H_ */
