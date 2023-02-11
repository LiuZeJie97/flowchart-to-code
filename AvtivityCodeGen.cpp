#include "AvtivityCodeGen.h"
#include <string>
#include <stack> 
#include <queue>
#include <map>
#include <iostream>
using namespace std;

#define OUTPUT_ONCE true //每个节点只被输出一次



string IntToString(const int n)
{
    return to_string(n);
}


void AvtivityCodeGen::AddKongGe(int n)
{
    for (int i = 0; i < n; i++)
    {
        std::cout << "    ";
    }
}
void AvtivityCodeGen::AddKongGe2(int n)
{
    for (int i = 0; i < n; i++)
    {
        sg_ofs << "    ";
    }
}

void AvtivityCodeGen::CodeGenCout(std::string f_str, GUID f_sGuid, std::string f_strType)
{
    if (f_sGuid != INVALID_GUID)
    {
        sg_aCodeGenCoutFullGUID.push_back(f_sGuid);
    }
    if (f_strType == "}" || f_strType == "{" || f_strType == "Return")
        return;
    else if(f_strType=="Effect" || f_strType == "For" || f_strType == "While" || f_strType == "If"){
        
    }
    else if (f_strType == "Continue")
    {
        f_str = "continue\n";
    }
    else if (f_strType == "Break")
    {
        f_str = "break\n";
    }
    else if (f_strType == "Else")
    {
        f_str = "else:\n";
    }
    else if (f_strType == "ElseIf")
    {
        f_str = "elif:\n";
    }
    else if (f_strType == "WHILE_TRUE")
    {
        f_str = "while true:\n";
    }
    else if (f_strType == "DO")
    {
        f_str = "while:\n";
    }
    else
    {
        std::cout << "f_strType error!\n"<< f_strType<<std::endl;
        system("pause");
    }
    sg_aCodeGenCoutStr.push_back(f_str);
    sg_aCodeGenCoutNSpace.push_back(sg_iKongGe);
    sg_aCodeGenCoutGUID.push_back(f_sGuid);
    sg_aCodeGenCoutType.push_back(f_strType);
    /*
    for (i = 0; i < strCleanString.length(); i++) {
        if (strCleanString[i] == '}')
            iTab--;
    }*/
    AddKongGe(sg_iKongGe);  
    std::cout << f_str;         /* 输出到控制台  */
    AddKongGe2(sg_iKongGe);
    sg_ofs  << f_str;         /* 输出到文件    */

    /*
    for (i = 0; i < strCleanString.length(); i++) {
        if (strCleanString[i] == '{')
            iTab++;
    }
    */
}

void AvtivityCodeGen::AddResult(int f_iErrorCode, std::vector<std::string> f_aGuid)
{
    for (ActivityDiagramResult res : aActivityDiagramResult)
    {
        if (f_iErrorCode == res.m_iErrorCode)
        {
            unsigned int i;
            for (i = 0; i < res.aGuid.size(); i++)
            {
                if (res.aGuid[i] != f_aGuid[i])
                {
                    break;
                }
            }
            if (i == res.aGuid.size())
            {
                return;
            }
        }
    }
    string stInf = "";
    ErrotType type;
    switch (f_iErrorCode) {
    case EVO_CODEGEN_WARNING_FEIJIEGOU:
        stInf += "非结构化活动图。";
        type = EN_EVO_CODEGEN_WARNING;
        break;
    case EVO_CODEGEN_WARNING_FINAL:
        stInf += "非终结点没有出边。";
        type = EN_EVO_CODEGEN_WARNING;
        break;
    case EVO_CODEGEN_WARNING_ASSOCIATION:
        stInf += "节点出边或入边数量不正确。";
        type = EN_EVO_CODEGEN_WARNING;
        break;
    case EVO_CODEGEN_WARNING_ASSOCIATION_2:
        stInf += "存在重复的边。";
        type = EN_EVO_CODEGEN_WARNING;
        break;
    case EVO_CODEGEN_WARNING_ELEMENT:
        stInf += "存在不支持的节点。";
        type = EN_EVO_CODEGEN_WARNING;
        break;
    case EVO_CODEGEN_WARNING_INTERNAL:
        stInf += "内部错误。";
        type = EN_EVO_CODEGEN_WARNING;
        break;
    case EVO_CODEGEN_ERROR_INITIAL:
        stInf += "没有找到INITIAL节点 或 diagram不是活动图。";
        type = EN_EVO_CODEGEN_ERROR;
        break;
    case EVO_CODEGEN_WARNING_IF:
        stInf += "分支结构汇点不一致。";
        type = EN_EVO_CODEGEN_WARNING;
        break;
    }
    switch (type) {
    case EN_EVO_CODEGEN_WARNING:
        stInf = "warning " + IntToString(f_iErrorCode) + ": " + stInf;
        break;
    case EN_EVO_CODEGEN_ERROR:
        stInf = "error " + IntToString(f_iErrorCode) + ": " + stInf;
        break;
    }
    switch (f_iErrorCode) {
    case EVO_CODEGEN_WARNING_ASSOCIATION_2:
        if (f_aGuid.size() >= 2)
        {
            ElementBasicInfo sInfo1;
            GetModelElementBasicInfo(StringToGuid(f_aGuid[0]), &sInfo1);
            ElementBasicInfo sInfo2;
            GetModelElementBasicInfo(StringToGuid(f_aGuid[1]), &sInfo2);
            stInf += "边 " + GuidToString(sInfo1.m_sGUID) + " 与边 " + GuidToString(sInfo2.m_sGUID) + " 重复，仅保留 " + GuidToString(sInfo2.m_sGUID);
        }
        break;
    default:
        for (std::string strGuid : f_aGuid)
        {
            stInf += "GUID: " + strGuid;
            ElementBasicInfo sSourceInfo;
            int res = GetModelElementBasicInfo(StringToGuid(strGuid), &sSourceInfo);
            if (NO_ERROR == res) {
                stInf += ", EnName: " + sSourceInfo.m_strElementEnName + "  ";
            }

        }
    }
    stInf += "。";
    ActivityDiagramResult sRes = { type, f_iErrorCode, stInf, f_aGuid };
    aActivityDiagramResult.push_back(sRes);
}

std::vector<Association> AvtivityCodeGen::GodeGenSortAssociation(std::vector<Association> ass)
{
    std::vector<Association> associationWithOutGuard, associationWithGuard, associationSorted;
    for (Association _ass : ass)
    {
        string guard = GetGuard(_ass.m_sBasicInfo.m_sGUID);
        if (guard != "")        {
            associationWithGuard.push_back(_ass);
        }
        else
        {
            associationWithOutGuard.push_back(_ass);
        }
    }
    for (Association ass : associationWithGuard)
    {
        associationSorted.push_back(ass);
    }
    for (Association ass : associationWithOutGuard)
    {
        associationSorted.push_back(ass);
    }
    return associationSorted;
}

std::vector<Association> AvtivityCodeGen::GodeGenGetModelOutGoingAssociation(GUID f_sModelGUID)
{
    std::vector<Association> asso = GetModelOutGoingAssociation(f_sModelGUID);
    std::vector<Association> _asso;
    unsigned int i, j, k;
    for (i = 0; i < asso.size(); i++) {
        /*
        for (k = 0; k < sg_aDeletModel.size(); k++)
        {
            if (asso[i].m_sBasicInfo.m_sGUID == sg_aDeletModel[k])
            {
                break;
            }
        }
        */
        for (j = 0; j < i; j++) {

            if (asso[i].m_sSourceGUID == asso[j].m_sSourceGUID
                && asso[i].m_sTargetGUID == asso[j].m_sTargetGUID)
            {
                std::vector<std::string> f_aGuid;
                f_aGuid.push_back(GuidToString(asso[i].m_sBasicInfo.m_sGUID));
                f_aGuid.push_back(GuidToString(asso[j].m_sBasicInfo.m_sGUID));
                AddResult(EVO_CODEGEN_WARNING_ASSOCIATION_2, f_aGuid);
                break;
            }
        }
        //if (j == i && k == sg_aDeletModel.size())
        if (j == i)
        {
            _asso.push_back(asso[i]);
        }
    }

    return GodeGenSortAssociation(_asso);
}

std::vector<Association> AvtivityCodeGen::CodeGenGetModelInComingAssociation(GUID f_sModelGUID)
{
    std::vector<Association> asso = GetModelInComingAssociation(f_sModelGUID);
    std::vector<Association> _asso;
    unsigned int i, j, k;
    for (i = 0; i < asso.size(); i++) {
        /*
        for (k = 0; k < sg_aDeletModel.size(); k++)
        {
            if (asso[i].m_sBasicInfo.m_sGUID == sg_aDeletModel[k])
            {
                break;
            }
        }*/

        for (j = 0; j < i; j++) {

            if (asso[i].m_sSourceGUID == asso[j].m_sSourceGUID
                && asso[i].m_sTargetGUID == asso[j].m_sTargetGUID)
            {
                std::vector<std::string> f_aGuid;
                f_aGuid.push_back(GuidToString(asso[i].m_sBasicInfo.m_sGUID));
                f_aGuid.push_back(GuidToString(asso[j].m_sBasicInfo.m_sGUID));
                AddResult(EVO_CODEGEN_WARNING_ASSOCIATION_2, f_aGuid);
                break;
            }
        }
        //if (j == i && k == sg_aDeletModel.size())
        if (j == i)
        {
            _asso.push_back(asso[i]);
        }
    }
    return _asso;
}

void AvtivityCodeGen::FindWhile(GUID f_sStartNode, std::map<std::string, int>& f_mapNodes, std::vector<std::map<std::string, int>>* f_mapWhileNodesList, std::map<std::string, int>& f_mapWhileEdges, std::vector<std::string>& f_aReturnNodesList)
{
    sg_mapVisit.clear();
    _FindWhile(f_sStartNode, f_mapNodes, f_mapWhileNodesList, f_mapWhileEdges, f_aReturnNodesList);
}


