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
 * QLState.h
 *
 *  Created on: Dec 15, 2015
 *      Author: Gianluca Tiepolo <tiepolo.gian@gmail.com>
 */

#ifndef QLSTATE_H_
#define QLSTATE_H_

namespace QLLib {

/*
 * QLState Class
 * The QLState class represents a 'state' that the agent can be in.
 * You can inherit from this class and structure it however you like
 */
class QLState {
public:
	/*
	 * QLState Constructor
	 * \param stateName The name that uniquely identifies the state
	 */
	QLState(std::string stateName) : _name(stateName) {};

	virtual ~QLState() {};

	/*
	 * Returns the state's printable name
	 */
	std::string getName() const {
		return _name;
	};
private:
	std::string _name;
};

} /* namespace QLLib */

#endif /* QLSTATE_H_ */
