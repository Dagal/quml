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
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

IDiagramController::~IDiagramController()
{
}

void IDiagramController::addActionListener(IActionListener * listener, int actionMask)
{
	removeActionListener(listener);
	_dd->_actionListeners.push_back(ActionListenerData(listener, actionMask));
}

void IDiagramController::removeActionListener(IActionListener * listener)
{
	IDiagramControllerPrivate::actionListenerVct::iterator i = _dd->findActionListener(listener);

	if(i != _dd->_actionListeners.end())
		_dd->_actionListeners.erase(i);
}

void IDiagramController::addErrorListener(IErrorListener * listener)
{
	removeErrorListener(listener);
	_dd->_errorListeners.push_back(listener);
}

void IDiagramController::removeErrorListener(IErrorListener * listener)
{
	IDiagramControllerPrivate::errorListenerVct::iterator i = _dd->findErrorListener(listener);

	if(i != _dd->_errorListeners.end())
		_dd->_errorListeners.erase(i);
}

const std::string & IDiagramController::errorMessage() const
{
	return _dd->_errorMessage;
}

void IDiagramController::setErrorMessage(const std::string & newError)
{
	if(newError == _dd->_errorMessage)
		return;

	_dd->_errorMessage = newError;
	_dd->sendErrorMessage();
}

void IDiagramController::IDiagramControllerPrivate::sendErrorMessage()
{
	std::for_each(
			_errorListeners.begin(),
			_errorListeners.end(),
			boost::bind(&IErrorListener::onErrorMessage, _1, boost::cref(_errorMessage))
			);
}

void IDiagramController::IDiagramControllerPrivate::sendAction(const IAction & action)
{
	std::for_each(
			_actionListeners.begin(),
			_actionListeners.end(),
			boost::bind(&ActionListenerData::sendAction, _1, boost::cref(action))
			);
}

bool IDiagramController::hasActionListener(IActionListener * listener) const
{
	return _dd->findActionListener(listener) != _dd->_actionListeners.end();
}

bool IDiagramController::hasErrorListener(IErrorListener * listener)
{
	return _dd->findErrorListener(listener) != _dd->_errorListeners.end();
}

bool test(IActionListener * data)
{
	return data != 0;
}

IDiagramController::IDiagramControllerPrivate::actionListenerVct::iterator IDiagramController::IDiagramControllerPrivate::findActionListener(IActionListener * listener)
{
	return std::find_if(
				_actionListeners.begin(),
				_actionListeners.end(),
				boost::lambda::bind(&ActionListenerData::_listener, boost::lambda::_1) == listener
				);
}

IDiagramController::IDiagramControllerPrivate::errorListenerVct::iterator IDiagramController::IDiagramControllerPrivate::findErrorListener(IErrorListener * listener)
{
	return std::find(
			_errorListeners.begin(),
			_errorListeners.end(),
			listener);
}

bool operator==(const ActionListenerData & first, IActionListener * second)
{
	return first._listener == second;
}

void ActionListenerData::sendAction(const IAction & action)
{
	if((action.type() & _mask) != 0)
		_listener->onActionExecuted(action);
}