//找到循环并识别循环内的元素
void AvtivityCodeGen::_FindWhile(GUID f_sStartNode, std::map<std::string, int>& f_mapNodes, std::vector<std::map<std::string, int>>* f_mapWhileNodesList, std::map<std::string, int>& f_mapWhileEdges, std::vector<std::string>& f_aReturnNodesList)
{
    /*
    ElementBasicInfo elementInfo;
    int res = MyGetModelElementBasicInfo(f_sStartNode, &elementInfo);
    EVO_CODEGEN_MYCOUT<< res << elementInfo.m_strElementName << std::endl;
    */
    //EVO_CODEGEN_MYCOUT << GuidToString(f_sStartNode) << std::endl;

    //sg_mapDFN[sU] = sg_mapLow[sU] = ++sg_iIndex                      // 为节点sU设定次序编号和sg_mapLow初值
    sg_mapDFN[GuidToString(f_sStartNode)] = sg_mapLow[GuidToString(f_sStartNode)] = ++sg_iIndex;
    GUID sU = f_sStartNode;
    sg_stack.push(sU);                            // 将节点sU压入栈中
    sg_stack_map[GuidToString(sU)] = 1;
    sg_mapVisit[GuidToString(sU)] = 1;
    //for each (sU, sV) in E                       // 枚举每一条边
    std::vector<Association> associationList = GodeGenGetModelOutGoingAssociation(f_sStartNode);
    for (Association association : associationList)
    {
        if ((1 == f_mapWhileEdges.count(GuidToString(association.m_sBasicInfo.m_sGUID))) && (f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] >= EVO_CODEGEN_HUIBIAN))continue;
        GUID sV = association.m_sTargetGUID;
        if ((0 == sg_mapVisit.count(GuidToString(sV))) && (0 == f_mapNodes.size() || 1 == f_mapNodes.count(GuidToString(sV))) && (!OUTPUT_ONCE || sg_mapDelete.count(GuidToString(f_sStartNode)) == 0))
        {              // 如果节点sV未被访问过
            _FindWhile(sV, f_mapNodes, f_mapWhileNodesList, f_mapWhileEdges, f_aReturnNodesList);                 // 继续向下找
            sg_mapLow[GuidToString(sU)] = min(sg_mapLow[GuidToString(sU)], sg_mapLow[GuidToString(sV)]);
        }
        else if (sg_stack_map.count(GuidToString(sV)))
        {                  // 如果节点sV还在栈内
            sg_mapLow[GuidToString(sU)] = min(sg_mapLow[GuidToString(sU)], sg_mapDFN[GuidToString(sV)]);
        }
    }
    if (sg_mapDFN[GuidToString(sU)] == sg_mapLow[GuidToString(sU)])
    {                      // 如果节点sU是强连通分量的根
        GUID sV;
        std::map<std::string, int> f_mapWhileNodes;
        do
        {
            sV = sg_stack.top();                 // 将sV退栈，为该强连通分量中一个顶点
            sg_stack.pop();
            sg_stack_map.erase(GuidToString(sV));
            f_mapWhileNodes[GuidToString(sV)] = 1;

        } while (sU != sV);
        if (f_mapWhileNodes.size() > 1)
        {
            GUID f_sHuiDian;
            FindWhileDoWhileCBR(sU, f_mapWhileNodes, f_mapWhileEdges, f_sHuiDian);
            f_mapWhileNodesList->push_back(f_mapWhileNodes);
            f_aReturnNodesList.push_back(GuidToString(f_sHuiDian));
        }
    }
}

void AvtivityCodeGen::BFS(GUID f_sStartNode, std::vector<std::string>& f_aVec, std::map<std::string, int>& f_mapWhileNodes)
{
    std::queue<std::string> que;
    que.push(GuidToString(f_sStartNode));
    while (!que.empty())
    {
        std::string front = que.front();
        f_aVec.push_back(front);
        que.pop();
        std::vector<Association> associationList = GodeGenGetModelOutGoingAssociation(StringToGuid(front));
        for (Association association : associationList)
        {
            std::string targetGUID = GuidToString(association.m_sTargetGUID);
            if (1 == f_mapWhileNodes.count(targetGUID))
            {
                continue;
            }
            unsigned int i;
            for (i = 0; i < f_aVec.size(); i++)
            {
                if (targetGUID == f_aVec[i])
                {
                    break;
                }
            }
            if (i == f_aVec.size())
            {
                que.push(targetGUID);
            }
        }
    }
}
/*
void DFS(GUID f_sStartNode, GUID endNode)
{
queue<GUID> que;
que.push(f_sStartNode);
while (!que.empty()) {
if (endNode == que.front())
{

}
que.pop();
}
}*/
void AvtivityCodeGen::FindWhileDoWhileCBR(GUID f_sRoot, std::map<std::string, int>& f_mapWhileNodes, std::map<std::string, int>& f_mapWhileEdges, GUID& f_sHuiDian) {
    std::map<std::string, int> breakMap;
    std::map<std::string, int>::iterator iter;
    std::vector<Association> associationList;
    f_sHuiDian = INVALID_GUID;
    /*
    在菱形选择节点中，只有while/do while的F分支、break、return可以跳出循环，因此，只有这些节点满足一个分支（指向的节点）位于循环内部，一个分支（指向的节点）位于循环外部。
    因此，我们首先找出所有一个分支位于循环内，另一个分支位于循环外的菱形选择节点。
    */
    for (iter = f_mapWhileNodes.begin(); iter != f_mapWhileNodes.end(); iter++)
    {
        GUID node = StringToGuid(iter->first);
        associationList = GodeGenGetModelOutGoingAssociation(node);
        if (associationList.size() > 1)
        {
            for (Association association : associationList)
            {

                //if(另一个分支位于循环外)
                if (0 == f_mapWhileNodes.count(GuidToString(association.m_sTargetGUID)))
                {
                    breakMap[GuidToString(node)] = 1;
                    f_mapWhileNodes[GuidToString(node)] = EVO_CODEGEN_BREAK;
                    f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] = EVO_CODEGEN_BREAK_BEGIN;


                }
            }
        }
    }

    std::vector<std::vector<std::string>> a_HuiDianListList;
    std::vector<std::string> a_HuiDianListHead;
    for (iter = f_mapWhileNodes.begin(); iter != f_mapWhileNodes.end(); iter++)
    {
        GUID sNode = StringToGuid(iter->first);
        associationList = GodeGenGetModelOutGoingAssociation(sNode);
        if (associationList.size() > 1)
        {
            for (Association association : associationList)
            {
                //if(另一个分支位于循环外)
                if (0 == f_mapWhileNodes.count(GuidToString(association.m_sTargetGUID)))
                {
                    //找出所有可能的会点
                    std::vector<std::string> f_aHuiDianList;
                    a_HuiDianListHead.push_back(GuidToString(sNode));
                    BFS(association.m_sTargetGUID, f_aHuiDianList, f_mapWhileNodes);
                    a_HuiDianListList.push_back(f_aHuiDianList);
                }
            }
        }
    }

    unsigned int i, max = 0;

    for (i = 0; i < a_HuiDianListList.size(); i++)
    {
        //if ((EVO_CODEGEN_WHILE == f_mapWhileNodes[a_HuiDianListHead[i]]) || (EVO_CODEGEN_DOWHILE == f_mapWhileNodes[a_HuiDianListHead[i]]))
        //{
        //    max = i;
        //    break;
        //}
        if (a_HuiDianListList[i].size() > a_HuiDianListList[max].size())
        {
            max = i;
        }

    }
    if (a_HuiDianListList.size() > 0)
    {
        std::vector<std::string> f_aHuiDianList = a_HuiDianListList[max];
        unsigned int j, k;
        int countJoin = 0;
        for (i = 0; i < a_HuiDianListList.size(); i++)
        {
            if (i == max)
            {
                continue;
            }
            std::vector<std::string> temp;
            for (j = 0; j < a_HuiDianListList[i].size(); j++)
            {
                for (k = 0; k < f_aHuiDianList.size(); k++)
                {
                    if (a_HuiDianListList[i][j] == f_aHuiDianList[k])
                    {
                        temp.push_back(a_HuiDianListList[i][j]);
                    }
                }
            }
            if (temp.size() <= 1)
            {
                std::string f_sHuiDian = a_HuiDianListHead[i];
                f_mapWhileNodes[f_sHuiDian] = EVO_CODEGEN_RETURN;
                associationList = GodeGenGetModelOutGoingAssociation(StringToGuid(f_sHuiDian));
                for (Association association : associationList)
                {
                    //if(另一个分支位于循环外)
                    if (a_HuiDianListList[i][0] == GuidToString(association.m_sTargetGUID))
                    {
                        f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] = EVO_CODEGEN_RETURN_BEGIN;
                    }
                }
            }
            else
            {
                f_aHuiDianList = temp;
                countJoin++;
            }
        }
        f_sHuiDian = StringToGuid(f_aHuiDianList[0]);//会点
        sg_mapJointCount[GuidToString(f_sHuiDian)] += countJoin;
    }
    else
    {
        f_sHuiDian = INVALID_GUID;
    }

    if (1 == breakMap.count(GuidToString(f_sRoot)) &&
        (
            "Action" == GetActivityType(f_sRoot)
            || "CallBehaviorAction" == GetActivityType(f_sRoot) || "SendSignalAction" == GetActivityType(f_sRoot)
            || "TimerEventAction" == GetActivityType(f_sRoot) || "AcceptEventAction" == GetActivityType(f_sRoot)
            )
        )
    {
        f_mapWhileNodes[GuidToString(f_sRoot)] = EVO_CODEGEN_WHILE_BREAK;
        associationList = CodeGenGetModelInComingAssociation(f_sRoot);
        for (Association association : associationList)
        {
            GUID sSourceGUID = association.m_sSourceGUID;
            if (1 == f_mapWhileNodes.count(GuidToString(sSourceGUID)))
            {
                f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] = EVO_CODEGEN_WHILE_BREAK_HUIBIAN;
            }
        }
    }
    else
    {
        int flagReachHuiDian = 0;
        associationList = GodeGenGetModelOutGoingAssociation(f_sRoot);
        for (Association association : associationList) {
            if (association.m_sTargetGUID == f_sHuiDian)
            {
                flagReachHuiDian = 1;
            }
        }

        //（1） 如果一个break节点是根节点，则将其调整为while节点
        if (1 == breakMap.count(GuidToString(f_sRoot)) && 1 == flagReachHuiDian)
        {
            f_mapWhileNodes[GuidToString(f_sRoot)] = EVO_CODEGEN_WHILE;
            associationList = GodeGenGetModelOutGoingAssociation(f_sRoot);
            for (Association association : associationList)
            {
                if ((EVO_CODEGEN_BREAK_BEGIN == f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)])
                    || (EVO_CODEGEN_BREAK_BEGIN_END == f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)])
                    || EVO_CODEGEN_RETURN_BEGIN == f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)])
                {
                    f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] = EVO_CODEGEN_WHILE_END;
                }
                else
                {
                    f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] = EVO_CODEGEN_WHILE_BEGIN;
                }
            }

            associationList = CodeGenGetModelInComingAssociation(f_sRoot);
            for (Association association : associationList)
            {
                GUID sSourceGUID = association.m_sSourceGUID;
                if (1 == f_mapWhileNodes.count(GuidToString(sSourceGUID)))
                {
                    f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] = EVO_CODEGEN_WHILE_HUIBIAN;
                }
            }
        }
        else {

            std::vector<GUID> aTailNodes;
            associationList = CodeGenGetModelInComingAssociation(f_sRoot);
            for (Association association : associationList)
            {
                GUID sSourceGUID = association.m_sSourceGUID;
                if (1 == f_mapWhileNodes.count(GuidToString(sSourceGUID)))
                {

                    aTailNodes.push_back(sSourceGUID);
                }
            }
            flagReachHuiDian = 0;
            if (1 == aTailNodes.size())
            {
                associationList = GodeGenGetModelOutGoingAssociation(aTailNodes[0]);
                for (Association association : associationList) {
                    if (association.m_sTargetGUID == f_sHuiDian)
                    {
                        flagReachHuiDian = 1;
                    }
                }
            }

            //（2） 如果一个break节点是尾节点，且尾节点只有一个，则将其调整为do while节点。
            if ((1 == aTailNodes.size()) && 1 == flagReachHuiDian && (1 == breakMap.count(GuidToString(aTailNodes[0])))) {
                GUID tailNode = aTailNodes[0];
                f_mapWhileNodes[GuidToString(f_sRoot)] = EVO_CODEGEN_DO;
                f_mapWhileNodes[GuidToString(tailNode)] = EVO_CODEGEN_DOWHILE;
                associationList = GodeGenGetModelOutGoingAssociation(f_sRoot);
                for (Association association : associationList)
                {
                    if (1 == f_mapWhileNodes.count(GuidToString(association.m_sTargetGUID)))
                    {
                        f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] = EVO_CODEGEN_DOWHILE_BEGIN;
                    }

                }

                associationList = GodeGenGetModelOutGoingAssociation(tailNode);
                for (Association association : associationList)
                {
                    if (association.m_sTargetGUID == f_sRoot)
                    {
                        f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] = EVO_CODEGEN_DOWHILE_HUIBIAN;
                    }
                }

                //associationList = CodeGenGetModelInComingAssociation(tailNode);
                //for (Association association : associationList)
                //{
                //    if (f_mapWhileNodes.size() != 0 && 1 == f_mapWhileNodes.count(GuidToString(association.m_sSourceGUID)))
                //    {
                //        f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] = EVO_CODEGEN_DOWHILE_HUIBIAN;
                //    }
                //}

                associationList = GodeGenGetModelOutGoingAssociation(tailNode);
                for (Association association : associationList) {
                    GUID m_sGUID = association.m_sBasicInfo.m_sGUID;
                    //if (1 == f_mapWhileNodes.count(GuidToString(association.m_sTargetGUID)))
                    //{
                    //    //f_mapWhileEdges[GuidToString(m_sGUID)] = CONTINUE;  //对于Do While，CONTINUE是if的特例
                    //}
                    if (1 != f_mapWhileEdges.count(GuidToString(association.m_sBasicInfo.m_sGUID)))
                    {
                        std::vector<std::string> f_aGuid;
                        f_aGuid.push_back(GuidToString(association.m_sBasicInfo.m_sGUID));
                        AddResult(EVO_CODEGEN_WARNING_INTERNAL, f_aGuid);
                    }
                    else if (EVO_CODEGEN_BREAK_BEGIN == f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)]
                        || EVO_CODEGEN_BREAK_BEGIN_END == f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)])
                    {
                        f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] = EVO_CODEGEN_DOWHILE_END;
                    }
                }
            }
            else {
                f_mapWhileNodes[GuidToString(f_sRoot)] = EVO_CODEGEN_WHILE_BREAK;

                //associationList = GodeGenGetModelOutGoingAssociation(f_sRoot);
                //for (Association association : associationList)
                //{
                //    if ((EVO_CODEGEN_BREAK_BEGIN == f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)]) || (EVO_CODEGEN_BREAK_BEGIN_END == f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)]))
                //    {
                //        f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] = EVO_CODEGEN_BREAK_BEGIN;
                //    }
                //    else
                //    {
                //        f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] = EVO_CODEGEN_WHILE_BREAK_BEGIN;
                //    }
                //}

                associationList = CodeGenGetModelInComingAssociation(f_sRoot);
                for (Association association : associationList)
                {
                    GUID sSourceGUID = association.m_sSourceGUID;
                    if (1 == f_mapWhileNodes.count(GuidToString(sSourceGUID)))
                    {
                        f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] = EVO_CODEGEN_WHILE_BREAK_HUIBIAN;
                    }
                }
            }
        }
    }

    if (GetActivityType(f_sHuiDian) != "ActivityFinalNode")
    {
        associationList = CodeGenGetModelInComingAssociation(f_sHuiDian);
        for (Association association : associationList)
        {
            if (f_mapWhileEdges.count(GuidToString(association.m_sBasicInfo.m_sGUID)) == 1
                && (f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] == EVO_CODEGEN_WHILE_BREAK_END
                    || f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] == EVO_CODEGEN_DOWHILE_END
                    || f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] == EVO_CODEGEN_WHILE_END)
                )
            {
                continue;
            }

            for (std::string aHuiHead : a_HuiDianListHead)
            {
                if (aHuiHead == GuidToString(association.m_sSourceGUID))
                {
                    if (f_mapWhileEdges.count(GuidToString(association.m_sBasicInfo.m_sGUID)) != 1)
                    {
                        std::vector<std::string> f_aGuid;
                        f_aGuid.push_back(GuidToString(association.m_sBasicInfo.m_sGUID));
                        AddResult(EVO_CODEGEN_WARNING_INTERNAL, f_aGuid);
                    }
                    else if (f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] == EVO_CODEGEN_BREAK_BEGIN)
                    {
                        f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] = EVO_CODEGEN_BREAK_BEGIN_END;
                        goto outside;
                    }
                    else
                    {
                        std::vector<std::string> f_aGuid;
                        f_aGuid.push_back(GuidToString(association.m_sBasicInfo.m_sGUID));
                        AddResult(EVO_CODEGEN_WARNING_INTERNAL, f_aGuid);
                    }

                }
            }
            for (std::vector<std::string> aHuiPath : a_HuiDianListList)
            {
                for (int ii = 0; ii < aHuiPath.size() - 1; ii++)
                {
                    if (aHuiPath[ii] == GuidToString(f_sHuiDian))
                    {
                        break;
                    }
                    if (aHuiPath[ii] == GuidToString(association.m_sSourceGUID))
                    {
                        if (f_mapWhileEdges.count(GuidToString(association.m_sBasicInfo.m_sGUID)) == 1)
                        {
                            std::vector<std::string> f_aGuid;
                            f_aGuid.push_back(GuidToString(association.m_sBasicInfo.m_sGUID));
                            AddResult(EVO_CODEGEN_WARNING_INTERNAL, f_aGuid);
                        }

                        f_mapWhileEdges[GuidToString(association.m_sBasicInfo.m_sGUID)] = EVO_CODEGEN_BREAK_END;
                        goto outside;
                    }
                }
            }
        outside:;
        }
    }
}

