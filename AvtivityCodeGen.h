#pragma once

#include"Activity.h"
#include<fstream> 
#include<string>
#include<vector>
#include<stack>

//#define EVO_CODEGEN_INVALID_TYPE          -1		/* 无效节点                  */

#define EVO_CODEGEN_IF                      0       /* if节点                    */
/*                           */
#define EVO_CODEGEN_HUIBIAN                 990     /* 回边                      */
													/*                           */
#define EVO_CODEGEN_WHILE                   10      /* while节点                 */
#define EVO_CODEGEN_WHILE_BEGIN             11      /* while结构的起始边         */
#define EVO_CODEGEN_WHILE_HUIBIAN           991     /* while结构的回边           */
#define EVO_CODEGEN_WHILE_END               13      /* while结构的结束边         */
													/*                           */
#define EVO_CODEGEN_DO                      20      /* do     节点               */
#define EVO_CODEGEN_DOWHILE                 21      /* while  节点               */
#define EVO_CODEGEN_DOWHILE_BEGIN           22      /* dowhile结构的起始边       */
#define EVO_CODEGEN_DOWHILE_HUIBIAN         992     /* dowhile结构的回边         */
#define EVO_CODEGEN_DOWHILE_END             24      /* dowhile结构的结束边       */
													/*                           */
#define EVO_CODEGEN_WHILE_BREAK             31      /* while(True)节点           */
#define EVO_CODEGEN_WHILE_BREAK_BEGIN       32      /* while(True)结构的起始边   */
#define EVO_CODEGEN_WHILE_BREAK_HUIBIAN     993     /* while(True)结构的回边     */
#define EVO_CODEGEN_WHILE_BREAK_END         33      /* while(True)结构的结束边   */
													/*                           */
													/*                           */
#define EVO_CODEGEN_BREAK                   100     /* break节点                 */
#define EVO_CODEGEN_BREAK_BEGIN             101     /* break结构的起始边         */
#define EVO_CODEGEN_BREAK_END               102     /* break结构的结束边         */
#define EVO_CODEGEN_BREAK_BEGIN_END         103     /* break结构的起始/结束边    */
													/*                           */

#define EVO_CODEGEN_RETURN                  130     /* return节点                */
#define EVO_CODEGEN_RETURN_BEGIN            131     /* return结构的起始边        */

//#define EVO_CODEGEN_MYCOUT EvoKongGe2(sg_iKongGe);sg_ofs   /* 输出到文件    */
#define EVO_CODEGEN_MYCOUT EvoKongGe(sg_iKongGe);std::cout  /* 输出到控制台  */

/* 操作返回值结果 */
#define EVO_CODEGEN_NO_ERROR				0				/* 成功										*/

/* WARNING 该活动图生成的代码可能有误 */
#define EVO_CODEGEN_WARNING_FEIJIEGOU		1				/* 非结构化活动图							*/
#define EVO_CODEGEN_WARNING_FINAL			2				/* 非终结点没有出边							*/
#define EVO_CODEGEN_WARNING_ASSOCIATION		3				/* 节点出边或入边数量不正确					*/
#define EVO_CODEGEN_WARNING_ASSOCIATION_2	4				/* 存在重复的边								*/
#define EVO_CODEGEN_WARNING_ELEMENT			5				/* 存在不支持的节点							*/
#define EVO_CODEGEN_WARNING_IF				6				/* 分支结构汇点不一致						*/
#define EVO_CODEGEN_WARNING_INTERNAL		7				/* 内部错误									*/

/* ERROR 该活动图无法生成代码 */
#define EVO_CODEGEN_ERROR_INITIAL			10				/* 没有找到INITIAL节点 或 diagram不是活动图	*/
#define EVO_CODEGEN_ERROR_POINT_NULL		11				/* 空指针									*/

/* 状态图代码生成结果 */
#define EVO_CODEGEN_ERR_DIAGRAM				12				/* 图存在错误                */

enum ErrotType {
	EN_EVO_CODEGEN_WARNING = 1,
	EN_EVO_CODEGEN_ERROR = 2
};

typedef struct _ActivityDiagramResult
{
	ErrotType m_errotType;
	/* int 错误码 */
	int m_iErrorCode;

	/* string 错误信息 */
	string m_stInf;

	/* 错误元素的guid */
	std::vector<string> aGuid;
	
}ActivityDiagramResult;

class AvtivityCodeGen :public Activity
{
public:
	std::string functionName;	/* 函数名称 */
	std::string parameters;		/* 函数参数 */

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
	*函数描述
	*	将FC2Code数据集转化为伪代码
	*输入参数
	*	f_strInputFile			: 流程图.txt文件的路径
	*	f_strOutputFile			: 伪代码.txt文件的路径
	*输出参数
	*
	*返回值
	*	std::vector<ActivityDiagramResult> :	操作结果编码
	*/
	std::vector<ActivityDiagramResult> FC2Pseudocode(string f_strInputFile, string f_strOutputFile);

