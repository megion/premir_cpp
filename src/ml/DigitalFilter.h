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

}

#endif /* SRC_ML_DIGITAL_FILTER_H_ */
