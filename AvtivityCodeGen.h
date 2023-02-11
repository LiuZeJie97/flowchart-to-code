#pragma once

#include"Activity.h"
#include<fstream> 
#include<string>
#include<vector>
#include<stack>

//#define EVO_CODEGEN_INVALID_TYPE          -1		/* ��Ч�ڵ�                  */

#define EVO_CODEGEN_IF                      0       /* if�ڵ�                    */
/*                           */
#define EVO_CODEGEN_HUIBIAN                 990     /* �ر�                      */
													/*                           */
#define EVO_CODEGEN_WHILE                   10      /* while�ڵ�                 */
#define EVO_CODEGEN_WHILE_BEGIN             11      /* while�ṹ����ʼ��         */
#define EVO_CODEGEN_WHILE_HUIBIAN           991     /* while�ṹ�Ļر�           */
#define EVO_CODEGEN_WHILE_END               13      /* while�ṹ�Ľ�����         */
													/*                           */
#define EVO_CODEGEN_DO                      20      /* do     �ڵ�               */
#define EVO_CODEGEN_DOWHILE                 21      /* while  �ڵ�               */
#define EVO_CODEGEN_DOWHILE_BEGIN           22      /* dowhile�ṹ����ʼ��       */
#define EVO_CODEGEN_DOWHILE_HUIBIAN         992     /* dowhile�ṹ�Ļر�         */
#define EVO_CODEGEN_DOWHILE_END             24      /* dowhile�ṹ�Ľ�����       */
													/*                           */
#define EVO_CODEGEN_WHILE_BREAK             31      /* while(True)�ڵ�           */
#define EVO_CODEGEN_WHILE_BREAK_BEGIN       32      /* while(True)�ṹ����ʼ��   */
#define EVO_CODEGEN_WHILE_BREAK_HUIBIAN     993     /* while(True)�ṹ�Ļر�     */
#define EVO_CODEGEN_WHILE_BREAK_END         33      /* while(True)�ṹ�Ľ�����   */
													/*                           */
													/*                           */
#define EVO_CODEGEN_BREAK                   100     /* break�ڵ�                 */
#define EVO_CODEGEN_BREAK_BEGIN             101     /* break�ṹ����ʼ��         */
#define EVO_CODEGEN_BREAK_END               102     /* break�ṹ�Ľ�����         */
#define EVO_CODEGEN_BREAK_BEGIN_END         103     /* break�ṹ����ʼ/������    */
													/*                           */

#define EVO_CODEGEN_RETURN                  130     /* return�ڵ�                */
#define EVO_CODEGEN_RETURN_BEGIN            131     /* return�ṹ����ʼ��        */

//#define EVO_CODEGEN_MYCOUT EvoKongGe2(sg_iKongGe);sg_ofs   /* ������ļ�    */
#define EVO_CODEGEN_MYCOUT EvoKongGe(sg_iKongGe);std::cout  /* ���������̨  */

/* ��������ֵ��� */
#define EVO_CODEGEN_NO_ERROR				0				/* �ɹ�										*/

/* WARNING �ûͼ���ɵĴ���������� */
#define EVO_CODEGEN_WARNING_FEIJIEGOU		1				/* �ǽṹ���ͼ							*/
#define EVO_CODEGEN_WARNING_FINAL			2				/* ���ս��û�г���							*/
#define EVO_CODEGEN_WARNING_ASSOCIATION		3				/* �ڵ���߻������������ȷ					*/
#define EVO_CODEGEN_WARNING_ASSOCIATION_2	4				/* �����ظ��ı�								*/
#define EVO_CODEGEN_WARNING_ELEMENT			5				/* ���ڲ�֧�ֵĽڵ�							*/
#define EVO_CODEGEN_WARNING_IF				6				/* ��֧�ṹ��㲻һ��						*/
#define EVO_CODEGEN_WARNING_INTERNAL		7				/* �ڲ�����									*/

/* ERROR �ûͼ�޷����ɴ��� */
#define EVO_CODEGEN_ERROR_INITIAL			10				/* û���ҵ�INITIAL�ڵ� �� diagram���ǻͼ	*/
#define EVO_CODEGEN_ERROR_POINT_NULL		11				/* ��ָ��									*/

/* ״̬ͼ�������ɽ�� */
#define EVO_CODEGEN_ERR_DIAGRAM				12				/* ͼ���ڴ���                */

enum ErrotType {
	EN_EVO_CODEGEN_WARNING = 1,
	EN_EVO_CODEGEN_ERROR = 2
};

