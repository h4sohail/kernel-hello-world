#include <iostream>
#include "Windows.h"

HANDLE deviceHandle = NULL;

void openDevice()
{
	deviceHandle = CreateFile(L"\\\\.\\hellodriversymlink", GENERIC_ALL, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
	if (deviceHandle == INVALID_HANDLE_VALUE) {
		std::cout << "[Handle Status]: Invalid \n\n";
	}
	else
		std::cout << "[Handle Status]: Valid \n\n";
}

void closeDevice() 
{
	if (deviceHandle != INVALID_HANDLE_VALUE) {
		CloseHandle(deviceHandle);
	}
}

void main()
{
	int a;
	while (true)
	{
		std::cout << "0: Exit the program \n";
		std::cout << "1: Request handle \n";
		std::cout << "2: Close handle \n";
		std::cout << "Input a number coresponding to an option above and press Enter: \n";
		std::cin >> a;
		switch (a)
		{
		case 0:
			std::exit(0);
			break;
		case 1:
			openDevice();
			break;
		case 2:
			closeDevice();
			break;
		}
	}
	  
	std::system("pause");
}