GUID AvtivityCodeGen::RecognizeIFPath(GUID f_sStartNode, std::map<std::string, int> f_mapNodes, std::vector<std::map<std::string, int>> f_mapWhileNodesOutSideList, std::vector<std::string> f_aHuiDianOutSideList, std::map<std::string, int>& f_mapEdges, std::map<std::string, int> f_mapVisitNodes)
{
    //从开始结点开始广度优先遍历
    while (true)
    {
        if (f_sStartNode == INVALID_GUID)
        {
            return INVALID_GUID;
        }
        if (OUTPUT_ONCE && sg_mapDelete.count(GuidToString(f_sStartNode)) == 1)
        {
            std::vector<std::string> f_aGuid;
            f_aGuid.push_back(GuidToString(f_sStartNode));
            AddResult(EVO_CODEGEN_WARNING_FEIJIEGOU, f_aGuid);
            return INVALID_GUID;
        }

      
        if ("ActivityFinalNode" == GetActivityType(f_sStartNode))
        {
            CodeGenCout(sg_sReturnString+"\n", f_sStartNode, "Return");
            return CBR_NODE;
        }

        std::vector<Association> associationList = GodeGenGetModelOutGoingAssociation(f_sStartNode);


        unsigned int i;
        for (i = 0; i < f_mapWhileNodesOutSideList.size(); i++)
        {
            if (f_mapWhileNodesOutSideList[i].count(GuidToString(f_sStartNode)) != 0)
            {
                break;
            }
        }

        //进入while循环
        if (i != f_mapWhileNodesOutSideList.size())
        {
            HandleLoop(f_sStartNode, i, f_mapWhileNodesOutSideList, f_aHuiDianOutSideList, f_mapEdges, f_mapVisitNodes);
            f_sStartNode = StringToGuid(f_aHuiDianOutSideList[i]);
            continue;
        }

        //处理汇合点
        int jointCount = 0;
        if (1 == sg_mapJointCount.count(GuidToString(f_sStartNode)))
        {
            jointCount = sg_mapJointCount[GuidToString(f_sStartNode)];
        }
        std::vector<Association> inList = CodeGenGetModelInComingAssociation(f_sStartNode);
        std::vector<Association> outList = GodeGenGetModelOutGoingAssociation(f_sStartNode);

        int in = 0, out = 0;

        //if（当前结点的入度－当前结点的JointCount－1＞＝1）找到汇合点
        if (int(inList.size() - jointCount - 1) >= 1)
        {
            return f_sStartNode;
        }
        if (associationList.size() == 0)
        {
            if ("ActivityFinalNode" != GetActivityType(f_sStartNode))
            {
                std::vector<std::string> f_aGuid;
                f_aGuid.push_back(GuidToString(f_sStartNode));
                AddResult(EVO_CODEGEN_WARNING_FINAL, f_aGuid);
            }
            OpaqueExpression f_psEffect;
            if (NO_ERROR == GetActionEffect(f_sStartNode, &f_psEffect) && f_psEffect.m_strBody.size() > 0)
            {
                sg_mapDelete[GuidToString(f_sStartNode)] = 1;
                CodeGenCout(f_psEffect.m_strBody.at(0)+"\n", f_sStartNode);
            }

            CodeGenCout(sg_sReturnString + "\n", f_sStartNode, "Return");
            return CBR_NODE;

        }
        //矩形结点
        else if (associationList.size() == 1)
        {
            if ("Action" != GetActivityType(f_sStartNode)
                && "CallBehaviorAction" != GetActivityType(f_sStartNode) && "SendSignalAction" != GetActivityType(f_sStartNode)
                && "TimerEventAction" != GetActivityType(f_sStartNode) && "AcceptEventAction" != GetActivityType(f_sStartNode)
                && "JoinNode" != GetActivityType(f_sStartNode))
            {
                std::vector<std::string> f_aGuid;
                f_aGuid.push_back(GuidToString(f_sStartNode));
                AddResult(EVO_CODEGEN_WARNING_ASSOCIATION, f_aGuid);
            }
            sg_mapDelete[GuidToString(f_sStartNode)] = 1;
            OpaqueExpression f_psEffect;
            if (NO_ERROR == GetActionEffect(f_sStartNode, &f_psEffect) && f_psEffect.m_strBody.size() > 0)
            {
                CodeGenCout(f_psEffect.m_strBody.at(0) +"\n", f_sStartNode);
            }
            std::vector<Association> associationList = GodeGenGetModelOutGoingAssociation(f_sStartNode);
            Association association = associationList.at(0);
            if (0 == f_mapEdges.count(GuidToString(association.m_sBasicInfo.m_sGUID)))
            {
                f_sStartNode = association.m_sTargetGUID;
            }
            else
            {
                int edgeType = f_mapEdges[GuidToString(association.m_sBasicInfo.m_sGUID)];
                if (edgeType >= EVO_CODEGEN_HUIBIAN)
                {
                    CodeGenCout("continue;\n", INVALID_GUID, "Continue");
                    return CBR_NODE;
                }
                else if (EVO_CODEGEN_BREAK_BEGIN_END == edgeType || EVO_CODEGEN_BREAK_END == edgeType)
                {
                    CodeGenCout("break;\n", INVALID_GUID, "Break");
                    return CBR_NODE;
                }
                else
                {
                    std::vector<std::string> f_aGuid;
                    f_aGuid.push_back(GuidToString(association.m_sBasicInfo.m_sGUID));
                    AddResult(EVO_CODEGEN_WARNING_INTERNAL, f_aGuid);
                    return INVALID_GUID;
                }
            }
        }

        //if结点
        else if (associationList.size() > 1)
        {
            f_sStartNode = HandelIF(f_sStartNode, f_mapNodes, f_mapWhileNodesOutSideList, f_aHuiDianOutSideList, f_mapEdges, f_mapVisitNodes);
            if (CBR_NODE == f_sStartNode)
            {
                return f_sStartNode;
            }
        }
    }
    return f_sStartNode;
}

