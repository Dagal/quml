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

#ifndef ELEMENTOBJECT_HPP
#define ELEMENTOBJECT_HPP

#include "defines.hpp"
#include <string>
#include <vector>


class ElementObject
{
	class ElementObjectPrivate;

public:
	struct comparator
	{
		bool operator()(const ElementObject * elementA, const ElementObject * elementB) const;
	};
	struct string_comparator
	{
		int operator()(const ElementObject * element, const std::string & name) const;
	};

public:
	// constructor & destructor
	ElementObject(ElementType type, ElementObject * parent);
	virtual ~ElementObject() = 0;

	// getters and setters
	void setName(const std::string & newName);
	const std::string & name() const;
	virtual std::string qualifiedName() const;
	ElementType type() const;

	// child/parent functions
	void setParent(ElementObject * parent);
	ElementObject * parent() const;
	std::vector<ElementObject *> children() const;

	// classdiagram functions
	UMLDiagram * umlDiagram() const;
	void setUMLDiagram(UMLDiagram * diagram);



protected:
	virtual void onChildAdded(ElementObject * child);
	virtual void onChildRemoved(ElementObject * child);
	virtual void onChildNameChanged(const std::string & oldName, ElementObject * /*child*/);

private:
	boost::shared_ptr<ElementObjectPrivate> _dd;
};


#endif // ELEMENTOBJECT_HPP
