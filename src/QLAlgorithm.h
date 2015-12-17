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
 * QLAlgorithm.h
 *
 *  Created on: Dec 16, 2015
 *      Author: Gianluca Tiepolo <tiepolo.gian@gmail.com>
 */

#ifndef QLALGORITHM_H_
#define QLALGORITHM_H_

#include "QLPolicy.h"
#include "QLLookupTable.h"

namespace QLLib {

/*
 * QLAlgorithm Class
 * The QLAlgorithm class is an abstract class that represents the base of an algorithm.
 * All algorithms must inherit from this class
 */
class QLAlgorithm {
public:
	/*
	 * QLAlgorithm Constructor
	 */
	QLAlgorithm(double initialQ) : _initialQ(initialQ) {};

	virtual ~QLAlgorithm() {};

	/*
	 * Initializes the algorithm by passing it all available states and actions
	 * \param states A vector of QLStates
	 * \param actions A vector of QLActions
	 */
	virtual void init(std::vector<QLLib::QLState*> states, std::vector<QLLib::QLAction*> actions) = 0;

	/*
	 * Assigns a policy to the algorithm
	 * \param policy An instance of QLPolicy
	 */
	void setPolicy(QLPolicy* policy) {
		_policy = policy;
	};

	/*
	 * Returns a pointer to the algorithm's policy
	 */
	QLPolicy* getPolicy() {
		return _policy;
	};

	/*
	 * Performs a step by passing the algorithm the current state.
	 * This method must be implemented by all algorithms and returns a pointer to the action to perform.
	 * \param currentState The state the agent is currently in
	 */
	virtual QLLib::QLAction* step(QLLib::QLState *currentState) = 0;

	/*
	 * Updates the Q-value for the given state-action combination.
	 * This method must be implemented by all algorithms.
	 * \param previousState An instance of QLState
	 * \param action An instance of QLAction
	 * \param r The reward received after the state->action
	 * \param currentState An instance of QLState
	 */
	virtual void updateQ(QLLib::QLState *previousState, QLLib::QLAction *action, double r, QLLib::QLState *currentState) = 0;
protected:
	double _initialQ;
	std::vector<QLLib::QLAction*> _actions;
private:
	QLPolicy *_policy = nullptr;
};

/*
 * QLearningAlgorithm Class
 * The QLearningAlgorithm class implements the Q-learning algorithm
 * (http://www.cs.huji.ac.il/~nir/Papers/DFR1.pdf)
 */
class QLearningAlgorithm : public QLAlgorithm {
public:
	/*
	 * QLearningAlgorithm Constructor
	 * \param initialQ The default Q-value for all state-action combinations
	 */
	QLearningAlgorithm(double initialQ, double alpha, double gamma) : QLAlgorithm(initialQ), _alpha(alpha), _gamma(gamma) {};

	virtual ~QLearningAlgorithm() {};

	/*
	 * Initializes the Q-learning algorithm by setting the default value for all state-action combinations in the lookup table
	 * \param states A vector of all available states
	 * \param actions A vector of all available actions
	 */
	virtual void init(std::vector<QLLib::QLState*> states, std::vector<QLLib::QLAction*> actions) {
		_actions = actions;
		for(auto i : states) {
			for(auto j : actions) {
				_table.setStateAndAction(*i, *j, _initialQ);
			}
		}
	};

	/*
	 * Performs a step by passing the algorithm the current state.
	 * \param currentState The state the agent is currently in
	 */
	virtual QLLib::QLAction* step(QLLib::QLState *currentState) {
		// If no policy has been provided to the algorithm, use NormalPolicy
		if(getPolicy() == nullptr) {
			QLLib::QLPolicy *normalPolicy = new QLLib::NormalPolicy();
			setPolicy(normalPolicy);
			std::cout << "[WARNING] No policy specified for QLearningAlgorithm, defaulting to NormalPolicy" << std::endl;
		}
		// allocate space for all actions
		double q[_actions.size()];
		// load Q-value for all actions
		for(size_t i=0;i<_actions.size();i++) {
			q[i] = _table.lookupStateAndAction(*currentState, *_actions[i]);
		}
		// return the best action based on the algorithm's policy
		return _actions[getPolicy()->sampleAction(q, _actions.size())];
	};

	/*
	 * Updates the Q-value for the given state-action combination.
	 * \param previousState An instance of QLState
	 * \param action An instance of QLAction
	 * \param r The reward received after the state->action
	 * \param currentState An instance of QLState
	 *
	 * Q = Q(S,A) + alpha * (R + gamma * maxQ(S',A) - Q(S,A))
	 */
	virtual void updateQ(QLLib::QLState *previousState, QLLib::QLAction *action, double r, QLLib::QLState *currentState) {
		double oldQ = _table.lookupStateAndAction(*previousState, *action);
		double maxQ = getMaxQ(currentState);
		double newQ = oldQ + _alpha * (r + (_gamma * maxQ) - oldQ);
		_table.setStateAndAction(*previousState, *action, newQ);
	};
private:
	/*
	 * Finds max Q value for the given state
	 * \param state An instance of QLState
	 */
	double getMaxQ(QLLib::QLState *state) {
		// allocate space for all actions
		double q[_actions.size()];
		// load Q for all actions
		for(size_t i=0;i<_actions.size();i++) {
			q[i] = _table.lookupStateAndAction(*state, *_actions[i]);
		}
		// find max Q
		// TODO: Fix this, as it's slightly biased when two or more values are equal
		// FIX: Save all MAX indices in array, generate random int between 0 and array lenght-1 and return q[array[random]]
		int maxIndex = 0;
		for (size_t index = maxIndex; index < _actions.size(); index++) {
			if (q[maxIndex] < q[index]) {
				maxIndex = index;
			}
		}
		return q[maxIndex];
	};

	QLLookupTable _table;
	double _alpha;
	double _gamma;
};

} /* namespace QLLib */

#endif /* QLALGORITHM_H_ */
