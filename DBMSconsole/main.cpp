#include <QCoreApplication>
#include <iostream>
#include <string>
#include <conio.h>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QDir>
#define UNLEN 5//用户名最大长度
#define PWDLEN 10//密码最大长度
using namespace  std;
int a[] = {4, 5, 3, 2, 4, 5, 1};//加密用整型数组
string EnterPassword();//输入密码，返回值为输入的密码
bool JudgeUser(string username,string password);//判断用户名和密码是否存在、是否匹配
void encryption(string& c, int a[]);//对写入文件的密码进行加密操作
void decode(string& c,int a[]);//解密
int regUser(string username,string password);//注册用户
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    string username = "";
    string password;

    switch(regUser("zzz00","xyzzzzzz"))
    {
    case 0:
        cout<<"注册成功！"<<endl;
        break;
    case 1:
        cout<<"该用户名已经存在,注册失败！"<<endl;
        break;
    case 2:
        cout<<"用户名或者密码不符合要求，注册失败！"<<endl;//账号最多5位最少1位，密码最多10位最少1位，只能由字母（区分大小写）和数字组成
        break;
    case 3:
        cout<<"系统错误导致注册失败！"<<endl;//一般是文件不存在
        break;
    default:
        break;
    }

    while(1)
    {
        cout<<"请输入账号：";//默认账号是root
        cin>>username;
        getchar();
        cout<<"请输入密码：";//默认密码是root
        password = EnterPassword();
        if(JudgeUser(username,password))
        {
            cout<<"登录成功！"<<endl;
            break;
        }
        else
        {
            cout<<"账号或者密码输入错误，请重新输入..."<<endl;
        }
    }

    return a.exec();
}
string EnterPassword()
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
    return p;
}
bool JudgeUser(string username,string password)
{
    int usernum = 0;
    QString u,p;
    // 指定文件：
    QFile inputFile("D:/2qt projects/DBMSconsole/usernum.txt");
    // 只读打开：
    if(!inputFile.open(QIODevice::ReadOnly))
    {
        cout<<"打开usernum.txt文件失败！"<<endl;
        return false;
    }
    // 文本流：
    QTextStream in(&inputFile);
    // 将文本流读取到字符串中：
    QString line = in.readAll();
    // 关闭文本流：
    inputFile.close();
    usernum = line.toInt();
    QFile inputFile1("D:/2qt projects/DBMSconsole/user.txt");
    if(!inputFile1.open(QIODevice::ReadOnly))
    {
        cout<<"打开user.txt文件失败！"<<endl;
        return false;
    }
    QTextStream in1(&inputFile1);
    for(int i=0;i<usernum;i++)
    {
        u = in1.readLine();
        p = in1.readLine();
        string u1,p1;
        u1 = u.toStdString();
        p1 = p.toStdString();
        decode(p1,a);
        if(u1==username&&p1==password)
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
int regUser(string username,string password)
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
    QFile inputFile("D:/2qt projects/DBMSconsole/usernum.txt");
    // 只读打开：
    if(!inputFile.open(QIODevice::ReadOnly))
    {
        cout<<"打开usernum.txt文件失败！"<<endl;
        return 3;
    }
    // 文本流：
    QTextStream in(&inputFile);
    // 将文本流读取到字符串中：
    QString line = in.readAll();
    // 关闭文本流：
    inputFile.close();
    usernum = line.toInt();
    QFile inputFile1("D:/2qt projects/DBMSconsole/user.txt");
    if(!inputFile1.open(QIODevice::ReadOnly))
    {
        cout<<"打开user.txt文件失败！"<<endl;
        return 3;
    }
    QTextStream in1(&inputFile1);
    for(int i=0;i<usernum;i++)
    {
        QString u,p;
        u = in1.readLine();
        p = in1.readLine();
        string u1;
        u1 = u.toStdString();
        if(u1==username)
            return 1;
    }
    inputFile1.close();


    QFile f("D:/2qt projects/DBMSconsole/usernum.txt");
    QFile f1("D:/2qt projects/DBMSconsole/user.txt");

    f1.open(QIODevice::Append|QIODevice::Text);
    f.open(QIODevice::WriteOnly);
    QTextStream qts3(&f);
    usernum++;
    QString qusernum = QString::number(usernum,10);
    qts3<<qusernum;
    f.flush();
    f.close();
    QTextStream qts2(&f1);
    QString u = QString::fromStdString(username);
    encryption(password,a);
    QString p = QString::fromStdString(password);
    qts2<<u<<endl;
    qts2<<p<<endl;
    f1.flush();
    f1.close();

    QDir *temp = new QDir;
    QString folder = "D:/2qt projects/DBMSconsole/";
    folder += QString::fromStdString(username);
    bool exist = temp->exists(folder);
    if(!exist)
    {
        bool ok = temp->mkdir(folder);
        if(ok) return 0;
        else return 3;
    }
}
