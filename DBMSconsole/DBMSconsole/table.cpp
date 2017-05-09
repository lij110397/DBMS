#include "table.h"
//删除文件夹中的所有文件
void clearFiles(const QString &folderFullPath)
{
QDir dir(folderFullPath);
dir.setFilter(QDir::Files);
int fileCount = dir.count();
for (int i = 0; i < fileCount; i++)
dir.remove(dir[i]);
}



//显示所有表名
void showtables(QString user,QString DBname)//在屏幕上打印当前用户当前数据库下的所有表名
{
    int dbnum;
    QString u,p;

    // 打开指定文件database.txt。其中存储有所有数据库信息和数据库中的表的信息
    string Filename,s;
    s=qstringtostring(user);                          //用户名
    Filename="./DBMS/"+s+"/database.txt";             //找到文件所在位置
    QFile inputFile1(stringtoqstring(Filename));
    // 只读打开：
    if(!inputFile1.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        cout<<"打开database.txt文件失败！"<<endl;
    }

    //读取第一行，获取当前的数据库的数量
    QTextStream in1(&inputFile1);
    dbnum = in1.readLine().toInt();
    QStringList  v;
    for(int i=0;i<dbnum;i++)
    {
        //逐行查找
        u = in1.readLine();                            //数据库信息行
        string u1;
        u1 = u.toStdString();
        u=stringtoqstring(u1);

        //进行解析，从#分开
        v=u.split('#');                                //通过#分隔开
        i++;
        if(v[0]==DBname)                               //找到相应的数据库
          break;
    }

    //读取表的信息
    string p1;
    p =v[3];                                           //表信息字符串
    p1 = p.toStdString();
    p=stringtoqstring(p1);

    //解析表信息
    QStringList v2;
    v2=p.split('&');                                     //表名通过&分隔开
    for(vector<string>::size_type i = 0; i != v2.size(); ++i)
        cout << qstringtostring(v2[i])<<endl;

    inputFile1.close();
}

