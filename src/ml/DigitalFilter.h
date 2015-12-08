/*
 * DigitalFilter.h
 *
 * Implementation of digital filter
 *
 *  Created on: Dec 8, 2015
 *      Author: ilya
 */

#ifndef SRC_ML_DIGITALFILTER_H_
#define SRC_ML_DIGITALFILTER_H_

#include <iostream>

#include "utils/LinkedList.h"

namespace ml {

class DigitalFilter {
public:
	DigitalFilter(size_t _dimension) :
			dimension(_dimension) {
		samplesQueue = new utils::LinkedList<Sample>();
	}
	~DigitalFilter() {
		delete samplesQueue;
	}

	// filter sample
	struct Sample {
		double input; // input value
		double weight; // weight
	};

	void addInput(double input);
	double calculateSum();

private:

	// filter dimension
	size_t dimension;
	utils::LinkedList<Sample>* samplesQueue;

};

void DigitalFilter::addInput(double input) {
	Sample s = { input, 1.0 }; // default weight = 1.0
	samplesQueue->unshift(s); // add to start
	// check dimension
	if (samplesQueue->size() > dimension) {
		// delete last
		samplesQueue->pop();
	}
}

double DigitalFilter::calculateSum() {
	double sum = 0.0;
	utils::LinkedList<Sample>::Iterator iter = samplesQueue->iterator();
	while (iter.hasNext()) {
		utils::LinkedList<Sample>::Entry* e = iter.next();
		Sample& s = e->getValue();
		sum = sum + s.input * s.weight;
	}
	return sum;
}

} /* namespace ml */

#endif /* SRC_ML_DIGITALFILTER_H_ */
