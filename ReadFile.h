#pragma once

#include <fstream>
#include <iostream>

#include <windows.h>
#include <tchar.h>

#include <vector>
#include <map>

#include <string>


using namespace std;


string sdk_road = ".\\YuanShen_Data\\Plugins\\PCGameSDK.dll";
string vers_road = ".\\sdk_pkg_version";

string sdk_data = ".\\data\\plugins\\PCGameSDK.dll";
string vers_data = ".\\data\\plugins\\sdk_pkg_version";

string local_road = ".\\data\\locale\\local.ini";
string confg_road = ".\\data\\config.ini";

bool del_core(string road)
{
	if ((GetFileAttributes(road.c_str()) == INVALID_FILE_ATTRIBUTES) ||
		((GetFileAttributes(road.c_str()) != INVALID_FILE_ATTRIBUTES) &&
			!remove(road.c_str()))) { 
		return true; 
	}
	else { return false; }
}


bool wrt_core(string data, string road)
{
	if ((GetFileAttributes(road.c_str()) != INVALID_FILE_ATTRIBUTES) ||
		((GetFileAttributes(road.c_str()) == INVALID_FILE_ATTRIBUTES) &&
			CopyFile(data.c_str(), road.c_str(), true))) { 
		return true; 
	}
	else { return false; }
}


bool SDK_Delete(string& road)
{
	//文件路径名（需保证文件夹存在，否则会转而判断文件夹是否存在）
	if (del_core(road + sdk_road) &&
		del_core(road + vers_road)) {
		return true; 
	}
	else { return false; }
}


bool SDK_Write(string& road)
{
	//文件路径名（需保证文件夹存在，否则会转而判断文件夹是否存在）
	if (wrt_core(sdk_data, road + sdk_road) && 
		wrt_core(vers_data, road + vers_road)) {
		return true;
	}
	else { return false; }
}


bool Read_Set(string& road) 
{
	ifstream read(local_road);
	if (!read) { return false; }
	try {
		getline(read, road);
		read.close();
		return true;
	}
	catch (bool) {
		read.close();
		return false; 
	}
}


bool Write_Set(string& road)
{
	ofstream write(local_road);
	if (!write) { return false; }
	try {
		write << road;
		write.close();
		return true;
	}
	catch (bool) {
		write.close(); 
		return false;
	}
}


bool Read_Tem(map<string, string>& store)
{
	ifstream read(confg_road);
	if (!read) { return false; }
	try {
		string str = "";
		while (!read.eof()) {
			getline(read, str, '\n');
			store[str] = "";
		}
		return true;
	}
	catch (bool) { return false; }
}


bool Write_Tem(string& road, string& version)
{
	ofstream write(road + "\\config.ini");
	if (!write) { return false; }
	try {
		write << "[General]\n"
			<< "channel=1\n"
			<< "cps=mihoyo\n"
			<< "game_version=" + version + "\n"
			<< "plugin_5_version=" + version + "\n"
			<< "plugin_sdk_version=\n"
			<< "sdk_version=\n"
			<< "sub_channel=1";
		return true;
	}
	catch (bool) { return false; }
}


bool Read_New(map<string, string>& store, string& road)
{
	ifstream read(road + "\\config.ini");
	if (!read) { return false; }
	try {
		string type = "", value = "";
		getline(read, type, '\n');
		while (!read.eof()) {
			getline(read, type, '=');
			getline(read, value, '\n');
			if (type == "") { break; }
			store[type] = value;
		}
		read.close();
		return true;
	}
	catch (bool) { 
		read.close();
		return false; 
	}
}


bool Write_New(map<string, string>& store, string& road)
{
	ofstream write(road + "\\config.ini");
	if (!write) { return false; }
	string str("");
	int num = 0;
	try {
		write << "[General]\n";
		for (auto& iter = store.begin(); iter != store.end(); ++iter) {
			write << iter->first + "=" + iter->second << endl;
		}
		write.close();
		return true;
	}
	catch (bool) { 
		write.close();
		return false; 
	}
}
