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

#include "defines.hpp"
#include "algorithm.hpp"
#include "elementhelper.hpp"
#include <vector>
#include <string>

class UMLDiagram
{
	friend class ElementObject;

	class UMLDiagramPrivate;

public:
	UMLDiagram();
	~UMLDiagram();

	// create & add functions
	ElementObject * createElement(ElementType type, const std::string & name, const std::string & parentName);

	// element "find" functions
	ElementObject * findElement(const std::string & qualifiedName) const;
	const std::vector<ElementObject*> & allElements() const;
	std::vector<ElementObject *> findRelatedElements(ElementObject * elementObject) const;
	template <typename T> std::vector<T *> findElements() const;

private:
	boost::shared_ptr<UMLDiagramPrivate> _dd;
};

template <typename T> std::vector<T*> UMLDiagram::findElements() const
{
	std::vector<T*> vct(allElements().size());

	typename std::vector<T*>::iterator i = stf::copy_transformed_if(
			allElements().begin(),
			allElements().end(),
			vct.begin(),
			boost::bind<T*>(
					element_cast<T>,
					_1),
			boost::bind(
					std::not_equal_to<T*>(),
					_1,
					(T*)0)
			);

	vct.erase(i, vct.end());
	return vct;
}

#endif // UMLDIAGRAM_HPP
