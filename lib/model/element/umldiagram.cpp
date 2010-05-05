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

#include "umldiagram.hpp"
#include "_umldiagram.hpp"
#include "elementobject.hpp"
#include "elementhelper.hpp"
#include "_elementobject.hpp"
#include "algorithm.hpp"
#include "methodobject.hpp"
#include "propertyobject.hpp"
#include <QStack>

bool elementComparator(boost::shared_ptr<element::ElementObject> elementA, boost::shared_ptr<element::ElementObject> elementB)
{
	return elementA->qualifiedName() < elementB->qualifiedName();
}

int elementFinder(boost::shared_ptr<element::ElementObject> element, const QString & qualifiedName)
{
	return element->qualifiedName().compare(qualifiedName);
}

namespace element
{
	/*!
	  The ScopeOperator is a QString that contains the symbol used for scope operator. By default this is set to "::".
	*/
	const QString UMLDiagram::ScopeOperator = QString("::");

	/*!
	  Brackets is a list of starting and closing brackets that can be used. These brackets cannot be used in a name (they can be used in a umlName, e.g. a method).
	*/
	const QList<QPair<QChar, QChar> > UMLDiagram::Brackets = QList<QPair<QChar, QChar> >()
															 << QPair<QChar, QChar>('(', ')')
															 << QPair<QChar, QChar>('<', '>')
															 << QPair<QChar, QChar>('[', ']')
															 << QPair<QChar, QChar>('{', '}');

	/*!
	  This constructor creates an empty UMLDiagram
	*/
	UMLDiagram::UMLDiagram()
		: _dd(new UMLDiagram::UMLDiagramPrivate(this))
	{
	}

	/*!
	  This destructor destroys the UMLDiagram, but also all the attached elements
	*/
	UMLDiagram::~UMLDiagram()
	{
		// remove all the elements
		while(!_dd->_upperLevel.empty())
		{
			ElementObject * curElement = _dd->_upperLevel[0];
			curElement->setUMLDiagram(0);
			curElement->setParent(0);

			delete curElement;
		}
	}

	/*!
	  This function creates an ElementObject of type \c type, set's the name and the parent and adds it to the UMLDiagram.
	  if the parent is not found, the element has no parent.
	  This functions returns a pointer to the newly created element.
	*/
	ElementObject * UMLDiagram::createElement(ElementType type, const QString & name, const QString & parentQualifiedName)
	{
		if(name.isEmpty())
			return 0;

		ElementObject * element = createElementObject(type, name);
		ElementObject * parent = findElement(parentQualifiedName);

		element->setParent(parent);
		element->setUMLDiagram(this);

		return element;
	}

	/*!
	  This functions detaches \c element from its parent and its UMLDiagram. Afterwards it changes the name to \c newName and sets
	  the new parent to the element at \c parentQualifiedName. If the parentQualifiedName is not found, the element is added to this
	  UMLDiagram with no parent.
	*/
	void UMLDiagram::addElement(ElementObject * element, const QString & newName, const QString & parentQualifiedName)
	{
		if(!element || newName.isEmpty())
			return;

		// detach the element
		element->setUMLDiagram(0);
		element->setParent(0);

		// set the new name
		element->setName(newName);

		ElementObject * newParent = findElement(parentQualifiedName);
		element->setParent(newParent);
		element->setUMLDiagram(this);
	}

	/*!
	  This function searches the UMLDiagram for an element with the specified qualifiedName, and returns the element. If no element was found
	  the function returns zero.

	  \sa element::ElementObject::qualifiedName
	*/
	ElementObject* UMLDiagram::findElement(const QString & qualifiedName) const
	{
		QStringList lst = ParseQualifiedName(qualifiedName);

		const QList<ElementObject *> * elements = &rootElements();
		ElementObject * curElement = 0;

		for(int i = 0; i < lst.size(); i++)
		{
			curElement = findUMLName(*elements, lst[i]);
			if(curElement == 0)
				return 0;

			elements = &curElement->children();
		}

		return curElement;
	}

	/*!
	  This function return all the elements that depend on \c elementObject. For example, for a certain
	  datatype it will return all the methods that have this datatype as returntype of all the
	  parameters that have this as a datatype.
	*/
	QList<ElementObject *> UMLDiagram::findRelatedElements(ElementObject * elementObject) const
	{
		return _dd->_elementRelator.findAllRelatedElementsTo(elementObject);
	}

	/*!
	  This function will attach the supplied \c elementObject to the UMLDiagram. This means that the relations
	  for this element and all its children (recursively) will be checked and stored. The parent element is not set,
	  nor checked.
	*/
	void UMLDiagram::UMLDiagramPrivate::attachToUMLDiagram(ElementObject * elementObject)
	{
		INT_recursivelySetUMLDiagram(elementObject, _diagram);
		INT_recursivelyAddRelations(elementObject);
	}

	/*!
	  This function will detach the supplied \c elementObject from the UMLDiagram. The relations for this element and
	  all the children (recursively) will be checked and removed from the relator. Broken relations will be set to zero
	  afterwards.
	*/
	void UMLDiagram::UMLDiagramPrivate::removeFromUMLDiagram(ElementObject * elementObject)
	{
		INT_recursivelySetUMLDiagram(elementObject, 0);
		INT_recursivelyRemoveRelations(elementObject);
	}

	/*!
	  This function adds the elementObject to the upper level in the UMLDiagram. This makes sure the elements gets deleted
	  at the right time.
	*/
	void UMLDiagram::UMLDiagramPrivate::attachToUpperLevel(ElementObject * elementObject)
	{
		elementObject->_dd->_parent = 0;
		if(!_upperLevel.contains(elementObject))
			_upperLevel.push_back(elementObject);
	}