GUID AvtivityCodeGen::HandelIF(const GUID f_sStartNode, std::map<std::string, int> f_mapNodes, std::vector<std::map<std::string, int>> f_mapWhileNodesOutSideList, std::vector<std::string> f_aHuiDianOutSideList, std::map<std::string, int>& f_mapEdges, std::map<std::string, int>& f_mapVisitNodes)
{
    if (OUTPUT_ONCE && sg_mapDelete.count(GuidToString(f_sStartNode)) == 1)
    {
        std::vector<std::string> f_aGuid;
        f_aGuid.push_back(GuidToString(f_sStartNode));
        AddResult(EVO_CODEGEN_WARNING_FEIJIEGOU, f_aGuid);
        return INVALID_GUID;
    }
    sg_mapDelete[GuidToString(f_sStartNode)] = 1;


    OpaqueExpression f_psEffect;
    if (NO_ERROR == GetActionEffect(f_sStartNode, &f_psEffect) && f_psEffect.m_strBody.size() > 0)
    {
        CodeGenCout(f_psEffect.m_strBody.at(0)+"\n", f_sStartNode);
    }

    std::vector<Association> associationList = GodeGenGetModelOutGoingAssociation(f_sStartNode);
    if (associationList.size() == 0)
    {
        std::vector<std::string> f_aGuid;
        f_aGuid.push_back(GuidToString(f_sStartNode));
        AddResult(EVO_CODEGEN_WARNING_FINAL, f_aGuid);
        return INVALID_GUID;
    }
    if (associationList.size() == 1)
    {
        std::vector<std::string> f_aGuid;
        f_aGuid.push_back(GuidToString(f_sStartNode));
        AddResult(EVO_CODEGEN_WARNING_ASSOCIATION, f_aGuid);
        return associationList[0].m_sTargetGUID;
    }

    std::vector<Association> association, associationCBR, associationOut, associationHuiBian;

    for (Association ass : associationList)
    {
        if (f_mapEdges.size() == 0
            || 0 == f_mapEdges.count(GuidToString(ass.m_sBasicInfo.m_sGUID))
            || EVO_CODEGEN_WHILE_BEGIN == f_mapEdges[GuidToString(ass.m_sBasicInfo.m_sGUID)]         /* while结构的起始边         */
            || EVO_CODEGEN_DOWHILE_BEGIN == f_mapEdges[GuidToString(ass.m_sBasicInfo.m_sGUID)]       /* dowhile结构的起始边       */
            || EVO_CODEGEN_WHILE_BREAK_BEGIN == f_mapEdges[GuidToString(ass.m_sBasicInfo.m_sGUID)]   /* while(True)结构的起始边   */
            )
        {
            association.push_back(ass);
        }
        else if (EVO_CODEGEN_BREAK_BEGIN == f_mapEdges[GuidToString(ass.m_sBasicInfo.m_sGUID)]          /* break结构的起始边         */
            || EVO_CODEGEN_BREAK_END == f_mapEdges[GuidToString(ass.m_sBasicInfo.m_sGUID)]            /* break结构的结束边         */
            || EVO_CODEGEN_BREAK_BEGIN_END == f_mapEdges[GuidToString(ass.m_sBasicInfo.m_sGUID)]      /* break结构的起始/结束边    */
            || EVO_CODEGEN_RETURN == f_mapEdges[GuidToString(ass.m_sBasicInfo.m_sGUID)]               /* return节点                */
            || EVO_CODEGEN_RETURN_BEGIN == f_mapEdges[GuidToString(ass.m_sBasicInfo.m_sGUID)]         /* return结构的起始边        */

            )
        {
            associationCBR.push_back(ass);
        }
        else if (EVO_CODEGEN_DOWHILE_HUIBIAN == f_mapEdges[GuidToString(ass.m_sBasicInfo.m_sGUID)]       /* dowhile结构的回边         */
            || EVO_CODEGEN_WHILE_HUIBIAN == f_mapEdges[GuidToString(ass.m_sBasicInfo.m_sGUID)]           /* while结构的回边           */
            || EVO_CODEGEN_WHILE_BREAK_HUIBIAN == f_mapEdges[GuidToString(ass.m_sBasicInfo.m_sGUID)]     /* while(True)结构的回边     */

            )
        {
            associationHuiBian.push_back(ass);
        }
        else if (EVO_CODEGEN_WHILE_END == f_mapEdges[GuidToString(ass.m_sBasicInfo.m_sGUID)]             /* while结构的结束边         */
            || EVO_CODEGEN_DOWHILE_END == f_mapEdges[GuidToString(ass.m_sBasicInfo.m_sGUID)]             /* dowhile结构的结束边       */
            || EVO_CODEGEN_WHILE_BREAK_END == f_mapEdges[GuidToString(ass.m_sBasicInfo.m_sGUID)]         /* while(True)结构的结束边   */
            )
        {
            associationOut.push_back(ass);;
        }
        else
        {
            std::vector<std::string> f_aGuid;
            f_aGuid.push_back(GuidToString(f_sStartNode));
            AddResult(EVO_CODEGEN_WARNING_INTERNAL, f_aGuid);
            continue;
        }
    }

    if (associationOut.size() > 1)
    {
        std::vector<std::string> f_aGuid;
        f_aGuid.push_back(GuidToString(f_sStartNode));
        AddResult(EVO_CODEGEN_WARNING_FEIJIEGOU, f_aGuid);
    }

    std::vector<Association> associationCBRHuiBian;
    associationCBRHuiBian.insert(associationCBRHuiBian.end(), associationCBR.begin(), associationCBR.end());
    associationCBRHuiBian.insert(associationCBRHuiBian.end(), associationHuiBian.begin(), associationHuiBian.end());
    associationCBRHuiBian = GodeGenSortAssociation(associationCBRHuiBian);
    for (unsigned int i = 0; i < associationCBRHuiBian.size(); i++)
    {
        int iEdgesType = 0;
        if (f_mapEdges.count(GuidToString(associationCBRHuiBian[i].m_sBasicInfo.m_sGUID)) > 0)
        {
            iEdgesType = f_mapEdges[GuidToString(associationCBRHuiBian[i].m_sBasicInfo.m_sGUID)];
        }
        if (EVO_CODEGEN_DOWHILE_HUIBIAN == iEdgesType)            /* dowhile结构的回边           */
        {
            continue;
        }

        std::string strGuard = GetGuard(associationCBRHuiBian[i].m_sBasicInfo.m_sGUID);
        if ("" == strGuard && associationCBRHuiBian.size() == 1 && association.size() == 1 && "" != GetGuard(association[0].m_sBasicInfo.m_sGUID))
        {
            strGuard = "!(" + GetGuard(association[0].m_sBasicInfo.m_sGUID) + ")";
        }


        if (0 == i)
        {
            CodeGenCout("if (" + strGuard + ")\n", associationCBRHuiBian[i].m_sSourceGUID);
            CodeGenCout("{\n", INVALID_GUID, "{");
        }
        else if (associationCBRHuiBian.size() - 1 == i && "" == strGuard)
        {
            CodeGenCout("else\n", INVALID_GUID,"Else");
            CodeGenCout("{\n", INVALID_GUID, "{");
        }
        else
        {
            CodeGenCout("else if (" + strGuard + ")", associationCBRHuiBian[i].m_sSourceGUID, "ElseIf");
            CodeGenCout("{\n", INVALID_GUID, "{");
        }

        sg_iKongGe++;


        if (EVO_CODEGEN_BREAK_BEGIN == iEdgesType)
        {
            HandleLoop(associationCBRHuiBian[i].m_sTargetGUID, -1, f_mapWhileNodesOutSideList, f_aHuiDianOutSideList, f_mapEdges, f_mapVisitNodes);
        }
        else if (EVO_CODEGEN_BREAK_END == iEdgesType || EVO_CODEGEN_BREAK_BEGIN_END == iEdgesType)
        {
            CodeGenCout("break;\n", INVALID_GUID,"Break");
        }
        else if (EVO_CODEGEN_RETURN_BEGIN == iEdgesType)
        {
            HandleLoop(associationCBRHuiBian[i].m_sTargetGUID, -1, f_mapWhileNodesOutSideList, f_aHuiDianOutSideList, f_mapEdges, f_mapVisitNodes);
        }
        else if (EVO_CODEGEN_WHILE_HUIBIAN == iEdgesType            /* while结构的回边           */
            || EVO_CODEGEN_WHILE_BREAK_HUIBIAN == iEdgesType    /* while(True)结构的回边     */
            )
        {
            CodeGenCout("continue;\n", INVALID_GUID, "Continue");
        }

        else
        {
            std::vector<std::string> f_aGuid;
            f_aGuid.push_back(GuidToString(f_sStartNode));
            AddResult(EVO_CODEGEN_WARNING_INTERNAL, f_aGuid);
        }
        sg_iKongGe--;
        CodeGenCout("}\n", INVALID_GUID, "}");
    }

    std::vector<GUID> aIfPathReturn;
    if (association.size() == 0)
    {
    }
    else if (association.size() == 1)
    {
        //OpaqueExpression f_psEffect;
        //if (NO_ERROR == GetActionEffect(association[0].m_sTargetGUID, &f_psEffect) && f_psEffect.m_strBody.size() > 0)
        //{
        //    EVO_CODEGEN_MYCOUT << f_psEffect.m_strBody.at(0) << std::endl;
        //}
        aIfPathReturn.push_back(association[0].m_sTargetGUID);
    }
    else
    {
        unsigned int i = 0;
        for (; i < association.size(); i++)
        {
            std::string guard = GetGuard(association[i].m_sBasicInfo.m_sGUID);
            if (guard != "") {
                break;
            }
        }
        if (i != association.size())
        {
            Association ass0 = association[0];
            association[0] = association[i];
            association[i] = ass0;
        }

        for (unsigned int i = 0; i < association.size(); i++)
        {
            if (0 == i)
            {
                CodeGenCout("if (" + GetGuard(association[i].m_sBasicInfo.m_sGUID) + ")\n", association[i].m_sSourceGUID,"If" );
                CodeGenCout("{\n", INVALID_GUID, "{");
            }
            else if (association.size() - 1 == i)
            {
                CodeGenCout("else\n", INVALID_GUID, "Else");
                CodeGenCout("{\n", INVALID_GUID, "{");
            }
            else
            {
                CodeGenCout("else if (" + GetGuard(association[i].m_sBasicInfo.m_sGUID) + ")\n", association[i].m_sSourceGUID, "ElseIf");
                CodeGenCout("{\n", INVALID_GUID, "{");
            }
            sg_iKongGe++;
            GUID IfPathReturn = RecognizeIFPath(association[i].m_sTargetGUID, f_mapNodes, f_mapWhileNodesOutSideList, f_aHuiDianOutSideList, f_mapEdges, f_mapVisitNodes);
            sg_iKongGe--;
            CodeGenCout("}\n", INVALID_GUID, "}");
            if (CBR_NODE != IfPathReturn)
            {
                aIfPathReturn.push_back(IfPathReturn);
            }
        }
    }

    if (f_mapNodes.size() > 0
        && (EVO_CODEGEN_DOWHILE == f_mapNodes[GuidToString(f_sStartNode)] || EVO_CODEGEN_WHILE == f_mapNodes[GuidToString(f_sStartNode)]))
    {
        std::string strGuard = "";
        GUID guardGuid;
        if (EVO_CODEGEN_WHILE == f_mapNodes[GuidToString(f_sStartNode)] && association.size() > 0 && "" != GetGuard(association[0].m_sBasicInfo.m_sGUID))
        {
            strGuard = GetGuard(association[0].m_sBasicInfo.m_sGUID);
            guardGuid = association[0].m_sSourceGUID;
            if (association.size() > 1)
            {
                std::vector<std::string> f_aGuid;
                f_aGuid.push_back(GuidToString(f_sStartNode));
                AddResult(EVO_CODEGEN_WARNING_FEIJIEGOU, f_aGuid);
            }
        }
        else if (EVO_CODEGEN_DOWHILE == f_mapNodes[GuidToString(f_sStartNode)] && associationHuiBian.size() > 0 && "" != GetGuard(associationHuiBian[0].m_sBasicInfo.m_sGUID))
        {
            strGuard = GetGuard(associationHuiBian[0].m_sBasicInfo.m_sGUID);
            guardGuid = associationHuiBian[0].m_sSourceGUID;
            if (associationHuiBian.size() > 1)
            {
                std::vector<std::string> f_aGuid;
                f_aGuid.push_back(GuidToString(f_sStartNode));
                AddResult(EVO_CODEGEN_WARNING_FEIJIEGOU, f_aGuid);
            }
        }
        else if (associationOut.size() > 0 && "" != GetGuard(associationOut[0].m_sBasicInfo.m_sGUID))
        {
            strGuard = "!(" + GetGuard(associationOut[0].m_sBasicInfo.m_sGUID) + ")";
            guardGuid = associationOut[0].m_sSourceGUID;
        }

        if (EVO_CODEGEN_DOWHILE == f_mapNodes[GuidToString(f_sStartNode)])
        {
            sg_iKongGe--;
            CodeGenCout("}", INVALID_GUID, "}");
            CodeGenCout("while(" + strGuard + ");\n", guardGuid,"While");
        }
        else if (EVO_CODEGEN_WHILE == f_mapNodes[GuidToString(f_sStartNode)])
        {
            CodeGenCout("while(" + strGuard + ")\n", guardGuid, "While");
            CodeGenCout("{\n", INVALID_GUID, "{");
            sg_iKongGe++;
        }
    }

    if (aIfPathReturn.size() == 0)
    {
        return INVALID_GUID;
    }
    for (unsigned int i = 1; i < aIfPathReturn.size(); i++)
    {
        if (aIfPathReturn[0] != aIfPathReturn[i])
        {
            std::vector<std::string> aGuid;
            aGuid.push_back(GuidToString(f_sStartNode));
            AddResult(EVO_CODEGEN_WARNING_IF, aGuid);
            return INVALID_GUID;
        }
    }

    if (1 == sg_mapJointCount.count(GuidToString(aIfPathReturn[0])))
        sg_mapJointCount[GuidToString(aIfPathReturn[0])] += (aIfPathReturn.size() - 1);
    else
        sg_mapJointCount[GuidToString(aIfPathReturn[0])] = (aIfPathReturn.size() - 1);
    return aIfPathReturn[0];
}

