// BacnetSetting.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetSetting.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "Bacnetaddintodb.h"
#include "BacnetATCommand.h"
#include "BacnetSettingHealth.h"
#include "TstatZigbeeLogic.h"
#include "BacnetIOConfig.h"
#include "MainFrm.h"
#include "../SQLiteDriver/CppSQLite3.h"
// CBacnetSetting dialog
extern bool cancle_send ;
bool show_user_list_window = false;

#define TIMER_SYNC_TIMER    1
#define TIMER_REFRESH_READ    2
#define TIMER_REFRESH_READ_DELAY    15


IMPLEMENT_DYNAMIC(CBacnetSetting, CDialogEx)

CBacnetSetting::CBacnetSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetSetting::IDD, pParent)
{
		window_max = true;
}

CBacnetSetting::~CBacnetSetting()
{
}

void CBacnetSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TIME_PICKER, m_cm5_time_picker);
	DDX_Control(pDX, IDC_DATE_PICKER, m_cm5_date_picker);
	DDX_Control(pDX, IDC_COMBO_BACNET_SETTING_COM0, m_device_com0);
	DDX_Control(pDX, IDC_STATIC_SETTING_PANEL_TITLE, m_static_panel_title);
	DDX_Control(pDX, IDC_EDIT_SETTING_PANEL, m_edit_panel);
	DDX_Control(pDX, IDC_STATIC_SETTING_NODES_LABEL_TITLE, m_static_nodes_title);
	DDX_Control(pDX, IDC_EDIT_SETTING_NODES_LABEL_SETTING, m_edit_nodes_label);
	DDX_Control(pDX, IDC_EDIT_DYNDNS_USER_NAME, m_dyndns_user);
	DDX_Control(pDX, IDC_EDIT_DYNDNS_PASSWORD, m_dyndns_password);
	DDX_Control(pDX, IDC_EDIT_DYNDNS_DOMAIN, m_dyndns_domain);
	DDX_Control(pDX, IDC_EDIT_TIME_UPDATE, m_edit_ddns_update_time);
	DDX_Control(pDX, IDC_EDIT_SETTING_PORT, m_edit_port);
	DDX_Control(pDX, IDC_EDIT_SETTING_OBJ_INSTANCE, m_setting_obj_instance);
	DDX_Control(pDX, IDC_EDIT_SETTING_MODBUS_ID, m_edit_modbus_id);
}


BEGIN_MESSAGE_MAP(CBacnetSetting, CDialogEx)
	ON_MESSAGE(WM_FRESH_SETTING_UI,Fresh_Setting_UI)
	ON_BN_CLICKED(IDC_BUTTON_BAC_TEST, &CBacnetSetting::OnBnClickedButtonBacTest)
	ON_NOTIFY(NM_KILLFOCUS, IDC_DATE_PICKER, &CBacnetSetting::OnNMKillfocusDatePicker)
	ON_NOTIFY(NM_KILLFOCUS, IDC_TIME_PICKER, &CBacnetSetting::OnNMKillfocusTimePicker)
	ON_NOTIFY(NM_SETFOCUS, IDC_DATE_PICKER, &CBacnetSetting::OnNMSetfocusDatePicker)
	ON_NOTIFY(NM_SETFOCUS, IDC_TIME_PICKER, &CBacnetSetting::OnNMSetfocusTimePicker)
	ON_BN_CLICKED(IDC_BAC_SYNC_LOCAL_PC, &CBacnetSetting::OnBnClickedBtnBacSYNCTime)
	ON_BN_CLICKED(IDC_RADIO_BAC_IP_AUTO, &CBacnetSetting::OnBnClickedBtnBacIPAuto)
	ON_BN_CLICKED(IDC_RADIO_BAC_IP_STATIC, &CBacnetSetting::OnBnClickedBtnBacIPStatic)
	ON_BN_CLICKED(IDC_BUTTON_BAC_IP_CHANGED, &CBacnetSetting::OnBnClickedBtnBacIPChange)
	ON_WM_TIMER()
	ON_MESSAGE(MY_RESUME_DATA, ResumeMessageCallBack)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_COM0, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingCom0)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_COM1, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingCom1)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_COM2, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingCom2)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_LDF, &CBacnetSetting::OnBnClickedButtonSettingLdf)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_BAUDRATE0, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingBaudrate0)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_BAUDRATE1, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingBaudrate1)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_BAUDRATE2, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingBaudrate2)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_CLEAN_DB, &CBacnetSetting::OnBnClickedButtonSettingCleanDb)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_ADD_INTO_DB, &CBacnetSetting::OnBnClickedButtonSettingAddIntoDb)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_USER_LIST, &CBacnetSetting::OnBnClickedButtonSettingUserList)
	ON_BN_CLICKED(IDC_RADIO_USB_DEVICE, &CBacnetSetting::OnBnClickedRadioUsbDevice)
	ON_BN_CLICKED(IDC_RADIO_USB_HOST, &CBacnetSetting::OnBnClickedRadioUsbHost)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_GSM_MODUAL, &CBacnetSetting::OnBnClickedButtonSettingGsmModual)
	ON_WM_CLOSE()
	ON_EN_KILLFOCUS(IDC_EDIT_SETTING_PANEL, &CBacnetSetting::OnEnKillfocusEditSettingPanel)
	ON_EN_KILLFOCUS(IDC_EDIT_SETTING_NODES_LABEL_SETTING, &CBacnetSetting::OnEnKillfocusEditSettingNodesLabelSetting)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_TIME_SERVER, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingTimeServer)
	ON_BN_CLICKED(IDC_CHECK_SETTING_SYNC_TIME, &CBacnetSetting::OnBnClickedCheckSettingSyncTime)
	ON_EN_KILLFOCUS(IDC_EDIT_DYNDNS_USER_NAME, &CBacnetSetting::OnEnKillfocusEditDyndnsUserName)
	ON_EN_KILLFOCUS(IDC_EDIT_DYNDNS_PASSWORD, &CBacnetSetting::OnEnKillfocusEditDyndnsPassword)
	ON_EN_KILLFOCUS(IDC_EDIT_DYNDNS_DOMAIN, &CBacnetSetting::OnEnKillfocusEditDyndnsDomain)
	ON_BN_CLICKED(IDC_CHECK_SETTING_DYNDNS, &CBacnetSetting::OnBnClickedCheckSettingDyndns)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_DDNS_SERVER, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingDdnsServer)
	ON_EN_KILLFOCUS(IDC_EDIT_TIME_UPDATE, &CBacnetSetting::OnEnKillfocusEditTimeUpdate)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNET_SETTING_TIME_ZONE, &CBacnetSetting::OnCbnSelchangeComboBacnetSettingTimeZone)
	ON_BN_CLICKED(IDC_CHECK_SETTING_PAP, &CBacnetSetting::OnBnClickedCheckSettingPap)
	ON_EN_KILLFOCUS(IDC_EDIT_SETTING_PORT, &CBacnetSetting::OnEnKillfocusEditSettingPort)
	ON_BN_CLICKED(IDC_BUTTON_HEALTH, &CBacnetSetting::OnBnClickedButtonHealth)
	ON_EN_KILLFOCUS(IDC_EDIT_SETTING_OBJ_INSTANCE, &CBacnetSetting::OnEnKillfocusEditSettingObjInstance)
	ON_WM_HELPINFO()

	ON_BN_CLICKED(IDC_BUTTON_BAC_SETTING_OK, &CBacnetSetting::OnBnClickedButtonBacSettingOk)
	ON_BN_CLICKED(IDC_BUTTON_BAC_SETTING_CANCEL, &CBacnetSetting::OnBnClickedButtonBacSettingCancel)
	ON_BN_CLICKED(IDC_BUTTON_SYNC_TIME, &CBacnetSetting::OnBnClickedButtonSyncTime)
	ON_BN_CLICKED(IDC_CHECK_SETTING_ZONE_DAYLIGHT_TIME, &CBacnetSetting::OnBnClickedCheckSettingZoneDaylightTime)
	ON_BN_CLICKED(IDC_BUTTON_BAC_SHOW_ZIGBEE, &CBacnetSetting::OnBnClickedButtonBacShowZigbee)
	ON_CBN_KILLFOCUS(IDC_COMBO_BACNET_SETTING_TIME_SERVER, &CBacnetSetting::OnCbnKillfocusComboBacnetSettingTimeServer)
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_EN_KILLFOCUS(IDC_EDIT_SETTING_MODBUS_ID, &CBacnetSetting::OnEnKillfocusEditSettingModbusId)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_SETTING_IO_CONFIG, &CBacnetSetting::OnBnClickedButtonSettingIoConfig)
	ON_BN_CLICKED(IDC_BUTTON_REBOOT_DEVICE, &CBacnetSetting::OnBnClickedButtonRebootDevice)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CBacnetSetting message handlers





void CBacnetSetting::OnBnClickedButtonBacTest()
{
	

	int	resend_count = 0;
	do 
	{
		resend_count ++;
		if(resend_count>20)
			return;
		g_invoke_id = GetPrivateData(g_bac_instance,
			TIME_COMMAND,0,
			0,
			sizeof(Time_block_mini));
		Sleep(200);
	} while (g_invoke_id<0);

	CString temp_cs_show;
	temp_cs_show.Format(_T("Read time "));
	Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

	//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);


}


