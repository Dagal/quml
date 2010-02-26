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

#ifndef ELEMENTCOMMAND_HPP
#define ELEMENTCOMMAND_HPP

#include "command.hpp"
#include "macro.hpp"
#include "defines.hpp"
#include <string>


class DeleteElementCommand : public Command
{
public:
	DeleteElementCommand(const std::string & umlDiagram, const std::string & elementName);
	~DeleteElementCommand();

private:
	virtual bool redoAction();
	virtual bool undoAction();
	virtual Error checkAction();

	PtrVarGet(ElementObject, elementObject);

	ElementObject * _parentObject;
	UMLDiagram * _umlDiagram;
	const std::string _umlDiagramName;
	const std::string _elementName;
	bool _isOwner;
};

#endif // ELEMENTCOMMAND_HPP
