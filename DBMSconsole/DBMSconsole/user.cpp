#include "user.h"
#include <iostream>
#include "convert.h"
//全局变量
int a[] = {4, 5, 3, 2, 4, 5, 1};//加密用整型数组
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
        cout<<"open file user.txt failed!"<<endl;
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
    vector<QString> lines;
    if(user.username.size()>UNLEN||user.password.size()>PWDLEN) return 2;
    for(int i=0;i<user.username.size();i++)
    {
        if(!((user.username[i]<='z'&&user.username[i]>='a')||(user.username[i]<='Z'&&user.username[i]>='A')||(user.username[i]<='9'&&user.username[i]>='0'))) return 2;
    }
    for(int i=0;i<user.password.size();i++)
    {
        if(!((user.password[i]<='z'&&user.password[i]>='a')||(user.password[i]<='Z'&&user.password[i]>='A')||(user.password[i]<='9'&&user.password[i]>='0'))) return 2;
    }
    int usernum = 0;
    // 指定文件：
    QFile f1("./DBMS/user.txt");
    if(!f1.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        cout<<"open file user.txt failed!"<<endl;
        return 3;
    }
    QTextStream in1(&f1);
    usernum = in1.readLine().toInt();
    for(int i=0;i<usernum;i++)
    {
        QString u,p;
        u = in1.readLine();
        lines.push_back(u);
        p = in1.readLine();
        lines.push_back(p);
        if(u==user.username)
            return 1;
    }
    f1.close();
    f1.open(QIODevice::WriteOnly|QIODevice::Text);
    usernum++;
    QString qusernum = QString::number(usernum,10);
    QTextStream qts1(&f1);
    qts1<<qusernum<<endl;
    for (vector<QString>::iterator iter=lines.begin();iter!=lines.end();iter++)
    {
        qts1<<(*iter)<<endl;
    }
    string p1 = user.password.toStdString();
    encryption(p1,a);
    QString p = QString::fromStdString(p1);
    qts1<<user.username<<endl;
    qts1<<p<<endl;
    f1.flush();
    f1.close();

    QDir *temp = new QDir;
    QString folder = "./DBMS/";
    folder += user.username;
    bool exist = temp->exists(folder);
    if(!exist)
    {
        bool ok = temp->mkdir(folder);
        if(ok)
        {
            QFile f2(folder+"/database.txt");
            f2.open(QIODevice::WriteOnly|QIODevice::Text);
            QTextStream qts4(&f2);
            qts4<<QString::number(0,10)<<endl;
            f2.flush();
            f2.close();
            return 0;
        }
        else return 3;
    }
    return 0;
}
