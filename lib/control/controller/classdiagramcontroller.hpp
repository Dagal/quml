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

#ifndef CLASSDIAGRAMCONTROLLER_HPP
#define CLASSDIAGRAMCONTROLLER_HPP

#include "umldiagram.hpp"
#include "iclassdiagramrules.hpp"
#include "singleton.hpp"

namespace controller
{
	class ClassDiagramController : public Singleton<ClassDiagramController>
	{
		friend class Singleton<ClassDiagramController>;

	public:
		// static elementChecker functions
		const IClassDiagramRules * RulesFor(element::ElementObject * elementObject) const;
		const IClassDiagramRules * RulesFor(element::ElementType elementType) const;

		void addRule(element::ElementType elementType, IClassDiagramRules * rule);

	private:
		ClassDiagramController();
		~ClassDiagramController() {};

	private:
		QMap<element::ElementType, boost::shared_ptr<IClassDiagramRules> > _rules;
	};
}

#endif // CLASSDIAGRAMCONTROLLER_HPP
