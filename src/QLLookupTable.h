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
 * QLLookupTable.h
 *
 *  Created on: Dec 15, 2015
 *      Author: Gianluca Tiepolo <tiepolo.gian@gmail.com>
 */

#ifndef QLLOOKUPTABLE_H_
#define QLLOOKUPTABLE_H_

#include <unordered_map>
#include "QLStateAction.h"

namespace QLLib {

/*
 * QLLookupTable Class
 * The QLLookupTable class is an in-memory hash table that contains Q-values for all mapped state-action combinations
 */
class QLLookupTable {
public:
	typedef std::unordered_map<QLStateAction, double, QLStateActionHash> DataMap;

	/*
	 * QLLookupTable Constructor
	 */
	QLLookupTable() {};

	virtual ~QLLookupTable() {};

	/*
	 * Save Q-value for the specified state-action combination
	 * \param state An instance of QLState
	 * \param action An instance of QLAction
	 * \param value The Q-value to save
	 */
	void setStateAndAction(QLLib::QLState state, QLLib::QLAction action, double value) {
		_lookupTable[QLStateAction(state, action)] = value;
	};

	/*
	 * Lookup Q-value for the specified state-action combination
	 * \param state An instance of QLState
	 * \param action An instance of QLAction
	 */
	double lookupStateAndAction(QLLib::QLState state, QLLib::QLAction action) {
		return _lookupTable.at(QLStateAction(state, action));
	};
private:
	DataMap _lookupTable;
};

} /* namespace QLLib */

#endif /* QLLOOKUPTABLE_H_ */
