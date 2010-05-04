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
		//ElementObject * createElement(ElementType type, const QString & name, const QString & parentQualifiedName);
		//void addElement(ElementObject * element, const QString & newName, const QString & parentQualifiedName);

		// element "find" functions
		//const QList<ElementObject*> & allElements() const;

		QList<ElementObject *> findRelatedElements(ElementObject * elementObject) const;
		const QList<ElementObject *> & upperLevelElements() const;

		template <typename T> QList<T *> findElements() const;

		ElementObject * findElement(const QString & qualifiedName) const;


		static QStringList parseQualifiedName(const QString & qualifiedName);
		static const QList<QPair<QChar, QChar> > Brackets;
		static const QString ScopeOperator;

	private:
		boost::shared_ptr<UMLDiagramPrivate> _dd;
	};

	template <typename T> QList<T*> UMLDiagram::findElements() const
	{
		struct ElementFinder : public ElementFunctor
		{
			ElementFinder(QList<T*> * foundElements)
				: _foundElements(foundElements) {}

			QList<T *> * _foundElements;

			virtual void operator()(ElementObject * elementObject)
			{
				T * element = element_cast<T>(elementObject);
				if(element)
					_foundElements->append(element);
			}
		};


		QList<T*> lst;
		ElementFinder f(&lst);
		recursivelyPerformOperation(upperLevelElements(), f);

		return lst;
	}
}

#endif // UMLDIAGRAM_HPP
