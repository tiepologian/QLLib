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
 * main.cpp
 *
 *  Created on: Dec 15, 2015
 *      Author: Gianluca Tiepolo <tiepolo.gian@gmail.com>
 */

#include "QL.h"
#include "RobotExample.h"

using namespace QLLib;

int main(int argc, char *argv[]) {
	// Create an instance of the RobotExample and pass it to the library
	QLProblem *example = new RobotExample();
	QLLib::QL *ql = new QLLib::QL(example);

	// Create an event listener that will be called at the end of each simulation
	ql->addEventListener([](QLLib::Utils::Stats stats) {
		std::cout << "Finished trial: " << stats.trialsCompleted << std::endl;
		std::cout << "Steps: " << stats.stepsPerTrial << std::endl;
		std::cout << "Rewards/trial: " << stats.rewardsPerTrial << std::endl;
		std::cout << "Rewards/step: " << stats.rewardsPerTrial/stats.stepsPerTrial << std::endl << std::endl;
	});

	// Run 10 simulations (if you don't specify a number, it will run forever)
	ql->start(2000);

	// Remember to clean-up!
	delete ql;
	delete example;
	return 0;
}
