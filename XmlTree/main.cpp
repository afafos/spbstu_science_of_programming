#include <iostream>
#include <stdexcept>

#include "XmlResource.h"

int main()
{
	try
	{
		auto xmlResource = XmlResource::Create("tree.xml");
		xmlResource->Print();
		xmlResource->Save("tree1.xml");
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
