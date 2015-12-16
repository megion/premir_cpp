#ifndef SRC_ML_DIGITAL_FILTER_H_
#define SRC_ML_DIGITAL_FILTER_H_

#include <iostream>

#include "utils/LinkedList.h"
#include "utils/CArrayList.h"

namespace ml {

class DigitalFilter {
public:
	DigitalFilter(size_t _dimension, double _speedStability,
			double _defaultWeight = 0.0) :
			dimension(_dimension), speedStability(_speedStability), //
			defaultWeight(_defaultWeight) {
		inputsQueue = new utils::LinkedList<double>();
		weightsArray = new utils::CArrayList<double>(dimension);
	}
	~DigitalFilter() {
		delete inputsQueue;
		delete weightsArray;
	}

	utils::LinkedList<double>* getInputsQueue() {
		return inputsQueue;
	}
	utils::CArrayList<double>* getWeightsArray() {
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
	double defaultWeight = 0.0; // default weight = 0.0
	// filter dimension
	size_t dimension;
	// параметр определяющий скорость и устойчивость процесса адаптации
	double speedStability;
	utils::LinkedList<double>* inputsQueue; // очередь входов x2,x1,x0
	utils::CArrayList<double>* weightsArray; // массив весов w0,w1,w2

};

void DigitalFilter::addInput(double input) {
	inputsQueue->unshift(input); // add to start
	// check dimension
	if (dimension > inputsQueue->size()) {
		weightsArray->push(defaultWeight); // add to end
	} else {
		inputsQueue->pop(); // delete last
	}

}

double DigitalFilter::calculateFilterOutSum() {
	double sum = 0.0;
	utils::LinkedList<double>::Iterator inputsIter = inputsQueue->iterator();
	size_t i = 0;
	while (inputsIter.hasNext()) {
		// iterators have same sizes
		utils::LinkedList<double>::Entry* inputsEntry = inputsIter.next();
		double& input = inputsEntry->getValue();
		double& weight = (*weightsArray)[i];
		sum = sum + input * weight;
		++i;
	}
	return sum;
}

double DigitalFilter::updateFilterWeightsByLeastSquaresAlgorithm(
		double signalAndNoise, double filterOutSum) {
	double evaluationError = signalAndNoise - filterOutSum;

	utils::LinkedList<double>::Iterator inputsIter = inputsQueue->iterator();
	size_t i = 0;
	double gk = 2.0 * speedStability * evaluationError;
	while (inputsIter.hasNext()) {
		// iterators have same sizes
		utils::LinkedList<double>::Entry* inputsEntry = inputsIter.next();
		double& input = inputsEntry->getValue();
		double& weight = (*weightsArray)[i];

		// алгоритм наименьших квадратов Уидроу-Хоффа
		// W(k+1) = W(k) + 2*u*e*x(k-i)
		double wk = weight + gk * input;
		weight = wk; // update value ref
		++i;
	}
	return evaluationError;
}

}

#endif /* SRC_ML_DIGITAL_FILTER_H_ */
