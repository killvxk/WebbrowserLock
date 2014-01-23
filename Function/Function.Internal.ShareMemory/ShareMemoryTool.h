#ifndef _SMPOOL_H
#define _SMPOOL_H

#include "InternalShareMemory.h"
#include "SMAccessObject.h"

#include "Serial.h"

#include <map>

using namespace std;

template<typename T>
class CShareMemory
{
public:
	CShareMemory()
	{
		m_uMapSize = sizeof(T);
		m_pSMAccessObject = NULL;
	};

	~CShareMemory()
	{
		if(m_pSMAccessObject != NULL)
		{
			delete m_pSMAccessObject;
			m_pSMAccessObject = NULL;
		}
	};

	bool Init(SMKey key,unsigned long uSizeEx = 0, int nMaxCount = 1)
	{
		bool blRet = false;

		//修正大小
		m_uMapSize += uSizeEx;

		//初始化序列化头
		m_Serial.Init(nMaxCount*sizeof(_SMBlock));

		m_pSMAccessObject = new CSMAccessObject();
		if(NULL == m_pSMAccessObject)
		{
			return false;
		}

		//首先尝试打开，看看是否存在已有，如果不存在，则创建新的。
		blRet = m_pSMAccessObject->Open(key, nMaxCount * m_uMapSize, nMaxCount*sizeof(_SMBlock));
		if(false == blRet)
		{
			printf("[Init]Create.\n");
			//如果没有，则新建。
			blRet = m_pSMAccessObject->Create(key, nMaxCount * m_uMapSize, nMaxCount*sizeof(_SMBlock));
			if(false == blRet)
			{
				return false;
			}

			//开始划分共享内存空间
			m_mapSMBlock.clear();
			m_mapFreeSMBlock.clear();
			m_mapUsedSMBlock.clear();
			for(int i = 0; i < nMaxCount; i++)
			{
				_SMBlock* pSMBlock = new _SMBlock();
				if(pSMBlock == NULL)
				{
					return false;
				}

				pSMBlock->m_nID = i;
				pSMBlock->m_pT  = reinterpret_cast<T*>(m_pSMAccessObject->GetData(i * m_uMapSize));
				if(NULL == pSMBlock->m_pT)
				{
					return false;
				}
				m_mapSMBlock.insert(typename mapSMBlock::value_type(i, pSMBlock));
				m_mapFreeSMBlock.insert(typename mapFreeSMBlock::value_type(i, pSMBlock));
				m_Serial.Serial(pSMBlock, sizeof(_SMBlock));
			}

			m_nMaxCount = nMaxCount;
			//写入消息头
			WriteSMHead();
		}
		else
		{
			printf("[Init]Open.\n");
			//如果存在则读取
			char* pHeadData = m_pSMAccessObject->GetHeadData();
			if(NULL == pHeadData)
			{
				return false;
			}
			//printf("[Init]Open pHeadData = 0x%08x.\n", pHeadData);

			//读取共享内存中关于数据的判定
			m_Serial.Serial(pHeadData, nMaxCount*sizeof(_SMBlock));

			//还原回vector对象
			m_mapSMBlock.clear();
			m_mapFreeSMBlock.clear();
			m_mapUsedSMBlock.clear();
			for(int i = 0; i < nMaxCount; i++)
			{
				_SMBlock* pSMBlock = new _SMBlock();
				if(pSMBlock == NULL)
				{
					return false;
				}

				m_Serial.GetData(pSMBlock, sizeof(_SMBlock));
				pSMBlock->m_pT  = reinterpret_cast<T*>(m_pSMAccessObject->GetData(i * m_uMapSize));
				m_mapSMBlock.insert(typename mapSMBlock::value_type(pSMBlock->m_nID, pSMBlock));

				//分类
				if(pSMBlock->m_blUse == true)
				{
					//放入正在使用的列表
					m_mapUsedSMBlock.insert(typename mapUsedSMBlock::value_type(pSMBlock->m_pT, pSMBlock));
				}
				else
				{
					//放入没有使用的列表
					m_mapFreeSMBlock.insert(typename mapFreeSMBlock::value_type(i, pSMBlock));
				}
			}

			m_nMaxCount = nMaxCount;
		}


		return true;
	};

