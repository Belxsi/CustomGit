#pragma once
#include <string>
#include <map>
#include <any>
#include <functional>
#include <iostream>
#include <utility>
#include <stdexcept>
struct DataBlock
{
	std::map<std::string, std::any> m_params;
	bool error=false;
};
struct Result
{
	std::string message;
	bool error=false;
	bool quit = false;
};
class CommandManager
{
private:
	static std::any prefixStringToType(std::string prefix, std::string value);
	static std::pair< std::string, std::string > prefix_delimiter(std::string prefix);
public:
	static const char quote = '\"';
	static bool initedCommand;
	using CommandHandler = std::pair<std::string, std::function<Result(DataBlock& datablock)>>;
	static std::map<const std::string, CommandHandler> handlers;
public:
	static DataBlock stringToDataBlock(std::string s);
	
	template <typename T> static T anycast(std::string prefix, std::any a) {
		if (prefix == "-i") {
			return std::any_cast<T>(a);
		}
		if (prefix == "-s") {
			return std::any_cast<T>(a);
		}
		throw std::invalid_argument("Неизвестный префикс!");
		
	}
	template <typename T> static T get_param(const std::string& prefix,DataBlock& b) {
		
		try {
			auto pair=	prefix_delimiter(prefix);
			auto it = b.m_params.find(prefix);
			if (it == b.m_params.end())
				throw std::runtime_error("Не задан параметр " + prefix);
			return anycast<T>(pair.first, it->second);
		}
		catch (const std::exception& ex) {
			std::cerr << "Ошибка получения параметра! ";
			throw;
		}

	}
	static void initCommands();
	static Result executorCommands(DataBlock& datablock);
};