//GUID EvoHandelBreakReturn(GUID f_sStartNode, std::map<std::string, int>& f_mapEdges, std::map<std::string, int>& f_mapVisitNodes)
//{
//    OpaqueExpression f_psEffect;
//    if (GetActionEffect(f_sStartNode, &f_psEffect) == NO_ERROR && f_psEffect.m_strBody.size() > 0)
//    {
//        EVO_CODEGEN_MYCOUT << f_psEffect.m_strBody.at(0) << std::endl;
//
//    }
//    std::vector<Association> associationList = GodeGenGetModelOutGoingAssociation(f_sStartNode);
//    if (associationList.size() == 1)
//    {
//        std::vector<std::string> f_aGuid;
//        f_aGuid.push_back(GuidToString(f_sStartNode));
//        AddResult(EVO_CODEGEN_WARNING_ASSOCIATION, f_aGuid);
//        return associationList[0].m_sTargetGUID;
//    }
//    if (associationList.size() == 0)
//    {
//        std::vector<std::string> f_aGuid;
//        f_aGuid.push_back(GuidToString(f_sStartNode));
//        AddResult(EVO_CODEGEN_WARNING_FINAL, f_aGuid);
//        return INVALID_GUID;
//    }
//    GUID sStartNodeT, sStartNodeF, sAssoT, sAssoF;
//    GUID sAdge0 = associationList.at(0).m_sBasicInfo.m_sGUID;
//    GUID sEdge1 = associationList.at(1).m_sBasicInfo.m_sGUID;
//
//    int iEdgesType0 = 0;
//    if (f_mapEdges.count(GuidToString(sAdge0)) > 0)
//    {
//        iEdgesType0 = f_mapEdges[GuidToString(sAdge0)];
//    }
//    int iEdgesType1 = 0;
//    if (f_mapEdges.count(GuidToString(sEdge1)) > 0)
//    {
//        iEdgesType1 = f_mapEdges[GuidToString(sEdge1)];
//    }
//
//    if (iEdgesType0 == EVO_CODEGEN_BREAK_BEGIN || iEdgesType0 == EVO_CODEGEN_BREAK_END || iEdgesType0 == EVO_CODEGEN_BREAK_BEGIN_END || iEdgesType0 == EVO_CODEGEN_RETURN_BEGIN)
//    {
//        sStartNodeT = associationList.at(0).m_sTargetGUID;
//        sAssoT = associationList.at(0).m_sBasicInfo.m_sGUID;
//        sStartNodeF = associationList.at(1).m_sTargetGUID;
//        sAssoF = associationList.at(1).m_sBasicInfo.m_sGUID;
//    }
//    else if (iEdgesType1 == EVO_CODEGEN_BREAK_BEGIN || iEdgesType1 == EVO_CODEGEN_BREAK_END || iEdgesType1 == EVO_CODEGEN_BREAK_BEGIN_END || iEdgesType1 == EVO_CODEGEN_RETURN_BEGIN)
//    {
//        sStartNodeT = associationList.at(1).m_sTargetGUID;
//        sAssoT = associationList.at(1).m_sBasicInfo.m_sGUID;
//        sStartNodeF = associationList.at(0).m_sTargetGUID;
//        sAssoF = associationList.at(0).m_sBasicInfo.m_sGUID;
//    }
//    else
//    {
//        //std::vector<std::string> f_aGuid;
//        //f_aGuid.push_back(GuidToString(f_sStartNode));
//        //AddResult(EVO_CODEGEN_WARNING_INTERNAL, f_aGuid);
//        return INVALID_GUID;
//
//    }
//    OpaqueExpression f_psGuard;
//    GetGuard(sAssoT, &f_psGuard);
//    if (f_psGuard.m_strBody.size() != 0)
//    {
//        EVO_CODEGEN_MYCOUT << "if (" << f_psGuard.m_strBody.at(0) << "){" << std::endl; sg_iKongGe++;
//    }
//    else
//    {
//        GetGuard(sAssoF, &f_psGuard);
//        if (f_psGuard.m_strBody.size() != 0) {
//            EVO_CODEGEN_MYCOUT << "if (!" << f_psGuard.m_strBody.at(0) << "){" << std::endl; sg_iKongGe++;
//        }
//        else
//        {
//            EVO_CODEGEN_MYCOUT << "if (){" << std::endl; sg_iKongGe++;
//        }
//    }
//    if (EVO_CODEGEN_BREAK_BEGIN == iEdgesType0 || EVO_CODEGEN_BREAK_BEGIN == iEdgesType1 ||
//        EVO_CODEGEN_BREAK_END == iEdgesType0 || EVO_CODEGEN_BREAK_END == iEdgesType1 ||
//        EVO_CODEGEN_BREAK_BEGIN_END == iEdgesType0 || EVO_CODEGEN_BREAK_BEGIN_END == iEdgesType1)
//    {
//        if (EVO_CODEGEN_BREAK_BEGIN == iEdgesType0 || EVO_CODEGEN_BREAK_BEGIN == iEdgesType1)
//        {
//            std::map<std::string, int> emptyMap;
//            //调用 HandleLoop函数处理CBR结点的T分支
//            HandleLoop(sStartNodeT, emptyMap, f_mapEdges, f_mapVisitNodes);
//        }
//        else
//        {
//            EVO_CODEGEN_MYCOUT << "break;" << std::endl;
//            sg_iKongGe--;
//            CodeGenCout("}\n", INVALID_GUID, "}");
//        }
//    }
//    else if (EVO_CODEGEN_RETURN_BEGIN == iEdgesType0 || EVO_CODEGEN_RETURN_BEGIN == iEdgesType1)
//    {
//        std::map<std::string, int> emptyMap;
//        //调用 HandleLoop函数处理CBR结点的T分支
//        HandleLoop(sStartNodeT, emptyMap, f_mapEdges, f_mapVisitNodes);
//
//    }
//
//
//    f_sStartNode = sStartNodeF;
//    return f_sStartNode;
//}


