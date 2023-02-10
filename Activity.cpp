#include "Activity.h"
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <codecvt>
using namespace std;

std::map<std::string, std::string> model;
string line;
string res;

std::string Activity::GuidToString(GUID f_sGuid)
{
    return (f_sGuid.type + f_sGuid.id);
}

void Activity::ClearActivity()
{
    GUID_TEXT.clear();
    GUID_OUT_ASSO.clear();
    GUID_IN_ASSO.clear();
    GUID_ACTIVITY_TYPE.clear();
    GUID_FLOWCHART_TYPE.clear();
    GUID_GUARD.clear();
}

GUID Activity::StringToGuid(string s)
{
    if (s == "") {
        return INVALID_GUID;
    }
    GUID guid;
    int i;
    for (i = 0; ; i++)
    {
        if (isdigit(s[i]))
        {
            break;
        }
        guid.type += s[i];
    }

    int j;
    unsigned long  id = 0;
    for (j = i; j < s.size() ; j++)
    {
        id = id *10 + s[j] - '0';
    }
    guid.id = to_string(id);
    return guid;
}

std::wstring readFile(const char* filename)
{
    std::wifstream wif(filename);
    wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
    std::wstringstream wss;
    wss << wif.rdbuf();
    return wss.str();
}

GUID Activity::ReadFC(string file_name) {
    ClearActivity();
    ifstream myfile(file_name, ios::in);
    while (getline(myfile, line)) {
        if (line.size() == 0)
        {
            break;
        }
        string guid = "";
        string type = "";
        string text = "";
        int div;
        for (div = 0;; div++)
        {
            if (line[div] == '=' && line[div + 1] == '>')
            {
                break;
            }
            guid += line[div];
        }
        div += 2;
        for (; ; div++)
        {
            if (line[div] == ':')
            {
                break;
            }
            type += line[div];
        }
        GUID_FLOWCHART_TYPE[guid] = type;
        string evo_type;
        if (type == "start"){
            evo_type = "InitialNode";
            initial = StringToGuid(guid);
        }
        else if (type == "inputoutput") {
            evo_type = "Action";
        }
        else if (type == "operation") {
            evo_type = "Action";
        }
        else if (type == "condition") {
            evo_type = "DecisionNode";
        }
        else if (type == "end") {
            evo_type = "ActivityFinalNode";
        }
        else {
            std::cout << "invalid type!" << std::endl;
            return INVALID_GUID;
        }
        GUID_ACTIVITY_TYPE[guid] = evo_type;
        div+=2;
        for (; div < line.size(); div++)
        {
            text += line[div];
        }
        GUID_TEXT[guid] = text;
    }
    while (getline(myfile, line)) {
        string s_from = "";
        string s_to = "";
        std::string guard = "";
        int div;
        for (div = 0;; div++)
        {
            if (line[div] == '-' && line[div + 1] == '>')
            {
                break;
            }
            if (line[div] == '(')
            {
                break;
            }
            s_from += line[div];
        }
        
        if (line[div] == '(' &&
            line[div + 1] == 'n' &&
            line[div + 2] == 'o' &&
            line[div + 3] == ')')
        {
            div += 4;
        }
        else if (
            line[div] == '(' &&
            line[div + 1] == 'y' &&
            line[div + 2] == 'e' &&
            line[div + 3] == 's' &&
            line[div + 4] == ')'
            )
        {
            guard=GUID_TEXT[s_from];
            div += 5;
        }
        else if (
            line[div] == '(' &&
            line[div + 1] == 'l' &&
            line[div + 2] == 'e' &&
            line[div + 3] == 'f' &&
            line[div + 4] == 't' &&
            line[div + 5] == ')'
            )
        {
            div += 6;
        }
        else if (
            line[div] == '(' &&
            line[div + 1] == 'r' &&
            line[div + 2] == 'i' &&
            line[div + 3] == 'g' &&
            line[div + 4] == 'h' &&
            line[div + 4] == 't' &&
            line[div + 5] == ')'
            )
        {
            div += 7;
        }
        div += 2;
        
        for (;div < line.size(); div++)
        {
            s_to += line[div];
        }
        GUID from = StringToGuid(s_from);
        GUID to = StringToGuid(s_to);
        Association ass;
        int j = 0;
        ass.m_sBasicInfo.m_sGUID.id = (s_from + "TO" + s_to);
        ass.m_sSourceGUID = from;
        ass.m_sTargetGUID = to;
        GUID_OUT_ASSO[s_from].push_back(ass);
        GUID_IN_ASSO[s_to].push_back(ass);
        if(guard != "")
            GUID_GUARD[GuidToString(ass.m_sBasicInfo.m_sGUID)] = GUID_TEXT[s_from];
    }
    return initial;
}

std::string Activity::GetActivityType(const GUID f_sModelGUID)
{
    return GUID_ACTIVITY_TYPE[GuidToString(f_sModelGUID)];
}

std::string Activity::GetFlowchartType(const GUID f_sModelGUID)
{
    return GUID_FLOWCHART_TYPE[GuidToString(f_sModelGUID)];
}

std::vector<Association> Activity::GetModelOutGoingAssociation(GUID f_sModelGUID)
{
    return GUID_OUT_ASSO[GuidToString(f_sModelGUID)];
}

std::vector<Association> Activity::GetModelInComingAssociation(GUID f_sModelGUID)
{
    return GUID_IN_ASSO[GuidToString(f_sModelGUID)];
}

std::string Activity::GetGuard(GUID model)
{
    return GUID_GUARD[GuidToString(model)];
}

ReturnCode Activity::GetActionEffect(const GUID f_sModelGUID, OpaqueExpression* f_psEffect) {
    string type = GetActivityType(f_sModelGUID);
    if(type!="DecisionNode" && type != "InitialNode" && type != "ActivityFinalNode")
        f_psEffect->m_strBody.push_back(GUID_TEXT[GuidToString(f_sModelGUID)]);
    return NO_ERROR;
}

ReturnCode Activity::GetModelElementBasicInfo(const GUID f_sModelGUID, ElementBasicInfo* f_psElementInfo)
{
    f_psElementInfo->m_sGUID = f_sModelGUID;
    f_psElementInfo->m_strElementCnName = GUID_TEXT[GuidToString(f_sModelGUID)];
    return NO_ERROR;
}

std::string Activity::GuidToText(GUID guid)
{
    return GUID_TEXT[GuidToString(guid)];
}