//创建表
int createTable(QString user,QString DBname,QString TBname,vector<Field> fields,vector<Constraint> constraints)//创建表
{
//往database.txt中写入表的信息
    int dbnum,tablenum;
    QString u,p;
    string outFilename;
    string inFilename,s;

    //读文件
    // 打开指定文件database.txt
    s=qstringtostring(user);
    inFilename="./DBMS/"+s+"/database.txt";
    QFile inputFile1(stringtoqstring(inFilename));
    // 只读打开：
    if(!inputFile1.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        cout<<"打开database.txt文件失败！"<<endl;
        return 0;
    }
    QTextStream in1(&inputFile1);

    //写文件
    // 建立临时文件1.txt
    outFilename="./DBMS/"+s+"/1.txt";
    QFile outFile1(stringtoqstring(outFilename));
    //
    if(!outFile1.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        cout<<"open file failed"<<endl;
        return 0;
     //   qDebug() << "open file failed";
    }
    QTextStream out1(&outFile1);


    //读取database.txt的第一行
    dbnum = in1.readLine().toInt();                        //数据库的数量
    out1<<dbnum<<endl;

    //找到对应的DBname，除了被修改行之前的全都被写入
    int i;
    QStringList v;
    for(i=0;i<dbnum;i++)
    {
        u = in1.readLine();
        string u1;
        u1 = u.toStdString();
        u=stringtoqstring(u1);

        //进行解析，从#分开

        v=u.split('#');
        if(v[0]==DBname)                                  //找到相应的数据库
            break;
        out1<<u<<endl;                   //如果不是要找的那行之前的都直接写入
    }

    //更新表数量
    tablenum=v[2].toInt();                                //表数量
    string n;
    n.append(qstringtostring(v[0]));n.append("#");
    n.append(qstringtostring(v[1]));n.append("#");
    n.append(inttostring(tablenum+1));n.append("#");//表名信息前的信息

    //在该行最后加上表名
    string tn;
    tn.append(qstringtostring(v[3]));tn.append("&");
    tn.append(qstringtostring(TBname));
    n.append(tn);

    //写入修改后的行信息
    out1<<stringtoqstring(n)<<endl;

    //写入剩下的信息
    for(int h=i+1;h<dbnum;h++)
    {
        u = in1.readLine();
        string u1;
        u1 = u.toStdString();

        out1<<u<<endl;                                   //如果不是要找的那行之前的都直接写入
    }
    out1.flush();

    //重写入
    outFile1.setFileName(stringtoqstring(inFilename));
    inputFile1.close();
    outFile1.close();



//往tdf.txt中加入字段信息
    outFilename="./DBMS/"+s+"/"+qstringtostring(DBname)+"/"+qstringtostring(TBname)+"/"+"/tdf.txt";
    QFile outFile2(stringtoqstring(outFilename));
    if(!outFile2.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        cout<<"open file failed"<<endl;
        return 0;
    }
    QTextStream out2(&outFile2);

    //逐个逐行写入字段信息
    //第一行记录当前表中的字段数量
    out2<<fields.size()<<endl;
    //第二行起每行都记录一个字段的信息，不同信息间用#分割，信息包括字段名、类型
    for(int i=0;i<fields.size();i++)
        out2<<fields[i].name<<"#"<<fields[i].type<<endl;
    out2.flush();
    outFile2.close();


//往tic.txt中加入约束条件
    outFilename="./DBMS/"+s+"/"+qstringtostring(DBname)+"/"+qstringtostring(TBname)+"/"+"/tic.txt";
    QFile outFile3(stringtoqstring(outFilename));
    if(!outFile3.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        cout<<"open file failed"<<endl;
        return 0;
    }
    QTextStream out3(&outFile3);

    //第一行中记录当前表中的约束条件的数量
    out3<<constraints.size()<<endl;
    //第二行起每行都记录一个约束条件的信息
    //格式：约束条件名#约束条件类型的整型编号#约束条件作用的列（多列用&分隔）#约束作用参考的表名（外键才有）#约束条件参考的列名（外键才有）
   for(int i=0;i<constraints.size();i++){
       //约束条件名及约束条件类型的整型编号
       out3<<constraints[i].name<<"#"<<constraints[i].type;
       //约束条件作用行
       vector<QString> c=constraints[i].colnos;
       for(int h=0;i<c.size();i++){
           if(h==0)//只有一个
               out3<<"#"<<c[h];
           else if(h>0){//有多列用&分隔
               out3<<"&"<<c[h];
           }
       }

       //一些特殊的约束条件的写法
       //如果有外键的话
       if(constraints[i].type==1)
           out3<<"#"<<constraints[i].outertable<<"#"<<constraints[i].outerprino;
       //如果是check条件的话
       else if(constraints[i].type==4){
        out3<<"#"<<constraints[i].checktype;
              if(constraints[i].checktype==0)//连续型范围
                  out3<<"#"<<constraints[i].min<<" "<<constraints[i].max;
              else if(constraints[i].checktype==1){//离散型范围
                   out3<<"#";
                   //离散型的取值集合
                   set<QString> a=constraints[i].valueset;
                   for(set<QString>::iterator setai=a.begin();setai!=a.end();setai++)
                           out3<<*setai<<" ";
              }
       }
       else if(constraints[i].type==5)
           out3<<"#"<<constraints[i].defaultvalue;

       out3.flush();
   }
   outFile3.close();
    return 1;
}

