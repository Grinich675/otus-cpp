#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <list>
#include <memory>
#include <string>
#include <utility>

enum class cmd_type_t
{
	Insert,
	Delete,
	Create,
	Truncate,
	Intersect,
	Symm_diff,
	Broken,
	Unknown

};

class CMD
{
	cmd_type_t type;
protected:
	CMD();
public:
	CMD(cmd_type_t);

	cmd_type_t Type();

	virtual ~CMD()=default;
};

class InsertCMD:public CMD
{
public:
	std::string table_name;
	std::list<std::string> values;

	InsertCMD()=delete;
	InsertCMD(std::string );

	void AddValue(std::string );

	~InsertCMD()=default;

};

class TruncateCMD:public CMD
{
	std::string table_name;
public:
	TruncateCMD()=delete;
	TruncateCMD(std::string );
	std::string TableName();

	~TruncateCMD()=default;
};

class DeleteCMD:public CMD
{

	//type
	//entity_name
};

class IntersectCMD:public CMD
{
	std::string first_table_name;
	std::string second_table_name;
public:
	IntersectCMD()=delete;
	IntersectCMD(std::string fst_t_name_,std::string sec_t_name_);

	std::pair<std::string,std::string> TableNames();

	~IntersectCMD()=default;
};

class Symm_diff_CMD:public CMD
{
	std::string first_table_name;
	std::string second_table_name;
public:
	Symm_diff_CMD()=delete;
	Symm_diff_CMD(std::string fst_t_name_,std::string sec_t_name_);

	std::pair<std::string,std::string> TableNames();

	~Symm_diff_CMD()=default;
};

class BrokenCMD:public CMD
{
public:
	std::string err_msg;

	BrokenCMD()=delete;
	BrokenCMD(std::string err_);

	std::string ErrMsg();

	~BrokenCMD()=default;
};

class CMDCreator
{
	using tokenizer= boost::tokenizer<boost::char_separator<char>> ;
	static boost::char_separator<char> sep;

	std::unique_ptr<CMD> create_insert_cmd(tokenizer::iterator& beg,tokenizer::iterator end);

	std::unique_ptr<CMD> create_truncate_cmd(tokenizer::iterator& beg,tokenizer::iterator end);

	std::unique_ptr<CMD> create_intersect_cmd(tokenizer::iterator& beg,tokenizer::iterator end);

	std::unique_ptr<CMD> create_diff_cmd(tokenizer::iterator& beg,tokenizer::iterator end);

public:

	CMDCreator()=default;
	~CMDCreator()=default;

	std::unique_ptr<CMD> create_cmd(std::string _cmd_str);
};
