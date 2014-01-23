// ShareMemory.cpp : 定义控制台应用程序的入口点。
//

#include "InternalShareMemory.h"


//
//
//{
//	CSMPool<_Data> UserPool;
//
//	SMKey key;
//
//#ifdef _WIN32	
//	key = 1111;
//#else
//	key = ftok("/home/freeeyes/SMData/", 0);
//#endif
//
//	printf("[Test]%d.\n", key);
//	UserPool.Init(key, 100);
//
//	_Data* pData1 = UserPool.NewObject();
//	if(NULL == pData1)
//	{
//		printf("[main]Get Data1 NULL.\n");
//	}
//	else
//	{
//		pData1->m_nData = 1;
//		sprintf(pData1->m_szData, "shiqiang");
//		printf("[main]Free Count = %d.\n", UserPool.GetFreeObjectCount());
//		printf("[main]Used Count = %d.\n", UserPool.GetUsedObjectCount());
//		printf("[main]Get Data1 OK[%d] - [%s].\n", pData1->m_nData, pData1->m_szData);
//	}
//
//	_Data* pData3 = UserPool.NewObject();
//	if(NULL == pData1)
//	{
//		printf("[main]Get Data1 NULL.\n");
//	}
//	else
//	{
//		pData3->m_nData = 3;
//		sprintf(pData3->m_szData, "shiqiang");
//		printf("[main]Free Count = %d.\n", UserPool.GetFreeObjectCount());
//		printf("[main]Used Count = %d.\n", UserPool.GetUsedObjectCount());
//		printf("[main]Get Data3 OK[%d] - [%s].\n", pData3->m_nData, pData3->m_szData);
//	}
//	_Data* pData2 = UserPool.GetUsedObject(0);
//	if(pData2 != NULL)
//	{
//		printf("[main]GetUsedObject pData2 OK[%d] - [%s].\n", pData1->m_nData, pData1->m_szData);
//	}
//	_Data* pData4 = UserPool.GetUsedObject(1);
//	if(pData2 != NULL)
//	{
//		printf("[main]GetUsedObject pData4 OK[%d] - [%s].\n", pData4->m_nData, pData4->m_szData);
//	}
//
//	UserPool.DeleteObject(pData1);
//	printf("[main]Free Count = %d.\n", UserPool.GetFreeObjectCount());
//
//	pData1 = UserPool.NewObject();
//	if(NULL == pData1)
//	{
//		printf("[main]Get Data1 NULL.\n");
//	}
//	else
//	{
//		pData1->m_nData = 1;
//		sprintf(pData1->m_szData, "shiqiang");
//		printf("[main]Free Count = %d.\n", UserPool.GetFreeObjectCount());
//		printf("[main]Get Data1 OK[%d] - [%s].\n", pData1->m_nData, pData1->m_szData);
//	}
//
//	//UserPool.Close();
//	getchar();
//	return 0;
//}
//
