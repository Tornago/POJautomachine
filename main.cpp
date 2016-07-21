﻿#define 做题平台 "poj"
#define 搜索引擎 "cn.bing.com"
//不可用baidu.com作为搜索引擎->百度将搜索结果加密,利用重定向功能,无法准确判断是否为CSDN论坛地址
#define 搜索目标 "CSDN"
#define 目标地址  "blog.csdn.net/"
#define COOKIE "Cookie: JSESSIONID=4C8FD88A5DA72A97067A3547F88A4E3F; __utmc=79247125; __utma=79247125.1915142156.1468666850.1468845393.1469062220.6; __utmz=79247125.1468669205.2.2.utmcsr=baidu|utmccn=(organic)|utmcmd=organic; __utmb=79247125.2.10.1469062220\r\n"
//自行登录并截取cookie信息 

#include<cstring>
#include"Socket.h"
#include"HtmlAnalyzer.h"
#include"CurrentTime.h"
#include"CodeAnalysis.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;
string host = 搜索引擎;
string searchSuffix = "/search?q=";
string blogName = "+";
string *searchResult;//搜索结果页
queue<string> *toCheck;//结果中是CSDN论坛地址的列表
queue<string> filenames;//已存储的文件列表，供代码分析用
CodeFetcher *extractor;//从论坛地址指向的网页中提取代码
string filename;//存储时使用
string resUnchecked;//初步扒下的代码，待检查
FILE *result;//存储文件流、检查文件流
FILE *Log;//日志文件流
int ProblemID,EndProblemID;
string toString(int a) {
	char temp[10];
	sprintf(temp, "%d", a);
	string result(temp);
	return result;
}
void printString(string &a) {
	int len = a.length();
	for (int i = 0; i <len; i++)putchar(a[i]);
}
int main() {

	WSADATA word;
	WSAStartup(MAKEWORD(2, 2), &word);

	searchSuffix += 做题平台; searchSuffix += "+"; blogName += 搜索目标;

	//print search infos
	{
		cout << "using " << (char)34 << host.c_str() << (char)34 << "as search engine" << endl;
		cout << "hacking for " << 做题平台 << endl;
		cout << "start hacking from problem ";
		cin >> ProblemID; cout << "to problem "; cin >> EndProblemID;
	}
	
	for (ProblemID; ProblemID <= EndProblemID; ProblemID++) {
		Log = fopen("log.txt", "a");
		string pID = toString(ProblemID);
		fprintf(Log, "[log][%s]Current ProblemID:%d\n", CurrentTime().c_str(), ProblemID);
		fclose(Log);

		searchResult=getPage(host, searchSuffix + pID + blogName);

		toCheck = getBlogURL(*searchResult,目标地址);
		extractor = new CodeFetcher(toCheck);
		
		system(((string)"md " + (string)做题平台 + pID).c_str());
		int versions = 1;
		for (int i = 1; !extractor->empty(); i++) {
			resUnchecked = string(extractor->front());
			if (examine(resUnchecked) && strcmp(resUnchecked.c_str(), "none")) {
				filename = 做题平台 + pID + '\\' + (string)"version" + toString(versions++) + (string)".cpp";
				result = fopen(filename.c_str(), "w");
				filenames.push(filename);
				fprintf(result, "%s", resUnchecked.c_str());
				fclose(result);
				
			}
			extractor->pop();
		}
		free(extractor);
	}

	WSACleanup();
}