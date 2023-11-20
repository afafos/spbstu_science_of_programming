#pragma once

#include <optional>
#include <stack>
#include <string>
#include <tuple>
#include <vector>

// Name, value, depth
using XmlNode = std::tuple<std::string, std::optional<int>, size_t>;

class XmlParser
{
public:
	XmlParser() = delete;
	explicit XmlParser(const std::vector<char>& data);
	explicit XmlParser(std::vector<char>&& data) noexcept;

	static bool isOpenTag(const XmlNode& node);
	static bool isCloseTag(const XmlNode& node);

	XmlNode GetNextNode();

private:
	std::string NameFromTag(std::string tag);

	std::vector<char> m_data;
	std::stack<std::string> m_nestedNodes;
	size_t m_pos;
};
