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
#include <math.h>

#include "utils/LinkedList.h"

namespace ml {

class DigitalFilter {
public:
	DigitalFilter(size_t _dimension, double _speedStability) :
			dimension(_dimension), speedStability(_speedStability) {
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

	utils::LinkedList<Sample>* getSamplesQueue() {
		return samplesQueue;
	}

	void addInput(double input);

	/**
	 * вычисление выходного значения фильтра Виннера
	 */
	double calculateFilterOutSum();

	/**
	 * Пересчет весов фильтра по алгоритму наименьших квадратов Уидроу-Хоффа
	 * и вернуть оценку ошибки e = signalAndNoise - filterOutSum
	 */
	double updateFilterWeightsByLeastSquaresAlgorithm(double signalAndNoise,
			double filterOutSum);

private:

	// filter dimension
	size_t dimension;
	// параметр определяющий скорость и устойчивость процесса адаптации
	double speedStability;
	utils::LinkedList<Sample>* samplesQueue;

};

void DigitalFilter::addInput(double input) {
	Sample s = { input, 1.0 }; // default weight = 0.0
	samplesQueue->unshift(s); // add to start
	// check dimension
	if (samplesQueue->size() > dimension) {
		// delete last
		samplesQueue->pop();
	}
}

double DigitalFilter::calculateFilterOutSum() {
	double sum = 0.0;
	utils::LinkedList<Sample>::Iterator iter = samplesQueue->iterator();
	while (iter.hasNext()) {
		utils::LinkedList<Sample>::Entry* e = iter.next();
		Sample& s = e->getValue();
		sum = sum + s.input * s.weight;
	}
	return sum;
}

double DigitalFilter::updateFilterWeightsByLeastSquaresAlgorithm(
		double signalAndNoise, double filterOutSum) {
	double evaluationError = signalAndNoise - filterOutSum;

	utils::LinkedList<Sample>::Iterator iter = samplesQueue->iterator();
	while (iter.hasNext()) {
		utils::LinkedList<Sample>::Entry* e = iter.next();
		Sample& s = e->getValue();

		// алгоритм наименьших квадратов Уидроу-Хоффа
		// W(k+1) = W(k) + 2*u*e*s.input
		s.weight = s.weight + 2.0 * speedStability * evaluationError * s.input;
	}
	return evaluationError;
}

} /* namespace ml */

#endif /* SRC_ML_DIGITALFILTER_H_ */
