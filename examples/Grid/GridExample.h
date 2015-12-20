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
 * GridExample.h
 *
 *  Created on: Dec 20, 2015
 *      Author: Gianluca Tiepolo <tiepolo.gian@gmail.com>
 */

#ifndef GRIDEXAMPLE_H_
#define GRIDEXAMPLE_H_

#include <iostream>
#include <unistd.h>
#include "QLProblem.h"
#include "Grid.h"

class State: public QLLib::QLState {
public:
	State(std::string name, int x, int y, std::string type) : QLLib::QLState(name), _x(x), _y(y), _type(type) {};
	int _x;
	int _y;
	std::string _type = "normal";
};

class GridExample: public QLLib::QLProblem {
public:
	GridExample() : QLLib::QLProblem() {};
	virtual ~GridExample() {};
private:
	virtual void setupStates() {
		for (int i = 1; i <= 8; i++) {
			for (int j = 1; j <= 3; j++) {
				if(j == 3) {
					// set "danger" state
					State *dangerState = new State(QLLib::Utils::itos(i) + "," + QLLib::Utils::itos(j), i, j, "danger");
					addState(dangerState);
				} else if((j == 1) && (i != 1)) {
					State *dangerState = new State(QLLib::Utils::itos(i) + "," + QLLib::Utils::itos(j), i, j, "danger");
					addState(dangerState);
				} else {
					State *aState = new State(QLLib::Utils::itos(i) + "," + QLLib::Utils::itos(j), i, j, "normal");
					addState(aState);
				}
			}
		}
		// Set the initial state
		QLLib::QLState *initialState = getStateByName("1,1");
		getAgent()->setAgentState(initialState);

		// tell the grid which cells are "dangerous"
		myGrid = new Grid(8, 3);
		myGrid->setPosition(1,1);
		myGrid->setDangerPosition(1, 3);
		myGrid->setDangerPosition(2, 3);
		myGrid->setDangerPosition(3, 3);
		myGrid->setDangerPosition(4, 3);
		myGrid->setDangerPosition(5, 3);
		myGrid->setDangerPosition(6, 3);
		myGrid->setDangerPosition(7, 3);
		myGrid->setDangerPosition(8, 3);
		myGrid->setDangerPosition(2, 1);
		myGrid->setDangerPosition(3, 1);
		myGrid->setDangerPosition(4, 1);
		myGrid->setDangerPosition(5, 1);
		myGrid->setDangerPosition(6, 1);
		myGrid->setDangerPosition(7, 1);
		myGrid->setDangerPosition(8, 1);

		myGrid->print();
	};

	virtual void setupActions() {
		// Move the agent towards the left
		QLLib::QLAction *action1 = new QLLib::QLAction("Move Left", [this](QLLib::QLState *currentState) {
			State *now = dynamic_cast<State*>(currentState);
			if(isValidXPosition(now->_x-1)) {
				QLLib::QLState* newState = getStateByName(QLLib::Utils::itos(now->_x-1) + "," + QLLib::Utils::itos(now->_y));
				getAgent()->setAgentState(newState);
			} else {
				_visitedInvalidPosition = true;
			}
		});

		QLLib::QLAction *action2 = new QLLib::QLAction("Move Right", [this](QLLib::QLState *currentState) {
			State *now = dynamic_cast<State*>(currentState);
			if(isValidXPosition(now->_x+1)) {
				QLLib::QLState* newState = getStateByName(QLLib::Utils::itos(now->_x+1) + "," + QLLib::Utils::itos(now->_y));
				getAgent()->setAgentState(newState);
			} else {
				_visitedInvalidPosition = true;
			}
		});

		QLLib::QLAction *action3 = new QLLib::QLAction("Move Up", [this](QLLib::QLState *currentState) {
			State *now = dynamic_cast<State*>(currentState);
			if(isValidYPosition(now->_y+1)) {
				QLLib::QLState* newState = getStateByName(QLLib::Utils::itos(now->_x) + "," + QLLib::Utils::itos(now->_y+1));
				getAgent()->setAgentState(newState);
			} else {
				_visitedInvalidPosition = true;
			}
		});

		QLLib::QLAction *action4 = new QLLib::QLAction("Move Down", [this](QLLib::QLState *currentState) {
			State *now = dynamic_cast<State*>(currentState);
			if(isValidYPosition(now->_y-1)) {
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

	bool isValidXPosition(int position) {
		if(position < 1 || position > 8) return false;
		else return true;
	};

	bool isValidYPosition(int position) {
		if(position < 1 || position > 3) return false;
		else return true;
	};

	virtual void setupAlgorithm() {
		// We use 0.0 as the default Q-value, 0.9 for alpha (learning rate) and 0.9 for gamma (discount factor)
		QLLib::QLAlgorithm *algorithm = new QLLib::QLearningAlgorithm(0.0, 1.0, 0.9);
		QLLib::QLPolicy *policy = new QLLib::EpsilonGreedyPolicy(0.2);
		algorithm->setPolicy(policy);
		setAlgorithm(algorithm);
	};

	virtual bool step() {
		steps++;
		// Check if we reached the goal (the agent is in position 8x2)
		State *currentState = dynamic_cast<State*>(getAgent()->getCurrentState());
		if(getAgent()->getCurrentState()->getName() == "8,2") {
			myGrid->setPosition(1,1);
			myGrid->print();
			steps = 0;
			// sleep a bit so user can see grid updates
#ifdef __linux__
			usleep(1000*300);
#endif
			return false;
		} else if(currentState->_type == "danger") {
			myGrid->setPosition(1,1);
			myGrid->print();
			steps = 0;
			// sleep a bit so user can see grid updates
#ifdef __linux__
			usleep(1000*300);
#endif
			return false;
		} else {
			return true;
		}
	};

	virtual double reward() {
		// If it's an invalid state (crashing into the walls), the reward is -50
		if(_visitedInvalidPosition) {
			_visitedInvalidPosition = false;
			getAgent()->_previousState = getAgent()->getCurrentState();
			return -50.0;
		} else {
			// If the new state is valid, we calculate the reward
			State *currentState = dynamic_cast<State*>(getAgent()->getCurrentState());

			// Check if it's a danger state
			if(currentState->_type == "danger") {
				return -50.0;
			}

			myGrid->setPosition(currentState->_x,currentState->_y);
			myGrid->update(currentState->_x, currentState->_y, getAgent()->getLastAction()->getName(), steps);
			// sleep a bit so user can see grid updates
#ifdef __linux__
			usleep(1000*300);
#endif

			double distanceX = abs(_goal->_x - currentState->_x);
			double distanceY = abs(_goal->_y - currentState->_y);
			double totalDistance = distanceX+distanceY;
			// We use relative rewards here: if the new state is nearer to the goal than the previous one,
			// we return a positive reward; if not, we return a negative reward
			if(totalDistance <= _distance) {
				_distance = totalDistance;
				return 10.0;
			} else {
				return -10.0;
			}
		}
	};

	virtual void endOfTrial() {
		// Trial ended, reset the agent to the initial state
		QLLib::QLState *initialState = getStateByName("1,1");
		getAgent()->setAgentState(initialState);
		myGrid->_iterations++;
	};

	State *_goal = new State("Goal", 8, 2, "normal");
	bool _visitedInvalidPosition = false;
	double _distance = 8.0;
	int steps = 0;
	Grid *myGrid = nullptr;
};

#endif /* GRIDEXAMPLE_H_ */
