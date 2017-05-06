#ifndef INDEX_H
#define INDEX_H
#include <QCoreApplication>
#include <vector>
using namespace std;
/*索引管理模块*/
int createIndex(QString user,QString DBname,QString TBname,QString indexname,vector<QString> colnames);//建立索引，传入参数为建立索引的列名
#endif // INDEX_H