	/********************************************************/
	/*
	*函数描述
	*   找到循环并识别循环内的元素。
	*输入参数
	*   f_sStartNode			: 外层连通子图根节点
	*   f_mapNodes				: 外层连通子图包含的节点
	*输出参数
	*   f_mapWhileNodesList		: 内层连通子图包含的节点
	*   f_mapWhileEdges			: 内层连通子图的特殊边
	*	f_aReturnNodesList		: 回边的会点
	*返回值
	*   无
	*/
	void FindWhile(GUID f_sStartNode, std::map<std::string, int>& f_mapNodes, std::vector<std::map<std::string, int>>* f_mapWhileNodesList, std::map<std::string, int>& f_mapWhileEdges, std::vector<std::string>& f_aReturnNodesList);
	void _FindWhile(GUID f_sStartNode, std::map<std::string, int>& f_mapNodes, std::vector<std::map<std::string, int>>* f_mapWhileNodesList, std::map<std::string, int>& f_mapWhileEdges, std::vector<std::string>& f_aReturnNodesList);

	/********************************************************/
	/*
	*函数描述
	*	识别极大连通子图内的CBR元素
	*输入参数
	*	f_sRoot						: 根节点
	*输出参数
	*	f_mapWhileNodes				: 内层连通子图包含的节点
	*   whf_mapWhileEdgesileEdges	: 内层连通子图的特殊边
	*	f_sHuiDian					: 回边
	*返回值
	*
	*/
	void FindWhileDoWhileCBR(GUID f_sRoot, std::map<std::string, int>& f_mapWhileNodes, std::map<std::string, int>& f_mapWhileEdges, GUID& f_sHuiDian);

	/********************************************************/
	/*
	* 函数描述
	*   处理分支节点
	* 输入
	*   f_sStartNode		: 分支节点GUID
	*	f_mapNodes			: 特殊节点
	*	f_mapEdges			: 特殊边
	*	f_mapWhileNodesList	: 循环内的节点
	*	f_aHuiDianList		: 回边
	* 	f_mapVisitNodes		: 已经遍历过的节点
	* 输出
	*   GUID  :分支汇合点或者半结构化结点，NULL表示未找到汇点．
	*/
	GUID HandelIF(const GUID f_sStartNode, std::map<std::string, int> f_mapNodes, std::vector<std::map<std::string, int>> f_mapWhileNodesOutSideList, std::vector<std::string> f_aHuiDianOutSideList, std::map<std::string, int>& f_mapEdges, std::map<std::string, int>& f_mapVisitNodes);

	/********************************************************/
	/*
	*函数描述
	*	处理循环节点
	*输入参数
	*	f_sStartNode	: 开始节点
	*	f_mapNodes		: 特殊节点，每一次调用该函数时“f_mapNodes”不相同，因此参数不能共享
	*	f_mapEdges		: 特殊边，每一次调用该函数时“f_mapEdges”不相同，因此参数不能共享
	* 	f_mapVisitNodes	: 已经遍历过的节点
	*输出参数
	*	
	*返回值
	*	GUID		: 下一个还未遍历的节点
	*/
	GUID HandleLoop(GUID f_sStartNode, int f_iMapNodes, std::vector<std::map<std::string, int>> f_mapWhileNodesOutSideList, std::vector <std::string> f_aHuiDianOutSideList, std::map<std::string, int> f_mapEdges, std::map<std::string, int>& f_mapVisitNodes);

	/********************************************************/
	/*
	*函数描述
	*	处理break和return
	*输入参数
	*	f_sStartNode	: 开始节点
	*	f_mapEdges		: 特殊边
	* 	f_mapVisitNodes	: 已经遍历过的节点
	*输出参数
	*
	*返回值
	*	GUID		: 下一个还未遍历的节点
	*/

	/********************************************************/
	/*
	*函数描述
	*	输出流程图节点到控制台和文件中. 
	*	The current output code is python, you can rewrite this function to output in other languages such as c or java. 
	*输入参数
	*	f_str		: 节点内的文字
	*	f_sGuid		: 节点的id
	* 	f_strType	: 节点的类型
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
	*函数描述
	*	使用活动图生成代码
	*输入参数
	*	f_sGUID		: 活动图的GUID
	*	f_sFunName	: 函数的名称
	*	f_sSavePath	: 代码保存路径
	*输出参数
	*
	*返回值
	*	std::vector<ActivityDiagramResult> :	操作结果编码
	*/
	//std::vector<ActivityDiagramResult> GenerateActivityCode(GUID f_sGUID, std::string f_sFunName, std::string f_sSavePath);

	/********************************************************/
	/*
	*函数描述
	*	检测活动图是否可以生成代码
	*输入参数
	*	f_sDiagramGUID		: 活动图的GUID
	*	f_iTroubleshooting	: 是否自动修改活动图中的存在的错误
	*输出参数
	*
	*返回值
	*	std::vector<ActivityDiagramResult> : 操作结果编码
	*/
	//std::vector<ActivityDiagramResult>  CheckActivity(GUID f_sDiagramGUID, bool f_iTroubleshooting = false);
};