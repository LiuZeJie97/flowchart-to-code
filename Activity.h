#pragma once

#include<string>
#include<vector>
#include<map>
using namespace std;

#define CLASSNAME_CONTROL_FLOW                          "ControlFlow"             /* ������               */
#define CLASSNAME_OBJECT_FLOW                           "ObjectFlow"              /* ������               */

/* �ڵ�*/
typedef struct _GUID
{
	string type;                                    /* �ڵ�����       */
	string id;                                      /* �ڵ�ID         */
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


/* ģ�ͻ�����Ϣ */
typedef struct _ElementBasicInfo
{
	std::string m_strElementCnName;                 /* ģ��Ԫ���������� */
	std::string m_strElementEnName;                 /* ģ��Ԫ��Ӣ������ */
	GUID m_sGUID;                                   /* GUID             */

	_ElementBasicInfo() {
		m_strElementCnName = "";
		m_sGUID = INVALID_GUID;
		m_strElementEnName = "";
	}
}ElementBasicInfo;

/* �� */
typedef struct _Association
{
	ElementBasicInfo        m_sBasicInfo;           /* ������Ϣ         */
	std::string             m_strAssociationType;   /* ��ϵ����         */
	GUID                    m_sSourceGUID;          /* Դ�ڵ�GUID       */
	GUID                    m_sTargetGUID;          /* Ŀ��ڵ�GUID     */
}Association;

/* ��͸�����ʽ */
typedef struct _OpaqueExpression
{
	std::vector<std::string> m_strBody;             /* ���ʽ���� */
}OpaqueExpression;

typedef enum _ReturnCode
{
	NO_ERROR = 0,                                   /* �ɹ� */
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
	*��������
	*   ��ȡ����ͼ.txt�ļ�
	*�������
	*   file_name	: ����ͼ·��
	*�������
	*   ��
	*����ֵ
	*   
	*/
	GUID ReadFC(string file_name);

	/********************************************************/
	/*
	*��������
	*   ��ȡģ�͵ĳ��ߡ�
	*�������
	*   f_sModelGUID	: ģ��GUID��
	*�������
	*   ��
	*����ֵ
	*   std::vector<Association> : �����б�
	*/
	std::vector<Association> GetModelOutGoingAssociation(GUID f_sModelGUID);

	/********************************************************/
	/*
	*��������
	*   ��ȡģ�͵���ߡ�
	*�������
	*   f_sModelGUID	: ģ��GUID��
	*�������
	*   ��
	*����ֵ
	*   std::vector<Association> : ����б�
	*/
	std::vector<Association> GetModelInComingAssociation(GUID f_sModelGUID);


	/********************************************************/
	/*
	*��������
	*   ��ȡģ�ͻ������ԡ�
	*�������
	*   f_sModelGUID    : ģ��Ԫ��GUID��
	*�������
	*   f_psElementInfo : ģ��Ԫ�ػ�����Ϣ��
	*����ֵ
	*   ReturnCode   : ����������롣
	*/
	ReturnCode GetModelElementBasicInfo(const GUID f_sModelGUID, ElementBasicInfo* f_psElementInfo);


	/********************************************************/
	/*
	*��������
	*   ��ȡģ���������ơ�
	*�������
	*   f_sModelGUID    : ģ��Ԫ��GUID��
	*�������
	*   �ޡ�
	*����ֵ
	*   std::string     : ģ��Ԫ�����͡�
	*/
	std::string GetActivityType(const GUID f_sModelGUID);
	std::string GetFlowchartType(const GUID f_sModelGUID);

	/********************************************************/
	/*
	*��������
	*   ��ȡ����������effect����͸�����ʽ��α���룩��
	*�������
	*   f_sModelGUID  : ģ��GUID��
	*�������
	*   f_psEffect    : ��͸�����ʽ��
	*����ֵ
	*   ReturnCode : ����������롣
	*/
	ReturnCode GetActionEffect(const GUID f_sModelGUID, OpaqueExpression* f_psEffect);

	std::string GetGuard(GUID model);

	std::string GuidToString(GUID f_sGuid);

	void ClearActivity();

	GUID StringToGuid(string s);

	std::string GuidToText(GUID guid);


};