	void Close()
	{
		typename mapSMBlock::iterator b = m_mapSMBlock.begin();
		typename mapSMBlock::iterator e = m_mapSMBlock.end();
		for(b; b != e; b++)
		{
			_SMBlock* pSMBlock = (_SMBlock* )b->second;
			if(pSMBlock != NULL)
			{
				delete pSMBlock;
			}
		}
		m_mapSMBlock.clear();

		m_pSMAccessObject->Destory();
	}

	T* NewObject()
	{
		//在m_mapSMBlock中查找一个空余的对象。
		if(m_mapFreeSMBlock.size() > 0)
		{
			typename mapFreeSMBlock::iterator b = m_mapFreeSMBlock.begin();
			_SMBlock* pSMBlock = (_SMBlock* )b->second;
			if(NULL == pSMBlock)
			{
				return NULL;
			}
			else
			{
				m_mapFreeSMBlock.erase(b);
				pSMBlock->m_blUse = true;
				m_mapUsedSMBlock.insert(typename mapUsedSMBlock::value_type(pSMBlock->m_pT, pSMBlock));

				m_Serial.WriteData(pSMBlock->m_nID*sizeof(_SMBlock), pSMBlock, sizeof(_SMBlock));
				//写入消息头
				WriteSMHead();

				return pSMBlock->m_pT;
			}
		}
		else
		{
			return NULL;
		}
	};

	bool DeleteObject(T* pData)
	{
		typename mapUsedSMBlock::iterator f = m_mapUsedSMBlock.find(pData);
		if(f == m_mapUsedSMBlock.end())
		{
			return false;
		}
		else
		{
			_SMBlock* pSMBlock = (_SMBlock* )f->second;
			m_mapUsedSMBlock.erase(f);
			pSMBlock->m_blUse = false;
			m_mapFreeSMBlock.insert(typename mapFreeSMBlock::value_type(pSMBlock->m_nID, pSMBlock));

			m_Serial.WriteData(pSMBlock->m_nID*sizeof(_SMBlock), pSMBlock, sizeof(_SMBlock));
			//写入消息头
			WriteSMHead();
			return true;
		}
	};

	int GetFreeObjectCount()
	{
		return (int)m_mapFreeSMBlock.size();
	}

	int GetUsedObjectCount()
	{
		return (int)m_mapUsedSMBlock.size();
	};

	T* GetUsedObject(int nIndex)
	{
		if(nIndex >= (int)m_mapUsedSMBlock.size())
		{
			return NULL;
		}

		typename mapUsedSMBlock::iterator b = m_mapUsedSMBlock.begin();
		typename mapUsedSMBlock::iterator e = m_mapUsedSMBlock.end();

		int nPos = 0;
		for(b; b != e; b++)
		{
			if(nPos == nIndex)
			{
				_SMBlock* pSMBlock = (_SMBlock* )b->second;
				if(NULL != pSMBlock)
				{
					return pSMBlock->m_pT;
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				if(nPos > nIndex)
				{
					return NULL;
				}
				nPos++;
			}
		}
		return NULL;
	}

private:
	void WriteSMHead()
	{
		char* pHeadData = m_pSMAccessObject->GetHeadData();
		if(NULL == pHeadData)
		{
			return;
		}

		//将头信息写入共享内存头
		memcpy(pHeadData, m_Serial.GetBase(), m_nMaxCount*sizeof(_SMBlock));
	};

private:
	//记录每个队列的数据容器
	struct _SMBlock
	{
		T*   m_pT;
		int  m_nID;
		bool m_blUse; 

		_SMBlock()
		{
			m_pT    = NULL;
			m_nID   = 0;
			m_blUse = false;
		}
	};
	typedef map<int, _SMBlock*> mapSMBlock;
	typedef map<T*, _SMBlock*>  mapUsedSMBlock;
	typedef map<int, _SMBlock*> mapFreeSMBlock;

private:
	CSMAccessObject* m_pSMAccessObject;
	mapSMBlock       m_mapSMBlock;
	mapUsedSMBlock   m_mapUsedSMBlock;
	mapFreeSMBlock   m_mapFreeSMBlock;
	CSerial          m_Serial;
	unsigned long	 m_uMapSize;
	int              m_nMaxCount;
};

#endif
