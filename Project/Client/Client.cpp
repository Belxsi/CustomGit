
#include "Client.h"
#include <iostream>
#include <string>
#include "Windows.h"
#include "CommandManager.h"
void Client::headLoop() {
	printBanner();
	std::cout << "Введите /help чтобы увидеть список комманд...\n";
	std::string s;
	while (std::getline(std::cin, s)) {
		if (s.empty()) continue;
		auto datablock= CommandManager::stringToDataBlock(s);
		if (datablock.error) continue;
		Result result= CommandManager::executorCommands(datablock);
		if (result.error) continue;
		if(result.message!="")
		std::cout << result.message<<"\n";
		if (result.quit) break;
	}
}
void Client::printBanner() {
	std::cout << R"(
   _____          _                     _____ _ _   
  / ____|        | |                   / ____(_) |  
 | |    _   _ ___| |_ ___  _ __ ___   | |  __ _| |_ 
 | |   | | | / __| __/ _ \| '_ ` _ \  | | |_ | | __|
 | |___| |_| \__ \ || (_) | | | | | | | |__| | | |_ 
  \_____\__,_|___/\__\___/|_| |_| |_|  \_____|_|\__|
----------------------------------------------------
              Система контроля версий.                                                    
)" << '\n';
}
int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Client::headLoop();
}