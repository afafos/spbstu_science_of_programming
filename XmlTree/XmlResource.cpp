#include "XmlResource.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stack>
#include <stdexcept>

std::unique_ptr<XmlResource> XmlResource::Create(const std::string& xmlFilePath)
{
	return std::unique_ptr<XmlResource>(new XmlResource(xmlFilePath));
}

void XmlResource::Print()
{
	Output(std::cout);
}

void XmlResource::Save(const std::string& xmlFilePath)
{
	std::ofstream xmlFile(xmlFilePath);

	if (!xmlFile.is_open())
	{
		throw std::runtime_error("Could not open file " + xmlFilePath);
	}

	Output(xmlFile);
	xmlFile.close();
}

XmlResource::XmlResource(const std::string& xmlFilePath)
	: m_xmlTree(XmlTree<XmlNode>::Create())
{
	std::ifstream xmlFile(xmlFilePath);

	if (!xmlFile.is_open())
	{
		throw std::runtime_error("Could not open file " + xmlFilePath);
	}

	std::vector<char> data;
	std::copy(std::istream_iterator<char>(xmlFile), {}, std::back_inserter(data));
	xmlFile.close();

	m_xmlParser = std::make_unique<XmlParser>(std::move(data));
	Parse();
}

void XmlResource::Parse()
{
	const auto isValidTag = [](const XmlNode& node) -> bool
	{
		return XmlParser::isOpenTag(node) || XmlParser::isCloseTag(node);
	};

	std::stack<XmlTree<XmlNode>::Iterator> curPath;
	auto node = m_xmlParser->GetNextNode();
	curPath.push(m_xmlTree->begin());

	while (isValidTag(node))
	{
		if (XmlParser::isOpenTag(node))
		{
			curPath.push(m_xmlTree->Insert(curPath.top(), std::make_unique<XmlNode>(std::move(node))));
		}
		else
		{
			curPath.pop();
		}

		node = m_xmlParser->GetNextNode();
	}
}

void XmlResource::Output(std::ostream& os)
{
	std::stack<std::string> curPath;

	auto const closeTags = [&curPath, &os](size_t depth)
	{
		while (depth < curPath.size())
		{
			std::string name = curPath.top();
			curPath.pop();
			os << std::string(curPath.size() * 4, ' ') << "</" << name << ">" << std::endl;
		}
	};

	for (const auto& v : *m_xmlTree)
	{
		closeTags(std::get<2>(v));
		os << std::string(curPath.size() * 4, ' ') << "<" << std::get<0>(v) << "> " << std::get<1>(v).value() << std::endl;
		curPath.push(std::get<0>(v));
	}

	closeTags(0);
}
