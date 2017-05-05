//包含文件
#include <QCoreApplication>
#include <iostream>
#include <string>
#include <conio.h>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <vector>
#include <set>
#include <windows.h>
#include <map>
//宏定义
#define UNLEN 5//用户名最大长度
#define PWDLEN 10//密码最大长度
//命名空间
using namespace  std;
//结构体
typedef struct
{
    QString username;
    QString password;
}User;
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
    int type;//0代表是连续型范围，存在最大和最小值；1代表是离散型范围，用集合表示，值从集合的元素中选择
    QString max;//最大值
    QString min;//最小值
    set<QString> valueset;//取值的集合

    //defalut需要
    QString defaultvalue;//默认值
}Constraint;
//全局变量
int a[] = {4, 5, 3, 2, 4, 5, 1};//加密用整型数组
//辅助功能函数
/*类型转换函数*/
string inttostring(int tempint);
string doubletostring(double d);
string floattostring(float f);
string qstringtostring(QString qs);
string booltostring(bool b);

QString inttoqstring(int i);
QString doubletoqstring(double d);
QString floattoqstring(float f);
QString booltoqstring(bool b);

int qstringtoint(QString qs);
float qstringtofloat(QString qs);
double qstringtodouble(QString qs);
bool qstringtobool(QString qs);

int stringtoint(string s);
float stringtofloat(string s);
double stringtodouble(string s);
QString stringtoqstring(string s);
bool stringtobool(string s);
//功能函数
/*用户管理模块*/
QString EnterPassword();//输入密码，返回值为输入的密码
bool JudgeUser(User user);//判断用户名和密码是否存在、是否匹配
void encryption(string& c, int a[]);//对写入文件的密码进行加密操作
void decode(string& c,int a[]);//解密
int regUser(User user);//注册用户
/*SQL语句分析*/
int sqlAnalysis(string sql,vector<string> &sqlkey);//解析sql语法并返回各部分内容
/*数据库管理模块*/
void showdatabases(QString user);//在屏幕上打印当前用户的所有数据库名
int createDBEntity(QString user,QString DBname);//创建数据库实体
int renameDBEntity(QString user,QString newName,QString oldName);//重命名数据库实体
int dropDBEntity(QString user,QString DBname);//删除数据库实体
int useDBEntity(QString user,QString DBname);//打开并使用数据库
int initDBEntity(QString user,QString DBname);//初始化数据库
int backupDBEntity(QString user,QString DBname);//备份数据库
/*表管理模块*/
void showtables(QString user,QString DBname);//在屏幕上打印当前用户当前数据库下的所有表名
int createTable(QString user,QString DBname,QString TBname,vector<Field> fields,vector<Constraint> constraints);//创建表
int dropTable(QString user,QString DBname,QString TBname);//删除表
int deleteField(QString user,QString DBname,QString TBname,QString col);//删除表中一列
int addField(QString user,QString DBname,QString TBname,Field col,bool notnull);//增加一列，只允许添加非空约束条件，true代表添加，false代表不添加
int modifyCol(QString user,QString DBname,QString oldname,QString newname,QString type,bool notnull);//修改一列的名称及类型，只允许添加非空约束条件，true代表添加，false代表不添加
int renameTable(QString user,QString DBname,QString oldname,QString newname);//重命名表
int removePri(QString user,QString DBname,QString TBname);//删除主键
int addPri(QString user,QString DBname,QString TBname,Constraint primarykey);//为一到多列添加主键
/*记录管理模块*/
int insertRecord(QString user,QString DBname,QString TBname,vector<map<QString,QString>> records);//插入记录
int updateRecord(QString user,QString DBname,QString TBname,vector<map<QString,QString>> colnames);//更新记录
int selectRecord(QString user,QString DBname,vector<QString> TBnames,vector<QString> colname,vector<int> rdno);//查询记录，传入参数为列名和记录的序号
int deleteRecord(QString user,QString DBname,QString TBname,QString TBname,int rdno);//删除记录，传入参数为记录的序号
vector<int> analyzeWhere(QString condition);//传入参数为where筛选条件，返回值为记录序号的集合
vector<int> analyzeGroupby(QString condition);//传入参数为groupby筛选条件，返回值为记录序号的集合
/*索引管理模块*/
int createIndex(QString user,QString DBname,QString TBname,QString indexname,vector<QString> colnames);//建立索引，传入参数为建立索引的列名
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    User user;
    User reg;
    reg.username="";//注册用的用户名
    reg.password="";//注册用的密码

