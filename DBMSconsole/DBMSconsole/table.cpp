#include "table.h"
void showtables(QString user,QString DBname)//在屏幕上打印当前用户当前数据库下的所有表名
{

}

int createTable(QString user,QString DBname,QString TBname,vector<Field> fields,vector<Constraint> constraints)//创建表
{
    return 1;
}

int dropTable(QString user,QString DBname,QString TBname)//删除表
{
    return 1;
}

int deleteField(QString user,QString DBname,QString TBname,QString col)//删除表中一列
{
    return 1;
}

int addField(QString user,QString DBname,QString TBname,Field col,bool notnull)//增加一列，只允许添加非空约束条件，true代表添加，false代表不添加
{
    return 1;
}

int modifyCol(QString user,QString DBname,QString oldname,QString newname,QString type,bool notnull)//修改一列的名称及类型，只允许添加非空约束条件，true代表添加，false代表不添加
{
    return 1;
}

int renameTable(QString user,QString DBname,QString oldname,QString newname)//重命名表
{
    return 1;
}

int removePri(QString user,QString DBname,QString TBname)//删除主键
{
    return 1;
}

int addPri(QString user,QString DBname,QString TBname,Constraint primarykey)//为一到多列添加主键
{
    return 1;
}