//从开始结点开始广度优先遍历，对于每一个结点进行如下操作
GUID AvtivityCodeGen::HandleLoop(GUID f_sStartNode, const int f_iMapNodes, std::vector<std::map<std::string, int>> f_mapWhileNodesOutSideList, std::vector <std::string> f_aHuiDianOutSideList, std::map<std::string, int> f_mapEdges, std::map<std::string, int>& f_mapVisitNodes)
{
    if (f_sStartNode == INVALID_GUID)
    {
        return INVALID_GUID;
    }

    std::map<std::string, int> f_mapNodes;
    if (f_iMapNodes >= 0 && f_mapWhileNodesOutSideList.size() > 0)
    {
        f_mapNodes = f_mapWhileNodesOutSideList[f_iMapNodes];
    }

    std::vector <std::string> f_aHuiDianList;
    std::vector<std::map<std::string, int>> f_mapWhileNodesList;
    FindWhile(f_sStartNode, f_mapNodes, &f_mapWhileNodesList, f_mapEdges, f_aHuiDianList);


    for (unsigned int i = 0; i < f_mapWhileNodesOutSideList.size(); i++)
    {
        if (f_iMapNodes == i)
        {
            continue;
        }
        f_mapWhileNodesList.push_back(f_mapWhileNodesOutSideList[i]);
        f_aHuiDianList.push_back(f_aHuiDianOutSideList[i]);
    }

    bool inWhile = false;
    /* if (f_mapNodes[GuidToString(f_sStartNode)] == EVO_CODEGEN_WHILE || f_mapNodes[GuidToString(f_sStartNode)] == WHILE_BREAK)
    {
    for (int i = 0; i < _f_mapWhileNodesList.size(); i++)
    {
    if (_f_mapWhileNodesList[i].count(GuidToString(f_sStartNode)) == 0)
    {
    f_mapWhileNodesList.push_back(_f_mapWhileNodesList[i]);
    }
    else
    {
    std::vector<Association> associationList = CodeGenGetModelInComingAssociation(f_sStartNode);
    for (Association association : associationList)
    {
    if (_f_mapWhileNodesList[i].count(GuidToString(association.m_sSourceGUID)) == 1)
    {

    }
    }
    }
    }
    }*/
    GUID sReturnNode = INVALID_GUID;
    while (true)
    {
        if (OUTPUT_ONCE && sg_mapDelete.count(GuidToString(f_sStartNode)) == 1)
        {
            std::vector<std::string> f_aGuid;
            f_aGuid.push_back(GuidToString(f_sStartNode));
            AddResult(EVO_CODEGEN_WARNING_FEIJIEGOU, f_aGuid);
            if (inWhile)
            {
                sg_iKongGe--;
                CodeGenCout("}\n",INVALID_GUID,"}") ;
            }
            return INVALID_GUID;
        }

        //ElementBasicInfo f_psElementInfo;
        //int res = MyGetModelElementBasicInfo(f_sStartNode, &f_psElementInfo);
        //EVO_CODEGEN_MYCOUT << GuidToString(f_sStartNode) << " " << GetActivityType(f_sStartNode) <<std::endl;

        unsigned int i;
        if (f_mapNodes.size() > 0 && 0 == f_mapNodes.count(GuidToString(f_sStartNode)))
        {
            if (inWhile)
            {
                sg_iKongGe--;
                CodeGenCout("}\n", INVALID_GUID,"}");
            }
            return f_sStartNode;
        }
        for (i = 0; i < f_mapWhileNodesList.size(); i++)
        {
            if (f_mapWhileNodesList[i].count(GuidToString(f_sStartNode)) != 0)
            {
                break;
            }
        }

        //进入while循环
        if (i != f_mapWhileNodesList.size())
        {
            HandleLoop(f_sStartNode, i, f_mapWhileNodesList, f_aHuiDianList, f_mapEdges, f_mapVisitNodes);
            f_sStartNode = StringToGuid(f_aHuiDianList[i]);
        }

        else if (f_mapNodes.size() > 0 &&
            (EVO_CODEGEN_DO == f_mapNodes[GuidToString(f_sStartNode)] || EVO_CODEGEN_WHILE_BREAK == f_mapNodes[GuidToString(f_sStartNode)])
            ) {

            if (EVO_CODEGEN_DO == f_mapNodes[GuidToString(f_sStartNode)])
            {
                CodeGenCout("do\n", f_sStartNode, "DO");
                CodeGenCout("{\n", INVALID_GUID, "{");
            }
            else if (EVO_CODEGEN_WHILE_BREAK == f_mapNodes[GuidToString(f_sStartNode)])
            {
                CodeGenCout("while(True)", f_sStartNode,"WHILE_TRUE");
                CodeGenCout("{\n", INVALID_GUID, "{");
                inWhile = true;
            }
            sg_iKongGe++;

            std::vector<Association> associationList = GodeGenGetModelOutGoingAssociation(f_sStartNode);
            if (associationList.size() == 0)
            {
                OpaqueExpression f_psEffect;
                if (GetActionEffect(f_sStartNode, &f_psEffect) == NO_ERROR && f_psEffect.m_strBody.size() > 0)
                {
                    CodeGenCout(f_psEffect.m_strBody.at(0) + "\n", f_sStartNode);
                }
                sg_mapDelete[GuidToString(f_sStartNode)] = 1;

                std::vector<std::string> f_aGuid;
                f_aGuid.push_back(GuidToString(f_sStartNode));
                AddResult(EVO_CODEGEN_WARNING_FINAL, f_aGuid);
                if (inWhile)
                {
                    sg_iKongGe--;
                    CodeGenCout("}\n" ,INVALID_GUID, "}");
                }
                return INVALID_GUID;
            }
            else if (associationList.size() > 1)
            {
                f_sStartNode = HandelIF(f_sStartNode, f_mapNodes, f_mapWhileNodesList, f_aHuiDianList, f_mapEdges, f_mapVisitNodes);
                if (f_sStartNode == CBR_NODE)
                {
                    std::vector<std::string> f_aGuid;
                    f_aGuid.push_back(GuidToString(INVALID_GUID));
                    AddResult(EVO_CODEGEN_WARNING_FEIJIEGOU, f_aGuid);

                    if (inWhile)
                    {
                        sg_iKongGe--;
                        CodeGenCout("}\n", INVALID_GUID, "}");
                    }
                    return INVALID_GUID;
                }
            }
            else
            {
                OpaqueExpression f_psEffect;
                if (GetActionEffect(f_sStartNode, &f_psEffect) == NO_ERROR && f_psEffect.m_strBody.size() > 0)
                {
                    CodeGenCout(f_psEffect.m_strBody.at(0), f_sStartNode);
                }
                sg_mapDelete[GuidToString(f_sStartNode)] = 1;
                f_sStartNode = associationList[0].m_sTargetGUID;

            }
        }

        else {
            std::vector<Association> associationList = GodeGenGetModelOutGoingAssociation(f_sStartNode);

            //if结点
            if (associationList.size() > 1)
            {
                if (f_mapNodes.size() > 0 && (EVO_CODEGEN_WHILE == f_mapNodes[GuidToString(f_sStartNode)]))
                {
                    inWhile = true;
                }
                f_sStartNode = HandelIF(f_sStartNode, f_mapNodes, f_mapWhileNodesList, f_aHuiDianList, f_mapEdges, f_mapVisitNodes);
                if (INVALID_GUID == f_sStartNode)
                {
                    if (inWhile)
                    {
                        sg_iKongGe--;
                        CodeGenCout("}\n",INVALID_GUID,"}");
                    }

                    return INVALID_GUID;
                }
            }

            //终止节点
            else if (associationList.size() == 0)
            {
                if ("ActivityFinalNode" != GetActivityType(f_sStartNode))
                {
                    std::vector<std::string> f_aGuid;
                    f_aGuid.push_back(GuidToString(f_sStartNode));
                    AddResult(EVO_CODEGEN_WARNING_FINAL, f_aGuid);
                }
                OpaqueExpression f_psEffect;
                if (GetActionEffect(f_sStartNode, &f_psEffect) == NO_ERROR && f_psEffect.m_strBody.size() > 0)
                {
                    CodeGenCout(f_psEffect.m_strBody.at(0)+"\n", f_sStartNode);

                }
                CodeGenCout(sg_sReturnString + "\n",INVALID_GUID,"Return");
                if (inWhile)
                {
                    sg_iKongGe--;
                    CodeGenCout("}\n",INVALID_GUID,"}");
                }
                return sReturnNode;
            }

            //开始结点或矩形框结点
            else if (associationList.size() == 1)
            {
                if ("InitialNode" != GetActivityType(f_sStartNode) && "Action" != GetActivityType(f_sStartNode)
                    && "CallBehaviorAction" != GetActivityType(f_sStartNode) && "SendSignalAction" != GetActivityType(f_sStartNode)
                    && "TimerEventAction" != GetActivityType(f_sStartNode) && "AcceptEventAction" != GetActivityType(f_sStartNode)
                    && "JoinNode" != GetActivityType(f_sStartNode))
                {
                    std::vector<std::string> f_aGuid;
                    f_aGuid.push_back(GuidToString(f_sStartNode));
                    AddResult(EVO_CODEGEN_WARNING_ASSOCIATION, f_aGuid);
                }
                sg_mapDelete[GuidToString(f_sStartNode)] = 1;

                OpaqueExpression f_psEffect;
                if (GetActionEffect(f_sStartNode, &f_psEffect) == NO_ERROR && f_psEffect.m_strBody.size() > 0)
                {
                    CodeGenCout(f_psEffect.m_strBody.at(0)+"\n", f_sStartNode);

                }

                std::vector<Association> associationList = GodeGenGetModelOutGoingAssociation(f_sStartNode);
                if (associationList.size() == 0)
                {
                    CodeGenCout(sg_sReturnString + "\n", INVALID_GUID, "Return");
                    if (inWhile)
                    {
                        sg_iKongGe--;
                        CodeGenCout("}\n",INVALID_GUID,"}");
                    }
                    return INVALID_GUID;
                }
                for (Association association : associationList)
                {
                    //普通边：当前结点＝当前结点的后继结点
                    if (0 == f_mapEdges.count(GuidToString(association.m_sBasicInfo.m_sGUID)))
                    {
                        f_sStartNode = association.m_sTargetGUID;
                    }
                    else
                    {
                        int edgeType = f_mapEdges[GuidToString(association.m_sBasicInfo.m_sGUID)];
                        if (EVO_CODEGEN_WHILE_BEGIN == edgeType)
                        {
                            f_sStartNode = association.m_sTargetGUID;
                        }
                        else if (EVO_CODEGEN_DOWHILE_BEGIN == edgeType)
                        {
                            f_sStartNode = association.m_sTargetGUID;
                        }
                        else if (EVO_CODEGEN_BREAK_BEGIN_END == edgeType || EVO_CODEGEN_BREAK_END == edgeType)
                        {
                            CodeGenCout("break;\n", INVALID_GUID,"Break");
                            if (inWhile)
                            {
                                sg_iKongGe--;
                                CodeGenCout("}\n",INVALID_GUID,"}");
                            }
                            return CBR_NODE;
                        }
                        else if (EVO_CODEGEN_WHILE_HUIBIAN == edgeType
                            || EVO_CODEGEN_WHILE_BREAK_HUIBIAN == edgeType

                            )
                        {
                            if (inWhile)
                            {
                                sg_iKongGe--;
                                CodeGenCout("}\n", INVALID_GUID, "}");
                            }
                            return INVALID_GUID;
                        }
                        else if (EVO_CODEGEN_DOWHILE_HUIBIAN == edgeType)
                        {
                            std::vector<std::string> f_aGuid;
                            f_aGuid.push_back(GuidToString(association.m_sBasicInfo.m_sGUID));
                            AddResult(EVO_CODEGEN_WARNING_INTERNAL, f_aGuid);

                            if (inWhile)
                            {
                                sg_iKongGe--;
                                CodeGenCout("}\n", INVALID_GUID, "}");
                            }

                            return INVALID_GUID;
                        }
                        else
                        {
                            std::vector<std::string> f_aGuid;
                            f_aGuid.push_back(GuidToString(association.m_sBasicInfo.m_sGUID));
                            AddResult(EVO_CODEGEN_WARNING_INTERNAL, f_aGuid);
                            if (inWhile)
                            {
                                sg_iKongGe--;
                                CodeGenCout("}\n", INVALID_GUID, "}");
                            }
                            return INVALID_GUID;
                        }
                        //WHILE_END 和 DO_WHILE_END不做操作
                    }
                }

            }
        }
    }
}

