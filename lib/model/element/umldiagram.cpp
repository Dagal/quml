///*******************************************************************
//* Copyright (c) 2009 Thomas Fannes (thomasfannes@gmail.com)
//*
//* Permission is hereby granted, free of charge, to any person
//* obtaining a copy of this software and associated documentation
//* files (the "Software"), to deal in the Software without
//* restriction, including without limitation the rights to use,
//* copy, modify, merge, publish, distribute, sublicense, and/or sell
//* copies of the Software, and to permit persons to whom the
//* Software is furnished to do so, subject to the following
//* conditions:
//*
//* The above copyright notice and this permission notice shall be
//* included in all copies or substantial portions of the Software.
//*
//* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//* OTHER DEALINGS IN THE SOFTWARE.
//*******************************************************************/
//
//#include "umldiagram.hpp"
//#include "_umldiagram.hpp"
//#include "elementobject.hpp"
//#include <boost/bind.hpp>
//#include "algorithm.hpp"
//#include "packageobject.hpp"
//#include "methodobject.hpp"
//#include "parameterobject.hpp"
//
//UMLDiagram::UMLDiagram()
//	: _dd(new UMLDiagramPrivate(this))
//{
//}
//
//
//UMLDiagram::UMLDiagramPrivate::elementvct::iterator UMLDiagram::UMLDiagramPrivate::findInElements(const std::string & name)
//{
//	return  stf::binary_find_if(
//			_elements.begin(),
//			_elements.end(),
//			boost::bind<int>(
//					ElementObject::string_comparator(),
//					_1,
//					boost::ref(name)
//					)
//			);
//}
//
//UMLDiagram::UMLDiagramPrivate::elementvct::const_iterator UMLDiagram::UMLDiagramPrivate::findInElements(const std::string & name) const
//{
//	return  stf::binary_find_if(
//			_elements.begin(),
//			_elements.end(),
//			boost::bind<int>(
//					ElementObject::string_comparator(),
//					_1,
//					boost::ref(name)
//					)
//			);
//}
//
//ElementObject * UMLDiagram::findElement(const std::string & qualifiedName) const
//{
//	UMLDiagramPrivate::elementvct::const_iterator i = _dd->findInElements(qualifiedName);
//	if(i != _dd->_elements.end())
//		return *i;
//	else
//		return 0;
//}
//
//std::vector<ElementObject *> UMLDiagram::elements() const
//{
//	return _dd->_elements;
//
//}
//
//void UMLDiagram::UMLDiagramPrivate::attachElementObject(ElementObject * element)
//{
//	if(!element)
//		return;
//
//	assert(element->umlDiagram() == _diagram);
//
//	// an item already in this collection?
//	emptyLocation(element->qualifiedName());
//
//	// add the element
//	_elements.push_back(element);
//	resortElements();
//
//	addToRelatedElements(element);
//}
//
//void UMLDiagram::UMLDiagramPrivate::detachElementObject(const std::string & qualifiedName)
//{
//	elementvct::iterator it = findInElements(qualifiedName);
//
//	// something usefull to do?
//	if(it == _elements.end())
//		return;
//
//	ElementObject * obj = *it;
//
//	// erase from the total list
//	_elements.erase(it);
//
//	// erase from the relators, value side
//	removeFromRelatedElements(obj);
//}
//
//void UMLDiagram::UMLDiagramPrivate::changeElementName(const std::string & oldName, const std::string & newName)
//{
//	// just update the complete list
//	resortElements();
//
//	relatedElementMap::iterator i = _relatedElements.find(oldName);
//	if(i == _relatedElements.end() || i->second == 0)
//		return;
//
//	boost::shared_ptr<elementvct> data = i->second;
//	_relatedElements.erase(i);
//
//	_relatedElements[newName] = data;
//}
//
//void UMLDiagram::UMLDiagramPrivate::emptyLocation(const std::string & name)
//{
//	elementvct::iterator  i = findInElements(name);
//	if(i != _elements.end())
//		(*i)->setUMLDiagram(0);
//}
//
//void UMLDiagram::UMLDiagramPrivate::resortElements()
//{
//	std::sort(_elements.begin(),
//			  _elements.end(),
//			  ElementObject::comparator());
//}
//
//std::vector<ElementObject *> UMLDiagram::findRelatedElements(const std::string & qualifiedName) const
//{
//	std::vector<ElementObject*> vct;
//
//	UMLDiagramPrivate::relatedElementMap::iterator i = _dd->_relatedElements.find(qualifiedName);
//
//	if(i != _dd->_relatedElements.end())
//		vct.assign(i->second->begin(), i->second->end());
//
//	return vct;
//}
//
//std::string findRelatedElement(ElementObject * elementObject)
//{
//	switch(elementObject->type())
//	{
//	case Element_Method:
//	case Element_Operation:
//		{
//			MethodObject * methodObject = element_cast<MethodObject>(elementObject);
//			return methodObject->returnType();
//			break;
//		}
//
//	case Element_Parameter:
//	case Element_Property:
//		{
//			ParameterObject * parameterObject = element_cast<ParameterObject>(elementObject);
//			return parameterObject->datatype();
//			break;
//		}
//
//	default:
//		return std::string();
//	}
//}
//
//void UMLDiagram::UMLDiagramPrivate::removeFromRelatedElements(ElementObject * object)
//{
//
//
//	for(relatedElementMap::iterator i = _relatedElements.begin(); i != _relatedElements.end(); ++i)
//	{
//		if(i->second == 0)
//			_relatedElements.erase(i);
//		else
//		{
//			elementvct & vct = *(i->second);
//			elementvct::iterator j = std::find(vct.begin(),vct.end(), object);
//
//			if(j != vct.end())
//			{
//				vct.erase(j);
//
//				if(vct.empty())
//					_relatedElements.erase(i);
//			}
//		}
//	}
//}
//
//void UMLDiagram::UMLDiagramPrivate::addToRelatedElements(ElementObject * elementObject)
//{
//	std::string relatedElementName = findRelatedElement(elementObject);
//
//	if(relatedElementName.empty())
//		return;
//
//	relatedElementMap::iterator i = _relatedElements.find(relatedElementName);
//
//	boost::shared_ptr<elementvct> vctPtr;
//
//	if(i == _relatedElements.end() || i->second == 0)
//	{
//		vctPtr = boost::shared_ptr<elementvct>(new elementvct);
//		_relatedElements[relatedElementName] = vctPtr;
//	}
//	else
//	{
//		vctPtr = i->second;
//	}
//
//	vctPtr->push_back(elementObject);
//}
