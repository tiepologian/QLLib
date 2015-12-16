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
 * QLAgent.h
 *
 *  Created on: Dec 16, 2015
 *      Author: Gianluca Tiepolo <tiepolo.gian@gmail.com>
 */

#ifndef QLAGENT_H_
#define QLAGENT_H_

#include "QLState.h"
#include "QLAction.h"

namespace QLLib {

/*
 * QLAgent Class
 * The QLAgent class represents the agent that moves from one state to another in the environment, performing actions.
 * Note that the agent known nothing at all about the environment:
 * it only knows its current and previous state and it remembers the last action it performed.
 */
class QLAgent {
public:
	/*
	 * QLAgent Constructor
	 */
	QLAgent() {};

	virtual ~QLAgent() {};

	/*
	 * Returns a pointer to the agent's current state
	 */
	QLLib::QLState* getCurrentState() {
		return _currentState;
	};

	/*
	 * Returns a pointer to the agent's previous state
	 */
	QLLib::QLState* getPreviousState() {
		return _previousState;
	};

	/*
	 * Returns a pointer to the last action the agent performed
	 */
	QLLib::QLAction* getLastAction() {
		return _currentAction;
	};

	/*
	 * Sets the agent's new state
	 * \param s An instance of QLState
	 */
	void setAgentState(QLLib::QLState *s) {
		_previousState = _currentState;
		_currentState = s;
	};

	/*
	 * Rollback the agent's position
	 * This is useful in the case where the agent moves to an invalid state
	 */
	void rollbackAgentState() {
		_currentState = _previousState;
	};

	/*
	 * Sets the agent's latest action
	 * \param a An instance of QLAction
	 */
	void setAgentAction(QLLib::QLAction *a) {
		_currentAction = a;
	};

	QLLib::QLState *_previousState = nullptr;
private:
	QLLib::QLState *_currentState = nullptr;

	QLLib::QLAction *_currentAction = nullptr;
};

} /* namespace QLLib */

#endif /* QLAGENT_H_ */
