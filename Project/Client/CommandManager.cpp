#include "CommandManager.h"
#include "Utilities.h"
#include <utility>
bool CommandManager::initedCommand = false;

std::map<const std::string, CommandManager::CommandHandler> CommandManager::handlers;
std::pair< std::string, std::string > CommandManager::prefix_delimiter(std::string prefix) { //разделяет в префиксе префикс для опредения типа параметра и суффикс указывающий имя параметры
	std::string clear_prefix, sufix = {};
	auto v = Utilities::splitString(prefix, "_",quote,true);
	switch (v.size()) {
	case 2:
		clear_prefix = v[0];
		sufix = v[1];
		break;
	case 1:
		clear_prefix = v[0];

		break;
	default:
		throw std::invalid_argument("Неверный синтаксис префикса!");
		break;
	}
	return { clear_prefix,sufix };
}
DataBlock CommandManager::stringToDataBlock(std::string s){ //преобразует команду в машиночитаемый datablock
	DataBlock dataBlock;
	bool tokenize = true;
	std::string temp_token="";
	for (std::string token : Utilities::splitString(s, " ",quote,true)) {
		if (token.empty()) continue;
		if (tokenize) {
			if (token[0] == '/') {
				dataBlock.m_params["/"] = token.erase(0, 1);
			}
			else if (token[0] == '-') {
				tokenize = false;
				temp_token = token;
			}
			else {
				std::cerr << "Неверный синтаксис! "
					<<  token  << '\n';
				dataBlock.error = true;
				return dataBlock;
			}
		}
		else {
			try {
				auto pair = prefix_delimiter(temp_token);
				auto prefix = pair.first;
				std::string name_par = "";
				if (pair.second != "") {
					name_par = prefix + "_" + pair.second;
				}
				else {
					name_par = prefix;
				}
				dataBlock.m_params[name_par] = prefixStringToType(prefix, token);
			}
			catch (const std::exception& e) {

				std::cerr << "Ошибка преобразования значения для " << temp_token
					<< " (\"" << token << "\"): " << e.what() << '\n';
				dataBlock.error = true;
			}
			catch (...) {
				std::cerr << "Неизвестная ошибка при обработке " << temp_token << '\n';
				dataBlock.error = true;
			}
			
			tokenize = true;
		}
	}
	if (!tokenize) {
		std::cerr << "Не хватает значения для " << temp_token << '\n';
		dataBlock.error = true;
	}
	return dataBlock;
}
void CommandManager::initCommands() { //Здесь мы добавляем новые команды через handlers.emplace
	if (!initedCommand) {
		initedCommand = true;
		handlers.emplace(
			"help", CommandHandler{ 
			"", [](DataBlock& datablock) {
				std::cout << "Список комманд:\n";
				for (const auto& [name, handler] : handlers)
					std::cout << "/" << name << " " << handler.first << "\n";				
			return Result{};
			} });
		handlers.emplace(
			"clear", CommandHandler{ 
			"", [](DataBlock& datablock) {
				std::cout << "\033[2J\033[H" << std::flush;
			return Result{};
			} });
		handlers.emplace(
			"exit", CommandHandler{ 
			"", [](DataBlock& b) {
			
			return Result{"",false,true};
			} });
	}
}
Result CommandManager::executorCommands(DataBlock& datablock) { //исполняет любую команду
	initCommands();
	try {
		auto it = datablock.m_params.find("/");
		if (it == datablock.m_params.end())
			return Result{ "Не указана команда" };
		std::string nameCommand = anycast<std::string>("-s", it->second);
		if (handlers.contains(nameCommand)) {
			Result result = handlers[nameCommand].second(datablock);
			return result;
		}
		else {
			return Result{ "Данная команда не найдена!" };
		}
	}
	catch (const std::exception& e) {
		
		std::cerr << "Неверный синтаксис команды! " << e.what() << '\n';
		return Result("", true);
	}
}

std::any CommandManager::prefixStringToType(std::string prefix, std::string value) { //нужен чтобы вписать в datablock.params значение value конкретного типа по prefix.
		
	
		if (prefix == "-s") {
			return value;
		}
		else if (prefix == "-i") {
			return std::stoi(value);
		}
		
		throw std::invalid_argument("Неизвестный префикс!");
	
}
