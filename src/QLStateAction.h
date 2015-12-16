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
 * QLStateAction.h
 *
 *  Created on: Dec 15, 2015
 *      Author: Gianluca Tiepolo <tiepolo.gian@gmail.com>
 */

#ifndef QLSTATEACTION_H_
#define QLSTATEACTION_H_

#include "QLState.h"
#include "QLAction.h"

namespace QLLib {

/*
 * QLStateAction Class
 * This is a utiliy class that maps together a state and an action.
 * This is used so that the combination of the two can be used as the map's key
 */
class QLStateAction {
public:
	/*
	 * QLStateAction Constructor
	 * \param s An instance of QLState
	 * \param a An instance of QLAction
	 */
	QLStateAction(QLLib::QLState s, QLLib::QLAction a) : state(s), action(a) {};

	virtual ~QLStateAction() {};

	QLLib::QLState state;
	QLLib::QLAction action;

	/*
	 * Overloading of the equality operator (the map needs this to compare states-actions)
	 */
	bool operator==(const QLStateAction& sa) const {
		return ((state.getName() == sa.state.getName())
				&& (action.getName() == sa.action.getName()));
	};
};

/*
 * QLStateActionHash Class
 * This class creates a unique hash based on the state's and action's name
 */
class QLStateActionHash {
public:
	size_t operator()(const QLStateAction& x) const {
		return std::hash<std::string>()(x.state.getName()) ^ std::hash<std::string>()(x.action.getName());
	};
};

} /* namespace QLLib */

#endif /* QLSTATEACTION_H_ */
