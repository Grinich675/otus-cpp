#include "Commands.hpp"


CMD::CMD():
	type(cmd_type_t::Unknown)
{}

CMD::CMD(cmd_type_t t):
	type(t)
{}

cmd_type_t CMD::Type()
{
	return type;
}

InsertCMD::InsertCMD(std::string t_name_):
	CMD(cmd_type_t::Insert),table_name(t_name_),values{}
{}

void InsertCMD::AddValue(std::string val_)
{
	values.emplace_back(val_);
}


TruncateCMD::TruncateCMD(std::string t_name):
					CMD(cmd_type_t::Truncate),table_name(t_name)
{}

std::string TruncateCMD::TableName()
{
	return table_name;
}




IntersectCMD::IntersectCMD(std::string fst_t_name_,std::string sec_t_name_):
				CMD(cmd_type_t::Intersect),first_table_name(fst_t_name_),second_table_name(sec_t_name_)
{}

std::pair<std::string,std::string> IntersectCMD::TableNames()
{
	return std::make_pair(first_table_name,second_table_name);
}



Symm_diff_CMD::Symm_diff_CMD(std::string fst_t_name_,std::string sec_t_name_):
				CMD(cmd_type_t::Symm_diff),first_table_name(fst_t_name_),second_table_name(sec_t_name_)
{}

std::pair<std::string,std::string> Symm_diff_CMD::TableNames()
{
	return std::make_pair(first_table_name,second_table_name);
}




BrokenCMD::BrokenCMD(std::string err_):
				CMD(cmd_type_t::Broken),err_msg(err_)
{}

std::string BrokenCMD::ErrMsg()
{
	return err_msg;
}


boost::char_separator<char> CMDCreator::sep{" "};

std::unique_ptr<CMD> CMDCreator::create_insert_cmd(tokenizer::iterator& beg,tokenizer::iterator end)
{
	auto ret = std::make_unique<InsertCMD>(*beg);
	++beg;
	while(beg!= end)
	{
		ret->AddValue(*beg);
		++beg;
	}

	return ret;
}


std::unique_ptr<CMD> CMDCreator::create_truncate_cmd(tokenizer::iterator& beg,tokenizer::iterator end)
{
	auto ret = std::make_unique<TruncateCMD>(*beg);
	++beg;

	if(beg!= end)
	{
		return std::make_unique<BrokenCMD>("Too many parameters for TRUNCATE command");
	}

	return ret;
}

std::unique_ptr<CMD> CMDCreator::create_intersect_cmd(tokenizer::iterator& beg,tokenizer::iterator end)
{

	std::string first_tab_name = *beg;
	++beg;

	if(beg!= end)
	{
		std::string second_tab_name = *beg;

		++beg;
		if(beg!= end)
		{
			return std::make_unique<BrokenCMD>("Too many parameters for INTERSECTION command");
		}

		return std::make_unique<IntersectCMD>(first_tab_name,second_tab_name);
	}

	return std::make_unique<BrokenCMD>("Not enough parameters for INTERSECTION command");
}

std::unique_ptr<CMD> CMDCreator::create_diff_cmd(tokenizer::iterator& beg,tokenizer::iterator end)
{
	std::string first_tab_name = *beg;
	++beg;

	if(beg!= end)
	{
		std::string second_tab_name = *beg;

		++beg;
		if(beg!= end)
		{
			return std::make_unique<BrokenCMD>("Too many parameters for  SYMMETRIC_DIFFERENCE command");
		}

		return std::make_unique<Symm_diff_CMD>(first_tab_name,second_tab_name);
	}

	return std::make_unique<BrokenCMD>("Not enough parameters for SYMMETRIC_DIFFERENCE command");
}

std::unique_ptr<CMD> CMDCreator::create_cmd(std::string _cmd_str)
{
	tokenizer tok{_cmd_str, sep};

	auto token = tok.begin();

	if (boost::iequals(*token, "INSERT"))
	{
		++token;
		if (token!=tok.end())
		{
			return create_insert_cmd(token,tok.end());
		}
		else
		{
			return std::make_unique<BrokenCMD>("Not enough parameters for INSERT command");
		}
	}
	else if(boost::iequals(*token, "TRUNCATE"))
	{
		++token;
		if (token!=tok.end())
		{
			return create_truncate_cmd(token,tok.end());
		}
		else
		{
			return std::make_unique<BrokenCMD>("Not enough parameters for TRUNCATE command");
		}
	}
	else if(boost::iequals(*token, "INTERSECTION"))
	{
		++token;
		if (token!=tok.end())
		{
			return create_intersect_cmd(token,tok.end());
		}
		else
		{
			return std::make_unique<BrokenCMD>("Not enough parameters for INTERSECTION command");
		}

	}
	else if(boost::iequals(*token, "SYMMETRIC_DIFFERENCE"))
	{
		++token;
		if (token!=tok.end())
		{
			return create_diff_cmd(token,tok.end());
		}
		else
		{
			return std::make_unique<BrokenCMD>("Not enough parameters for SYMMETRIC_DIFFERENCE command");
		}
	}
	else
	{
		return std::make_unique<BrokenCMD>("Unknown command:"+*token);
	}

	return std::unique_ptr<CMD>();

}


