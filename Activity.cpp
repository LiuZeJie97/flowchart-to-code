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
    for (i = 0; i<s.size(); i++)
    {
        if (isdigit(s[i]) or s[i]=='-')
        {
            break;
        }
        guid.type += s[i];
    }
    int flag = 1;
    if (s[i] == '-')
    {
        flag = -1;
        ++i;
    }
    int j;
    long  id = 0;
    for (j = i; j < s.size() ; j++)
    {
        id = id *10 + s[j] - '0';
    }
    guid.id = to_string(id * flag);
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

    if (!myfile.is_open())
    {
        cout << file_name <<" do not exist!" << endl;
        exit(-1);
    }

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
        if (!isdigit(guid[guid.size() - 1]))
        {
            guid += "-1";
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
        string activity_type;
        if (type == "start"){
            activity_type = "InitialNode";
            initial = StringToGuid(guid);
        }
        else if (type == "inputoutput") {
            activity_type = "Action";
        }
        else if (type == "operation") {
            activity_type = "Action";
        }
        else if (type == "condition") {
            activity_type = "DecisionNode";
        }
        else if (type == "end") {
            activity_type = "ActivityFinalNode";
        }
        else {
            std::cout << "invalid type!" << std::endl;
            return INVALID_GUID;
        }
        GUID_ACTIVITY_TYPE[guid] = activity_type;
        div+=2;
        for (; div < line.size(); div++)
        {
            text += line[div];
        }
        GUID_TEXT[guid] = text;
    }
    while (getline(myfile, line)) {
        if (line.size() == 0)
        {
            break;
        }
        string s_from = "";
        string s_to = "";
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
        if (!isdigit(s_from[s_from.size() - 1]))
        {
            s_from += "-1";
        }
        string guard = "";
        if (line[div] == '(')
        {
            ++div;
            while(line[div]!=')')
            {
                guard += line[div];
                ++div;
            }
            ++div;
        }
        if (guard=="yes")
        {
            guard=GUID_TEXT[s_from];
        }
        else if (guard == "no")
        {
            guard = "";
        }
        /*else if (
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
        }*/
        div += 2;
        
        for (;div < line.size(); div++)
        {
            if (line[div] == ' ')
                break;
            s_to += line[div];
        }
        if (!isdigit(s_to[s_to.size() - 1]))
        {
            s_to += "-1";
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
            GUID_GUARD[GuidToString(ass.m_sBasicInfo.m_sGUID)] = guard;
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

