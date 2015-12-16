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
 * QLUtils.h
 *
 *  Created on: Dec 15, 2015
 *      Author: Gianluca Tiepolo <tiepolo.gian@gmail.com>
 */

#ifndef QLUTILS_H_
#define QLUTILS_H_

#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <random>

namespace QLLib {
namespace Utils {

/*
 * Stats Struct
 * A utility struct to group stats data
 */
struct Stats {
	int trialsCompleted = 0;
	int totalSteps = 0;
	int stepsPerTrial = 0;
	double rewardsPerTrial = 0.0;
};

/*
 * A utility function to convert an int to a string
 * This is for Windows users: MinGW on Windows doesn't have std::to_string yet :-(
 */
std::string itos(int i) {
	std::stringstream ss;
	ss << i;
	return ss.str();
}

/*
 * Check if all elements of an array are equal
 */
bool arrayValuesEqual(double arr[], int size) {
	double oldValue = arr[0];
	for(int i=0;i<size;i++) {
		if(oldValue != arr[i]) return false;
		else oldValue = arr[i];
	}
	return true;
}

/*
 * Generates a random float between fMin and fMax
 * This is NOT very precise
 */
double fRand(double fMin, double fMax) {
	double f = (double) rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

/*
 * Generates a random int between min and max
 * This is very precise, as it uses C++11
 */
int iRand(int min, int max) {
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(min,max);
	int randomNum = uni(rng);
	return randomNum;
}

} /* namespace Utils */
} /* namespace QLLib */

#endif /* QLUTILS_H_ */
