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
	ElementObject::ElementObject(const QString & name)
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

		QList<ElementObject*> childrenCopy = children();

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
	const QString & ElementObject::name() const
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
	QString ElementObject::umlName() const
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
	QString ElementObject::qualifiedName() const
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
	const QList<ElementObject *> & ElementObject::children() const
	{
		return _dd->_children;
	}

	/*!
	  This method is used to attach or detach an element from a UMLDiagram. If the diagram is the same as the current,
	  no action is performed. Otherwise, the parent is set to zero and the diagram is set to the current diagram. This
	  means that the element is added to the default namespace. If \c diagram is set to zero, the element is detached
	  from its parent and from the current diagram.
*/
	void ElementObject::setUMLDiagram(UMLDiagram * diagram)
	{
		// stupid function call?
		if(umlDiagram() == diagram)
			return;

		// we were attached to a umlDiagram? remove us completely
		if(umlDiagram())
			_dd->INT_detachFromUMLAndParent();

		// should we be attached to a new umlDiagram()
		if(diagram)
			_dd->INT_attachToUMLDiagram(diagram);
	}

	/*!
	Changes the name of the ElementObject. The name is used to compose the umlName and the qualifiedName. Make sure that the new qualifiedElement
	is unique within the umlDiagram, otherwise the other element will be destroyed.
	*/
	void ElementObject::setName(const QString & name)
	{
		// emtpy name?
		if(name.isEmpty())
			return;

		// stupid function call?
		if(name == _dd->_name)
			return;

		_dd->_name = name;
	}

	/*!
	This function changes the parent ElementObject. If parent is set to zero, the element will be detached from it's parent. If the parent is in a different
	umlDiagram, it is detached from its current diagram.


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

		bool isSameUML = (parent == 0) || (parent->umlDiagram() == umlDiagram());

		if(isSameUML)
		{
			// remove from upperList
			if(_dd->_parent == 0)
			{
				if(umlDiagram()) umlDiagram()->_dd->removeFromUpperLevel(this);
			}
			// remove from parent
			else
			{
				_dd->INT_detachFromParent();
			}

			// attach to new parent?
			if(parent)
				_dd->INT_attachToParent(parent);
		}
		else
		{
			// start with detaching from the current umlDiagram
			setUMLDiagram(0);

			// attach to new parent && umlDiagram
			_dd->INT_attachToUMLAndParent(parent);
		}
	}

	/*!
	  Reimplement this method to return the number of attached elements to this element.
	*/
	int ElementObject::attachedElementCount() const
	{
		return 0;
	}

	/*!
	  Reimplement this method to return an attached element at a specified position. This method is used to keep track of all the attached elements.
	  If position is invalid, this function should return 0.
	*/
	ElementObject * ElementObject::getAttachedElementAt(int /*position*/) const
	{
		return 0;
	}

	/*!
	  Reimplement this method to update the value of the attached element at a certain position.
	*/
	void ElementObject::setAttachedElementAt(int /*position*/, ElementObject * /*attachedElement*/)
	{
	}

	/*!
	This method should be called whenever the attached element of this ElementObject is changed. For
	example, if the return type of a operation is changed, this method is called. This informs the
	UMLDiagram of recent changes.
	*/
	void ElementObject::attachedElementChanged(int position, ElementObject * oldAttachedElement)
	{
		if(umlDiagram())
			umlDiagram()->_dd->_elementRelator.update(this, position, oldAttachedElement);
	}

	/*!
	This method returns a list of all the ancestors. The first ancestor in the line is the parent and so further on.
	*/
	QList<ElementObject*> ElementObject::ancestors() const
	{
		QList<ElementObject*> ancestors;

		ElementObject * curElement = parent();

		while(curElement != 0)
		{
			ancestors.push_back(curElement);
			curElement = curElement->parent();
		}

		return ancestors;
	}


	/*!
	  This method removes the element from its parent and from its umlDiagram.
	*/
	void ElementObject::ElementObjectPrivate::detachFromParentAndDiagram()
	{
		if(_diagram)
		{
			if(_parent == 0)
				_diagram->_dd->removeFromUpperLevel(_element);

			_diagram->_dd->removeFromUMLDiagram(_element);
		}

		if(_parent)
			_parent->_dd->_children.removeAll(_element);
	}

	/*!
	  This helper method removes an element from its parent and sends the parent the message.
	*/
	void ElementObject::ElementObjectPrivate::INT_detachFromParent()
	{
		assert(_parent);

		ElementObject * oldParent = _parent;
		_parent->_dd->_children.removeAll(_element);
		_parent = 0;

		oldParent->onChildRemoved(_element);
	}

	/*!
	  This helper method attaches an element to its parent and sends the parent the message.
	*/
	void ElementObject::ElementObjectPrivate::INT_attachToParent(ElementObject * parent)
	{
		assert(parent && !_parent);

		_parent = parent;
		_parent->_dd->_children.push_back(_element);

		_parent->onChildAdded(_element);
	}

	/*!
	  This helper method removes an element from its diagram
	*/
	void ElementObject::ElementObjectPrivate::INT_detachFromUMLAndParent()
	{
		assert(_diagram);

		if(!_parent)
			_diagram->_dd->removeFromUpperLevel(_element);
		else
			INT_detachFromParent();

		_diagram->_dd->removeFromUMLDiagram(_element);
	}

	/*!
	  This helper method adds an element to a new diagram to the upperLevel
	*/
	void ElementObject::ElementObjectPrivate::INT_attachToUMLDiagram(UMLDiagram * diagram)
	{
		assert(diagram && ! _diagram && !_parent);

		diagram->_dd->attachToUpperLevel(_element);
		diagram->_dd->attachToUMLDiagram(_element);
	}

	/*!
	  This helper methods adds an element to a new parent. If the parent is attached to a umlDiagram,
	  the element is also added to that umlDiagram.
	*/
	void ElementObject::ElementObjectPrivate::INT_attachToUMLAndParent(ElementObject * parent)
	{
		assert(parent && !_parent && !_diagram);

		if(parent->umlDiagram())
			parent->umlDiagram()->_dd->attachToUMLDiagram(_element);

		INT_attachToParent(parent);
	}
}

