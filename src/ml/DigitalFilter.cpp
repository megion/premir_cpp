#include "DigitalFilter.h"

namespace ml {

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

