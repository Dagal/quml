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

#include "elementobject.hpp"
#include "_elementobject.hpp"
#include "umldiagram.hpp"
#include "_umldiagram.hpp"
#include <boost/bind.hpp>

template<typename T> void deleter(T * element)
{
	delete element;
}


namespace element
{
	/*!
  Base constructor for an element with a specified name
  Note that this class is abstract, so calling this constructor won't work directly
*/
	ElementObject::ElementObject(const std::string & name)
		: _dd(new ElementObjectPrivate(this))
	{
		_dd->_name = name;
	}

	/*!
  Deconstructor for an ElementObject. This functions detaches this ElementObject from it's
  umlDiagram, it's parent and then deletes all the attached child elements.
*/
	ElementObject::~ElementObject()
	{
		setUMLDiagram(0);
		setParent(0);

		std::vector<ElementObject*> childrenCopy = children();

		std::for_each(
				childrenCopy.begin(),
				childrenCopy.end(),
				boost::bind<void>(&deleter<ElementObject>, _1)
				);
	}

	/*!
  Returns the name of the ElementObject. There is a difference between a name, a umlName and a qualifiedName:
  \li \c name: The actual name
  \li \c umlName: The name together with all the options (e.g. a operation with all the parameters). This name should be unique within a parent.
  \li \c qualifiedName: An unique qualified name. This name is composed out of all the parents and if necessary also this umlName.

  e.g. An operation test(int a) in class A will have the following properties:
  \li \c name: test
  \li \c umlName: test(int a)
  \li \c qualifiedName: A::test(int a)
*/
	const string & ElementObject::name() const
	{
		return _dd->_name;
	}

	/*!
  Returns the umlName of the ElementObject. There is a difference between a name, a umlName and a qualifiedName:
  \li \c name: The actual name
  \li \c umlName: The name together with all the options (e.g. a operation with all the parameters). This name should be unique within a parent.
  \li \c qualifiedName: An unique qualified name. This name is composed out of all the parents and if necessary also this umlName.

  e.g. An operation test(int a) in class A will have the following properties:
  \li \c name: test
  \li \c umlName: test(int a)
  \li \c qualifiedName: A::test(int a)
*/
	std::string ElementObject::umlName() const
	{
		return name();
	}

	/*!
  Returns the qualifiedName of the ElementObject. There is a difference between a name, a umlName and a qualifiedName:
  \li \c name: The actual name
  \li \c umlName: The name together with all the options (e.g. a operation with all the parameters). This name should be unique within a parent.
  \li \c qualifiedName: An unique qualified name. This name is composed out of all the parents and if necessary also this umlName.

  e.g. An operation test(int a) in class A will have the following properties:
  \li \c name: test
  \li \c umlName: test(int a)
  \li \c qualifiedName: A::test(int a)
*/
	string ElementObject::qualifiedName() const
	{
		if(parent())
			return parent()->qualifiedName() + "::" + umlName();
		else
			return umlName();
	}

	/*!
  This method can be reimplemented in a base class to be notified when a child is added. Because you can't specify a parent in the constructor,
  the ElementObject \c child will be fully constructed.
*/
	void ElementObject::onChildAdded(ElementObject * /*child*/)
	{
	}

	/*!
  This method can be reimplemented in a base class to be notified when a child is removed.
*/
	void ElementObject::onChildRemoved(ElementObject * /*child*/)
	{
	}

	/*!
  Returns a pointer to the parent ElementObject if present
*/
	ElementObject * ElementObject::parent() const
	{
		return _dd->_parent;
	}

	/*!
  Returns a pointer to the umlDiagram if present
*/
	UMLDiagram * ElementObject::umlDiagram() const
	{
		return _dd->_diagram;
	}

	/*!
  Returns a list of all the attached children
*/
	const std::vector<ElementObject *> & ElementObject::children() const
	{
		return _dd->_children;
	}

	/*!
  This method is used to change the attached UML Diagram. To detach an element, just supply 0 as input parameter.
  If the parent is selected and in a different umlDiagram, this element will be detached from the parent. All the children
  will also change to this new UMLDiagram.
*/
	void ElementObject::setUMLDiagram(UMLDiagram * diagram)
	{
		// stupid function call?
		if(umlDiagram() == diagram)
			return;

		// detach from parent if possible
		if(parent() != 0)
			_dd->detachFromParent();

		// detach from umlDiagram if possible
		if(umlDiagram() != 0)
			_dd->detachFromUML();

		if(diagram == 0)
			return;

		diagram->_dd->attachElement(this);
	}


	/*!
	Changes the name of the ElementObject. The name is used to compose the umlName and the qualifiedName. Make sure that the new qualifiedElement
	is unique within the umlDiagram, otherwise the other element will be destroyed.
	*/
	void ElementObject::setName(const std::string & name)
	{
		// emtpy name?
		if(name.empty())
			return;

		// stupid function call?
		if(name == _dd->_name)
			return;

		// attached to umlDiagram?
		if(umlDiagram())
		{
			// find the "new" qualified name
			std::string oldName = _dd->_name;
			_dd->_name  = name;
			std::string qualName = qualifiedName();
			_dd->_name = oldName;

			// name already taken?
			ElementObject * rivalElement = umlDiagram()->findElement(qualName);
			if(rivalElement != 0)
			{
				rivalElement->setUMLDiagram(0);
				rivalElement->setParent(0);

				delete rivalElement;
			}
		}

		_dd->_name = name;
		if(umlDiagram())
			umlDiagram()->_dd->resortElements();
	}