std::vector<ActivityDiagramResult> AvtivityCodeGen::FC2Pseudocode(string f_strInputFile, string f_strOutputFile) {
    CodeGenClear();
    GUID initial = ReadFC(f_strInputFile);

    string functionName = GetFunctionName();
    string parameters = GetFunctionParameters();
    if (functionName == "")
        functionName = "function";

    sg_aCodeGenCoutNSpace.push_back(sg_iKongGe);
    sg_aCodeGenCoutGUID.push_back(INVALID_GUID);
    std::string function = "def " + functionName + "(" + parameters + "):\n";
    sg_aCodeGenCoutStr.push_back(function);
    sg_aCodeGenCoutType.push_back("Function");
    sg_aCodeGenCoutFullGUID.push_back(initial);
    sg_iKongGe++;

    std::vector<Association> ass = GetModelOutGoingAssociation(initial);
    if (ass.size() != 1)
    {
        std::cout << "ass.size() != 1" << std::endl;
        system("pause");
    }
    GUID beginNode = ass[0].m_sTargetGUID;
    sg_aCodeGenCoutFullGUID.push_back(beginNode);
    string type = GetFlowchartType(beginNode);
    if (type != "inputoutput")
    {
        std::cout << "type!=inputoutput" << std::endl;
        system("pause");
    }
    ass = GetModelOutGoingAssociation(beginNode);
    if (ass.size() != 1)
    {
        std::cout << "ass.size() != 1" << std::endl;
        system("pause");
    }
    beginNode = ass[0].m_sTargetGUID;

    std::vector<std::map<std::string, int>> f_mapWhileNodesOutSideList;
    std::vector <std::string> f_aHuiDianOutSideList;
    std::map<std::string, int> emptyMap;
    HandleLoop(beginNode, -1, f_mapWhileNodesOutSideList, f_aHuiDianOutSideList, emptyMap, emptyMap);

    ofstream ofs;
    ofs.open(f_strOutputFile, ios::out);
    for (int i = 0; i < sg_aCodeGenCoutNSpace.size(); i++)
    {
        int j = 0;
        while (j < sg_aCodeGenCoutNSpace[i])
        {
            ofs << "    ";
            j++;
        }
        ofs << sg_aCodeGenCoutStr[i];
    }
    return aActivityDiagramResult;
}

//void PrepareActivityGraph(GUID f_sStartNode, std::map<std::string, int>& f_mapVisit)
//{
//    f_mapVisit[GuidToString(f_sStartNode)] = 1;
//    std::vector<Association> associationList = GodeGenGetModelOutGoingAssociation(f_sStartNode);
//    std::map<std::string, int> targetNodeMap;
//    for (Association association : associationList)
//    {
//        GUID targetID = association.m_sTargetGUID;
//        if (targetNodeMap.count(GuidToString(targetID)) == 0)
//        {
//            targetNodeMap[GuidToString(targetID)] = 1;
//        }
//        else
//        {
//            EVO_CODEGEN_MYCOUT << "发现重复的边：" << GuidToString(association.m_sBasicInfo.m_sGUID) << std::endl;
//
//            sg_iReturnCode = sg_iReturnCode | EVO_CODEGEN_ERROR_CHONGFUBIAN;
//        }
//        if (f_mapVisit.count(GuidToString(association.m_sTargetGUID)) == 0)
//        {
//            PrepareActivityGraph(association.m_sTargetGUID, f_mapVisit);
//        }
//    }
//}