void CBacnetSetting::Get_Time_Edit_By_Control()
{
	CTime temp_day;

	m_cm5_date_picker.GetTime(temp_day);

	int temp_year	= temp_day.GetYear();
	if(temp_year > 2000)
		temp_year = temp_year - 2000;
	Device_time.year = temp_year;
	Device_time.month = temp_day.GetMonth();
	Device_time.dayofmonth = temp_day.GetDay();
	Device_time.dayofweek = temp_day.GetDayOfWeek() -1;
	Device_time.dayofyear = 1;


	CTime temp_time;
	m_cm5_time_picker.GetTime(temp_time);
	Device_time.ti_hour = temp_time.GetHour();
	Device_time.ti_min = temp_time.GetMinute();
	Device_time.ti_sec = temp_time.GetSecond();
	CString temp_task_info;
	temp_task_info.Format(_T("Write Device Time.Changed to %02d-%02d %02d:%02d  "),
		Device_time.month,Device_time.dayofmonth,
		Device_time.ti_hour,Device_time.ti_min);
	Post_Write_Message(g_bac_instance,RESTARTMINI_COMMAND,0,0,sizeof(Time_block_mini),this->m_hWnd,temp_task_info);
}

void CBacnetSetting::OnNMKillfocusDatePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	

	Get_Time_Edit_By_Control();
	//GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(0);
	*pResult = 0;
}


void CBacnetSetting::OnNMKillfocusTimePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	Get_Time_Edit_By_Control();
	//GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(0);
	*pResult = 0;
}




void CBacnetSetting::OnNMSetfocusDatePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	//GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(TRUE);
	Get_Time_Edit_By_Control();
	*pResult = 0;
}


void CBacnetSetting::OnNMSetfocusTimePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	//GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(TRUE);
	Get_Time_Edit_By_Control();
	*pResult = 0;
}

void CBacnetSetting::OnBnClickedBtnBacSYNCTime()
{
	CTime	TimeTemp;
	CTime temp_time;
	int nyear,nmonth,nday,nhour,nmin,nsec;
	temp_time = CTime::GetCurrentTime();
	if(temp_time.GetYear()<2000)
		nyear = temp_time.GetYear() + 2000;
	else
		nyear = temp_time.GetYear();
	nmonth = temp_time.GetMonth();
	nday = temp_time.GetDay();
	nhour = temp_time.GetHour();
	nmin = temp_time.GetMinute();
	nsec = temp_time.GetSecond();
	TimeTemp = CTime(nyear,nmonth,nday,nhour,nmin,nsec);

	m_cm5_time_picker.SetFormat(_T("HH:mm"));
	m_cm5_time_picker.SetTime(&TimeTemp);

	//m_cm5_date_picker.SetFormat(_T("YY/MM/DD"));
	m_cm5_date_picker.SetTime(&TimeTemp);

	//GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(TRUE);
	Get_Time_Edit_By_Control();

}

void CBacnetSetting::OnBnClickedBtnBacWriteTime()
{
	

	Get_Time_Edit_By_Control();

}

void CBacnetSetting::OnBnClickedBtnBacIPAuto()
{
	Device_Basic_Setting.reg.tcp_type = 1;

	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(FALSE);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(FALSE);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(FALSE);

	CString temp_task_info;
	temp_task_info.Format(_T("Change  DHCP auto "));
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
}

void CBacnetSetting::OnBnClickedBtnBacIPStatic()
{
	Device_Basic_Setting.reg.tcp_type = 0;
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(true);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(true);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(true);
}

void CBacnetSetting::OnBnClickedBtnBacIPChange()
{
	BYTE address1,address2,address3,address4;
	BYTE subnet1, subnet2, subnet3, subnet4;
	BYTE gatway1,gatway2,gatway3,gatway4;
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->GetAddress(address1,address2,address3,address4);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->GetAddress(subnet1,subnet2,subnet3,subnet4);
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->GetAddress(gatway1,gatway2,gatway3,gatway4);

		Device_Basic_Setting.reg.ip_addr[0] = address1;
		Device_Basic_Setting.reg.ip_addr[1] = address2;
		Device_Basic_Setting.reg.ip_addr[2] = address3;
		Device_Basic_Setting.reg.ip_addr[3] = address4;
		Device_Basic_Setting.reg.subnet[0]  = subnet1;
		Device_Basic_Setting.reg.subnet[1]  = subnet2;
		Device_Basic_Setting.reg.subnet[2]  = subnet3;
		Device_Basic_Setting.reg.subnet[3]  = subnet4;
		Device_Basic_Setting.reg.gate_addr[0] = gatway1;
		Device_Basic_Setting.reg.gate_addr[1] = gatway2;
		Device_Basic_Setting.reg.gate_addr[2] = gatway3;
		Device_Basic_Setting.reg.gate_addr[3] = gatway4;
		bool isstatic = ((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_STATIC))->GetCheck(); //����1��ʾѡ�ϣ�0��ʾûѡ��;
		if(isstatic == true)
			Device_Basic_Setting.reg.tcp_type = 0;
		else
			Device_Basic_Setting.reg.tcp_type = 1;
		//Device_Basic_Setting.reg.tcp_type = isstatic;
		//CString temp_task_info;
		//temp_task_info.Format(_T("Change IP Address Information !"));
		//Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
		if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
		{
			CString temp_task_info;
			temp_task_info.Format(_T("Change IP Address Information Timeout!"));
			MessageBox(temp_task_info);
		}
		else
		{
			//��Ip �޸ĳɹ��� �������ݿ�;
			CString strnewipadress;
			strnewipadress.Format(_T("%u.%u.%u.%u"),address1,address2,address3,address4);
			CString temp_task_info;
			temp_task_info.Format(_T("Change IP Address Information OK!"));

			CString strSql;
			CppSQLite3DB SqliteDBBuilding;
			CppSQLite3Table table;
			CppSQLite3Query q;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

			CString temp_serial_cs;
			temp_serial_cs.Format(_T("%u"),g_selected_serialnumber);
			strSql.Format(_T("select * from ALL_NODE where Serial_ID = '%s' "),temp_serial_cs);
			//m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
			q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
			while(!q.eof())
			{
				strSql.Format(_T("update ALL_NODE set Bautrate='%s' where Serial_ID= '%s'"),strnewipadress,temp_serial_cs);
				SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
			    q.nextRow();
			}
			 
			CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
			pFrame->m_product.at(selected_product_index).BuildingInfo.strIp = strnewipadress;
			MessageBox(temp_task_info);
			refresh_tree_status_immediately = true;
		}


}

CString Check_Time(unsigned long local_time , unsigned int device_time)
{
	CString ret_cstring;
	if (local_time < device_time)
	{
		ret_cstring = _T("No Reply");
	}
	else if (device_time + 3600 * 24 * 30 < local_time)
	{
		ret_cstring = _T("1 month ago");
	}
	else if (device_time + 3600 * 24 * 7 < local_time)
	{
		ret_cstring = _T("7 days ago");
	}
	else if (device_time + 3600 * 24 < local_time)
	{
		ret_cstring = _T("1 day ago");
	}
	else if (device_time + 3600 * 12 < local_time)
	{
		ret_cstring = _T(" 12 hours ago");
	}
	else if (device_time + 3600 < local_time)
	{
		ret_cstring = _T(" 1 hour ago");
	}
	else if (device_time + 1800 < local_time)
	{
		ret_cstring = _T(" 30 minutes ago");
	}
	else if (device_time + 600 < local_time)
	{
		ret_cstring = _T(" 10 minutes ago");
	}
	else if (device_time + 300 < local_time)
	{
		ret_cstring = _T(" 5 minutes ago");
	}
	else if (device_time + 60 < local_time)
	{
		ret_cstring = _T(" 1 minute ago");
	}
	else if (device_time < local_time)
	{
		ret_cstring = _T("10 seconds ago");
	}

	return ret_cstring;
}