typedef struct _ActivityDiagramResult
{
	ErrotType m_errotType;
	/* int ������ */
	int m_iErrorCode;

	/* string ������Ϣ */
	string m_stInf;

	/* ����Ԫ�ص�guid */
	std::vector<string> aGuid;
	
}ActivityDiagramResult;

class AvtivityCodeGen :public Activity
{
public:
	std::string functionName;	/* �������� */
	std::string parameters;		/* �������� */

	int sg_iIndex;
	int sg_iKongGe;
	std::ofstream sg_ofs;
	std::map<std::string, int> sg_mapJointCount;
	std::map<std::string, int> sg_mapDFN;
	std::map<std::string, int> sg_mapLow;
	std::map<std::string, int> sg_mapVisit;
	std::map<std::string, int> sg_mapDelete;
	std::map<std::string, int> sg_stack_map;
	std::stack<GUID> sg_stack;
	std::vector<int> sg_aCodeGenCoutNSpace;							
	std::vector<GUID> sg_aCodeGenCoutGUID;
	std::vector<GUID> sg_aCodeGenCoutFullGUID;
	std::vector<std::string> sg_aCodeGenCoutStr;
	std::vector<std::string> sg_aCodeGenCoutType;
	std::vector<ActivityDiagramResult> aActivityDiagramResult;
	std::string sg_sReturnString;
	GUID CBR_NODE = INVALID_GUID;

	AvtivityCodeGen()
	{
		sg_iIndex = 0;
		sg_iKongGe = 0;
	}

	/********************************************************/
	/*
	*��������
	*	��FC2Code���ݼ�ת��Ϊα����
	*�������
	*	f_strInputFile			: ����ͼ.txt�ļ���·��
	*	f_strOutputFile			: α����.txt�ļ���·��
	*�������
	*
	*����ֵ
	*	std::vector<ActivityDiagramResult> :	�����������
	*/
	std::vector<ActivityDiagramResult> FC2Pseudocode(string f_strInputFile, string f_strOutputFile);

	/********************************************************/
	/*
	*��������
	*   �ҵ�ѭ����ʶ��ѭ���ڵ�Ԫ�ء�
	*�������
	*   f_sStartNode			: �����ͨ��ͼ���ڵ�
	*   f_mapNodes				: �����ͨ��ͼ�����Ľڵ�
	*�������
	*   f_mapWhileNodesList		: �ڲ���ͨ��ͼ�����Ľڵ�
	*   f_mapWhileEdges			: �ڲ���ͨ��ͼ�������
	*	f_aReturnNodesList		: �رߵĻ��
	*����ֵ
	*   ��
	*/
	void FindWhile(GUID f_sStartNode, std::map<std::string, int>& f_mapNodes, std::vector<std::map<std::string, int>>* f_mapWhileNodesList, std::map<std::string, int>& f_mapWhileEdges, std::vector<std::string>& f_aReturnNodesList);
	void _FindWhile(GUID f_sStartNode, std::map<std::string, int>& f_mapNodes, std::vector<std::map<std::string, int>>* f_mapWhileNodesList, std::map<std::string, int>& f_mapWhileEdges, std::vector<std::string>& f_aReturnNodesList);

	/********************************************************/
	/*
	*��������
	*	ʶ�𼫴���ͨ��ͼ�ڵ�CBRԪ��
	*�������
	*	f_sRoot						: ���ڵ�
	*�������
	*	f_mapWhileNodes				: �ڲ���ͨ��ͼ�����Ľڵ�
	*   whf_mapWhileEdgesileEdges	: �ڲ���ͨ��ͼ�������
	*	f_sHuiDian					: �ر�
	*����ֵ
	*
	*/
	void FindWhileDoWhileCBR(GUID f_sRoot, std::map<std::string, int>& f_mapWhileNodes, std::map<std::string, int>& f_mapWhileEdges, GUID& f_sHuiDian);

	/********************************************************/
	/*
	* ��������
	*   �����֧�ڵ�
	* ����
	*   f_sStartNode		: ��֧�ڵ�GUID
	*	f_mapNodes			: ����ڵ�
	*	f_mapEdges			: �����
	*	f_mapWhileNodesList	: ѭ���ڵĽڵ�
	*	f_aHuiDianList		: �ر�
	* 	f_mapVisitNodes		: �Ѿ��������Ľڵ�
	* ���
	*   GUID  :��֧��ϵ���߰�ṹ����㣬NULL��ʾδ�ҵ���㣮
	*/
	GUID HandelIF(const GUID f_sStartNode, std::map<std::string, int> f_mapNodes, std::vector<std::map<std::string, int>> f_mapWhileNodesOutSideList, std::vector<std::string> f_aHuiDianOutSideList, std::map<std::string, int>& f_mapEdges, std::map<std::string, int>& f_mapVisitNodes);