std::string AvtivityCodeGen::EvoGetCType(std::string f_sType)
{
    //if ("Boolean" == f_sType)
    //{
    //    return "bool";
    //}
    //else if ("Integer" == f_sType || "Short" == f_sType || "UShort" == f_sType)
    //{
    //    return "int";
    //}
    //else if ("Long" == f_sType || "ULong" == f_sType || "Real" == f_sType || "LongLong" == f_sType || "ULongLong" == f_sType)
    //{
    //    return "long";
    //}
    //else if ("String" == f_sType)
    //{
    //    return "std::string";
    //}
    //else if ("Double" == f_sType || "LongDouble" == f_sType)
    //{
    //    return "double";
    //}
    //else if ("Float" == f_sType)
    //{
    //    return "float";
    //}
    return f_sType;
}
/*
std::vector<ActivityDiagramResult> AvtivityCodeGen::_GenerateActivityCode(GUID f_sGUID, std::string f_sFunName, std::string f_sSavePath)
{
    if (f_sGUID == INVALID_GUID)
    {
        std::vector<std::string> f_aGuid;
        AddResult(EVO_CODEGEN_ERROR_POINT_NULL, f_aGuid);
        return aActivityDiagramResult;
    }

    sg_ofs.open(f_sSavePath, std::ios::out);

    std::vector<DiagramElementInfo> list = GetDiagramElementList(f_sGUID);
    std::vector<Parameter> parameterInList, parameterInOutList, parameterOutList, parameterReturnList;
    GUID intialNode = INVALID_GUID;
    for (DiagramElementInfo temp : list)
    {
        if ("ActivityParameterNode" == GetActivityType(temp.m_sGUID))
        {
            EvoParameter parameter;
            EvoGetActivityParameterNode(temp.m_sGUID, &parameter);
            if (EVO_ENUM_DIRECTION_INPUT == parameter.m_eDirection)
            {
                parameterInList.push_back(parameter);
            }
            else if (EVO_ENUM_DIRECTION_INOUT == parameter.m_eDirection)
            {
                parameterInOutList.push_back(parameter);
            }
            else if (EVO_ENUM_DIRECTION_OUTPUT == parameter.m_eDirection)
            {
                parameterOutList.push_back(parameter);
            }
            else if (EVO_ENUM_DIRECTION_RETURN == parameter.m_eDirection)
            {
                parameterReturnList.push_back(parameter);
            }
        }
        if ("InitialNode" == GetActivityType(temp.m_sGUID))
        {
            intialNode = temp.m_sGUID;
        }
    }

    //PrepareActivityGraph(temp.m_sGUID, emptyMap);
    ElementBasicInfo f_psElementInfo;
    MyGetModelElementBasicInfo(f_sGUID, &f_psElementInfo);
    if (0 == parameterReturnList.size())
    {
        EVO_CODEGEN_MYCOUT << "void" << " ";
        sg_sReturnString = "return;";
    }
    else
    {
        EVO_CODEGEN_MYCOUT << EvoGetCType(parameterReturnList[0].m_ePrimitiveType) << " ";
        sg_sReturnString = "return " + parameterReturnList[0].m_strName + ";";
    }
    int douHaoFlag = 0;
    EVO_CODEGEN_MYCOUT << f_sFunName << "(";
    for (unsigned int i = 0; i < parameterInList.size(); i++)
    {
        if (douHaoFlag)
        {
            EVO_CODEGEN_MYCOUT << ", ";
        }
        else {
            douHaoFlag = 1;
        }
        std::string sType = EvoGetCType(parameterInList[i].m_ePrimitiveType);
        std::string sName = parameterInList[i].m_strName;
        EVO_CODEGEN_MYCOUT << sType << " " << sName;

    }

    for (unsigned int i = 0; i < parameterInOutList.size(); i++)
    {
        if (douHaoFlag)
        {
            EVO_CODEGEN_MYCOUT << ", ";
        }
        else {
            douHaoFlag = 1;
        }
        std::string sType = EvoGetCType(parameterInOutList[i].m_ePrimitiveType);
        std::string sName = parameterInOutList[i].m_strName;
        EVO_CODEGEN_MYCOUT << sType << " &" << sName;
    }

    for (unsigned int i = 0; i < parameterOutList.size(); i++)
    {
        if (douHaoFlag)
        {
            EVO_CODEGEN_MYCOUT << ", ";
        }
        else {
            douHaoFlag = 1;
        }
        std::string sType = EvoGetCType(parameterOutList[i].m_ePrimitiveType);
        std::string sName = parameterOutList[i].m_strName;
        EVO_CODEGEN_MYCOUT << sType << " &" << sName;
    }
    EVO_CODEGEN_MYCOUT << ")\n{" << std::endl; sg_iKongGe++;
    if (intialNode != INVALID_GUID)
    {
        std::vector<std::map<std::string, int>> f_mapWhileNodesOutSideList;
        std::vector <std::string> f_aHuiDianOutSideList;
        std::map<std::string, int> emptyMap;
        HandleLoop(intialNode, -1, f_mapWhileNodesOutSideList, f_aHuiDianOutSideList, emptyMap, emptyMap);
    }
    else
    {
        std::vector<std::string> f_aGuid;
        AddResult(EVO_CODEGEN_ERROR_INITIAL, f_aGuid);

    }
    sg_iKongGe--;
    CodeGenCout("}\n", INVALID_GUID, "}");
    return aActivityDiagramResult;
}
*/
void AvtivityCodeGen::CodeGenClear()
{
    ClearActivity();
    /* 清状态 */
    sg_ofs.close();
    sg_mapJointCount.clear();
    sg_iIndex = 0;
    sg_mapDFN.clear();
    sg_mapLow.clear();
    sg_mapVisit.clear();
    sg_mapDelete.clear();
    sg_aCodeGenCoutNSpace.clear();
    sg_aCodeGenCoutGUID.clear();
    sg_aCodeGenCoutFullGUID.clear();
    sg_aCodeGenCoutStr.clear();
    sg_aCodeGenCoutType.clear();
    sg_iKongGe = 0;
    while (!sg_stack.empty())
    {
        sg_stack.pop();
    }
    sg_stack_map.clear();
    sg_iKongGe = 0;
    sg_sReturnString = "";
    aActivityDiagramResult.clear();
}

/*
std::vector<ActivityDiagramResult> AvtivityCodeGen::GenerateActivityCode(GUID f_sGUID, std::string f_sFunName, std::string f_sSavePath)
{

    std::vector<ActivityDiagramResult> aCheckActivityRes2 = CheckActivity(f_sGUID, false);
    aActivityDiagramResult.insert(aActivityDiagramResult.end(), aCheckActivityRes2.begin(), aCheckActivityRes2.end());
    _GenerateActivityCode(f_sGUID, f_sFunName, f_sSavePath);

    std::vector<ActivityDiagramResult> _aActivityDiagramResult(aActivityDiagramResult);
    CodeGenClear();
    return _aActivityDiagramResult;
}
void AvtivityCodeGen::_CheckActivity(GUID f_sStartNode, GUID f_sDiagramGUID, bool f_iTroubleshooting, std::map<std::string, int>& f_mapVisit)
{
    if (f_mapVisit.count(GuidToString(f_sStartNode)) == 1)
    {
        return;
    }
    f_mapVisit[GuidToString(f_sStartNode)] = 1;

    std::vector<Association> associationList = GodeGenGetModelOutGoingAssociation(f_sStartNode);
    while (associationList.size() == 1)
    {

        f_sStartNode = associationList[0].m_sTargetGUID;
        if (f_mapVisit.count(GuidToString(f_sStartNode)) == 1)
        {
            return;
        }
        else
        {
            f_mapVisit[GuidToString(f_sStartNode)] = 1;
            associationList = GodeGenGetModelOutGoingAssociation(f_sStartNode);
        }
    }
    if (associationList.size() == 0)
    {
        return;
    }
    Association assL, assR;
    unsigned int max;
    if ("DecisionNode" == GetActivityType(f_sStartNode) || "ForkNode" == GetActivityType(f_sStartNode))
    {
        max = 2;
    }
    else
    {
        max = 1;
    }

    if (associationList.size() > max)
    {
        std::vector<std::string> f_aGuid;
        f_aGuid.push_back(GuidToString(f_sStartNode));
        AddResult(EVO_CODEGEN_WARNING_ASSOCIATION, f_aGuid);
        if (f_iTroubleshooting == true)
        {
            assL = associationList[max - 1];

            for (unsigned int i = max; i < associationList.size(); i++)
            {
                assR = associationList[i];

                GUID If;
                EvoCodeGenCreateModel(f_sDiagramGUID, NULL, CLASSNAME_DECISION_NODE, f_sDiagramGUID, &If);

                Association associationAct;
                associationAct.m_strAssociationType = CLASSNAME_CONTROL_FLOW;
                associationAct.m_sSourceGUID = assL.m_sSourceGUID;
                associationAct.m_sTargetGUID = If;
                EvoCodeGenCreateAssociation(f_sDiagramGUID, &associationAct, f_sDiagramGUID, &associationAct.m_sBasicInfo.m_sGUID);

                Association assL_new(assL);
                assL_new.m_sSourceGUID = If;
                assL_new.m_sTargetGUID = assL.m_sTargetGUID;
                EvoCodeGenCreateAssociation(f_sDiagramGUID, &assL_new, f_sDiagramGUID, &assL_new.m_sBasicInfo.m_sGUID);

                OpaqueExpression guardL;
                if (NO_ERROR == GetGuard(assL.m_sBasicInfo.m_sGUID, &guardL))
                {
                    SetGuard(assL_new.m_sBasicInfo.m_sGUID, guardL);
                }

                Trigger triggerL;
                if (NO_ERROR == GetTrigger(assL.m_sBasicInfo.m_sGUID, &triggerL))
                {
                    SetTrigger(assL_new.m_sBasicInfo.m_sGUID, &triggerL);
                }

                EvoGodeGenDeleteModel(assL.m_sBasicInfo.m_sGUID);

                Association assR_new(assR);
                assR_new.m_sSourceGUID = If;
                assR_new.m_sTargetGUID = assR_new.m_sTargetGUID;
                EvoCodeGenCreateAssociation(f_sDiagramGUID, &assR_new, f_sDiagramGUID, &assR_new.m_sBasicInfo.m_sGUID);

                OpaqueExpression guardR;
                if (NO_ERROR == GetGuard(assR.m_sBasicInfo.m_sGUID, &guardR))
                {
                    SetGuard(assR_new.m_sBasicInfo.m_sGUID, guardR);
                }

                Trigger triggerR;
                if (NO_ERROR == GetTrigger(assR.m_sBasicInfo.m_sGUID, &triggerR))
                {
                    SetTrigger(assR_new.m_sBasicInfo.m_sGUID, &triggerR);
                }

                EvoGodeGenDeleteModel(assR.m_sBasicInfo.m_sGUID);

                assL = assR_new;
            }
        }

    }
    std::vector<Association> associationListNew = GodeGenGetModelOutGoingAssociation(f_sStartNode);
    _CheckActivity(associationListNew[0].m_sTargetGUID, f_sDiagramGUID, f_iTroubleshooting, f_mapVisit);
    if (max == 2)
    {
        _CheckActivity(associationListNew[1].m_sTargetGUID, f_sDiagramGUID, f_iTroubleshooting, f_mapVisit);
    }
    return;
}

std::vector<ActivityDiagramResult> AvtivityCodeGen::CheckActivity(GUID f_sDiagramGUID, bool f_iTroubleshooting)
{
    aActivityDiagramResult.clear();

    if (f_sDiagramGUID == INVALID_GUID)
    {
        std::vector<std::string> f_aGuid;
        AddResult(EVO_CODEGEN_ERROR_POINT_NULL, f_aGuid);
    }
    else {
        std::vector<DiagramElementInfo> list = GetDiagramElementList(f_sDiagramGUID);
        GUID intialNode = EVO_INVALID_GUID;
        for (DiagramElementInfo temp : list)
        {
            if ("InitialNode" == GetActivityType(temp.m_sGUID))
            {
                intialNode = temp.m_sGUID;
            }
        }
        if (intialNode != EVO_INVALID_GUID)
        {
            std::map<std::string, int> f_mapVisit;
            _CheckActivity(intialNode, f_sDiagramGUID, f_iTroubleshooting, f_mapVisit);
        }
        else {
            std::vector<std::string> f_aGuid;
            AddResult(EVO_CODEGEN_ERROR_INITIAL, f_aGuid);

        }
    }
    std::vector<ActivityDiagramResult> _aActivityDiagramResult(aActivityDiagramResult);
    aActivityDiagramResult.clear();
    return _aActivityDiagramResult;
}
*/

string AvtivityCodeGen::GetFunctionName()
{

    for (auto iter = GUID_TEXT.begin(); iter != GUID_TEXT.end(); ++iter) {
        std::string guid = iter->first;
        std::string text = iter->second;

        if (GUID_FLOWCHART_TYPE[guid] == "start")
        {
            functionName = text.substr(6);
        }
    }
    return functionName;
}

string AvtivityCodeGen::GetFunctionParameters()
{
    std::string parameters = "";
    for (auto iter = GUID_TEXT.begin(); iter != GUID_TEXT.end(); ++iter) {
        std::string guid = iter->first;
        std::string text = iter->second;

        if (GUID_FLOWCHART_TYPE[guid] == "inputoutput")
        {
            std::string temp = text.substr(0, 6);
            if (temp == "input:") {
                parameters = text.substr(7);
            }
        }
    }
    return parameters;
}