LRESULT CBacnetSetting::Fresh_Setting_UI(WPARAM wParam,LPARAM lParam)
{
	int command_type = wParam;
	int button_click = 0;
	CString temp_cs;
	int temp_year;
	CTime	TimeTemp;
	CString temp_hw_version = _T("Unknown");
	CString temp_mcu_version = _T("Unknown");
	CString temp_pic_version = _T("Unknown");
	CString temp_c8051_version = _T("Unknown");
	CString temp_5964_version = _T("Unknown");
	CString temp_bootloader_version = _T("Unknown");
	CString temp_serial_number = _T("Unknown");
	
	TIME_ZONE_INFORMATION lp_time_zone;
	memset(&lp_time_zone,0,sizeof(TIME_ZONE_INFORMATION));

	::GetTimeZoneInformation(&lp_time_zone);
	 LONG n_tempBias;
	 n_tempBias = 0 - lp_time_zone.Bias;
	 n_tempBias = (n_tempBias/60)*100;

	 //�ж� minipanel ��ʱ�� �뱾�ص���ʱ���Ƿ�һ�£���һ���Զ�����;
	 /*if(Device_Basic_Setting.reg.time_zone != n_tempBias)
	 {
		 Device_Basic_Setting.reg.time_zone = n_tempBias;
		 if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) > 0)
		 {
			 CString temp_task_info;
			 temp_task_info.Format(_T("SYNC Time zone OK!"));
			 SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_task_info);
		 }
	 }*/

	switch(command_type)
	{
	case READ_SETTING_COMMAND:
		{

			((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->SetAddress(Device_Basic_Setting.reg.ip_addr[0],
				Device_Basic_Setting.reg.ip_addr[1],Device_Basic_Setting.reg.ip_addr[2],Device_Basic_Setting.reg.ip_addr[3]);
			((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->SetAddress(Device_Basic_Setting.reg.subnet[0],
				Device_Basic_Setting.reg.subnet[1],Device_Basic_Setting.reg.subnet[2],Device_Basic_Setting.reg.subnet[3]);
			((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->SetAddress(Device_Basic_Setting.reg.gate_addr[0],
				Device_Basic_Setting.reg.gate_addr[1],Device_Basic_Setting.reg.gate_addr[2],Device_Basic_Setting.reg.gate_addr[3]);

			if(Device_Basic_Setting.reg.tcp_type == 1)
			{
				((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_AUTO))->SetCheck(true);
				((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_STATIC))->SetCheck(false);
				((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(FALSE);
				((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(FALSE);
				((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(FALSE);
			}
			else if(Device_Basic_Setting.reg.tcp_type == 0)
			{
				((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_AUTO))->SetCheck(false);
				((CButton *)GetDlgItem(IDC_RADIO_BAC_IP_STATIC))->SetCheck(true);
				((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_IP))->EnableWindow(true);
				((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_SUBNET))->EnableWindow(true);
				((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_BAC_GATEWAY))->EnableWindow(true);
			}

			//Ӳ���汾���� 26 ������arm�İ汾. 
			if(Device_Basic_Setting.reg.pro_info.harware_rev >= 26)
			{
				GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1)->EnableWindow(FALSE);
			}
			else
			{
				GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1)->EnableWindow(TRUE);
			}

			//�汾����38.6 �Ĳ�����setting �����port �Ĺ���
			if(Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 +Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 386)
			{
				CString temp_port;
				temp_port.Format(_T("%u"),Device_Basic_Setting.reg.modbus_port);
				m_edit_port.EnableWindow(true);
				m_edit_port.SetWindowTextW(temp_port);
			}
			else
			{
				m_edit_port.EnableWindow(false);
			}

			if(Device_Basic_Setting.reg.en_plug_n_play == 1)
			{
				((CButton *)GetDlgItem(IDC_CHECK_SETTING_PAP))->SetCheck(true);
			}
			else
			{
				((CButton *)GetDlgItem(IDC_CHECK_SETTING_PAP))->SetCheck(false);
			}


			unsigned long time_setting_now = (unsigned long)time(NULL);

			if((debug_item_show == DEBUG_SHOW_BACNET_ALL_DATA) || (debug_item_show == DEBUG_SHOW_ALL))
			{
				CString temp123;
				temp123.Format(_T("%u"),Device_Basic_Setting.reg.time_update_since_1970);
				DFTrace(temp123);
			}

			if((Device_Basic_Setting.reg.time_update_since_1970 < 1420041600)  || (Device_Basic_Setting.reg.time_update_since_1970 > 1735660800))
			{
				((CEdit *)GetDlgItem(IDC_EDIT_SETTING_LAST_UPDATE_TIME))->SetWindowTextW(_T("No Reply"));
			}
			else
			{
				//CTime time_scaletime;
				CString strTime;
				time_t scale_time  = Device_Basic_Setting.reg.time_update_since_1970;

				strTime = Check_Time(time_setting_now,Device_Basic_Setting.reg.time_update_since_1970);

				//time_scaletime = scale_time;
				//strTime = time_scaletime.Format("%y/%m/%d %H:%M:%S");
				((CEdit *)GetDlgItem(IDC_EDIT_SETTING_LAST_UPDATE_TIME))->SetWindowTextW(strTime);
			}


			if(Device_Basic_Setting.reg.en_dyndns == 0)
			{
				GetDlgItem(IDC_CHECK_SETTING_DYNDNS)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC_DYNDNS)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC_SETTING_USER_NAME)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_DYNDNS_USER_NAME)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC_SETTING_PASSWORD)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_DYNDNS_PASSWORD)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC_SETTING_DOMAIN)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_DYNDNS_DOMAIN)->EnableWindow(FALSE);

				GetDlgItem(IDC_STATIC_DDNS_SERVER)->EnableWindow(FALSE);
				GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC_CHECK_AUTO)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_TIME_UPDATE)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC_MIN_TIME)->EnableWindow(FALSE);
	
			}
			else
			{

				GetDlgItem(IDC_CHECK_SETTING_DYNDNS)->EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC_DYNDNS)->EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC_SETTING_USER_NAME)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_DYNDNS_USER_NAME)->EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC_SETTING_PASSWORD)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_DYNDNS_PASSWORD)->EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC_SETTING_DOMAIN)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_DYNDNS_DOMAIN)->EnableWindow(TRUE);

				GetDlgItem(IDC_STATIC_DDNS_SERVER)->EnableWindow(TRUE);
				GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER)->EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC_CHECK_AUTO)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_TIME_UPDATE)->EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC_MIN_TIME)->EnableWindow(TRUE);

				if(Device_Basic_Setting.reg.en_dyndns == 2)
				{
					((CButton *)GetDlgItem(IDC_CHECK_SETTING_DYNDNS))->SetCheck(true);
				}
				else
				{
					Device_Basic_Setting.reg.en_dyndns = 1;
					((CButton *)GetDlgItem(IDC_CHECK_SETTING_DYNDNS))->SetCheck(false);
				}



				
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->ResetContent();
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->AddString(DDNS_Server_Name[0]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->AddString(DDNS_Server_Name[1]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->AddString(DDNS_Server_Name[2]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->AddString(DDNS_Server_Name[3]);

				//if(Device_Basic_Setting.reg.dyndns_provider == 0)
				//	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetWindowTextW(DDNS_Server_Name[0]);
				//else if(Device_Basic_Setting.reg.dyndns_provider == 1)
				//	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetWindowTextW(DDNS_Server_Name[1]);
				//else if(Device_Basic_Setting.reg.dyndns_provider == 2)
				//	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetWindowTextW(DDNS_Server_Name[2]);
				//else if(Device_Basic_Setting.reg.dyndns_provider == 3)
				//	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetWindowTextW(DDNS_Server_Name[3]);
				//else
				//	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetWindowTextW(_T(""));

				if(Device_Basic_Setting.reg.dyndns_provider == 0)
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetCurSel(0);
				else if(Device_Basic_Setting.reg.dyndns_provider == 1)
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetCurSel(1);
				else if(Device_Basic_Setting.reg.dyndns_provider == 2)
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetCurSel(2);
				else if(Device_Basic_Setting.reg.dyndns_provider == 3)
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetCurSel(3);
				else
				{
					Device_Basic_Setting.reg.dyndns_provider = 3;
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->SetCurSel(3);
				}


				CString temp_time;
				temp_time.Format(_T("%u"),Device_Basic_Setting.reg.dyndns_update_time);
				m_edit_ddns_update_time.SetWindowText(temp_time);

				CString temp_dyndns_user;
				CString temp_dyndns_password;
				CString temp_dyndns_domain;
				MultiByteToWideChar( CP_ACP, 0, (char *)Device_Basic_Setting.reg.dyndns_user, 	(int)strlen((char *)Device_Basic_Setting.reg.dyndns_user)+1,temp_dyndns_user.GetBuffer(MAX_PATH), MAX_PATH );
				temp_dyndns_user.ReleaseBuffer();	
				MultiByteToWideChar( CP_ACP, 0, (char *)Device_Basic_Setting.reg.dyndns_pass, 	(int)strlen((char *)Device_Basic_Setting.reg.dyndns_pass)+1,temp_dyndns_password.GetBuffer(MAX_PATH), MAX_PATH );
				temp_dyndns_password.ReleaseBuffer();	
				MultiByteToWideChar( CP_ACP, 0, (char *)Device_Basic_Setting.reg.dyndns_domain, 	(int)strlen((char *)Device_Basic_Setting.reg.dyndns_domain)+1,temp_dyndns_domain.GetBuffer(MAX_PATH), MAX_PATH );
				temp_dyndns_domain.ReleaseBuffer();	
				m_dyndns_user.SetWindowTextW(temp_dyndns_user);
				m_dyndns_password.SetWindowTextW(temp_dyndns_password);
				m_dyndns_domain.SetWindowTextW(temp_dyndns_domain);
			}

			if(Device_Basic_Setting.reg.time_zone_summer_daytime == 0)
			{
				((CButton *)GetDlgItem(IDC_CHECK_SETTING_ZONE_DAYLIGHT_TIME))->SetCheck(false);
			}
			else
			{
				Device_Basic_Setting.reg.time_zone_summer_daytime = 1;
				((CButton *)GetDlgItem(IDC_CHECK_SETTING_ZONE_DAYLIGHT_TIME))->SetCheck(true);
			}

			if(Device_Basic_Setting.reg.zegbee_exsit == 0x74)
			{
				((CButton *)GetDlgItem(IDC_BUTTON_BAC_SHOW_ZIGBEE))->EnableWindow(TRUE);
			}
			else
			{
				((CButton *)GetDlgItem(IDC_BUTTON_BAC_SHOW_ZIGBEE))->EnableWindow(FALSE);
			}
			

			if(Device_Basic_Setting.reg.en_sntp == 0)
			{
				GetDlgItem(IDC_STATIC_SETTING_TIMER_SERVER)->ShowWindow(FALSE);
				GetDlgItem(IDC_CHECK_SETTING_SYNC_TIME)->ShowWindow(FALSE);
				GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER)->ShowWindow(FALSE);

				GetDlgItem(IDC_STATIC_SETTING_TIME_ZONE)->ShowWindow(FALSE);
				GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE)->ShowWindow(FALSE);			
			}
			else
			{
				GetDlgItem(IDC_STATIC_SETTING_TIMER_SERVER)->ShowWindow(TRUE);
				GetDlgItem(IDC_CHECK_SETTING_SYNC_TIME)->ShowWindow(TRUE);
				GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER)->ShowWindow(TRUE);
				GetDlgItem(IDC_STATIC_SETTING_TIME_ZONE)->ShowWindow(TRUE);
				GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE)->ShowWindow(TRUE);	

				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->ResetContent();
				for(int j=0;j<sizeof(Time_Server_Name)/sizeof(Time_Server_Name[0]);j++)
				{
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->AddString(Time_Server_Name[j]);
				}
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE))->ResetContent();
				for (int z=0;z< sizeof(Time_Zone_Name)/sizeof(Time_Zone_Name[0]);z++)
				{
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE))->AddString(Time_Zone_Name[z]);
					if(Device_Basic_Setting.reg.time_zone == Time_Zone_Value[z])
						((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE))->SetWindowTextW(Time_Zone_Name[z]);
				}

				if(Device_Basic_Setting.reg.en_sntp == 1)	//disable
				{
					((CButton *)GetDlgItem(IDC_CHECK_SETTING_SYNC_TIME))->SetCheck(false);
					//((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetWindowTextW(Time_Server_Name[0]);

					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetCurSel(0);
				}
				else if(Device_Basic_Setting.reg.en_sntp == 2)
				{
					((CButton *)GetDlgItem(IDC_CHECK_SETTING_SYNC_TIME))->SetCheck(true);
					//((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetWindowTextW(Time_Server_Name[0]);
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetCurSel(0);
				}
				else if(Device_Basic_Setting.reg.en_sntp == 3)
				{
					((CButton *)GetDlgItem(IDC_CHECK_SETTING_SYNC_TIME))->SetCheck(true);
					//((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetWindowTextW(Time_Server_Name[1]);
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetCurSel(1);
				}
				else if(Device_Basic_Setting.reg.en_sntp == 4)
				{
					((CButton *)GetDlgItem(IDC_CHECK_SETTING_SYNC_TIME))->SetCheck(true);
					//((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetWindowTextW(Time_Server_Name[2]);
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetCurSel(2);
				}
				else if(Device_Basic_Setting.reg.en_sntp == 5)
				{
					((CButton *)GetDlgItem(IDC_CHECK_SETTING_SYNC_TIME))->SetCheck(true);

					CString temp_cs1;
					MultiByteToWideChar( CP_ACP, 0, Device_Basic_Setting.reg.sntp_server, (int)strlen((char *)Device_Basic_Setting.reg.sntp_server)+1, 
						temp_cs1.GetBuffer(MAX_PATH), MAX_PATH );
					temp_cs1.ReleaseBuffer();
				
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->AddString(temp_cs1);
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetCurSel(3);
					//((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetWindowTextW(temp_cs1);
				}
				//else if(Device_Basic_Setting.reg.en_sntp == 6)
				//{
				//	((CButton *)GetDlgItem(IDC_CHECK_SETTING_SYNC_TIME))->SetCheck(true);
				//	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetWindowTextW(Time_Server_Name[4]);
				//}
				else//��������ֵ���� ���Զ���λΪ��һ�� ͬ��������;
				{
					Device_Basic_Setting.reg.en_sntp = 2;
				}
			}

			CString temp_panel_number;
			CString temp_nodes_label;
			if(Device_Basic_Setting.reg.en_panel_name)
			{
				temp_panel_number.Format(_T("%u"),Device_Basic_Setting.reg.panel_number);
				MultiByteToWideChar( CP_ACP, 0, (char *)Device_Basic_Setting.reg.panel_name, 	(int)strlen((char *)Device_Basic_Setting.reg.panel_name)+1,temp_nodes_label.GetBuffer(MAX_PATH), MAX_PATH );
				temp_nodes_label.ReleaseBuffer();	

			}
			else
			{
				temp_panel_number.Format(_T("%u"),bac_gloab_panel);
				CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
				if(selected_product_index <  pFrame->m_product.size())
				{
					temp_nodes_label = pFrame->m_product.at(selected_product_index).NameShowOnTree;
				}
				else
				{
					temp_nodes_label.Empty();
				}
				
			}
			m_edit_panel.SetWindowTextW(temp_panel_number);
			m_edit_nodes_label.SetWindowTextW(temp_nodes_label);

			if(Device_Basic_Setting.reg.usb_mode == 1)
			{
				((CButton *)GetDlgItem(IDC_RADIO_USB_DEVICE))->SetCheck(false);
				((CButton *)GetDlgItem(IDC_RADIO_USB_HOST))->SetCheck(true);
				(CButton *)GetDlgItem(IDC_BUTTON_SETTING_GSM_MODUAL)->ShowWindow(TRUE);
			}
			else if(Device_Basic_Setting.reg.usb_mode == 0)
			{
				((CButton *)GetDlgItem(IDC_RADIO_USB_DEVICE))->SetCheck(true);
				((CButton *)GetDlgItem(IDC_RADIO_USB_HOST))->SetCheck(false);
				(CButton *)GetDlgItem(IDC_BUTTON_SETTING_GSM_MODUAL)->ShowWindow(FALSE);

			}

			if(Device_Basic_Setting.reg.sd_exist == 1)
			{
				((CStatic *)GetDlgItem(IDC_STATIC_BAC_SETTING_SD_CARD))->SetWindowTextW(_T("No SD Card"));
			}
			else if(Device_Basic_Setting.reg.sd_exist == 2)
			{
				((CStatic *)GetDlgItem(IDC_STATIC_BAC_SETTING_SD_CARD))->SetWindowTextW(_T("Normal"));
			}
			else
			{
				((CStatic *)GetDlgItem(IDC_STATIC_BAC_SETTING_SD_CARD))->SetWindowTextW(_T("unknown"));
			}

			if((bacnet_device_type == BIG_MINIPANEL) || (bacnet_device_type == SMALL_MINIPANEL) || (bacnet_device_type == TINY_MINIPANEL) || (bacnet_device_type == TINY_EX_MINIPANEL) || (bacnet_device_type == PRODUCT_CM5))
			{
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->ResetContent();
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->AddString(Device_Serial_Port_Status[NOUSE]);
				//((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->AddString(Device_Serial_Port_Status[MAIN_PTP]);
				//((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->AddString(Device_Serial_Port_Status[MAIN_ZIG]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->AddString(Device_Serial_Port_Status[SUB_MODBUS]);
				//((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->AddString(Device_Serial_Port_Status[SUB_GSM]);

				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->ResetContent();
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->AddString(Device_Serial_Port_Status[NOUSE]);
				//((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->AddString(Device_Serial_Port_Status[MAIN_MSTP]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->AddString(Device_Serial_Port_Status[MAIN_MODBUS]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->AddString(Device_Serial_Port_Status[SUB_MODBUS]);

				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->ResetContent();
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->AddString(Device_Serial_Port_Status[NOUSE]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->AddString(Device_Serial_Port_Status[MAIN_MSTP]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->AddString(Device_Serial_Port_Status[MAIN_MODBUS]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->AddString(Device_Serial_Port_Status[SUB_MODBUS]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->AddString(Device_Serial_Port_Status[MSTP_MASTER]);

				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->EnableWindow(TRUE);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->EnableWindow(TRUE);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->EnableWindow(TRUE);
				if(Device_Basic_Setting.reg.com0_config < MAX_COM_TYPE)
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->SetWindowTextW(Device_Serial_Port_Status[Device_Basic_Setting.reg.com0_config]);
				if(Device_Basic_Setting.reg.com1_config < MAX_COM_TYPE)
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->SetWindowTextW(Device_Serial_Port_Status[Device_Basic_Setting.reg.com1_config]);
				if(Device_Basic_Setting.reg.com2_config < MAX_COM_TYPE)
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->SetWindowTextW(Device_Serial_Port_Status[Device_Basic_Setting.reg.com2_config]);


				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->EnableWindow(1);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->EnableWindow(1);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->EnableWindow(1);

				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->ResetContent();
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->ResetContent();
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->ResetContent();


				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->AddString(Baudrate_Array[UART_9600]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->AddString(Baudrate_Array[UART_19200]);


				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->AddString(Baudrate_Array[UART_19200]);
				((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->EnableWindow(FALSE);
				for (int x=0;x< (sizeof(Baudrate_Array)/sizeof(Baudrate_Array[0]) - 1);x++)
				{
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->AddString(Baudrate_Array[x]);
				}


				if(Device_Basic_Setting.reg.com_baudrate0 == UART_9600)
				{
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->SetCurSel(0);
				}
				else if(Device_Basic_Setting.reg.com_baudrate0 == UART_19200)
				{
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->SetCurSel(1);
				}


				if(Device_Basic_Setting.reg.com_baudrate1 == UART_19200)
				{
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->SetCurSel(0);
				}
				else
				{
					Device_Basic_Setting.reg.com_baudrate1 = UART_19200;
				}
					
				if(Device_Basic_Setting.reg.com_baudrate2 < sizeof(Baudrate_Array)/sizeof(Baudrate_Array[0]))
					((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->SetCurSel(Device_Basic_Setting.reg.com_baudrate2);

			}

			if(bacnet_device_type == PRODUCT_CM5)
			{
				((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("CM5"));
			}
			else if(bacnet_device_type == BIG_MINIPANEL)
			{
				((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("T3-BB"));
			}
			else if(bacnet_device_type == SMALL_MINIPANEL)
			{
				((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("T3-LB"));
			}
			else if((bacnet_device_type == TINY_MINIPANEL) || (bacnet_device_type == TINY_EX_MINIPANEL))
			{
				((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("T3-TB"));
			}
			else
			{
				((CEdit *)GetDlgItem(IDC_STATIC_SEETING_DEVICE_NAME))->SetWindowTextW(_T("Unknown device"));
			}

			if((Device_Basic_Setting.reg.mini_type == BIG_MINIPANEL) || 
				(Device_Basic_Setting.reg.mini_type == SMALL_MINIPANEL) ||
				(Device_Basic_Setting.reg.mini_type == TINY_MINIPANEL) || 
				(Device_Basic_Setting.reg.mini_type == TINY_EX_MINIPANEL) ||
				(Device_Basic_Setting.reg.mini_type == PRODUCT_CM5))
			{
				temp_hw_version.Format(_T("%d"),Device_Basic_Setting.reg.pro_info.harware_rev);
				temp_pic_version.Format(_T("%d"),Device_Basic_Setting.reg.pro_info.frimware1_rev);
				temp_c8051_version.Format(_T("%d"),Device_Basic_Setting.reg.pro_info.frimware2_rev);
				temp_5964_version.Format(_T("%d"),Device_Basic_Setting.reg.pro_info.frimware3_rev);	
				temp_mcu_version.Format(_T("%d.%d"),Device_Basic_Setting.reg.pro_info.firmware0_rev_main,Device_Basic_Setting.reg.pro_info.firmware0_rev_sub);
				temp_bootloader_version.Format(_T("%d"),Device_Basic_Setting.reg.pro_info.bootloader_rev);
			}
			//temp_serial_number.Format(_T("%u"),g_selected_serialnumber);
			if(  ((int)Device_Basic_Setting.reg.pro_info.firmware0_rev_main) *10   +  (int)Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 417)
				temp_serial_number.Format(_T("%u"),Device_Basic_Setting.reg.n_serial_number);
			else
				temp_serial_number.Format(_T("%u"),g_selected_serialnumber);
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_HARDWARE_VERSION))->SetWindowTextW(temp_hw_version);
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_MCU_VERSION))->SetWindowTextW(temp_mcu_version);
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_PIC_VERSION))->SetWindowTextW(temp_pic_version);
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_C8051_VERSION))->SetWindowTextW(temp_c8051_version);
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_SM5964_VERSION2))->SetWindowTextW(temp_5964_version);
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_BOOTLOADER_VERSION))->SetWindowTextW(temp_bootloader_version);
			((CEdit *)GetDlgItem(IDC_STATIC_SEETING_SERIAL_NUMBER_2))->SetWindowTextW(temp_serial_number);

			if((Device_Basic_Setting.reg.user_name == 2) || (Device_Basic_Setting.reg.user_name = 1))
			{
				GetDlgItem(IDC_BUTTON_SETTING_USER_LIST)->ShowWindow(true);
			}
			else
			{
				GetDlgItem(IDC_BUTTON_SETTING_USER_LIST)->ShowWindow(false);
			}
			CString temp_object;
			CString temp_mac_address;
			CString temp_mstp_network;
			CString temp_bip_network;
			CString temp_modbus_id;
			temp_object.Format(_T("%u"),Device_Basic_Setting.reg.object_instance);
			temp_mac_address.Format(_T("%02x-%02x-%02x-%02x-%02x-%02x"),Device_Basic_Setting.reg.mac_addr[0],Device_Basic_Setting.reg.mac_addr[1],Device_Basic_Setting.reg.mac_addr[2],
															Device_Basic_Setting.reg.mac_addr[3],Device_Basic_Setting.reg.mac_addr[4],Device_Basic_Setting.reg.mac_addr[5]);
			temp_mac_address.MakeUpper();
			temp_mstp_network.Format(_T("%u"),Device_Basic_Setting.reg.mstp_network_number);
			temp_bip_network.Format(_T("%u"),Device_Basic_Setting.reg.network_number);
			if(Device_Basic_Setting.reg.modbus_id == 0)
			{
				temp_modbus_id.Empty();
				((CEdit *)GetDlgItem(IDC_EDIT_SETTING_MODBUS_ID))->EnableWindow(FALSE);
			}
			else
			{
				temp_modbus_id.Format(_T("%d"),Device_Basic_Setting.reg.modbus_id);
				((CEdit *)GetDlgItem(IDC_EDIT_SETTING_MODBUS_ID))->EnableWindow(TRUE);
			}
			((CEdit *)GetDlgItem(IDC_EDIT_SETTING_OBJ_INSTANCE))->SetWindowTextW(temp_object);
			((CEdit *)GetDlgItem(IDC_EDIT_SETTING_MAC_ADDRESS))->SetWindowTextW(temp_mac_address);
			((CEdit *)GetDlgItem(IDC_EDIT_SETTING_MSTP_NETWORK))->SetWindowTextW(temp_mstp_network);
			((CEdit *)GetDlgItem(IDC_EDIT_SETTING_BIP_NETWORK2))->SetWindowTextW(temp_bip_network);
			((CEdit *)GetDlgItem(IDC_EDIT_SETTING_MODBUS_ID))->SetWindowTextW(temp_modbus_id);

		}
		break;
	case TIME_COMMAND:
		{
			#pragma  region about_time
			if(Device_time.year<2000)
				temp_year = Device_time.year + 2000;
			if((Device_time.month == 0) || (Device_time.dayofmonth == 0))
				return 1;
			TimeTemp = CTime(temp_year,Device_time.month,Device_time.dayofmonth,Device_time.ti_hour,Device_time.ti_min,Device_time.ti_sec);


			m_cm5_time_picker.SetFormat(_T("HH:mm"));
			m_cm5_time_picker.SetTime(&TimeTemp);

			//m_cm5_date_picker.SetFormat(_T("YY/MM/DD"));
			m_cm5_date_picker.SetTime(&TimeTemp);
			#pragma endregion about_time
		}
		break;
	default: 
		break;
	}
	//GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->SetFocus();
	return 0;
}




