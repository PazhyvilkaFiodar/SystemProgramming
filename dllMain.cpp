#include <stdio.h>
#include <iostream> 
#include <windows.h>
#include "..\Functional\plugins_manager.h"

#pragma comment (lib, "functional.lib")

int main()
{
	bool menu, choosing;
	int choice;
	menu = true;
	while (menu)
	{
		printf("%s\n", "Menu:");
		printf("%s\n", "1 - Load plugins");
		printf("%s\n", "2 - Unload plugins");
		printf("%s\n", "3 - Show info about loaded plugins");
		printf("%s\n", "4 - Execute plugins");
		printf("%s\n", "5 - Clean console");
		printf("%s\n", "6 - Exit");

		choosing = true;

		while (choosing)
		{
			printf("%s", "Your choice: ");
			scanf_s("%d", &choice);

			switch (choice)
			{
			case 1:
				LoadPlugins();
				choosing = false;
				break;
			case 2:
				UnloadPlugins();
				choosing = false;
				break;
			case 3:
				ShowInfo();
				choosing = false;
				break;
			case 4:
				RunPlugins();
				choosing = false;
				break;
			case 5:
				system("cls");
				choosing = false;
				break;
			case 6:
				Exit();
				choosing = false;
				menu = false;
				break;
			default:
				printf("%s\n\n", "Incorrect choice.");
				break;
			}
		}
	}

	system("pause");
	return 0;
}