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


ElementObject * createElement(ElementType type, ElementObject * parent)
{
	ElementObject * element = 0;

	switch(type)
	{
	case Element_Class:
		element = new ClassObject;
		break;

	case Element_Method:
		element = new MethodObject;
		break;

	case Element_Operation:
		element = new OperationObject;
		break;

	case Element_Package:
		element = new PackageObject;
		break;

	case Element_Property:
		element = new PropertyObject;
		break;

	case Element_Parameter:
		element = new ParameterObject;
		break;

	default:
		return 0;
	}

	element->setParent(parent);
	return element;
}