BOOL CBacnetSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	InitScrollbar();
	SetWindowTextW(_T("Setting"));
	m_setting_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_setting_dlg_hwnd;
	m_cm5_date_picker.EnableWindow(1);
	m_cm5_time_picker.EnableWindow(1);
	GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(1);
	//GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(FALSE);
	m_cm5_time_picker.SetFormat(_T("HH:mm"));

	HICON hIcon = NULL; 
	HINSTANCE hInstResource    = NULL; 
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_OK), RT_GROUP_ICON); 
	hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_OK), IMAGE_ICON, 24, 24, 0);
	((CButton *)GetDlgItem(IDC_BUTTON_BAC_IP_CHANGED))->SetIcon(hIcon);

	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_SETTING);
	SetIcon(m_hIcon,TRUE);
	ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BAC_SETTING_OK)->SetFocus();

	SetTimer(TIMER_REFRESH_READ, 1000, NULL);

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBacnetSetting::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		int temp_focus_id = GetFocus()->GetDlgCtrlID();
		if((temp_focus_id == IDC_EDIT_SETTING_PANEL ) ||
		   (temp_focus_id == IDC_EDIT_SETTING_NODES_LABEL_SETTING) ||
			(temp_focus_id == IDC_EDIT_DYNDNS_USER_NAME) ||
			(temp_focus_id == IDC_EDIT_DYNDNS_PASSWORD) ||
			(temp_focus_id == IDC_EDIT_DYNDNS_DOMAIN) ||
			(temp_focus_id == IDC_EDIT_TIME_UPDATE) ||
			(temp_focus_id == IDC_EDIT_SETTING_PORT) ||
			(temp_focus_id == IDC_EDIT_SETTING_OBJ_INSTANCE) ||
			(temp_focus_id == IDC_COMBO_BACNET_SETTING_TIME_SERVER))
		{

			GetDlgItem(IDC_BUTTON_BAC_SETTING_OK)->SetFocus();
		}
		return 1;
	}
	else if(pMsg->message==WM_NCLBUTTONDBLCLK)
	{
		if(!window_max)
		{
			window_max = true;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//��ȡ view�Ĵ����С;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			window_max = false;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//��ȡ view�Ĵ����С;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left  + 90 ,temp_mynew_rect.top + 100,500,700,SWP_SHOWWINDOW);
		}

		return 1; 
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetSetting::OnTimer(UINT_PTR nIDEvent)
{
	 
	switch(nIDEvent)
	{
	case TIMER_SYNC_TIMER:
		{
			   ::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SETTING);
			   KillTimer(TIMER_SYNC_TIMER);

			   if((Device_Basic_Setting.reg.time_update_since_1970 < 1420041600)  || (Device_Basic_Setting.reg.time_update_since_1970 > 1735660800))
			   {
				   MessageBox(_T("SYNC time failed , No Reply from server"));
				   //SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("SYNC time failed , No Reply from server")); 
			   }
			   else
			   {
				   CTime time_scaletime;
				   CString strTime;
				   time_t scale_time  = Device_Basic_Setting.reg.time_update_since_1970;
				   time_scaletime = scale_time;
				   strTime = time_scaletime.Format("%y/%m/%d %H:%M:%S");
				    SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("SYNC time sucess")); 
					((CEdit *)GetDlgItem(IDC_EDIT_SETTING_LAST_UPDATE_TIME))->SetWindowTextW(strTime);
			   }

		}
		break;
	case TIMER_REFRESH_READ:
	{
		CString str;
		//KillTimer(TIMER_REFRESH_READ);
		if (g_hwnd_now != m_setting_dlg_hwnd)
		{
			if (TIMER_1s_Count>0) TIMER_1s_Count = 0;
			SetWindowText(_T(""));
			break;
		}
		if (TIMER_1s_Count < TIMER_REFRESH_READ_DELAY)
		{
			str.Format(_T("Auto refresh after %d"), TIMER_REFRESH_READ_DELAY - TIMER_1s_Count);
			SetWindowText(str);
			TIMER_1s_Count += 1;
			break;
		}
		str.Format(_T("Refreshing"));
		SetWindowText(str);
		TIMER_1s_Count = 0;
		if (this->IsWindowVisible())
		{

			if (g_protocol == MODBUS_RS485)
			{
				::PostMessage(BacNet_hwd, WM_RS485_MESSAGE, 0, READ_SETTING_COMMAND);//�ڶ������� In
			}
			else
			{
				if (bac_select_device_online)
					::PostMessage(BacNet_hwd, WM_FRESH_CM_LIST, MENU_CLICK, TYPE_SETTING);
			}
		}

	}
	break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}



