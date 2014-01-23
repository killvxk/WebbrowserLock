#include "Lock.h"

//---------------------------------------------------------------------------

//创建一个匿名互斥对象
Mutex::Mutex()
{
	m_mutex = ::CreateMutex(NULL, FALSE, NULL);
}
//创建一个互斥对象
Mutex::Mutex(LPCTSTR lpMutexName)
{
	m_mutex = ::CreateMutex(NULL, FALSE, lpMutexName);
}

//销毁互斥对象，释放资源
Mutex::~Mutex()
{
	::CloseHandle(m_mutex);
}

//确保拥有互斥对象的线程对被保护资源的独自访问
void Mutex::Lock() const
{
	DWORD d = WaitForSingleObject(m_mutex, INFINITE);
}

bool Mutex::IsLock() const
{
	if (WAIT_OBJECT_0 == WaitForSingleObject(m_mutex,0))
		return false;
	else
		return true;
}

//释放当前线程拥有的互斥对象，以使其它线程可以拥有互斥对象，对被保护资源进行访问
void Mutex::Unlock() const
{
	::ReleaseMutex(m_mutex);
}

//---------------------------------------------------------------------------

//初始化临界资源对象
CriSection::CriSection()
{
	::InitializeCriticalSection(&m_critclSection);
}

//释放临界资源对象
CriSection::~CriSection()
{
	::DeleteCriticalSection(&m_critclSection);
}

//进入临界区，加锁
void CriSection::Lock() const
{
	::EnterCriticalSection((LPCRITICAL_SECTION)&m_critclSection);
}	
bool CriSection::IsLock() const
{
		return true;
}

//离开临界区，解锁
void CriSection::Unlock() const
{
	::LeaveCriticalSection((LPCRITICAL_SECTION)&m_critclSection);
}

//---------------------------------------------------------------------------

//利用C++特性，进行自动加锁
CLock::CLock(const ILock& m) : m_lock(m)
{
	m_lock.Lock();
}

//利用C++特性，进行自动加锁
CLock::CLock(const ILock * m) : m_lock(*m)
{
	m_lock.Lock();
}

//利用C++特性，进行自动解锁
CLock::~CLock()
{
	m_lock.Unlock();
}
