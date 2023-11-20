#include "XmlParser.h"

#include <algorithm>
#include <iterator>
#include <regex>

XmlParser::XmlParser(const std::vector<char>& data)
	: m_pos(0)
{
	std::copy(data.cbegin(), data.cend(), std::back_inserter(m_data));
}

XmlParser::XmlParser(std::vector<char>&& data) noexcept
	: m_data(std::move(data))
	, m_pos(0)
{
}

bool XmlParser::isOpenTag(const XmlNode& node)
{
	return !std::get<0>(node).empty() && std::get<1>(node) != std::nullopt;
}

bool XmlParser::isCloseTag(const XmlNode& node)
{
	return !std::get<0>(node).empty() && std::get<1>(node) == std::nullopt;
}

XmlNode XmlParser::GetNextNode()
{
	static const std::regex openNodeRegex("^\\<[\\w_]+\\>");
	static const std::regex valueRegex("^[\\-]?\\d+");
	static const std::regex closeNodeRegex("^\\<\\/[\\w_]+\\>");

	std::match_results<decltype(m_data)::const_iterator> nodeMatch;
	std::match_results<decltype(m_data)::const_iterator> valueMatch;

	if (m_pos >= m_data.size())
	{
		return {};
	}

	if (std::regex_search(m_data.cbegin() + m_pos, m_data.cend(), nodeMatch, openNodeRegex))
	{
		m_pos += nodeMatch[0].length();

		if (std::regex_search(m_data.cbegin() + m_pos, m_data.cend(), valueMatch, valueRegex))
		{
			const auto nodeName = NameFromTag(nodeMatch[0].str());
			XmlNode node = { nodeName, std::stoi(valueMatch[0].str()), m_nestedNodes.size() };
			m_pos += valueMatch[0].length();
			m_nestedNodes.push(nodeName);
			return node;
		}
		else
		{
			// Invalid XML file
		}
	}
	else if (std::regex_search(m_data.cbegin() + m_pos, m_data.cend(), nodeMatch, closeNodeRegex))
	{
		const auto nodeName = NameFromTag(nodeMatch[0].str());
		m_pos += nodeMatch[0].length();

		if (!m_nestedNodes.empty() && nodeName == m_nestedNodes.top())
		{
			m_nestedNodes.pop();
			return { nodeName, std::nullopt, m_nestedNodes.size() };
		}
		else
		{
			// Invalid XML file
		}
	}

	throw std::runtime_error("Invalid XML file");
}

std::string XmlParser::NameFromTag(std::string tag)
{
	tag.erase(tag.begin());
	tag.erase(std::prev(tag.end()));

	if (const auto pos = tag.find('/'); pos != std::string::npos)
	{
		tag.erase(pos, 1);
	}

	return tag;
}