// CBacnetInput message handlers
LRESULT  CBacnetSetting::ResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	CString Show_Results;
	CString temp_cs = pInvoke->task_info;
	if(msg_result)
	{
		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		TIMER_1s_Count = 0;
	}
	else
	{
		//memcpy_s(&m_Input_data.at(pInvoke->mRow),sizeof(Str_in_point),&m_temp_Input_data[pInvoke->mRow],sizeof(Str_in_point));//��ԭû�иĶԵ�ֵ
		PostMessage(WM_FRESH_SETTING_UI,READ_SETTING_COMMAND,NULL);//������� ˢ���߳�����ˢ�»᷽��һ��;
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);

	}
	//KillTimer(TIMER_REFRESH_READ);
	//SetTimer(TIMER_REFRESH_READ, TIMER_REFRESH_READ_DELAY, NULL);

	if(pInvoke)
		delete pInvoke;
	return 0;
}




void CBacnetSetting::OnCbnSelchangeComboBacnetSettingCom0()
{
	
	UpdateData();
	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM0))->GetCurSel();	
	m_device_com0.GetLBText(nSel,temp_string);


	for(int i=0;i<MAX_COM_TYPE;i++)
	{
		if(temp_string.CompareNoCase(Device_Serial_Port_Status[i]) == 0 )
		{
			Device_Basic_Setting.reg.com0_config = i;
			break;
		}
	}
	CString temp_task_info;
	temp_task_info.Format(_T("Change serial port 0 "));
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
}


