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

#ifndef UMLDIAGRAM_HPP
#define UMLDIAGRAM_HPP

#include "element.hpp"
#include "algorithm.hpp"
#include "elementhelper.hpp"
#include <QVector>
#include <QString>
#include <QList>
#include <QStringList>
#include <QPair>

namespace element
{
	/*!
	  \class element::UMLDiagram
	  \brief The UMLDiagram is a container for all different types of ElementObject

	  A UMLDiagram contains ElementObjects in an hierarchical way. It is possible to search a complete UMLDiagram for specific elements by qualifiedName.
	  Next to this, a UMLDiagram makes it also possible to find related elements. If an element is added to a UMLDiagram, the changing of a name, parent,
	  relatedElements is automatically passed on to the UMLDiagram and so is updated.
	  If a UMLDiagram is destroyed, all contained elements are also destroyed.

		\sa element::findRelatedElement
	*/
	class UMLDiagram
	{
		friend class ElementObject;

		class UMLDiagramPrivate;

	public:
		UMLDiagram();
		~UMLDiagram();

		// create & add functions
		ElementObject * createElement(ElementType type, const QString & name, const QString & parentQualifiedName);
		void addElement(ElementObject * element, const QString & newName, const QString & parentQualifiedName);

		// simple accessors
		const QList<ElementObject *> & rootElements() const;

		// element "find" functions
		const QList<ElementObject *> & allElements() const;
		QList<ElementObject *> findRelatedElements(ElementObject * elementObject) const;
		template <typename T> QList<T *> findElements(const QString & name = "") const;
		ElementObject * findElement(const QString & qualifiedName) const;

		// static helper functions / variables
		static QStringList ParseQualifiedName(const QString & qualifiedName);
		static bool IsValidName(const QString & name);
		static const QString ScopeOperator;

	private:
		static const QList<QPair<QChar, QChar> > Brackets;
		boost::shared_ptr<UMLDiagramPrivate> _dd;
	};

	template <typename T> QList<T*> UMLDiagram::findElements(const QString & name) const
	{
		QList<T*> lst;

		bool checkName = !name.isEmpty();

		for(int i = 0; i < allElements().size(); i++)
		{
			T * element = element_cast<T>(allElements()[i]);

			if(element)
				if(!checkName || (checkName && element->name() == name))
					lst << element;
		}

		return lst;
	}
}

#endif // UMLDIAGRAM_HPP
