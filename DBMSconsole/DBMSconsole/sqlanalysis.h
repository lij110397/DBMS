#ifndef SQLANALYSIS_H
#define SQLANALYSIS_H
#include <QCoreApplication>
#include <vector>
using namespace std;
/*SQL语句分析*/
int sqlAnalysis(QString sql,vector<QString> &sqlkey);//解析sql语法并返回各部分内容
#endif // SQLANALYSIS_H
