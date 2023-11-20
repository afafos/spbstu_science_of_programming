#pragma once

#include <memory>
#include <string>

#include "XmlParser.h"
#include "XmlTree.h"

class XmlResource
{
public:
	XmlResource() = delete;

	static std::unique_ptr<XmlResource> Create(const std::string& xmlFilePath);

	void Print();
	void Save(const std::string& xmlFilePath);

private:
	explicit XmlResource(const std::string& xmlFilePath);

	void Parse();
	void Output(std::ostream& os);

	std::unique_ptr<XmlParser> m_xmlParser;
	std::unique_ptr<XmlTree<XmlNode>> m_xmlTree;
	
};
