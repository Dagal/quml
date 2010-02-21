/*******************************************************************
* Copyright (c) 2009 Thomas Fannes (thomasfannes@gmail.com)
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************/

#include "idiagramcontroller.hpp"
#include "_idiagramcontroller.hpp"
#include "algorithm.hpp"
#include <boost/bind.hpp>


void IDiagramController::addActionListener(IActionListener * listener, int actionMask)
{
	_dd->_actionListeners.insert(IDiagramControllerPrivate::ActionListenerData(listener, actionMask));
}

void IDiagramController::removeActionListener(IActionListener * listener)
{
	IDiagramControllerPrivate::actionListenerSet::iterator i = std::find_if(
			_dd->_actionListeners.begin(),
			_dd->_actionListeners.end(),
			stf::is_equal(
					boost::bind(&IDiagramControllerPrivate::ActionListenerData::_listener, _1),
					listener
					)
			);

	if(i != _dd->_actionListeners.end())
		_dd->_actionListeners.erase(i);
}

void IDiagramController::addErrorListener(IErrorListener * listener)
{
	_dd->
}

void IDiagramController::removeErrorListener(IErrorListener * listener);

const std::string & IDiagramController::errorMessage() const;

void IDiagramController::setErrorMessage(const std::string & newError);

void IDiagramController::IDiagramControllerPrivate::sendErrorMessage();
void IDiagramController::IDiagramControllerPrivate::sendAction(const IAction & action);


bool operator==(const IDiagramController::IDiagramControllerPrivate::ActionListenerData & first, IActionListener * second)
{
	return first._listener == second;
}

