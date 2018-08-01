// ServerClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CommuWin.h"
#include <iostream>
#include <string>
int main(int argc, char *argv[])
{
	std::cout << argc <<std::endl;

	int Inport = std::stoi(argv[1]);
	int Outport = std::stoi(argv[2]);
	//std::cout << "inport = " << argv[1] << " outport = " << argv[2] << std::endl;
	std::cout << "inport = " << Inport << " outport = " << Outport << std::endl;

	CommuWin com(Inport, Outport);
	com.connectTo();

	while (true)
	{
		std::cout << "Enter Send Message" << std::endl;
		std::string msg;
		std::cin >> msg;
		com.sendMsg(msg.c_str());
	}

    return 0;
}

