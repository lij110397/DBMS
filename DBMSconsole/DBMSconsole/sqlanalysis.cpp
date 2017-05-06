#include "sqlanalysis.h"
int sqlAnalysis(QString sql,vector<QString> &sqlkey)
{
    vector<QString> regVector;          //存放正则表达式
    vector<int> regSize;                //对应的正则表达式要获取几个值，如create需要获取database名

    regVector.push_back(QString("create(?:\\s*)database(?:\\s*)(\\b[a-z0-9_]+\\b)(?:\\s*);"));      //匹配创建数据库的正则
    regSize.push_back(1);
    regVector.push_back(QString("rename(?:\\s*)database(?:\\s*)(\\b[a-z0-9_]+\\b)(?:\\s*)to(?:\\s*)(\\b[a-z0-9_]+\\b)(?:\\s*);"));     //匹配重命名数据库的正则
    regSize.push_back(2);
    regVector.push_back(QString("drop(?:\\s*)database(?:\\s*)(\\b[a-z0-9_]+\\b)(?:\\s*);"));        //匹配删除数据库的正则
    regSize.push_back(1);
    regVector.push_back(QString("use(?:\\s*)database(?:\\s*)(\\b[a-z0-9_]+\\b)(?:\\s*);"));         //匹配使用数据库的正则
    regSize.push_back(1);

    //开始解析sql语句
    for (unsigned int i = 0; i < regVector.size(); i++)
    {
        QRegExp reg(regVector[i]);
        int pos = reg.indexIn(sql);       //匹配的第一个字符的位置，从0开始，若为-1则不匹配

        if (pos >= 0)                      //若匹配
        {
            for (int j = 0; j < regSize[i]; j++)
            {
                sqlkey.push_back(reg.cap(j + 1));   //将对应的值放入sqlkey
            }
            return i;                    //返回数据库操作对应的编号，从0开始
        }
    }

    return -1;                              //没有对应的操作，返回-1，表示该类型操作暂未支持，或者sql语句错误
}
