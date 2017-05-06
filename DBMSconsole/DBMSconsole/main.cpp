//包含文件
#include <QCoreApplication>
#include <iostream>
#include <vector>
#include "convert.h"
#include "database.h"
#include "index.h"
#include "record.h"
#include "table.h"
#include "user.h"
#include "sqlanalysis.h"
//命名空间
using namespace  std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    User user;
    User reg;
    reg.username="root";//注册用的用户名
    reg.password="root";//注册用的密码

    switch(regUser(reg))//TODO:测试用
    {
    case 0:
        cout<<"Registion success!"<<endl;
        break;
    case 1:
        cout<<"Registion failed because this user exists!"<<endl;
        break;
    case 2:
        cout<<"Registion failed because account or password is not valid!"<<endl;//账号最多5位最少1位，密码最多10位最少1位，只能由字母（区分大小写）和数字组成
        break;
    case 3:
        cout<<"Registion failed because of system error!"<<endl;//一般是文件不存在
        break;
    default:
        break;
    }

    while(1)
    {
        cout<<"Please input account:";//默认账号是root
        string u;
        cin>>u;
        user.username = QString::fromStdString(u);
        getchar();
        cout<<"Please input password:";//默认密码是root
        user.password = EnterPassword();
        if(JudgeUser(user))
        {
            cout<<"Login success!"<<endl;

            QString curuser = user.username;
            QString curDBname = "";
			while(1)
			{
                cout<<"sql>>";
				int sqlType;//sql命令的类型，包括创建数据库、创建表、添加字段等正确操作，之后可扩展添加语法错误检查
                string sql;//输入的sql命令
                vector<QString> sqlkey;
				
                getline(cin,sql);//TODO:暂时没考虑换行的问题，即一句SQL语句是在一行中写完的，以分号结尾
                QString qsql = stringtoqstring(sql);
                sqlType = sqlAnalysis(qsql,sqlkey);
                switch(sqlType)
				{
                case 0://创建数据库
                    if(createDBEntity(curuser,sqlkey.at(0))==1)
                        cout<<"Create database successfully!"<<endl;
                    else
                    {

                    }
                    break;
                case 1://重命名数据库
                    if(renameDBEntity(curuser,sqlkey.at(0),sqlkey.at(1))==1)
                        cout<<"Rename database successfully!"<<endl;
                    else
                    {

                    }
                    break;
                case 2://删除数据库
                    if(dropDBEntity(curuser,sqlkey.at(0))==1)
                        cout<<"Drop database successfully!"<<endl;
                    else
                    {

                    }
                    break;
                case 3://使用数据库
                    if(useDBEntity(curuser,sqlkey.at(0))==1)
                    {
                        cout<<"Database changed."<<endl;
                        curDBname = sqlkey.at(0);
                    }
                    else
                    {

                    }
                    break;
                case -1:
                    cout<<"syntax error!"<<endl;
                    break;
				default:
					break;
				}
			}
        }
        else
        {
            cout<<"Acount or passeord is not valid,please reenter..."<<endl;
        }
    }

    return a.exec();
}
