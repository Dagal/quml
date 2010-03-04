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

#include "diagramcontroller.hpp"
#include "_diagramcontroller.hpp"
#include "algorithm.hpp"
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include "event.hpp"

DiagramController::~DiagramController()
{
}

void DiagramController::addEventListener(IEventListener * listener, int mask)
{
	removeEventListener(listener);
	_dd->_eventListeners.push_back(EventListenerData(listener, mask));
}

void DiagramController::removeEventListener(IEventListener * listener)
{
	DiagramControllerPrivate::eventListenerVct::iterator i = _dd->findEventListener(listener);

	if(i != _dd->_eventListeners.end())
		_dd->_eventListeners.erase(i);
}

void DiagramController::addErrorListener(IErrorListener * listener)
{
	removeErrorListener(listener);
	_dd->_errorListeners.push_back(listener);
}

void DiagramController::removeErrorListener(IErrorListener * listener)
{
	DiagramControllerPrivate::errorListenerVct::iterator i = _dd->findErrorListener(listener);

	if(i != _dd->_errorListeners.end())
		_dd->_errorListeners.erase(i);
}

void DiagramController::sendError(const Event & event, Error error) const
{
	std::for_each(
			_dd->_errorListeners.begin(),
			_dd->_errorListeners.end(),
			boost::bind(&IErrorListener::onError, _1, boost::cref(event), error)
			);
}

void DiagramController::sendEvent(const Event & event) const
{
	std::for_each(
			_dd->_eventListeners.begin(),
			_dd->_eventListeners.end(),
			boost::bind(&EventListenerData::sendEvent, _1, boost::cref(event))
			);
}

bool DiagramController::hasEventListener(IEventListener * listener) const
{
	return _dd->findEventListener(listener) != _dd->_eventListeners.end();
}

bool DiagramController::hasErrorListener(IErrorListener * listener)
{
	return _dd->findErrorListener(listener) != _dd->_errorListeners.end();
}


DiagramController::DiagramControllerPrivate::eventListenerVct::iterator DiagramController::DiagramControllerPrivate::findEventListener(IEventListener * listener)
{
	return std::find_if(
				_eventListeners.begin(),
				_eventListeners.end(),
				boost::lambda::bind(&EventListenerData::_listener, boost::lambda::_1) == listener
				);
}

DiagramController::DiagramControllerPrivate::errorListenerVct::iterator DiagramController::DiagramControllerPrivate::findErrorListener(IErrorListener * listener)
{
	return std::find(
			_errorListeners.begin(),
			_errorListeners.end(),
			listener);
}

bool operator==(const EventListenerData & first, IEventListener * second)
{
	return first._listener == second;
}

void EventListenerData::sendEvent(const Event & event)
{
	if((event.type() & _mask) != 0)
		_listener->onEvent(event);
}
