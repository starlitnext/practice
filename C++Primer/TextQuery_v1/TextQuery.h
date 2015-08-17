#pragma once
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <memory>

class QueryResult;
class TextQuery
{
public:
	using line_no = std::vector<std::string>::size_type;
	TextQuery(std::ifstream&);
	~TextQuery() = default;
	QueryResult query(const std::string &) const;

private:
	std::shared_ptr<std::vector<std::string>> file; // 输入文件
	// 每个单词到它所在的行号的集合的映射
	std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;
};

