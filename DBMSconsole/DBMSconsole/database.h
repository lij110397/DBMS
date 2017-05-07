#ifndef DATABASE_H
#define DATABASE_H
#include <QCoreApplication>
/*数据库管理模块*/
void showdatabases(QString user);//在屏幕上打印当前用户的所有数据库名
int createDBEntity(QString user,QString DBname);//创建数据库实体
int renameDBEntity(QString user,QString newName,QString oldName);//重命名数据库实体
int dropDBEntity(QString user,QString DBname);//删除数据库实体
int useDBEntity(QString user,QString DBname,QString &curDBname);//打开并使用数据库
int initDBEntity(QString user,QString DBname);//初始化数据库
int backupDBEntity(QString user,QString DBname);//备份数据库
#endif // DATABASE_H