void CBacnetSetting::OnCbnSelchangeComboBacnetSettingCom1()
{
	

	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->GetCurSel();	
	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM1))->GetLBText(nSel,temp_string);
	for(int i=0;i<MAX_COM_TYPE;i++)
	{
		if(temp_string.CompareNoCase(Device_Serial_Port_Status[i]) == 0 )
		{
			Device_Basic_Setting.reg.com1_config = i;
			break;
		}
	}
	CString temp_task_info;
	temp_task_info.Format(_T("Change serial port 1 "));
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
}


void CBacnetSetting::OnCbnSelchangeComboBacnetSettingCom2()
{
	

	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->GetCurSel();	
	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_COM2))->GetLBText(nSel,temp_string);
	for(int i=0;i<MAX_COM_TYPE;i++)
	{
		if(temp_string.CompareNoCase(Device_Serial_Port_Status[i]) == 0 )
		{
			Device_Basic_Setting.reg.com2_config = i;
			break;
		}
	}
	CString temp_task_info;
	temp_task_info.Format(_T("Change serial port 2 "));
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
}



void CBacnetSetting::OnCancel()
{
	

	//CDialogEx::OnCancel();
}


void CBacnetSetting::OnBnClickedButtonSettingLdf()
{
	
	if(IDYES == MessageBox(_T("Are you sure you want load factory default"),_T("Warning"),MB_YESNOCANCEL | MB_ICONINFORMATION))
	{
		Device_Basic_Setting.reg.reset_default = 88;
		CString temp_task_info;
		temp_task_info.Format(_T("Load Factory Default "));
		Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
	}
	
}


void CBacnetSetting::OnCbnSelchangeComboBacnetSettingBaudrate0()
{
	
	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->GetCurSel();	
	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE0))->GetLBText(nSel,temp_string);
	for(int i=0;i<sizeof(Baudrate_Array)/sizeof(Baudrate_Array[0]);i++)
	{
		if(temp_string.CompareNoCase(Baudrate_Array[i]) == 0 )
		{
			Device_Basic_Setting.reg.com_baudrate0 = i;
			break;
		}
	}
	CString temp_task_info;
	temp_task_info.Format(_T("Change serial port 0 baudrate "));
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);

}


void CBacnetSetting::OnCbnSelchangeComboBacnetSettingBaudrate1()
{
	
	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->GetCurSel();	
	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE1))->GetLBText(nSel,temp_string);
	for(int i=0;i<sizeof(Baudrate_Array)/sizeof(Baudrate_Array[0]);i++)
	{
		if(temp_string.CompareNoCase(Baudrate_Array[i]) == 0 )
		{
			Device_Basic_Setting.reg.com_baudrate1 = i;
			break;
		}
	}
	CString temp_task_info;
	temp_task_info.Format(_T("Change serial port 1 baudrate "));
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
}


void CBacnetSetting::OnCbnSelchangeComboBacnetSettingBaudrate2()
{
	
	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->GetCurSel();	
	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_BAUDRATE2))->GetLBText(nSel,temp_string);
	for(int i=0;i<sizeof(Baudrate_Array)/sizeof(Baudrate_Array[0]);i++)
	{
		if(temp_string.CompareNoCase(Baudrate_Array[i]) == 0 )
		{
			Device_Basic_Setting.reg.com_baudrate2 = i;
			break;
		}
	}
	CString temp_task_info;
	temp_task_info.Format(_T("Change serial port 2 baudrate "));
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
}


void CBacnetSetting::OnBnClickedButtonSettingCleanDb()
{
	
	if(IDYES == MessageBox(_T("Are you sure you want clean the device database"),_T("Warning"),MB_YESNOCANCEL | MB_ICONINFORMATION))
	{
		Device_Basic_Setting.reg.reset_default = 150;
		CString temp_task_info;
		temp_task_info.Format(_T("Clear device "));
		Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
	}
}


extern bool cancle_send ;
void CBacnetSetting::OnBnClickedButtonSettingAddIntoDb()
{
	
	CBacnetaddintodb dlg;
	dlg.DoModal();

	if(!cancle_send)
	{
		CString temp_task_info;
		temp_task_info.Format(_T("Add device into database "));
		Post_Write_Message(g_bac_instance,(int8_t)WRITE_SUB_ID_BY_HAND,0,0,254,this->m_hWnd,temp_task_info);
	}

}


void CBacnetSetting::OnBnClickedButtonSettingUserList()
{
	
	show_user_list_window = true;
	::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_READ_USER_LOGIN_INFO);
}


void CBacnetSetting::OnBnClickedRadioUsbDevice()
{
	
	if(Device_Basic_Setting.reg.usb_mode != 0)
	{
		Device_Basic_Setting.reg.usb_mode = 0;
		CString temp_task_info;
		temp_task_info.Format(_T("Change USB feature (USB Device) "));
		Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
	}

}


void CBacnetSetting::OnBnClickedRadioUsbHost()
{
	
		if(Device_Basic_Setting.reg.usb_mode != 1)
		{
			Device_Basic_Setting.reg.usb_mode = 1;
			CString temp_task_info;
			temp_task_info.Format(_T("Change USB feature (USB Host) "));
			Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
		}

}


void CBacnetSetting::OnBnClickedButtonSettingGsmModual()
{
	
	CBacnetATCommand Dlg;
	Dlg.DoModal();
}


void CBacnetSetting::OnClose()
{
	 
	ShowWindow(FALSE);
	return;
	CDialogEx::OnClose();
}


void CBacnetSetting::OnEnKillfocusEditSettingPanel()
{
	
	CString temp_cstring;
	m_edit_panel.GetWindowTextW(temp_cstring);
	int temp_panel = _wtoi(temp_cstring);
	if((temp_panel >0) && (temp_panel <255) && (temp_panel != Device_Basic_Setting.reg.panel_number))
	{
		CString temp_warning;
		temp_warning.Format(_T("Do you really want to change the panel number to %u ?"),temp_panel);
		if(IDYES == MessageBox(temp_warning,_T("Notoce"),MB_YESNO))
		{
			unsigned char old_panel = Device_Basic_Setting.reg.panel_number;	//д֮ǰ�ȱ���������дʧ�� �ָ�ԭֵ;
			Device_Basic_Setting.reg.panel_number = (unsigned char)temp_panel;
			if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change panel number failed!"));
				Device_Basic_Setting.reg.panel_number = old_panel;
				PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
			}
			else
			{
				Station_NUM = temp_panel;
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change panel number success!"));
			}
		}
	}
}


void CBacnetSetting::OnEnKillfocusEditSettingNodesLabelSetting()
{
	
	CString temp_cstring;
	m_edit_nodes_label.GetWindowTextW(temp_cstring);
	if(temp_cstring.GetLength()>=20)
	{
		MessageBox(_T("Nodes label length must between 0-20"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
		return;
	}

	if(Device_Basic_Setting.reg.en_panel_name)
	{
		char temp_char[30];
		WideCharToMultiByte(CP_ACP,NULL,temp_cstring.GetBuffer(),-1,temp_char,20,NULL,NULL);
		int n_ret = memcmp(temp_char,Device_Basic_Setting.reg.panel_name,20);
		if(n_ret == 0)
			return;
		memcpy(Device_Basic_Setting.reg.panel_name,temp_char,20);
		if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change Nodes label failed!"));
			memset(Device_Basic_Setting.reg.panel_name,0,20);
			PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
		}
		else
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change Nodes label success!"));


			CppSQLite3DB SqliteDBBuilding;
			CppSQLite3Table table;
			CppSQLite3Query q;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);


			CString temp_serial;
			CString strSql;
			temp_serial.Format(_T("%u"),g_selected_serialnumber);
			strSql.Format(_T("select * from ALL_NODE where Serial_ID='%s'"),temp_serial);
			
			q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
			//m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);

			while(!q.eof())
			{
				strSql.Format(_T("update ALL_NODE set Product_name='%s' where Serial_ID='%s'"),temp_cstring,temp_serial);
				SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
				q.nextRow();
				 
			}

			SqliteDBBuilding.closedb();
			CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
			if(selected_product_index < pFrame->m_product.size())
			{
				pFrame->m_pTreeViewCrl->SetItemText(pFrame->m_product.at(selected_product_index).product_item,temp_cstring);
				pFrame->m_product.at(selected_product_index).NameShowOnTree = temp_cstring;
			}
		}
	}


}


void CBacnetSetting::OnCbnSelchangeComboBacnetSettingTimeServer()
{
	
	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->GetCurSel();	
	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->GetLBText(nSel,temp_string);
	if(nSel == 0)
		Device_Basic_Setting.reg.en_sntp = 2;
	else if(nSel == 1)
		Device_Basic_Setting.reg.en_sntp = 3;
	else if(nSel == 2)
		Device_Basic_Setting.reg.en_sntp = 4;
	else if(nSel == 3)
		Device_Basic_Setting.reg.en_sntp = 5;
	else if(nSel == 4)
		Device_Basic_Setting.reg.en_sntp = 6;
	((CButton *)GetDlgItem(IDC_CHECK_SETTING_SYNC_TIME))->SetCheck(true);
	CString temp_task_info;
	temp_task_info.Format(_T("Change sntp time server to "));
	temp_task_info = temp_task_info + temp_string;
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);

}


