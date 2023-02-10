#pragma once

#include<string>
#include<vector>
#include<map>
using namespace std;

#define CLASSNAME_CONTROL_FLOW                          "ControlFlow"             /* 控制流               */
#define CLASSNAME_OBJECT_FLOW                           "ObjectFlow"              /* 对象流               */

/* 节点*/
typedef struct _GUID
{
	string type;                                    /* 节点类型       */
	string id;                                      /* 节点ID         */
	_GUID() {
		type = "";
		id = "";
	}
	bool operator==(_GUID r)
	{
		if (type == r.type && id == r.id)
			return true;
		else
			return false;
	}
	bool operator!=(_GUID r)
	{
		if (type == r.type && id == r.id)
			return false;
		else
			return true;
	}
}GUID;
static GUID INVALID_GUID;


/* 模型基础信息 */
typedef struct _ElementBasicInfo
{
	std::string m_strElementCnName;                 /* 模型元素中文名称 */
	std::string m_strElementEnName;                 /* 模型元素英文名称 */
	GUID m_sGUID;                                   /* GUID             */

	_ElementBasicInfo() {
		m_strElementCnName = "";
		m_sGUID = INVALID_GUID;
		m_strElementEnName = "";
	}
}ElementBasicInfo;

/* 边 */
typedef struct _Association
{
	ElementBasicInfo        m_sBasicInfo;           /* 基础信息         */
	std::string             m_strAssociationType;   /* 关系类型         */
	GUID                    m_sSourceGUID;          /* 源节点GUID       */
	GUID                    m_sTargetGUID;          /* 目标节点GUID     */
}Association;

/* 不透明表达式 */
typedef struct _OpaqueExpression
{
	std::vector<std::string> m_strBody;             /* 表达式内容 */
}OpaqueExpression;

typedef enum _ReturnCode
{
	NO_ERROR = 0,                                   /* 成功 */
}ReturnCode;


class Activity
{
public:
	std::map<string, string> GUID_TEXT;
	std::map<string, std::vector<Association>> GUID_OUT_ASSO;
	std::map<string, std::vector<Association>> GUID_IN_ASSO;
	std::map<string, string> GUID_ACTIVITY_TYPE;
	std::map<string, string> GUID_FLOWCHART_TYPE;
	std::map<string, string> GUID_GUARD;
	GUID initial;

	/********************************************************/
	/*
	*函数描述
	*   读取流程图.txt文件
	*输入参数
	*   file_name	: 流程图路径
	*输出参数
	*   无
	*返回值
	*   
	*/
	GUID ReadFC(string file_name);

	/********************************************************/
	/*
	*函数描述
	*   获取模型的出边。
	*输入参数
	*   f_sModelGUID	: 模型GUID。
	*输出参数
	*   无
	*返回值
	*   std::vector<Association> : 出边列表。
	*/
	std::vector<Association> GetModelOutGoingAssociation(GUID f_sModelGUID);

	/********************************************************/
	/*
	*函数描述
	*   获取模型的入边。
	*输入参数
	*   f_sModelGUID	: 模型GUID。
	*输出参数
	*   无
	*返回值
	*   std::vector<Association> : 入边列表。
	*/
	std::vector<Association> GetModelInComingAssociation(GUID f_sModelGUID);


	/********************************************************/
	/*
	*函数描述
	*   获取模型基础属性。
	*输入参数
	*   f_sModelGUID    : 模型元素GUID。
	*输出参数
	*   f_psElementInfo : 模型元素基础信息。
	*返回值
	*   ReturnCode   : 操作结果编码。
	*/
	ReturnCode GetModelElementBasicInfo(const GUID f_sModelGUID, ElementBasicInfo* f_psElementInfo);


	/********************************************************/
	/*
	*函数描述
	*   获取模型类型名称。
	*输入参数
	*   f_sModelGUID    : 模型元素GUID。
	*输出参数
	*   无。
	*返回值
	*   std::string     : 模型元素类型。
	*/
	std::string GetActivityType(const GUID f_sModelGUID);
	std::string GetFlowchartType(const GUID f_sModelGUID);

	/********************************************************/
	/*
	*函数描述
	*   获取基本动作的effect（不透明表达式：伪代码）。
	*输入参数
	*   f_sModelGUID  : 模型GUID。
	*输出参数
	*   f_psEffect    : 不透明表达式。
	*返回值
	*   ReturnCode : 操作结果编码。
	*/
	ReturnCode GetActionEffect(const GUID f_sModelGUID, OpaqueExpression* f_psEffect);

	std::string GetGuard(GUID model);

	std::string GuidToString(GUID f_sGuid);

	void ClearActivity();

	GUID StringToGuid(string s);

	std::string GuidToText(GUID guid);


};