#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomControl.h"


BOOL CALLBACK CCustomMonitors::CustomMonitorNumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	CCustomMonitors* pmon = (CCustomMonitors*)dwData;

	// j^îñÌæ¾
	MONITORINFOEX mInfo;
	mInfo.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &mInfo);

	// j^ÌæÌæ¾
	CRect rect(lprcMonitor);
	pmon->AddMonitor(hMonitor, rect, &mInfo);

	return TRUE;
}

CCustomMonitors::CCustomMonitors()
{
	// }`j^Î
	EnumDisplayMonitors(NULL, NULL, CustomMonitorNumProc, (LPARAM)this);
}

CCustomMonitors::~CCustomMonitors()
{
}

//------------------------------------------------------------------------------------ 
//	TvF
//------------------------------------------------------------------------------------
CCustomControl::CCustomControl()
{
}

CCustomControl::~CCustomControl()
{
}

/*============================================================================*/
/*! JX^Rg[

-# JX^Rg[Ìú»

@param	pParent		CEBhEnh
@param	appPath		AvP[VpX
@retval

*/
/*============================================================================*/
void CCustomControl::Initialize(CWnd* pParent, CString appPath)
{
	mMainWnd = pParent;

	/// }X^pf[^i[pX
	mAppMasterDataPath = appPath + _T("\\") + mAppDataMasterPath;

	/// fpf[^i[pX
	mAppUserDataPath = appPath + _T("\\") + mAppDataUserPath;

	/// f[^x[XpX
	mAppDataBasePath = appPath + _T("\\") + _T("db");

	/// SÄÌEBhEnhAm[hÌí
	GetDataManager().DeleteAll();

	/// }X^ÝõÚ×f[^ðÇÝÞ
	createMasterEquipment();

	/// [UÝõÚ×f[^ðÇÝÞ
	createUserEquipment();

	// f[^x[Xæ¾
	const CString mpDBName[] = {
		_T("eqmon.db.obj"),
		_T("eqctl.db.obj"),
	};
	// ÄDBæ¾
	CString str = mAppDataBasePath + _T("\\") + mpDBName[0];
	GetDataManager().GetDataMonitor().eq_db_read(str);
	// §äDBæ¾
	str = mAppDataBasePath + _T("\\") + mpDBName[1];
	GetDataManager().GetDataControl().eq_db_read(str);

	CreateCustomData(appPath + _T("\\CustomData.txt"));
}

/*============================================================================*/
/*! JX^Rg[

-# ÝõÚ×EBhEÌì¬

@param		pnode		m[hîñ
@retval

*/
/*============================================================================*/
CCustomDetail* CCustomControl::CreateEquipment(CTreeNode* pnode, UINT mode/* = eTreeItemMode_Monitor*/)
{
	// ÝõÚ×æÊÌì¬
	CCustomDetail* pitem = new CCustomDetail(theApp.GetMainWnd(), (pnode == NULL) ? false : true);
	if (pnode != NULL) {
		pnode->GetEquipment().wnd = pitem;
		pnode->GetEquipment().manager = &mCustomManager;
		pnode->GetEquipment().placement.showCmd = SW_HIDE;
	}
	// ÝõÚ×æÊÌì¬
	pitem->Create(IDD_DIALOG_EQDETAIL, theApp.GetMainWnd());
	//pitem->ShowWindow((pnode == NULL) ? SW_SHOW : pnode->GetEquipment().placement.showCmd);
	pitem->ShowWindow(SW_HIDE);

	// ÇEBhEÌÝè
	if (pnode == NULL) {
		CTreeNode* pnodeitem = GetDataManager().SearchWndNode(pitem);
		pnodeitem->GetEquipment().wnd = pitem;
		pnodeitem->GetEquipment().manager = &mCustomManager;
		//swprintf_s(pnodeitem->GetWindowInfo().groupname, mNameSize, _T("%s"), (LPCTSTR)mNoGroupText);
		pnodeitem->GetManager().groupno = 0;
		pnodeitem->GetEquipment().mode = mode;
		pnodeitem->GetEquipment().kind = eTreeItemKind_User;
	}
	return pitem;
}

/*============================================================================*/
/*! JX^Rg[

-# }X^ÝõÚ×f[^Ìì¬

@param
@retval

*/
/*============================================================================*/
void CCustomControl::createMasterEquipment()
{
	CWaitCursor wait;

	vector<CString> filelist;
	getMasterEquipmentFiles(filelist);
	vector<CString>::iterator itrmaster;
	for (itrmaster = filelist.begin(); itrmaster != filelist.end(); itrmaster++) {
		//=====================================================//
		//«««««««««««« Log ««««««««««««//
		CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("createMasterEquipment"), (*itrmaster), _T(""), nLogEx::info);
		//ªªªªªªªªªªªª Log ªªªªªªªªªªªª//
		//=====================================================//
		GetDataManager().LoadTreeDataXml((*itrmaster), eTreeItemKind_Master);
	}
}

