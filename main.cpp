﻿#define 做题平台 "poj"
#define 搜索引擎 "cn.bing.com"
//不可用baidu.com作为搜索引擎->百度将搜索结果加密,利用重定向功能,无法准确判断是否为CSDN论坛地址
#define 搜索目标 "CSDN"
#define 目标地址  "blog.csdn.net/"

#define Accepted 1
#define Waiting 2
#define WrongAnswer 3

#include<cstdio>
#include<cstring>
#include"core\Socket.h"
#include"headers\HtmlAnalyzer.h"
#include"core\CurrentTime.h"
#include"headers\CodeAnalysis.h"
#include"headers\POJsubmiter.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;
int resubmission;
string host = 搜索引擎;
string searchSuffix = "/search?q=";
string blogName = "+";
string *searchResult;//搜索结果页
queue<string> *toCheck;//结果中是CSDN论坛地址的列表
CodeFetcher *extractor;//从论坛地址指向的网页中提取代码
string filename;//存储时使用
string res_code;//初步扒下的代码，待检查
string username, password;//POJ 登录信息
FILE *result;//存储文件流、检查文件流
FILE *Log;//日志文件流
int ProblemID,EndProblemID;

string Int2String(int a) {
	char temp[100];
	sprintf(temp, "%d", a);
	return (string)temp;
}
void printString(string &a) {
	int len = a.length();
	for (int i = 0; i <len; i++)putchar(a[i]);
}
string Finalstatus[3] = { "Accepted","Waiting","Wrong Answer" };
string getFinalstatus(int status) { return Finalstatus[status - 1]; }
int main() {

	WSADATA word;
	WSAStartup(MAKEWORD(2, 2), &word);

	searchSuffix += 做题平台; searchSuffix += "+"; blogName += 搜索目标;

	//print search infos
	{
		cout << "using " << (char)34 << host.c_str() << (char)34 << "as search engine" << endl;
		cout << "hacking for " << 做题平台 << endl;
		cout << "hack between problem ";
		cin >> ProblemID; cout << "and "; cin >> EndProblemID;
		cout << "enter POJ login info" << endl<<"username:";
		cin >> username; cout << "password:"; cin >> password;
	}
	login();
	for (ProblemID; ProblemID <= EndProblemID; ProblemID++) {
		Log = fopen("log.txt", "a");
		string pID = Int2String(ProblemID);
		fprintf(Log, "[log][%s]Current ProblemID:%d\n", CurrentTime().c_str(), ProblemID);
		fclose(Log);
		cout << "[log][" << CurrentTime().c_str() << "]" << "Current PID:" << pID;

		searchResult=getPage(host, searchSuffix + pID + blogName);
		cout << ".";
		toCheck = getBlogURL(*searchResult,目标地址);
		extractor = new CodeFetcher(toCheck, pID);
		cout << ".";
		//system(((string)"md " + (string)做题平台 + pID).c_str());
		int versions = 1, finalstatus;
		for (int i = 1; !extractor->empty(); i++) {
			res_code = string(extractor->front());
			if (examine(res_code) && strcmp(res_code.c_str(), "none")) {//实时代码分析
				resubmission = 0;
			resubmit:;
				if (resubmission > 5) {
					FILE *temp = fopen((pID + ".cpp").c_str(), "w");
					fprintf(temp, "%s", res_code.c_str());
					fclose(temp);
					cout << "no response" << endl;
					extractor->pop();
					break;
				}
				cout << ".";
				SubmitOnPOJ(res_code, pID);
				//filename = 做题平台 + pID + '\\' + (string)"version" + toString(versions++) + (string)".cpp";
				//result = fopen(filename.c_str(), "w");
				//fprintf(result, "%s", resUnchecked.c_str());
				//fclose(result);
			WAIT:;
				Sleep(700);
				finalstatus = getStatus(pID);
				switch (finalstatus) {
				case Accepted:
					cout << "Success" << endl;
					goto AC;
				case Waiting:
					Sleep(1000);
					goto WAIT;
				case WrongAnswer:
					goto WA;
				case 10://not found
					Sleep(300);
					resubmission++;
					goto resubmit;
				}
			}
		WA:;
			extractor->pop();
			if (extractor->empty())cout << "Failed" << endl;
		}
	AC:;
		free(extractor);
		//TODO:add to SQL
	}

	WSACleanup();
}