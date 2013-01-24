#pragma once

#include "afxwin.h"
#include "msflexgrid1.h"
#include "T3000.h"
#include "MainFrm.h"

// CInputSetDlg dialog

class CInputSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputSetDlg)

public:
	CInputSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputSetDlg();

// Dialog Data
	enum { IDD = IDD_IPUTDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void Init5EGrid();
	

	void OnCbnSelchangeRangComboFor5E();
	void OnBnClickedUpbuttonFor5E();
	void OnBnClickedDownbuttonFor5E();
	void OnCbnSelchangeAmcomboFor5E();
	void OnEnKillfocusInvalueeditFor5E();
	void OnCbnKillfocusValuecomboFor5E();
	void OnClick5EGrid(int nRow, int nCol, CRect rcCell);

	void add_analog_rang(int i);
	void Fresh_Grid();
	void ClickMsflexgrid1();

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCancel();
	DECLARE_EVENTSINK_MAP()

	afx_msg void OnCbnSelchangeRangCombo();
	afx_msg void OnCbnKillfocusRangCombo();
	afx_msg void OnBnClickedUpbutton();
	afx_msg void OnBnClickedDownbutton();
	afx_msg void OnCbnKillfocusAmcombo();
	afx_msg void OnCbnSelchangeAmcombo();
	afx_msg void OnCbnSelchangeInputfuncombo();
	afx_msg void OnCbnKillfocusInputfuncombo();
	afx_msg void OnBnClickedCustombutton();
	afx_msg void OnBnKillfocusCustombutton();
	afx_msg void OnBnClickedExit();
	afx_msg void OnEnKillfocusInvalueedit();
	afx_msg void OnCbnKillfocusValuecombo();
	afx_msg void OnBnClickedRefreshbutton();
	afx_msg void OnEnKillfocusInputnameedit();

public:
	//int m1_inRows;
	CEdit m_inputNameEdt;
	CComboBox m_valueCombx;
	CEdit m_inValueEdit;
	CButton m_customBtn;	
	CComboBox m_inputFinCombox;

	CComboBox	m_RangCombox;
	CMsflexgrid		m_FlexGrid;
	int					m_nModel;
	float				m_fFirmwareVersion;
	vector<CString> m_strUnitList;
	int m_nCurRow;
	int m_nCurCol;

	CMFCButton m_upButton;
	CMFCButton m_downButton;
	CComboBox m_AmCombox;
	CString		m_strCustomsensorUnit;



};