//删除表
int dropTable(QString user,QString DBname,QString TBname)//删除表
{
//删除database.txt里面的表名
    int dbnum,tablenum;
    QString u,p;
    string outFilename;
    string Filename,s;

    //读文件
    // 打开指定文件database.txt
    s=qstringtostring(user);
    Filename="./DBMS/"+s+"/database.txt";
    QFile inputFile1(stringtoqstring(Filename));
    // 只读打开：
    if(!inputFile1.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        cout<<"打开database.txt文件失败！"<<endl;
        return 0;
    }
    QTextStream in1(&inputFile1);

    //写文件
    // 建立临时文件2.txt
    outFilename="./DBMS/"+s+"/2.txt";
    QFile outFile1(stringtoqstring(outFilename));
    //
    if(!outFile1.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        cout<<"open file failed"<<endl;
        return 0;
    }
    QTextStream out1(&outFile1);

    //读取database.txt的第一行
    dbnum = in1.readLine().toInt();
    out1<<dbnum<<endl;

    //找到对应的DBname，除了被修改行之前的全都被写入
    int i;
    QStringList v;
    for(i=0;i<dbnum;i++)
    {
        u = in1.readLine();
        string u1;
        u1 = u.toStdString();
        u=stringtoqstring(u1);

        //进行解析，从#分开

        v=u.split('#');

        if(v[0]==DBname)                                  //找到相应的数据库
            break;
        out1<<u<<endl;                                   //如果不是要找的那行之前的都直接写入
    }

    //找到相应的行并更新表数量
    tablenum=v[2].toInt();
    string n;
    n.append(qstringtostring(v[0]));n.append("#");
    n.append(qstringtostring(v[1]));n.append("#");
    n.append(inttostring(tablenum-1));n.append("#");//表名信息前的信息

    //去掉要删除的表名
    QStringList t;
    t=v[3].split('&');
    string tl;

    int h,find=0;                                         //检查第一个是否是我们要找的表名
    if(t[0]!=TBname)                     //第一个表名不是我们要找的，直接写入
        tl.append(qstringtostring(t[0]));
                                                          //第一个表名就是我们要找的，不写入
    for(h=1;h<t.size();h++){
        if(t[h]==TBname){                //检查是否是我们要找的表名，找到就不再写入了
            find=1;                                       //判断是否找到了
        }else{
            if(t[0]==TBname&&h==1)
                tl.append(qstringtostring(t[h]));
            else {
                tl.append("&");
                tl.append(qstringtostring(t[h]));
            }
        }

    }

    if(find==0)return 0;                                   //没有找到的话返回0
    n.append(tl);
    out1<<stringtoqstring(n)<<endl;

    //写入剩下的信息
    for(h=i+1;h<dbnum;h++)
    {
        u = in1.readLine();
        string u1;
        u1 = u.toStdString();

        out1<<u<<endl;                                   //如果不是要找的那行之前的都直接写入
    }
    out1.flush();
    inputFile1.close();

    //重写入
    outFile1.setFileName(stringtoqstring(Filename));
    outFile1.close();

    //删除相应的表的文件夹
    string folderFullPath;
    folderFullPath="./DBMS/"+s+"/"+qstringtostring(DBname)+"/"+qstringtostring(TBname);
    clearFiles(stringtoqstring(folderFullPath));
    //?这个路径是正确的吗？
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

//重命名表
int renameTable(QString user,QString DBname,QString oldname,QString newname)//重命名表
{
//修改database.txt里面表名
        int dbnum,tablenum;
        QString u,p;
        string outFilename;
        string Filename,s;

        //读文件
        // 打开指定文件database.txt
        s=qstringtostring(user);
        Filename="./DBMS/"+s+"/database.txt";
        QFile inputFile1(stringtoqstring(Filename));
        // 只读打开：
        if(!inputFile1.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            cout<<"打开database.txt文件失败！"<<endl;
            return 0;
        }

        //写文件
        // 建立临时文件2.txt
        outFilename="./DBMS/"+s+"/2.txt";
        QFile outFile1(stringtoqstring(outFilename));
        //
        if(!outFile1.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            cout<<"open file failed"<<endl;
            return 0;
         //   qDebug() << "open file failed";
        }
        QTextStream out1(&outFile1);

        //读取database.txt的第一行
        QTextStream in1(&inputFile1);
        dbnum = in1.readLine().toInt();
        out1<<dbnum<<endl;

        //找到对应的DBname，除了被修改行之前的全都被写入
        int i;
        QStringList v;
        for(i=0;i<dbnum;i++)
        {
            u = in1.readLine();
            string u1;
            u1 = u.toStdString();
            u=stringtoqstring(u1);

            //进行解析，从#分开
            v=u.split('#');

            if(v[0]==DBname)                                  //找到相应的数据库
                break;
            out1<<u<<endl;                                   //如果不是要找的那行之前的都直接写入
        }

        string n;
        n.append(qstringtostring(v[0]));n.append("#");
        n.append(qstringtostring(v[1]));n.append("#");
        n.append(qstringtostring(v[2]));n.append("#");//表名信息前的信息

        //找到相应的表名，然后进行替换
        QStringList t;
        t=v[3].split('&');


        int h,find=0;
        string tl;
        if(t[h]!=oldname)                     //检查第一个是否是我们要找的表名
            tl.append(qstringtostring(t[0]));
        else{                                                  //第一个就是我们要找的表名
            tl.append(qstringtostring(newname));
        }
        for(h=1;h<t.size();h++){
            if(t[h]==oldname){                //检查是否是我们要找的表名
                find=1;                                        //找到后用新名代替
                tl.append("&");
                tl.append(qstringtostring(newname));
            }else{
            tl.append("&");
            tl.append(qstringtostring(t[h]));
            }
        }
        if(find==0)return 0;

        out1<<stringtoqstring(tl)<<endl;

        //写入剩下的信息
        u = in1.readLine();
        string u1;
        u1 = u.toStdString();
        while (!u.isNull()){
            out1<<u<<endl;                                //如果不是要找的那行之前的都直接写入
            u = in1.readLine();
            string u1;
            u1 = u.toStdString();
        }
        out1.flush();
        inputFile1.close();

        //重写入
        outFile1.setFileName(stringtoqstring(Filename));
        outFile1.close();


//修改目录下文件名
        string filenamet;
        filenamet="./DBMS/"+s+"/"+qstringtostring(DBname);
        QDir dir(stringtoqstring(filenamet));
        if(!dir.rename(oldname,newname))
            return 0;

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
