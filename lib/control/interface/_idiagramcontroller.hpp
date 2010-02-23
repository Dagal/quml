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

#ifndef _IDIAGRAMCONTROLLER_HPP
#define _IDIAGRAMCONTROLLER_HPP

#include "idiagramcontroller.hpp"
#include <vector>

struct ActionListenerData
{
	ActionListenerData(IActionListener * listener, int mask = 0xffff)
		: _listener(listener), _mask(mask)
	{}

	IActionListener * _listener;
	int _mask;

	void sendAction(const IAction & action);
};

bool operator==(const ActionListenerData & first, IActionListener * second);

struct IDiagramController::IDiagramControllerPrivate
{

	typedef std::vector<ActionListenerData> actionListenerVct;
	typedef std::vector<IErrorListener*> errorListenerVct;

	IDiagramControllerPrivate()
	{
	}

	void sendErrorMessage();
	void sendAction(const IAction & action);

	actionListenerVct::iterator findActionListener(IActionListener * listener);
	errorListenerVct::iterator findErrorListener(IErrorListener * listener);


	actionListenerVct _actionListeners;
	errorListenerVct _errorListeners;
	std::string _errorMessage;
};




#endif // _IDIAGRAMCONTROLLER_HPP