/*============================================================================*/
/*! JX^Rg[

-# [UÝõÚ×f[^Ìì¬

@param
@retval

*/
/*============================================================================*/
void CCustomControl::createUserEquipment()
{
	CWaitCursor wait;

	vector<CString> filelist;
	getUserEquipmentFiles(filelist);
	vector<CString>::iterator itruser;
	for (itruser = filelist.begin(); itruser != filelist.end(); itruser++) {
		//=====================================================//
		//«««««««««««« Log ««««««««««««//
		CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("createUserEquipment"), (*itruser), _T(""), nLogEx::info);
		//ªªªªªªªªªªªª Log ªªªªªªªªªªªª//
		//=====================================================//
		GetDataManager().LoadTreeDataXml((*itruser), eTreeItemKind_User);
	}
}

/*============================================================================*/
/*! JX^Rg[

-# }X^ÝõÚ×t@CÌæ¾

@param
@retval

*/
/*============================================================================*/
void CCustomControl::getMasterEquipmentFiles(vector<CString>& list)
{
	//=====================================================//
	//«««««««««««« Log ««««««««««««//
	CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("getMasterEquipmentFiles"), _T("Start"), _T(""), nLogEx::debug);
	//ªªªªªªªªªªªª Log ªªªªªªªªªªªª//
	//=====================================================//
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;

	list.clear();

	//g£qÌÝè
	CString search_name = mAppMasterDataPath + _T("\\*.xml");

	hFind = FindFirstFile(search_name, &win32fd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		}
		else {
			list.push_back(mAppMasterDataPath + _T("\\") + CString(win32fd.cFileName));
		}
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);
	//=====================================================//
	//«««««««««««« Log ««««««««««««//
	CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("getMasterEquipmentFiles"), _T("Stop"), _T(""), nLogEx::debug);
	//ªªªªªªªªªªªª Log ªªªªªªªªªªªª//
	//=====================================================//
}

/*============================================================================*/
/*! JX^Rg[

-# [UÝõÚ×t@CÌæ¾

@param
@retval

*/
/*============================================================================*/
void CCustomControl::getUserEquipmentFiles(vector<CString>& list)
{
	//=====================================================//
	//«««««««««««« Log ««««««««««««//
	CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("getUserEquipmentFiles"), _T("Start"), _T(""), nLogEx::debug);
	//ªªªªªªªªªªªª Log ªªªªªªªªªªªª//
	//=====================================================//
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;

	list.clear();

	//g£qÌÝè
	CString search_name = mAppUserDataPath + _T("\\*.xml");

	hFind = FindFirstFile(search_name, &win32fd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		}
		else {
			list.push_back(mAppUserDataPath + _T("\\") + CString(win32fd.cFileName));
		}
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);
	//=====================================================//
	//«««««««««««« Log ««««««««««««//
	CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("getUserEquipmentFiles"), _T("Stop"), _T(""), nLogEx::debug);
	//ªªªªªªªªªªªª Log ªªªªªªªªªªªª//
	//=====================================================//
}

#include <locale.h>
/*============================================================================*/
/*! JX^Rg[

-# [UÝõÚ×t@CÌæ¾

@param
@retval

*/
/*============================================================================*/
void CCustomControl::CreateCustomData(CString fname)
{
	TCHAR* ploc = _wsetlocale(LC_ALL, _T("japanese"));

	CStdioFile	file(fname, CFile::modeRead | CFile::typeText);

	CString	cbuf, str;
	CString title, mainnode, subnode, leaf, mon, unit, cntl;
	BOOL bRead = file.ReadString(cbuf);
	while (bRead) {
		CString mon;
		stCustomData st;
		AfxExtractSubString(str, cbuf, 0, '\t');
		mon = str;
		AfxExtractSubString(str, cbuf, 1, '\t');
		swprintf_s(st.data, 80, _T("%s"), (LPCTSTR)str);
		AfxExtractSubString(str, cbuf, 2, '\t');
		swprintf_s(st.color, 10, _T("%s"), (LPCTSTR)str);
		mCustomDataList[mon] = st;
		bRead = file.ReadString(cbuf);
	}
	file.Close();
}
/*============================================================================*/
/*! JX^Rg[

-# [UÝõÚ×t@CÌæ¾

@param
@retval

*/
/*============================================================================*/
COLORREF CCustomControl::GetCustomData(CString mon_string, CString& data)
{
	stCustomData st;

	map<CString, stCustomData>::iterator itr = mCustomDataList.find(mon_string);
	if (itr == mCustomDataList.end()) {
		data.Empty();
		return mDefaultValueTextColor;
	}

	st = mCustomDataList[mon_string];

	data = CString(st.data);
	COLORREF col = mDefaultValueTextColor;
	if (CString(st.color) == _T("Ô"))
		col = mDefaultErrorValueTextColor;
	if (CString(st.color) == _T("Î"))
		col = mDefaultValueTextColor;
	if (CString(st.color) == _T("©"))
		col = mDefaultWarningValueTextColor;

	return col;
}
