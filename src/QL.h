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
 * QL.h
 *
 *  Created on: Dec 15, 2015
 *      Author: Gianluca Tiepolo <tiepolo.gian@gmail.com>
 */

#ifndef QL_H_
#define QL_H_

#include "QLProblem.h"

namespace QLLib {

/*
 * QL Class - Controls simulation and event loop
 */
class QL {
public:
	/*
	 * The constructor initializes the QLProblem
	 * \param p An instance of a QLProblem
	 */
	QL(QLLib::QLProblem *p) : _problem(p) {
		_problem->init();
	};

	virtual ~QL() {};

	/*
	 * Start the simulation and run it 'n' times
	 * \param n The number of times you want the simulation to run
	 */
	void start(int n) {
		for (int i = 1; (i <= n && _runTrial); i++) {
			loop();
		}
	};

	/*
	 * Start the simulation and run it forever
	 */
	void start() {
		while(_runTrial) {
			loop();
		}
	};

	/*
	 * Stop the simulation
	 */
	void stop() {
		_runTrial = false;
	};

	/*
	 * Create an event listener that notifies when a simulation ends
	 * \param cb The callback function (lambda) that will be called when the simulation ends
	 */
	void addEventListener(std::function<void(QLLib::Utils::Stats)> cb) {
		_callback = cb;
	};
private:
	/*
	 * Starts the event loop
	 */
	void loop() {
		_stepsPerTrial = 0;
		_rewardsPerTrial = 0.0;
		QLLib::QLAgent *myAgent = _problem->getAgent();
		_problem->getAlgorithm()->initEpisode();
		while (!_trialEnded) {
			_stepsPerTrial++;
			_totalSteps++;
			// Run the algorithm and get the resulting action
			QLLib::QLAction *actionTaken = _problem->getAlgorithm()->step(myAgent->getCurrentState());
			// Tell the agent which action to perform
			myAgent->setAgentAction(actionTaken);
			// Run action
			actionTaken->performAction(myAgent->getCurrentState());
			// Check if we reached the goal
			_trialEnded = !_problem->step();
			// Get the reward...
			double reward = _problem->reward();
			_rewardsPerTrial += reward;
			// ...and pass it to the algorithm to update Q
			_problem->getAlgorithm()->updateQ(myAgent->getPreviousState(), myAgent->getLastAction(), reward, myAgent->getCurrentState());
		}
		// Signal the end of the simulation
		_problem->endOfTrial();
		_finishedTrials++;
		// Get some stats
		QLLib::Utils::Stats stats;
		stats.rewardsPerTrial = _rewardsPerTrial;
		stats.stepsPerTrial = _stepsPerTrial;
		stats.totalSteps = _totalSteps;
		stats.trialsCompleted = _finishedTrials;
		// Send the stats to the callback, if there is one
		if(_callback != nullptr) _callback(stats);
		_trialEnded = false;
	};

	QLLib::QLProblem *_problem;
	bool _trialEnded = false;
	bool _runTrial = true;
	int _stepsPerTrial = 0;
	int _totalSteps = 0;
	double _rewardsPerTrial = 0.0;
	int _finishedTrials = 0;
	std::function<void(QLLib::Utils::Stats)> _callback = nullptr;
};

} /* namespace QLLib */

#endif /* QL_H_ */
