#pragma once



template <typename T>
class CSingleton
{
public:
	static T* GetSingleObjPtr() {
		if(!m_pSingleObj)m_pSingleObj=new T;
		return m_pSingleObj;
	};
private:
	inline static T* m_pSingleObj = nullptr;
protected:
	CSingleton(){}
};