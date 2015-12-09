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
		inputsQueue = new utils::LinkedList<double>();
		weightsArray = new utils::LinkedList<double>();
	}
	~DigitalFilter() {
		delete inputsQueue;
		delete weightsArray;
	}

	utils::LinkedList<double>* getInputsQueue() {
		return inputsQueue;
	}
	utils::LinkedList<double>* getWeightsArray() {
		return weightsArray;
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
	utils::LinkedList<double>* inputsQueue; // очередь входов x2,x1,x0
	utils::LinkedList<double>* weightsArray; // массив весов w0,w1,w2

};

void DigitalFilter::addInput(double input) {
	double defaultWeight = 0.0; // default weight = 0.0
	inputsQueue->unshift(input); // add to start
	weightsArray->push(defaultWeight);
	// check dimension
	if (inputsQueue->size() > dimension) {
		inputsQueue->pop(); // delete last
		weightsArray->shift(); // delete first
	}
}

double DigitalFilter::calculateFilterOutSum() {
	double sum = 0.0;
	utils::LinkedList<double>::Iterator inputsIter = inputsQueue->iterator();
	utils::LinkedList<double>::Iterator weightsIter = weightsArray->iterator();
	while (inputsIter.hasNext()) {
		// iterators have same sizes
		utils::LinkedList<double>::Entry* inputsEntry = inputsIter.next();
		utils::LinkedList<double>::Entry* weightsEntry = weightsIter.next();
		double& input = inputsEntry->getValue();
		double& weight = weightsEntry->getValue();
		sum = sum + input * weight;
	}
	return sum;
}

double DigitalFilter::updateFilterWeightsByLeastSquaresAlgorithm(
		double signalAndNoise, double filterOutSum) {
	double evaluationError = signalAndNoise - filterOutSum;

	utils::LinkedList<double>::Iterator inputsIter = inputsQueue->iterator();
	utils::LinkedList<double>::Iterator weightsIter = weightsArray->iterator();
	double gk = 2.0 * speedStability * evaluationError;
	while (inputsIter.hasNext()) {
		// iterators have same sizes
		utils::LinkedList<double>::Entry* inputsEntry = inputsIter.next();
		utils::LinkedList<double>::Entry* weightsEntry = weightsIter.next();
		double& input = inputsEntry->getValue();
		double& weight = weightsEntry->getValue();

		// алгоритм наименьших квадратов Уидроу-Хоффа
		// W(k+1) = W(k) + 2*u*e*s.input
		double wk = weight + gk * input;
		weight = wk; // update value ref
	}
	return evaluationError;
}

} /* namespace ml */

#endif /* SRC_ML_DIGITALFILTER_H_ */
