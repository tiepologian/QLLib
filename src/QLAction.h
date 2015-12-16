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
 * QLAction.h
 *
 *  Created on: Dec 15, 2015
 *      Author: Gianluca Tiepolo <tiepolo.gian@gmail.com>
 */

#ifndef QLACTION_H_
#define QLACTION_H_

#include <functional>

namespace QLLib {

/*
 * QLAction Class
 * This class represents an action that the agent can perform
 * You can inherit from this class and create your own actions as you like
 */
class QLAction {
public:
	/*
	 * QLAction Constructor
	 * \param actionName The name that uniquely identifies the action
	 * \param actionFunction The lambda that will be called when the action is performed
	 */
	QLAction(std::string actionName, std::function<void(QLLib::QLState *state)> actionFunction) : _name(actionName), _action(actionFunction) {};

	virtual ~QLAction() {};

	/*
	 * Returns the action's printable name
	 */
	std::string getName() const {
		return _name;
	};

	/*
	 * Performs the action by running the associated lambda function
	 * \param s The state the agent is currently in
	 */
	void performAction(QLLib::QLState *s) {
		_action(s);
	};
private:
	std::string _name;
	std::function<void(QLLib::QLState *state)> _action;
};

} /* namespace QLLib */

#endif /* QLACTION_H_ */