	/********************************************************/
	/*
	*��������
	*	����ѭ���ڵ�
	*�������
	*	f_sStartNode	: ��ʼ�ڵ�
	*	f_mapNodes		: ����ڵ㣬ÿһ�ε��øú���ʱ��f_mapNodes������ͬ����˲������ܹ���
	*	f_mapEdges		: ����ߣ�ÿһ�ε��øú���ʱ��f_mapEdges������ͬ����˲������ܹ���
	* 	f_mapVisitNodes	: �Ѿ��������Ľڵ�
	*�������
	*	
	*����ֵ
	*	GUID		: ��һ����δ�����Ľڵ�
	*/
	GUID HandleLoop(GUID f_sStartNode, int f_iMapNodes, std::vector<std::map<std::string, int>> f_mapWhileNodesOutSideList, std::vector <std::string> f_aHuiDianOutSideList, std::map<std::string, int> f_mapEdges, std::map<std::string, int>& f_mapVisitNodes);

	/********************************************************/
	/*
	*��������
	*	����break��return
	*�������
	*	f_sStartNode	: ��ʼ�ڵ�
	*	f_mapEdges		: �����
	* 	f_mapVisitNodes	: �Ѿ��������Ľڵ�
	*�������
	*
	*����ֵ
	*	GUID		: ��һ����δ�����Ľڵ�
	*/

	/********************************************************/
	/*
	*��������
	*	�������ͼ�ڵ㵽����̨���ļ���. 
	*	The current output code is python, you can rewrite this function to output in other languages such as c or java. 
	*�������
	*	f_str		: �ڵ��ڵ�����
	*	f_sGuid		: �ڵ��id
	* 	f_strType	: �ڵ������
	*/
	void CodeGenCout(std::string f_str, GUID f_sGuid = INVALID_GUID, std::string f_strType = "Effect");

	void CodeGenClear();

	void AddResult(int f_iErrorCode, std::vector<std::string> f_aGuid);

	void AddKongGe(int n);

	void AddKongGe2(int n);

	std::vector<Association> GodeGenSortAssociation(std::vector<Association> ass);

	std::vector<Association> GodeGenGetModelOutGoingAssociation(GUID f_sModelGUID);

	std::vector<Association> CodeGenGetModelInComingAssociation(GUID f_sModelGUID);

	string GetFunctionName();

	string GetFunctionParameters();

	std::string EvoGetCType(std::string f_sType);

	void BFS(GUID f_sStartNode, std::vector<std::string>& f_aVec, std::map<std::string, int>& f_mapWhileNodes);

	GUID RecognizeIFPath(GUID f_sStartNode, std::map<std::string, int> f_mapNodes, std::vector<std::map<std::string, int>> f_mapWhileNodesOutSideList, std::vector<std::string> f_aHuiDianOutSideList, std::map<std::string, int>& f_mapEdges, std::map<std::string, int> f_mapVisitNodes);

	//std::vector<ActivityDiagramResult> _GenerateActivityCode(GUID f_sGUID, std::string f_sFunName, std::string f_sSavePath);

	//void _CheckActivity(GUID f_sStartNode, GUID f_sDiagramGUID, bool f_iTroubleshooting, std::map<std::string, int>& f_mapVisit);

	/********************************************************/
	/*
	*��������
	*	ʹ�ûͼ���ɴ���
	*�������
	*	f_sGUID		: �ͼ��GUID
	*	f_sFunName	: ����������
	*	f_sSavePath	: ���뱣��·��
	*�������
	*
	*����ֵ
	*	std::vector<ActivityDiagramResult> :	�����������
	*/
	//std::vector<ActivityDiagramResult> GenerateActivityCode(GUID f_sGUID, std::string f_sFunName, std::string f_sSavePath);

	/********************************************************/
	/*
	*��������
	*	���ͼ�Ƿ�������ɴ���
	*�������
	*	f_sDiagramGUID		: �ͼ��GUID
	*	f_iTroubleshooting	: �Ƿ��Զ��޸Ļͼ�еĴ��ڵĴ���
	*�������
	*
	*����ֵ
	*	std::vector<ActivityDiagramResult> : �����������
	*/
	//std::vector<ActivityDiagramResult>  CheckActivity(GUID f_sDiagramGUID, bool f_iTroubleshooting = false);
};