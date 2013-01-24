#pragma once
#include "tstatbase.h"

// TStat5 ϵ���豸�Ļ���
class CTStat_Dev :
	public CTStatBase
{
public:
	CTStat_Dev(void);
	~CTStat_Dev(void);

	virtual int ReadOneReg(int nRegAddr, OUT int& nVal);
	virtual int ReadOneReg(int nRegAddr);
	virtual int WriteOneReg(int nRegAddr, IN int& nVal);

	virtual int ReadMultiReg(int nRegAddrStart, int nRegAddrEnd, OUT int* nVal);
	virtual int WriteMultiReg(int nRegAddrStart, int nRegAddrEnd, OUT int* nVal);
	// Ҳ����ͨ����д�Ĵ�����ʵ��
	virtual BOOL IsOnLine();	

	virtual int WriteDevID(int nID)const;	
	virtual int ReadDevID(int& nID);
	//-------------------------------------------------------------------

	int GetBaudRate()const;
	void  SetBaudRate(int nBaudRate);

	int GetComPort()const;
	void SetComPort(int nComPort);
	
	int GetEPSize()const;
	void SetEPSize(int nEPSize);
	
	CString GetProductName();
// 	int GetAddrID()const;
// 	void SetAddrID(int nID);

	
	BOOL CTStat_Dev::operator==(const CTStat_Dev& dev);



protected:
	
	int				m_nBaudRate;				// 
	int				m_nComPort;				//
	int				m_nEPSize;					// 
	//int				m_nAddrID;				// 
};