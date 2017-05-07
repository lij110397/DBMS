#include "convert.h"
void showdatabases(QString user)//在屏幕上打印当前用户的所有数据库
{

}

int createDBEntity(QString user,QString DBname)//创建数据库实体
{

    return 1;
}

int renameDBEntity(QString user,QString newName,QString oldName)//重命名数据库实体
{

    return 1;
}

int dropDBEntity(QString user,QString DBname)//删除数据库实体
{

    return 1;
}

int useDBEntity(QString user,QString DBname,QString &curDBname)//打开并使用数据库
{

    //如果存在，那么curDBname=DBname;
    return 1;
}

int initDBEntity(QString user,QString DBname)//初始化数据库
{
    if(dropDBEntity(user,DBname)!=1) return -1;//删除失败
    if(createDBEntity(user,DBname)!=1) return -2;//创建失败
    return 1;
}

int backupDBEntity(QString user,QString DBname)//备份数据库
{
    return 1;
}
