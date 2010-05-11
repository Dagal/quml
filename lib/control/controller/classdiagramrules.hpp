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

#ifndef CLASSDIAGRAMRULES_HPP
#define CLASSDIAGRAMRULES_HPP

#include "iclassdiagramrules.hpp"
#include "classdiagramcontroller.hpp"

namespace controller
{
	class DefaultClassDiagramRules : public IClassDiagramRules
	{
	protected:
		// helper functions
		bool isValidName(element::ElementObject * elementObject) const;
		bool isNameValidWithinParent(element::ElementObject * elementObject, element::ElementObject * parent, const QString & newName) const;

	private:
		const element::UMLDiagram * _diagram;
	};

	class ClassObjectDiagramRules : public DefaultClassDiagramRules
	{
	public:
		virtual int checkElement(element::ElementObject *element) const;
	};

	class PackageObjectDiagramRules : public DefaultClassDiagramRules
	{
	public:
		virtual int checkElement(element::ElementObject *element) const;
	};
	class MethodObjectDiagramRules : public DefaultClassDiagramRules
	{
	public:
		virtual int checkElement(element::ElementObject *element) const;
	};

	class OperationObjectDiagramRules : public MethodObjectDiagramRules
	{
	public:
		virtual int checkElement(element::ElementObject *element) const;
	};

	class PropertyObjectDiagramRules : public DefaultClassDiagramRules
	{
	public:
		virtual int checkElement(element::ElementObject *element) const;
	};
}


#endif // CLASSDIAGRAMRULES_HPP