void CBacnetSetting::OnBnClickedCheckSettingSyncTime()
{
	
	CString temp_task_info;
	if(Device_Basic_Setting.reg.en_sntp == 1)
	{
		Device_Basic_Setting.reg.en_sntp = 2;
		((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetWindowTextW(Time_Server_Name[0]);
		temp_task_info.Format(_T("Enable sntp "));
	}
	else
	{
		Device_Basic_Setting.reg.en_sntp = 1;
		((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_SERVER))->SetWindowTextW(Time_Server_Name[0]);
		temp_task_info.Format(_T("Disable sntp "));
	}

	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);

}


void CBacnetSetting::OnEnKillfocusEditDyndnsUserName()
{
	
	CString temp_cstring;
	m_dyndns_user.GetWindowTextW(temp_cstring);
	if(temp_cstring.GetLength()>=DYNDNS_MAX_USERNAME_SIZE)
	{
		MessageBox(_T("Nodes label length must between 0-32"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
		return;
	}
	char temp_char[DYNDNS_MAX_USERNAME_SIZE + 1];
	memset(temp_char , 0 ,DYNDNS_MAX_USERNAME_SIZE + 1);
	WideCharToMultiByte(CP_ACP,NULL,temp_cstring.GetBuffer(),-1,temp_char,DYNDNS_MAX_USERNAME_SIZE,NULL,NULL);
	int n_ret = memcmp(temp_char,Device_Basic_Setting.reg.dyndns_user,DYNDNS_MAX_USERNAME_SIZE);
	if(n_ret == 0)
		return;
	memcpy(Device_Basic_Setting.reg.dyndns_user,temp_char,DYNDNS_MAX_USERNAME_SIZE);
	if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change Dyndns user name failed!"));
		memset(Device_Basic_Setting.reg.dyndns_user,0,DYNDNS_MAX_USERNAME_SIZE);
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change Dyndns user name success!"));
	}
}


void CBacnetSetting::OnEnKillfocusEditDyndnsPassword()
{
	
	CString temp_cstring;
	m_dyndns_password.GetWindowTextW(temp_cstring);
	if(temp_cstring.GetLength()>=DYNDNS_MAX_PASSWORD_SIZE)
	{
		MessageBox(_T("Nodes label length must between 0-32"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
		return;
	}
	char temp_char[DYNDNS_MAX_PASSWORD_SIZE + 1];
	memset(temp_char , 0 ,DYNDNS_MAX_PASSWORD_SIZE + 1);
	WideCharToMultiByte(CP_ACP,NULL,temp_cstring.GetBuffer(),-1,temp_char,DYNDNS_MAX_PASSWORD_SIZE,NULL,NULL);
	int n_ret = memcmp(temp_char,Device_Basic_Setting.reg.dyndns_pass,DYNDNS_MAX_PASSWORD_SIZE);
	if(n_ret == 0)
		return;
	memcpy(Device_Basic_Setting.reg.dyndns_pass,temp_char,DYNDNS_MAX_PASSWORD_SIZE);
	if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change Dyndns password failed!"));
		memset(Device_Basic_Setting.reg.dyndns_pass,0,DYNDNS_MAX_PASSWORD_SIZE);
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change Dyndns password success!"));
	}
}


void CBacnetSetting::OnEnKillfocusEditDyndnsDomain()
{
	
	CString temp_cstring;
	m_dyndns_domain.GetWindowTextW(temp_cstring);
	if(temp_cstring.GetLength()>=DYNDNS_MAX_DOMAIN_SIZE)
	{
		MessageBox(_T("Nodes label length must between 0-32"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
		return;
	}
	char temp_char[DYNDNS_MAX_DOMAIN_SIZE + 1];
	memset(temp_char , 0 ,DYNDNS_MAX_DOMAIN_SIZE + 1);
	WideCharToMultiByte(CP_ACP,NULL,temp_cstring.GetBuffer(),-1,temp_char,DYNDNS_MAX_DOMAIN_SIZE,NULL,NULL);
	int n_ret = memcmp(temp_char,Device_Basic_Setting.reg.dyndns_domain,DYNDNS_MAX_DOMAIN_SIZE);
	if(n_ret == 0)
		return;
	memcpy(Device_Basic_Setting.reg.dyndns_domain,temp_char,DYNDNS_MAX_DOMAIN_SIZE);
	if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change Dyndns domain failed!"));
		memset(Device_Basic_Setting.reg.dyndns_domain,0,DYNDNS_MAX_DOMAIN_SIZE);
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change dyndns domain success!"));
	}
}


void CBacnetSetting::OnBnClickedCheckSettingDyndns()
{
	

		
		CString temp_task_info;
		if(Device_Basic_Setting.reg.en_dyndns == 1)
		{
			Device_Basic_Setting.reg.en_dyndns = 2;
			temp_task_info.Format(_T("Enable Dyndns "));
		}
		else
		{
			Device_Basic_Setting.reg.en_dyndns = 1;
			temp_task_info.Format(_T("Disable Dyndns "));
		}

		Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);

}


void CBacnetSetting::OnCbnSelchangeComboBacnetSettingDdnsServer()
{
	
	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->GetCurSel();	
	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_DDNS_SERVER))->GetLBText(nSel,temp_string);
	if(nSel == 0)
		Device_Basic_Setting.reg.dyndns_provider = 0;
	else if(nSel == 1)
		Device_Basic_Setting.reg.dyndns_provider = 1;
	else if(nSel == 2)
		Device_Basic_Setting.reg.dyndns_provider = 2;
	else if(nSel == 3)
		Device_Basic_Setting.reg.dyndns_provider = 3;

	CString temp_task_info;
	temp_task_info.Format(_T("Change DDNS server to "));
	temp_task_info = temp_task_info + temp_string;
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
}


void CBacnetSetting::OnEnKillfocusEditTimeUpdate()
{
	
	CString temp_cstring;
	m_edit_ddns_update_time.GetWindowTextW(temp_cstring);
	int update_time = _wtoi(temp_cstring);
	
	
	if((temp_cstring.IsEmpty()) || (update_time ==0) ||  (update_time >65535))
	{
		MessageBox(_T("value must between 0-65535"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
		return;
	}

	Device_Basic_Setting.reg.dyndns_update_time = update_time;

	if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change DDNS Time failed!"));
		Device_Basic_Setting.reg.dyndns_update_time = 0;
		PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change DDNS Time success!"));
	}

}


void CBacnetSetting::OnCbnSelchangeComboBacnetSettingTimeZone()
{
	
	CString temp_string;
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE))->GetCurSel();	
	((CComboBox *)GetDlgItem(IDC_COMBO_BACNET_SETTING_TIME_ZONE))->GetLBText(nSel,temp_string);
	if(nSel >= sizeof(Time_Zone_Value)/sizeof(Time_Zone_Value[0]))
		return;
	Device_Basic_Setting.reg.time_zone = Time_Zone_Value[nSel];

	CString temp_task_info;
	temp_task_info.Format(_T("Change Time Zone to "));
	temp_task_info = temp_task_info + temp_string;
	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);

}


void CBacnetSetting::OnBnClickedCheckSettingPap()
{
	
		CString temp_task_info;
		if(Device_Basic_Setting.reg.en_plug_n_play != 1)
		{
			Device_Basic_Setting.reg.en_plug_n_play = 1;
			temp_task_info.Format(_T("Enable Plug And Play "));
		}
		else
		{
			Device_Basic_Setting.reg.en_plug_n_play = 0;
			temp_task_info.Format(_T("Disable Plug And Play "));
		}

		Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
}


void CBacnetSetting::OnEnKillfocusEditSettingPort()
{
	
	CString temp_cstring;
	m_edit_port.GetWindowTextW(temp_cstring);
	int temp_port = _wtoi(temp_cstring);
	if((temp_port >0) && (temp_port <65535) && (temp_port != Device_Basic_Setting.reg.modbus_port))
	{
		CString temp_warning;
		temp_warning.Format(_T("Do you really want to change the modbus port to %u ?"),temp_port);
		if(IDYES == MessageBox(temp_warning,_T("Notoce"),MB_YESNO))
		{
			unsigned short old_port = Device_Basic_Setting.reg.modbus_port;	//д֮ǰ�ȱ���������дʧ�� �ָ�ԭֵ;
			Device_Basic_Setting.reg.modbus_port = (unsigned short)temp_port;
			if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change port failed!"));
				Device_Basic_Setting.reg.modbus_port = old_port;
				PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
			}
			else
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change port success!"));
			}
		}
	}
}


void CBacnetSetting::OnBnClickedButtonHealth()
{
	
	GetPrivateData_Blocking(g_bac_instance,READ_MISC,0,0,sizeof(Str_MISC));
	CBacnetSettingHealth Health_Dlg;
	Health_Dlg.DoModal();
}


void CBacnetSetting::OnEnKillfocusEditSettingObjInstance()
{
	
	CString temp_cstring;
	m_setting_obj_instance.GetWindowTextW(temp_cstring);
	unsigned int temp_obj_instance = unsigned int(_wtoi(temp_cstring));
	if((temp_obj_instance >0) && (temp_obj_instance <= MAX_OBJ_INSTANCE) && (temp_obj_instance != Device_Basic_Setting.reg.object_instance))
	{
		CString temp_warning;
		temp_warning.Format(_T("Do you really want to change the bacnet object instance to %u ?"),temp_obj_instance);
		if(IDYES == MessageBox(temp_warning,_T("Notoce"),MB_YESNO))
		{
			unsigned int old_object_instance = Device_Basic_Setting.reg.object_instance;	//д֮ǰ�ȱ���������дʧ�� �ָ�ԭֵ;
			Device_Basic_Setting.reg.object_instance = (unsigned int)temp_obj_instance;
			if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change object instance failed!"));
				Device_Basic_Setting.reg.object_instance = old_object_instance;
				PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
			}
			else
			{
				g_bac_instance = temp_obj_instance;
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change object instance success!"));
			}
		}
		else
		{
			PostMessage(WM_FRESH_SETTING_UI,READ_SETTING_COMMAND,NULL);//������� ˢ���߳�����ˢ�»᷽��һ��;
		}
	}
}


