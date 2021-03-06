/*
 * Copyright 2015 Gianluca Tiepolo <tiepolo.gian@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * QLPolicy.h
 *
 *  Created on: Dec 16, 2015
 *      Author: Gianluca Tiepolo <tiepolo.gian@gmail.com>
 */

#ifndef QLPOLICY_H_
#define QLPOLICY_H_

#include "QLAction.h"

namespace QLLib {

/*
 * QLPolicy Class
 * The QLPolicy class is an abstract class that represents a generic policy
 */
class QLPolicy {
public:
	/*
	 * QLPolicy Constructor
	 */
	QLPolicy() {};

	virtual ~QLPolicy() {};

	/*
	 * Apply the policy to the provided Q-values and get the chosen action
	 * \param Q An array of Q-values
	 * \param count The size of Q
	 */
	virtual int sampleAction(double Q[], int count) = 0;
};

/*
 * NormalPolicy Class
 * This class represents the traditional policy (Greedy) for Q-learning algorithm
 */
class NormalPolicy : public QLPolicy {
public:
	/*
	 * NormalPolicy Constructor
	 */
	NormalPolicy() {};

	virtual ~NormalPolicy() {};

	/*
	 * Apply the policy to the provided Q-values and get the best possible action (greedy)
	 * \param Q An array of Q-values
	 * \param count The size of Q
	 */
	virtual int sampleAction(double Q[], int count) {
		if(QLLib::Utils::arrayValuesEqual(Q, count)) {
			// all actions have same Q, choose randomly
			return sampleRandomAction(count);
		} else {
			return getIndexOfLargestElement(Q, count);
		}
	};
private:
	/*
	 * Samples a random action within the provided range
	 */
	int sampleRandomAction(int count) {
		return Utils::iRand(0, count-1);
	};

	/*
	 * Samples the best action based on the largest Q-value
	 */
	int getIndexOfLargestElement(double arr[], int size) {
		int largestIndex = 0;
		// TODO: Fix this, as it gives slightly polarized results when there are equal values
		for (int index = largestIndex; index < size; index++) {
			if (arr[largestIndex] < arr[index]) {
				largestIndex = index;
			}
		}
		return largestIndex;
	}
};

/*
 * RandomPolicy Class
 */
class RandomPolicy : public QLPolicy {
public:
	/*
	 * RandomPolicy Constructor
	 */
	RandomPolicy() {};

	virtual ~RandomPolicy() {};

	/*
	 * Samples a random action
	 */
	virtual int sampleAction(double Q[], int count) {
		return sampleRandomAction(count);
	};
private:
	/*
	 * Samples a random action within the provided range
	 */
	int sampleRandomAction(int count) {
		return Utils::iRand(0, count-1);
	};
};

/*
 * EpsilonGreedyPolicy Class
 * This class implements the E-Greedy policy
 */
class EpsilonGreedyPolicy : public QLPolicy {
public:
	/*
	 * EpsilonGreedyPolicy Constructor
	 */
	EpsilonGreedyPolicy(double epsilon) : _epsilon(epsilon) {};

	virtual ~EpsilonGreedyPolicy() {};

	/*
	 * Apply the policy to the provided Q-values and get the chosen action
	 */
	virtual int sampleAction(double Q[], int count) {
		double probability = Utils::fRand(0.0, 1.0);
		if(probability < _epsilon) {
			// Choose random action
			return sampleRandomAction(count);
		} else {
			// Choose action with best Q
			return sampleBestAction(Q, count);
		}
	};
private:
	/*
	 * Samples the best action possible
	 */
	int sampleBestAction(double Q[], int count) {
		if(QLLib::Utils::arrayValuesEqual(Q, count)) {
			// all actions have same Q, choose randomly
			return sampleRandomAction(count);
		} else {
			return getIndexOfLargestElement(Q, count);
		}
	};

	/*
	 * Samples a random action
	 */
	int sampleRandomAction(int count) {
		return Utils::iRand(0, count-1);
	};

	/*
	 * Get action with largest Q
	 */
	int getIndexOfLargestElement(double arr[], int size) {
		int largestIndex = 0;
		// TODO: Fix this, as it gives slightly polarized results when there are equal values
		for (int index = largestIndex; index < size; index++) {
			if (arr[largestIndex] < arr[index]) {
				largestIndex = index;
			}
		}
		return largestIndex;
	}

	double _epsilon;
};

/*
 * SoftmaxPolicy Class
 * This class implements the Softmax policy
 */
class SoftmaxPolicy : public QLPolicy {
public:
	/*
	 * SoftmaxPolicy Constructor
	 */
	SoftmaxPolicy(double t) : _temperature(t) {};

	virtual ~SoftmaxPolicy() {};

	/*
	 * Samples the best possible action based on the Softmax algorithm
	 *
	 * A = exp(Q/t) / E(exp(Q/t))
	 *
	 * A = chosen action
	 * Q = Q-value for state-action
	 * E = sum
	 * t = temperature
	 */
	virtual int sampleAction(double Q[], int count) {
		double pQ[count];
		double totalP = 0.0;
		// Calculate P of all actions
		for(int i=0;i<count;i++) {
			pQ[i] = exp(Q[i]/_temperature);
			// Update sum of P
			totalP+=pQ[i];
		}
		// Divide P by sum of all P
		for(int i=0;i<count;i++) {
			pQ[i] = pQ[i] / totalP;
		}
		// Randomly choose based on probability - http://stackoverflow.com/a/2649761
		double p = (rand() / static_cast<double>(RAND_MAX)) * 1.0;
		double* current = &pQ[0];
		int index = 0;
		while ((p -= *current) > 0) {
		    ++current;
		    index++;
		}
		return index;
	};
private:
	double _temperature;
};

} /* namespace QLLib */

#endif /* QLPOLICY_H_ */