//     switch(regUser(reg))//TODO:测试用
//     {
//     case 0:
//         cout<<"Registion success!"<<endl;
//         break;
//     case 1:
//         cout<<"Registion failed because this user exists"<<endl;
//         break;
//     case 2:
//         cout<<"Registion failed because account or password is not valid"<<endl;//账号最多5位最少1位，密码最多10位最少1位，只能由字母（区分大小写）和数字组成
//         break;
//     case 3:
//         cout<<"Registion failed because of system error"<<endl;//一般是文件不存在
//         break;
//     default:
//         break;
//     }

    while(1)
    {
        cout<<"Please input account";//默认账号是root
        string u;
        cin>>u;
	user.username = QString::fromStdString(u);
        getchar();
        cout<<"Please input password";//默认密码是root
        user.password = EnterPassword();
        if(JudgeUser(user))
        {
            cout<<"Logon success"<<endl;
	    cout<<"sql>>";
	    QString curuser = user.username;
            QString curDBname = "";
			while(1)
			{
				int sqlType;//sql命令的类型，包括创建数据库、创建表、添加字段等正确操作，之后可扩展添加语法错误检查
				string sql;//输入的sql命令
				vector<string> sqlkey;
				
				getline(cin,sql);//TODO:暂时没考虑换行的问题，即一句SQL语句是在一行中写完的，以分号结尾
                sqlType = sqlAnalysis(sql,sqlkey);
                switch(sqlType)
				{
                case 0: //显示全部数据库
			showdatabases(curuser);
			break;
		case 1:
			break;
		default:
			break;
		}
			}
        }
        else
        {
            cout<<"Acount or passeord is not valid,please reenter"<<endl;
        }
    }

    return a.exec();
}

QString EnterPassword()
{
    char password[100];
    int index=0;
    while(1)
    {
        char ch;
        ch=getch();
        if(ch==8) //退格键
        {
            if(index!=0)
            {
                cout<<char(8)<<" "<<char(8);
                index--;
            }
        }
        else if(ch=='\r') //回车键
        {
            password[index]='\0';
            cout<<endl;
            break;
        }
        else if((ch<='Z'&&ch>='A')||(ch<='z'&&ch>='a')||(ch<='9'&&ch>='0'))//密码只支持英文字母（大小写均可）和数字
        {
            cout<<"*";
            password[index++]=ch;
        }
    }
    string p(password);
    QString qp=QString::fromStdString(p);
    return qp;
}

bool JudgeUser(User user)
{
    int usernum = 0;
    QString u,p;
    // 指定文件：
    QFile inputFile1("./DBMS/user.txt");
    // 只读打开：
    if(!inputFile1.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        cout<<"打开user.txt文件失败！"<<endl;
        return false;
    }
    QTextStream in1(&inputFile1);
    usernum = in1.readLine().toInt();
    for(int i=0;i<usernum;i++)
    {
        u = in1.readLine();
        p = in1.readLine();
        string u1,p1;
        u1 = u.toStdString();
        p1 = p.toStdString();
        decode(p1,a);
        u=stringtoqstring(u1);
        p=stringtoqstring(p1);
        if(u==user.username&&p==user.password)
            return true;
    }
    inputFile1.close();
    return false;
}

void encryption(string& c, int a[]){

    for(int i = 0, j = 0; c[j];j++, i = (i + 1) % 7){

        c[j]+=a[i];

        c[j] %= 90;
    }
}

void decode(string& c,int a[]){

    for(int i = 0, j = 0; c[j];j++, i = (i + 1) % 7){

        c[j]-=a[i];

        if(c[j]<48)
        {
            c[j]+=90;
        }
    }
}

