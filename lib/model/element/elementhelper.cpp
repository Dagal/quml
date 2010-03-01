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

#include "elementhelper.hpp"
#include "classobject.hpp"
#include "methodobject.hpp"
#include "propertyobject.hpp"
#include "parameterobject.hpp"
#include "packageobject.hpp"
#include "operationobject.hpp"

std::vector<ElementObject*> findChildren(ElementObject * element, const std::string & name)
{
	typedef std::vector<ElementObject*> elementvct;
	typedef elementvct::iterator elementvctit;

	const std::vector<ElementObject * >	& children = element->children();
	elementvct transf(children.size());

	boost::function<bool (ElementObject*)> pred;
	if(name.empty())
		pred = boost::bind(checkForValidElement<ElementObject>, _1);
	else
		pred = boost::bind(checkForValidNamedElement<ElementObject>, _1, boost::cref(name));


	elementvctit it = stf::copy_transformed_if(
			children.begin(),
			children.end(),
			transf.begin(),
			boost::bind<ElementObject*>(
					element_cast<ElementObject>,
					_1),
			pred
			);

	transf.erase(it, transf.end());
	return transf;
}