	/*!
	This function changes the parent ElementObject. If parent is set to zero, the element will be detached from it's parent.

	Depending on this elements ElementObject::umlDiagram and the parents ElementObject::umlDiagram this function performs different operations:
	\li simply detach from the old parent
	\li attach to a new parent in same UMLDiagram
	\li detach from parent and attach to a new parent in same UMLDiagram
	\li attach to a new parent in a new UMLDiagram
	\li detach from the old UMLDiagram, detach from the old parent and attach to a new UMLDiagram and a new parent
	\li detach from the old UMLDiagram, detach from the old parent and attach to a new parent (no new UMLDiagram)
	*/
	void ElementObject::setParent(ElementObject * parent)
	{
		// same parent?
		if(_dd->_parent == parent)
			return;

		/* **********************************************
	   Find which case we are dealing with:
	   1) simply detach from parent
	   2) attach to new parent
	   3) 1 & 2 together
	   4) attach to new parent in different umlDiagram
	   5) detach from umlDiagram, from parent and attach to new parent in different umlDiagram
	   6) detach from umlDiagram, from parent and attach to new parent (with no umlDiagram)
	   */

		if(parent == 0)
		{
			// (1)
			_dd->detachFromParent();
		}
		else
		{
			// uml diagram the same?
			if(parent->umlDiagram() == umlDiagram())
			{
				// (2) or (3)
				if(_dd->_parent != 0)
					_dd->detachFromParent();

				_dd->attachToParentInSameUML(parent);
			}
			else
			{
				// (4), (5) or (6)
				if(umlDiagram() != 0)
					_dd->detachFromUML();

				if(parent->umlDiagram() == 0)
					// (6)
					_dd->attachToParentInSameUML(parent);
				else
					// (5)
					_dd->attachToParentInDifferentUML(parent);
			}
		}
	}

	/*!
	  \internal
	  This function is a helper function for ElementObject::setUmlDiagram and ElementObject::setParent to detach an element from its parent
	*/
	void ElementObject::ElementObjectPrivate::detachFromParent()
	{
		assert(_parent != 0);

		// remove the parent
		ElementObject * oldParent = _parent;
		_parent = 0;

		// remove from the parents children
		oldParent->_dd->INT_removeChild(_element);

		// send the message to the parent
		oldParent->onChildRemoved(_element);
	}

	/*!
	  \internal
	  This function is an helper function for ElementObject::setParent to set the element's parent to \c newParent without changing the UMLDiagram
	*/
	void ElementObject::ElementObjectPrivate::attachToParentInSameUML(ElementObject * newParent)
	{
		assert(newParent != 0 && _parent == 0);

		// set the new parent
		_parent = newParent;

		// add to the parents children
		_parent->_dd->INT_addChild(_element);

		// send the message
		_parent->onChildAdded(_element);
	}

	/*!
	  \internal
	  This function is a helper function for ElementObject::setParent to set the element's parent to \c parent and also changing the UMLDiagram
	*/
	void ElementObject::ElementObjectPrivate::attachToParentInDifferentUML(ElementObject * parent)
	{
		assert(parent != 0 && parent->umlDiagram() != 0 && _parent == 0 && _diagram == 0);

		// set the new parent & diagram
		_parent = parent;
		_diagram = parent->umlDiagram();

		// add to the parent
		_parent->_dd->INT_addChild(_element);

		// add to the umlDiagram
		_diagram->_dd->attachElement(_element);

		// send the message
		_parent->onChildAdded(_element);
	}

	/*!
	  \internal
	  This function is helper function for ElementObject::setUMLDiagram and ElementObject::setParent
	*/
	void ElementObject::ElementObjectPrivate::detachFromUML()
	{
		assert(_diagram != 0);

		// detach the diagram
		_diagram->_dd->detachElement(_element);
	}

	/*!
	  \internal
	  This function is used internal in ElementObject to remove a child from the list of childs
	*/
	void ElementObject::ElementObjectPrivate::INT_removeChild(ElementObject * child)
	{
		std::vector<ElementObject*>::iterator i = std::find( _children.begin(), _children.end(), child);

		if(i != _children.end())
			_children.erase(i);
	}

	/*!
	  \internal
	  This function is used internal in ElementObject to add a child to the list of childs
	*/
	void ElementObject::ElementObjectPrivate::INT_addChild(ElementObject * child)
	{
		_children.push_back(child);
	}

	/*!
	This method should be called whenever the related element of this ElementObject is changed. For example, if the return type of a operation is changed,
	this method is called. This informs the UMLDiagram of recent changes.
	*/
	void ElementObject::relatedElementChanged(ElementObject * oldRelatedElement)
	{
		if(umlDiagram())
			umlDiagram()->_dd->_elementRelator.updateElementObject(this, oldRelatedElement);
	}

	/*!
	This method returns a list of all the ancestors. The first ancestor in the line is always this element.
	*/
	std::vector<const ElementObject*> ElementObject::ancestors() const
	{
		std::vector<const ElementObject*> ancestors;

		const ElementObject * curElement = this;

		while(curElement != 0)
		{
			ancestors.push_back(curElement);
			curElement = curElement->parent();
		}

		return ancestors;
	}
}
