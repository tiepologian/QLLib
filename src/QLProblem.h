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
 * QLProblem.h
 *
 *  Created on: Dec 15, 2015
 *      Author: Gianluca Tiepolo <tiepolo.gian@gmail.com>
 */

#ifndef QLPROBLEM_H_
#define QLPROBLEM_H_

#include <iostream>
#include <vector>
#include <cstdlib>
#include "QLAgent.h"
#include "QLUtils.h"
#include "QLAlgorithm.h"

namespace QLLib {

/*
 * QLProblem Class
 * The QLProblem class represents the model of a Q-learning problem
 */
class QLProblem {
	friend class QL;
public:
	/*
	 * QLProblem Constructor
	 */
	QLProblem() {
		_agent = new QLLib::QLAgent();
		_algorithm = nullptr;
	};

	/*
	 * QLProblem Destructor
	 */
	virtual ~QLProblem() {
		for(auto i:_states) delete i;
		for(auto i:_actions) delete i;
		delete _algorithm;
		delete _agent;
	};

	/*
	 * Gets a vector of all available states
	 */
	std::vector<QLLib::QLState*> getAllStates() {
		return _states;
	};

	/*
	 * Get a vector of all available actions
	 */
	std::vector<QLLib::QLAction*> getAllActions() {
		return _actions;
	};

	/*
	 * Returns a pointer to the agent
	 */
	QLLib::QLAgent* getAgent() {
		return _agent;
	};

	/*
	 * Returns a pointer to the chosen algorithm
	 */
	QLLib::QLAlgorithm* getAlgorithm() {
		if(_algorithm == nullptr) {
			_algorithm = new QLLib::QLearningAlgorithm(0.0);
			std::cout << "[WARNING] No algorithm specified, defaulting to QLearningAlgorithm" << std::endl;
		}
		return _algorithm;
	};

	/*
	 * Sets the algorithm for the problem
	 */
	void setAlgorithm(QLLib::QLAlgorithm *a) {
		_algorithm = a;
	};
protected:
	/*
	 * Adds a state to the states vector
	 */
	void addState(QLLib::QLState *s) {
		_states.push_back(s);
	};

	/*
	 * Adds an action to the actions vector
	 */
	void addAction(QLLib::QLAction *a) {
		_actions.push_back(a);
	};

	/*
	 * Returns a pointer to a QLState searching by name
	 */
	QLLib::QLState* getStateByName(std::string name) {
		QLLib::QLState *s = nullptr;
		for(auto i : _states) {
			if(i->getName() == name) return i;
		}
		if(s == nullptr) {
			std::cout << "[ERROR] Could not find state \"" << name << "\"" << std::endl;
			exit(1);
		}
		return s;
	};
private:
	void init() {
		setupStates();
		setupActions();
		setupAlgorithm();
		getAlgorithm()->init(getAllStates(), getAllActions());
	};
	virtual void setupStates() = 0;
	virtual void setupActions() = 0;
	virtual void setupAlgorithm() = 0;
	virtual bool step() = 0;
	virtual double reward() = 0;
	virtual void endOfTrial() = 0;
	std::vector<QLLib::QLState*> _states;
	std::vector<QLLib::QLAction*> _actions;
	QLLib::QLAgent *_agent;
	QLLib::QLAlgorithm *_algorithm;
};

} /* namespace QLLib */

#endif /* QLPROBLEM_H_ */
