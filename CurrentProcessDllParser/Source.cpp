#include <iostream>
#include <Windows.h>
#include <winternl.h>

extern "C" PLDR_DATA_TABLE_ENTRY GetDllListEntry(void);

bool printDllName(wchar_t* name)
{
	wchar_t check[5]; //Checks last 4 characters for .dll to know when to stop reading
	check[4] = '\0';  //Null terminating byte needs to be added for comparison function
	if (name == nullptr)
	{
		return false;
	}
	for (int i = 0;; i++)
	{
		std::wcout << name[i];
		check[0] = name[i];
		check[1] = name[i + 1];
		check[2] = name[i + 2];
		check[3] = name[i + 3];

		if (_wcsicmp(check, L".dll") == 0) //Checks for file extension in upcoming characters
		{
			std::wcout << L"dll" << std::endl;
			break;
		}
	}
	return true;
}

int main()
{
	PLDR_DATA_TABLE_ENTRY dllEntry;
	PLIST_ENTRY dllEnd;

	dllEntry = GetDllListEntry(); //Get address of the first dll entry

	//Typecasting works here because both structs utilize the same data but its parsed differently based on the use
	dllEnd = (PLIST_ENTRY)dllEntry; //Address of the blink member in the first entry contains the address of the last entry
	printf("\\___[ Dll List Entry ]\n\t\\_%p\n", dllEntry);
	printf("\\___[ Dll List End ]\n\t\\_%p\n", dllEnd->Blink);

	//The first entry is always the name of the executable so moving to the second to get the Dlls
	dllEntry = (PLDR_DATA_TABLE_ENTRY)dllEntry->Reserved1[0]; //Contains the memory address of the next dll in the list

	//Compares the addresses of dllEntry and dllEnd, if they are equal all the dlls have been listed, after each iteration the address of
	//the next dll get loaded into dllEntry
	for (; (PLIST_ENTRY)dllEntry != dllEnd->Blink; dllEntry = (PLDR_DATA_TABLE_ENTRY)dllEntry->Reserved1[0])
	{
		//The length member of the struct is often greater than the actual string in the buffer which is why the function
		//has to check the upcoming characters for when the dll name ends
		printDllName(dllEntry->FullDllName.Buffer);
	}
	return 0;
}