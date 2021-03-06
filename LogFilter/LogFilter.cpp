#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;
typedef struct _tagNode
{
	string _strID;
	int _nTimeStart;
	int _nTimeEnd;
	int _nFlag; // 0,new; 1,old
}Node;

Node* GetNewNode()
{
	Node* p = new Node();
	p->_nTimeStart = 0;
	p->_nTimeEnd = 0;
	return p;
}

void FreeNode(Node* pNode)
{
	delete pNode;
	pNode = NULL;
}
/********* Tools ***********/
vector<string> Split(const string& str, const string& delim) {
	vector<string> res;
	if ("" == str) return res;
	//先将要切割的字符串从string类型转换为char*类型  
	char * strs = new char[str.length() + 1]; //不要忘了  
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while (p) {
		string s = p; //分割得到的字符串转换为string类型  
		res.push_back(s); //存入结果数组  
		p = strtok(NULL, d);
	}

	return res;
}

int GetTime(string& strLine)
{
	string str = strLine;
	int find;
	while ((find = str.find(":")) != -1)
	{
		str.erase(find, 1);
	}

	int ret = atoi(str.c_str());
	return ret;
}

string GetSID(string& strLine)
{
	string strRet;
	int find = strLine.find("=");
	if (find == -1)
	{
		return "";
	}
	else
	{
		strRet = strLine.substr(find + 1);
	}
	return strRet;
}

map<string, Node> g_map;
int g_minTime = 999999;
int g_maxTime = 0;
int main()
{
	// Read File
	std::ifstream fin("src.log", std::ios::in);
	char line[1024] = { 0 };
	string strLine;
	vector<string> vecLine;
	string strDelim = "-";

	while (fin.getline(line, sizeof(line)))
	{
		std::stringstream word(line);
		strLine = line;

		// split
		vecLine = Split(strLine, strDelim);
		Node*pNode = NULL;
		string strId = GetSID(vecLine[3]);
		map<string, Node>::iterator it = g_map.find(strId);
		if (it == g_map.end())
		{
			pNode = new Node;
			pNode->_nFlag = 0;
		}
		else
		{
			pNode = &(it->second);
		}
		pNode->_strID = strId;

		if (vecLine[2] == "Endsessionsuccess:")
		{
			pNode->_nTimeEnd = GetTime(vecLine[1]);
			g_maxTime = pNode->_nTimeEnd > g_maxTime ? pNode->_nTimeEnd : g_maxTime;
		}
		else
		{
			pNode->_nTimeStart = GetTime(vecLine[1]);
			g_minTime = pNode->_nTimeStart < g_minTime ? pNode->_nTimeStart : g_minTime;
		}

		if (pNode->_nFlag == 0)
		{
			pNode->_nFlag++;
			g_map.insert(map<string,Node>::value_type(pNode->_strID, *pNode));
			delete pNode;
		}
	}
	fin.clear();
	fin.close();

	// Travel
	int retCnt = 0;
	for (int i = g_minTime; i <= g_maxTime; i++)
	{
		map<string, Node>::iterator it;
		int cnt = 0;
		for (it = g_map.begin(); it != g_map.end(); it++) {
			if (it->second._nTimeStart <= i && it->second._nTimeEnd >= i)
			{
				cnt++;
			}
		}
		retCnt = retCnt > cnt ? retCnt : cnt;
	}

	printf("Start Time: %d\n", g_minTime);
	printf("End Time: %d\n", g_maxTime);
	printf("Max Cnt: %d\n", retCnt);


    return 0;
}

