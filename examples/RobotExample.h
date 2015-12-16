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
 * RobotExample.h
 *
 *  Created on: Dec 15, 2015
 *      Author: Gianluca Tiepolo <tiepolo.gian@gmail.com>
 */

#ifndef ROBOTEXAMPLE_H_
#define ROBOTEXAMPLE_H_

#include "QLProblem.h"
#include <iostream>

/*
 * We're creating our custom State class, so that we can save X and Y coordinates
 */
class State: public QLLib::QLState {
public:
	State(std::string name, int x, int y) : QLLib::QLState(name), _x(x), _y(y) {};
	int _x;
	int _y;
};

/*
 * In this simple example our agent is a robot that starts in position 3x3
 * and searches for the 'goal' that is in position 10x10
 */
class RobotExample: public QLLib::QLProblem {
public:
	RobotExample() : QLLib::QLProblem() {};
	virtual ~RobotExample() {};
private:
	/*
	 * This is the first method that needs to be implemented
	 * Here we create all available states and add them to the problem
	 */
	virtual void setupStates() {
		for (int i = 1; i < 11; i++) {
			for (int j = 1; j < 11; j++) {
				State *aState = new State(QLLib::Utils::itos(i) + "," + QLLib::Utils::itos(j), i, j);
				addState(aState);
			}
		}
		// Set the initial state
		QLLib::QLState *initialState = getStateByName("3,3");
		getAgent()->setAgentState(initialState);
	};

	/*
	 * This is the second method we need to impelement
	 * Here we create all available actions, associating them with a lambda function
	 * that is called when the action is performed
	 */
	virtual void setupActions() {
		// Move the agent towards the left
		QLLib::QLAction *action1 = new QLLib::QLAction("Move Left", [this](QLLib::QLState *currentState) {
			// Here we cast the current state to our custom State class, so that we can access X and Y members
			State *now = dynamic_cast<State*>(currentState);
			// Check if the agent moves outside the 'walls' (the grid)
			if(isValidPosition(now->_x-1)) {
				// Get a pointer to the new state (after the robot moves)
				QLLib::QLState* newState = getStateByName(QLLib::Utils::itos(now->_x-1) + "," + QLLib::Utils::itos(now->_y));
				// Set the agent's new position (new state)
				getAgent()->setAgentState(newState);
			} else {
				// As the new position would be invalid, we don't update the state
				_visitedInvalidPosition = true;
			}
		});

		QLLib::QLAction *action2 = new QLLib::QLAction("Move Right", [this](QLLib::QLState *currentState) {
			State *now = dynamic_cast<State*>(currentState);
			if(isValidPosition(now->_x+1)) {
				QLLib::QLState* newState = getStateByName(QLLib::Utils::itos(now->_x+1) + "," + QLLib::Utils::itos(now->_y));
				getAgent()->setAgentState(newState);
			} else {
				_visitedInvalidPosition = true;
			}
		});

		QLLib::QLAction *action3 = new QLLib::QLAction("Move Up", [this](QLLib::QLState *currentState) {
			State *now = dynamic_cast<State*>(currentState);
			if(isValidPosition(now->_y+1)) {
				QLLib::QLState* newState = getStateByName(QLLib::Utils::itos(now->_x) + "," + QLLib::Utils::itos(now->_y+1));
				getAgent()->setAgentState(newState);
			} else {
				_visitedInvalidPosition = true;
			}
		});

		QLLib::QLAction *action4 = new QLLib::QLAction("Move Down", [this](QLLib::QLState *currentState) {
			State *now = dynamic_cast<State*>(currentState);
			if(isValidPosition(now->_y-1)) {
				QLLib::QLState* newState = getStateByName(QLLib::Utils::itos(now->_x) + "," + QLLib::Utils::itos(now->_y-1));
				getAgent()->setAgentState(newState);
			} else {
				_visitedInvalidPosition = true;
			}
		});
		// Add the actions to the problem
		addAction(action1);
		addAction(action2);
		addAction(action3);
		addAction(action4);
	};
	/*
	 * Check if the position is valid of if it is outside the 10x10 grid
	 */
	bool isValidPosition(int position) {
		if(position < 1 || position > 10) return false;
		else return true;
	};

	/*
	 * This is the third method we must implement
	 * Here, we specify the algorithm and policy we want to use
	 */
	virtual void setupAlgorithm() {
		// We use 1.0 as the default Q-value
		QLLib::QLAlgorithm *algorithm = new QLLib::QLearningAlgorithm(1.0);
		QLLib::QLPolicy *policy = new QLLib::NormalPolicy();
		algorithm->setPolicy(policy);
		setAlgorithm(algorithm);
	};

	/*
	 * This is the forth method we must implement
	 * The step() method must return true if the simulation must go on
	 * or return false if we reach the goal
	 */
	virtual bool step() {
		// Check if we reached the goal (the agent is in position 10x10)
		if(getAgent()->getCurrentState()->getName() == "10,10") return false;
		else return true;
	};

	/*
	 * This is the fifth method we must implement
	 * This method returns the reward for the new state
	 */
	virtual double reward() {
		// If it's an invalid state, the reward is 0
		if(_visitedInvalidPosition) {
			_visitedInvalidPosition = false;
			getAgent()->_previousState = getAgent()->getCurrentState();
			return 0.0;
		} else {
			// If the new state is valid, we calculate the reward (needs to be normalized)
			State *currentState = dynamic_cast<State*>(getAgent()->getCurrentState());
			double distanceX = abs(_goal->_x - currentState->_x);
			double distanceY = abs(_goal->_y - currentState->_y);
			double totalDistance = distanceX+distanceY;
			double maxDistance = 20;
			double reward = 1-(totalDistance/maxDistance);
			return reward;
		}
	};

	/*
	 * This method is called by the simulator when the simulation is over
	 */
	virtual void endOfTrial() {
		// Trial ended, reset the agent to the initial state
		QLLib::QLState *initialState = getStateByName("3,3");
		getAgent()->setAgentState(initialState);
	};

	State *_goal = new State("Goal", 10, 10);
	bool _visitedInvalidPosition = false;
};

#endif /* ROBOTEXAMPLE_H_ */