int regUser(User user)
{
    if(username.size()>(unsigned)UNLEN||password.size()>(unsigned)PWDLEN) return 2;
    for(int i=0;(unsigned)i<username.size();i++)
    {
        if(!((username[i]<='z'&&username[i]>='a')||(username[i]<='Z'&&username[i]>='A')||(username[i]<='9'&&username[i]>='0'))) return 2;
    }
    for(int i=0;(unsigned)i<password.size();i++)
    {
        if(!((password[i]<='z'&&password[i]>='a')||(password[i]<='Z'&&password[i]>='A')||(password[i]<='9'&&password[i]>='0'))) return 2;
    }
    int usernum = 0;
    // 指定文件：
    QFile f1("./DBMS/user.txt");
    if(!f1.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        cout<<"打开user.txt文件失败！"<<endl;
        return 3;
    }
    QTextStream in1(&f1);
    usernum = in1.readLine().toInt();
    for(int i=0;i<usernum;i++)
    {
        QString u,p;
        u = in1.readLine();
        p = in1.readLine();
        if(u==user.username)
            return 1;
    }
    f1.close();
    f1.open(QIODevice::WriteOnly|QIODevice::Text);
    usernum++;
    QString qusernum = QString::number(usernum,10);
    qts2<<qusernum;
    f1.flush();
    f1.close();
    f1.open(QIODevice::Append|QIODevice::Text);
    QTextStream qts2(&f1);
    string p1 = user.password.toStdString();
    encryption(p1,a);
    QString p = QString::fromStdString(p1);
    qts2<<user.username<<endl;
    qts2<<p<<endl;
    f1.flush();
    f1.close();

    QDir *temp = new QDir;
    QString folder = "./DBMS/";
    folder += user.username;
    bool exist = temp->exists(folder);
    if(!exist)
    {
        bool ok = temp->mkdir(folder);
        if(ok) return 0;
        else return 3;
    }
    QFile f2(folder+"/database.txt");
    f2.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream qts4(&f2);
    qts4<<QString::number(0,10)<<endl;
    f2.flush();
    f2.close();
}

int sqlAnalysis(string sql,vector<string> &sqlkey)
{
	
}

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

int useDBEntity(QString user,QString DBname)//打开并使用数据库
{

    return 1;
}

int initDBEntity(QString user,QString DBname)//初始化数据库
{
    if(dropDBEntity(user,DBname)!=1) return -1;//删除失败
    if(createDBEntity(user,name)!=1) return -2;//创建失败
    return 1;
}

int backupDBEntity(QString user,QString DBname)//备份数据库
{

    return 1;
}

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

int insertRecord(QString user,QString DBname,QString TBname,vector<map<QString,QString>> records)//插入记录
{
    return 1;
}

int updateRecord(QString user,QString DBname,QString TBname,vector<map<QString,QString>> colnames)//更新记录
{
    return 1;
}

int selectRecord(QString user,QString DBname,vector<QString> TBnames,vector<QString> colname,vector<int> rdno)//查询记录，传入参数为列名和记录的序号
{
    return 1;
}

int deleteRecord(QString user,QString DBname,QString TBname,QString TBname,int rdno)//删除记录，传入参数为记录的序号
{
    return 1;
}

vector<int> analyzeWhere(QString condition)//传入参数为where筛选条件，返回值为记录序号的集合
{

}

vector<int> analyzeGroupby(QString condition)//传入参数为groupby筛选条件，返回值为记录序号的集合
{

}

int createIndex(QString user,QString DBname,QString TBname,QString indexname,vector<QString> colnames)//建立索引，传入参数为建立索引的列名
{
    return 1;
}

string inttostring(int tempint){
    char t[256];
    string s;
    sprintf(t, "%d", tempint);
    s = t;
    return s;
}

string doubletostring(double d)
{
    char t[256];
    string s;
    sprintf(t, "%lf", d);
    s = t;
    return s;
}

string floattostring(float f)
{
    char t[256];
    string s;
    sprintf(t, "%f", f);
    s = t;
    return s;
}

string qstringtostring(QString qs)
{
    return qs.toStdString();
}

string booltostring(bool b)
{
    if(b) return "T";
    else return "F";
}

int stringtoint(string s)
{
    return stoi(s);
}

float stringtofloat(string s)
{
    return stof(s);
}

double stringtodouble(string s)
{
    return stod(s);
}

QString stringtoqstring(string s)
{
    return QString::fromStdString(s);
}

bool stringtobool(string s)
{
    if(s=="T") return true;
    else return false;
}

QString inttoqstring(int i)
{
    return stringtoqstring(inttostring(i));
}

QString doubletoqstring(double d)
{
    return stringtoqstring(doubletostring(d));
}

QString floattoqstring(float f)
{
    return stringtoqstring(floattostring(f));
}

QString booltoqstring(bool b)
{
    return stringtoqstring(booltostring(b));
}

int qstringtoint(QString qs)
{
    return stringtoint(qstringtostring(qs));
}

float qstringtofloat(QString qs)
{
    return stringtofloat(qstringtostring(qs));
}

double qstringtodouble(QString qs)
{
    return stringtodouble(qstringtostring(qs));
}

bool qstringtobool(QString qs)
{
    return stringtobool((qstringtostring(qs)));
}
