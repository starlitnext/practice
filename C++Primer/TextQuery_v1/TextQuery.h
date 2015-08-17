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
	std::shared_ptr<std::vector<std::string>> file; // �����ļ�
	// ÿ�����ʵ������ڵ��кŵļ��ϵ�ӳ��
	std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;
};