	/*!
	  This function removes the elementObject from the upper level in the UMLDiagram. The diagram still remains the same.
	*/
	void UMLDiagram::UMLDiagramPrivate::removeFromUpperLevel(ElementObject * elementObject)
	{
		elementObject->_dd->_parent = 0;
		_upperLevel.removeAll(elementObject);
	}

	/*!
	  This method checks if the attachElements for elementObject are in the UMLDiagram. If not, they are set to zero.
	  Afterwards the element is added to the list of related elements. This method is called recursively to all the
	  children.
	*/
	void UMLDiagram::UMLDiagramPrivate::INT_recursivelyAddRelations(ElementObject * elementObject)
	{
		// check the relations: elementObject --> ...
		for(int i = 0; i < elementObject->attachedElementCount(); i++)
		{
			ElementObject * attachedElement = elementObject->getAttachedElementAt(i);
			if(attachedElement && attachedElement->umlDiagram() != _diagram)
				attachedElement->setAttachedElementAt(i, 0);
		}

		// add to the list of related elements
		_elementRelator.add(elementObject);

		// add to the list of element
		_allElements << elementObject;

		// recursively on children
		foreach(ElementObject * child, elementObject->children())
			INT_recursivelyAddRelations(child);
	}

	/*!
	  This method checks if all the relation with this element are in this UMLDiagram. If not, they are set to zero.
	  Afterwards, this element is removed from the list of all the related elements.
	*/
	void UMLDiagram::UMLDiagramPrivate::INT_recursivelyRemoveRelations(ElementObject * elementObject)
	{
		// check the relations: elementObject --> ...
		for(int i = 0; i < elementObject->attachedElementCount(); i++)
		{
			ElementObject * attachedElement = elementObject->getAttachedElementAt(i);
			if(attachedElement && attachedElement->umlDiagram() != 0)
				elementObject->setAttachedElementAt(i, 0);
		}

		// check the relations: ... -> elementObject
		QList<ElementRelator::RelatedElementDetails> details = _elementRelator.findAllRelatedElementDetailsTo(elementObject);
		for(int i = 0; i < details.size(); i++)
			if(details[i]._relatedElement != 0 && details[i]._relatedElement->umlDiagram() != 0)
				details[i]._relatedElement->setAttachedElementAt(details[i]._position, 0);

		// remove from list of related elements
		_elementRelator.remove(elementObject);

		// remove from the list of element
		_allElements.removeAll(elementObject);

		// recursively on children
		foreach(ElementObject * child, elementObject->children())
			INT_recursivelyRemoveRelations(child);
	}


	/*!
	  This simple method sets the UMLDiagram to \c diagram for \c elementObject and all its children.
	*/
	void UMLDiagram::UMLDiagramPrivate::INT_recursivelySetUMLDiagram(ElementObject * elementObject, UMLDiagram * diagram)
	{
		elementObject->_dd->_diagram = diagram;

		foreach(ElementObject * child, elementObject->children())
			INT_recursivelySetUMLDiagram(child, diagram);
	}

	/*!
	  This method returns a list of all the upper level elements attached to this umlDiagram.
	*/
	const QList<ElementObject *> & UMLDiagram::rootElements() const
	{
		return _dd->_upperLevel;
	}

	/*!
	  This static method parses a given qualifiedName in all its different elements. It returns a list of all the names of the ancestors (where the
	  uppermost ancestor is stored at position zero in the array). If the name is unparsable (unclosed brackets, etc) it returns an empty QStringList.
	*/
	QStringList UMLDiagram::ParseQualifiedName(const QString & qualifiedName)
	{
		QStack<QPair<QChar, QChar> > stack;
		QStringList lst;

		int prevPos = 0;

		for(int i = 0; i < qualifiedName.size(); i++)
		{
			if(stack.isEmpty() && qualifiedName.mid(i, ScopeOperator.size()) == ScopeOperator)
			{
				// perform "intersection"
				lst.push_back(qualifiedName.mid(prevPos, i-prevPos));
				i += ScopeOperator.size();
				prevPos = i;
			}

			// check for new
			QChar curChar = qualifiedName.at(i);

			// check for popping of stack
			if(!stack.isEmpty() && curChar == stack.top().second)
			{
				stack.pop();
				continue;
			}

			// check for pushing onto the stack
			for(int i = 0; i < Brackets.size(); i++)
			{
				if(curChar == Brackets[i].first)
				{
					stack.push(Brackets[i]);
					break;
				}
			}
		}

		// is it a valid name?
		if(!stack.isEmpty())
			return QStringList();

		lst.push_back(qualifiedName.mid(prevPos));
		return lst;
	}

	/*!
	  This static method checks a given name. The name cannot contain Brackets or the ScopeOperator.

	  \sa ScopeOperator, \sa Brackets
	*/
	bool UMLDiagram::IsValidName(const QString & name)
	{
		if(name.isEmpty())
			return false;

		for(int i = 0; i < name.size(); i++)
		{
			if(name.mid(i, ScopeOperator.size()) == ScopeOperator)
				return false;

			QChar curChar = name[i];

			for(int i = 0; i < Brackets.size(); i++)
				if(curChar == Brackets[i].first || curChar == Brackets[i].second)
					return false;
		}

		return true;
	}

	/*!
	  This method returns a list of all the elements that are attached to this diagram.
	*/
	const QList<ElementObject *> & UMLDiagram::allElements() const
	{
		return _dd->_allElements;
	}
}