BOOL CBacnetSetting::OnHelpInfo(HELPINFO* pHelpInfo)
{
	 
	// 	if((m_latest_protocol == PROTOCOL_BACNET_IP) || (m_latest_protocol == MODBUS_BACNET_MSTP) || (g_protocol == PROTOCOL_BIP_TO_MSTP))
	// 	{
	HWND hWnd;

	if(pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
	else
		hWnd =  ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_PANEL_CONFIGURATION);
	return (hWnd != NULL);
	// 	}
	// 	else{
	// 		::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
	// 	}
	return CDialogEx::OnHelpInfo(pHelpInfo);
}

void CBacnetSetting::OnBnClickedButtonBacSettingOk()
{
	
	if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
	{
		CString temp_task_info;
		temp_task_info.Format(_T("Write into device timeout!"));
		SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_task_info);
	}
	refresh_tree_status_immediately = true;
}


void CBacnetSetting::OnBnClickedButtonBacSettingCancel()
{
	
	 PostMessage(WM_FRESH_SETTING_UI,READ_SETTING_COMMAND,NULL);
	 PostMessage(WM_FRESH_SETTING_UI,TIME_COMMAND,NULL);
}


void CBacnetSetting::OnBnClickedButtonSyncTime()
{
	

	Device_Basic_Setting.reg.reset_default = 99;
	CString temp_task_info;
	temp_task_info.Format(_T("SYNC Time Server "));
	((CButton *)GetDlgItem(IDC_BUTTON_SYNC_TIME))->EnableWindow(FALSE);
	SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("SYNC Timer Server Now.Please Wait....."));
	if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
	{
		Sleep(1);
	}
	else
	{
		SetTimer(TIMER_SYNC_TIMER,3000,NULL);
		//SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Send SYNC time success!"));
	}

	((CButton *)GetDlgItem(IDC_BUTTON_SYNC_TIME))->EnableWindow(TRUE);
	
}


void CBacnetSetting::OnBnClickedCheckSettingZoneDaylightTime()
{
	
	 
	CString temp_task_info;
	if(Device_Basic_Setting.reg.time_zone_summer_daytime == 1)
	{
		Device_Basic_Setting.reg.time_zone_summer_daytime = 0;
		temp_task_info.Format(_T("Disable Automatically adjust clock for Daylight Saving Time "));
	}
	else
	{
		Device_Basic_Setting.reg.time_zone_summer_daytime = 1;
		temp_task_info.Format(_T("Enable Automatically adjust clock for Daylight Saving Time"));
	}

	Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);

}


void CBacnetSetting::OnBnClickedButtonBacShowZigbee()
{
	
	CTstatZigbeeLogic dlg;
	dlg.DoModal();
}


void CBacnetSetting::OnCbnKillfocusComboBacnetSettingTimeServer()
{
	
	CString temp_string;
	GetDlgItemText(IDC_COMBO_BACNET_SETTING_TIME_SERVER,temp_string);
	if(!temp_string.IsEmpty())
	{
		if((temp_string.CompareNoCase(Time_Server_Name[0])  == 0) ||
			(temp_string.CompareNoCase(Time_Server_Name[1])  == 0) ||
			(temp_string.CompareNoCase(Time_Server_Name[2])  == 0))
		{

		}
		else
		{
			Device_Basic_Setting.reg.en_sntp = 5;
			WideCharToMultiByte(CP_ACP,NULL,temp_string.GetBuffer(),-1,Device_Basic_Setting.reg.sntp_server,30,NULL,NULL);
			CString temp_task_info;
			temp_task_info.Format(_T("Change sntp server to "));
			temp_task_info = temp_task_info + temp_string;
			Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
		}
	}



}



void CBacnetSetting::Reset_Setting_Rect()
{

	CRect temp_mynew_rect;
	::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//��ȡ view�Ĵ����С;

	CRect temp_window;
	GetWindowRect(&temp_window);

	if(window_max)
	{
		CRect temp_mynew_rect;
		::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//��ȡ view�Ĵ����С;
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), NULL);
	}
	else if((temp_window.Width() <= temp_mynew_rect.Width() ) && (temp_window.Height() <= temp_mynew_rect.Height()))
	{
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,0,0,SWP_NOSIZE );
	}
	else
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left + 90,temp_mynew_rect.top + 70,500,700, NULL);


	return;

}



void CBacnetSetting::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	
	CRect rc;
	GetClientRect(rc);
	if(this->m_hWnd != NULL)
	{
		::SetWindowPos(this->m_hWnd, HWND_TOP, 0,0, 0,0,  SWP_NOSIZE | SWP_NOMOVE);
	}
}


void CBacnetSetting::OnSysCommand(UINT nID, LPARAM lParam)
{
	 
	if(nID == SC_MAXIMIZE)
	{
		if(window_max == false)
		{
			window_max = true;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//��ȡ view�Ĵ����С;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			window_max = false;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//��ȡ view�Ĵ����С;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left  + 90 ,temp_mynew_rect.top + 100,500,700,SWP_SHOWWINDOW);
		}
		return;
	}

	CDialogEx::OnSysCommand(nID, lParam);
}




void CBacnetSetting::OnEnKillfocusEditSettingModbusId()
{
	
	CString temp_cstring;
	m_edit_modbus_id.GetWindowTextW(temp_cstring);
	unsigned int temp_modbusid = unsigned int(_wtoi(temp_cstring));
	if((temp_modbusid<1) || (temp_modbusid >254))
	{
		MessageBox(_T("Invalid value."));
		return;
	}
	if((temp_modbusid >0) && (temp_modbusid <= 254) && (temp_modbusid != Device_Basic_Setting.reg.modbus_id))
	{
		CString temp_warning;
		temp_warning.Format(_T("Do you really want to change the modbus ID to %u ?"),temp_modbusid);
		if(IDYES == MessageBox(temp_warning,_T("Notoce"),MB_YESNO))
		{
			unsigned char old_modbusid = Device_Basic_Setting.reg.modbus_id;	//д֮ǰ�ȱ���������дʧ�� �ָ�ԭֵ;
			Device_Basic_Setting.reg.modbus_id = (unsigned char)temp_modbusid;
			if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change object instance failed!"));
				Device_Basic_Setting.reg.modbus_id = old_modbusid;
				PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
			}
			else
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Change object instance success!"));
			}
		}
		else
		{
			PostMessage(WM_FRESH_SETTING_UI,READ_SETTING_COMMAND,NULL);//������� ˢ���߳�����ˢ�»᷽��һ��;
		}
	}
}


void CBacnetSetting::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	 
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CBacnetSetting::OnBnClickedButtonSettingIoConfig()
{
	
	//�汾����38.6 �Ĳ�����setting �����port �Ĺ���
	if(Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 +Device_Basic_Setting.reg.pro_info.firmware0_rev_sub < 438)
	{
		MessageBox(_T("This feature need the newest firmware."));
		return;
	}


	if(GetPrivateData_Blocking(g_bac_instance,READEXT_IO_T3000,0,BAC_EXTIO_COUNT - 1,sizeof(Str_Extio_point)) < 0)
	{
		MessageBox(_T("Read data timeout"));
		return;
	}
	CBacnetIOConfig IOdlg;
	IOdlg.DoModal();
}


void CBacnetSetting::OnBnClickedButtonRebootDevice()
{
	
	//�汾����38.6 �Ĳ�����setting �����port �Ĺ���
	if(Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 +Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 441)
	{
		if(IDYES == MessageBox(_T("Are you sure you want reboot device"),_T("Warning"),MB_YESNOCANCEL | MB_ICONINFORMATION))
		{
			Device_Basic_Setting.reg.reset_default = 111;
			CString temp_task_info;
			temp_task_info.Format(_T("Reboot device "));
			Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);
		}
	}
	else
	{
		MessageBox(_T("Your panel version is too old."),_T("Notice"),MB_OK);
	}

}

void CBacnetSetting::InitScrollbar()
{
	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);    
	scrollinfo.nPage=20;    //���û����С
	scrollinfo.nMax=75;     //���ù����������λ��0--75
	SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
}

void CBacnetSetting::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	 

	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
	int unit=3;        
	switch (nSBCode)  
	{      
	case SB_LINEUP:          //Scroll one line up
		scrollinfo.nPos -= 1;  
		if (scrollinfo.nPos<scrollinfo.nMin)
		{  
			scrollinfo.nPos = scrollinfo.nMin;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,unit); 
		break;  
	case SB_LINEDOWN:           //Scroll one line down
		scrollinfo.nPos += 1;  
		if (scrollinfo.nPos+scrollinfo.nPage>scrollinfo.nMax)  //�˴�һ��Ҫע����ϻ���ĳ��ȣ������ж�
		{  
			scrollinfo.nPos = scrollinfo.nMax;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,-unit);  
		break;  
	case SB_PAGEUP:            //Scroll one page up.
		scrollinfo.nPos -= 5;  
		if (scrollinfo.nPos<=scrollinfo.nMin)
		{  
			scrollinfo.nPos = scrollinfo.nMin;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,unit*5);  
		break;  
	case SB_PAGEDOWN:        //Scroll one page down        
		scrollinfo.nPos += 5;  
		if (scrollinfo.nPos+scrollinfo.nPage>=scrollinfo.nMax)  //�˴�һ��Ҫע����ϻ���ĳ��ȣ������ж�
		{  
			scrollinfo.nPos = scrollinfo.nMax;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,-unit*5);  
		break;  
	case SB_ENDSCROLL:      //End scroll     
		break;  
	case SB_THUMBPOSITION:  //Scroll to the absolute position. The current position is provided in nPos
		break;  
	case SB_THUMBTRACK:                  //Drag scroll box to specified position. The current position is provided in nPos
		ScrollWindow(0,(scrollinfo.nPos-nPos)*unit);  
		scrollinfo.nPos = nPos;  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;  
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

