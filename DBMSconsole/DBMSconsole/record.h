#ifndef RECORD_H
#define RECORD_H
#include <vector>
#include <QCoreApplication>
#include <map>
using namespace std;
/*记录管理模块*/
int insertRecord(QString user,QString DBname,QString TBname,vector<map<QString,QString>> records);//插入记录
int updateRecord(QString user,QString DBname,QString TBname,vector<map<QString,QString>> colnames);//更新记录
int selectRecord(QString user,QString DBname,vector<QString> TBnames,vector<QString> colname,vector<int> rdno);//查询记录，传入参数为列名和记录的序号
int deleteRecord(QString user,QString DBname,QString TBname,int rdno);//删除记录，传入参数为记录的序号
vector<int> analyzeWhere(QString condition);//传入参数为where筛选条件，返回值为记录序号的集合
vector<int> analyzeGroupby(QString condition);//传入参数为groupby筛选条件，返回值为记录序号的集合
#endif // RECORD_H
