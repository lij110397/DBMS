#ifndef TABLE_H
#define TABLE_H
#include <vector>
#include <set>
#include <QCoreApplication>
using namespace std;
typedef struct
{
    QString name;//字段名
    QString type;//字段类型
}Field;
typedef struct
{
    QString name;//约束条件名
    int type;//约束条件类型，0代表主键，1代表外键，2代表unique，3代表not null，4代表check，5代表default
    vector<QString> colnos;//施加约束的列，主键允许加于多列

    //外键需要
    QString outertable;//参考的表名
    QString outerprino;//参考的列名

    //check需要
    int checktype;//0代表是连续型范围，存在最大和最小值；1代表是离散型范围，用集合表示，值从集合的元素中选择
    QString max;//最大值
    QString min;//最小值
    set<QString> valueset;//取值的集合

    //defalut需要
    QString defaultvalue;//默认值
}Constraint;
/*表管理模块*/
void showtables(QString user,QString DBname);//在屏幕上打印当前用户当前数据库下的所有表名
int createTable(QString user,QString DBname,QString TBname,vector<Field> fields,vector<Constraint> constraints);//创建表
int dropTable(QString user,QString DBname,QString TBname);//删除表
int deleteField(QString user,QString DBname,QString TBname,QString col);//删除表中一列
int addField(QString user,QString DBname,QString TBname,Field col,Constraint constraint);//增加一列，只允许添加列一级约束条件
int modifyCol(QString user,QString DBname,QString oldname,QString newname,QString type,Constraint constraint);//修改一列的名称及类型，只允许添加非空约束条件，true代表添加，false代表不添加
int renameTable(QString user,QString DBname,QString oldname,QString newname);//重命名表
int removePri(QString user,QString DBname,QString TBname);//删除主键
int addPri(QString user,QString DBname,QString TBname,Constraint primarykey);//为一到多列添加主键
#endif // TABLE_H
