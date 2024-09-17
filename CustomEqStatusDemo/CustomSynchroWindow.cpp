#include "stdafx.h"
#include "CustomSynchroWindow.h"

CCustomSynchroWindow::CCustomSynchroWindow()
{
	mGroupWindowList.clear();
}


CCustomSynchroWindow::~CCustomSynchroWindow()
{
}

/*============================================================================*/
/*! �E�B���h�E�A��

-# ��ʘA���̐ݒ�

@param	group		�O���[�v�ԍ�(1-)
@param	pbase		�E�B���h�E�n���h��

@retval
*/
/*============================================================================*/
void CCustomSynchroWindow::Set(UINT group, CWnd* pbase)
{
	GroupInfo search = GroupInfo(LOWORD(group), pbase);
	map< UINT, vector<GroupInfo> >::iterator itr;
	itr = mGroupWindowList.find(HIWORD(group));
	if (itr != mGroupWindowList.end()) {
		vector<GroupInfo>::iterator itrlist;
		itrlist = std::find((*itr).second.begin(), (*itr).second.end(), search);
		if (itrlist == (*itr).second.end()) {
			(*itr).second.push_back(search);
		}
	}
	else {
		vector<GroupInfo> temp;
		temp.push_back(search);
		mGroupWindowList.insert(map<UINT, vector<GroupInfo>>::value_type(HIWORD(group), temp));
		itr = mGroupWindowList.find(HIWORD(group));
	}

	sort((*itr).second.begin(), (*itr).second.end());
}

/*============================================================================*/
/*! �E�B���h�E�A��

-# ��ʘA���̐擪�E�B���h�E�̎擾

@param	group		�O���[�v�ԍ�(1-)

@retval
*/
/*============================================================================*/
void CCustomSynchroWindow::Start()
{
	CRect rect;
	map< UINT, vector<GroupInfo> >::iterator itr;
	for (itr = mGroupWindowList.begin(); itr != mGroupWindowList.end(); itr++) {
		vector<GroupInfo>::iterator itrlist;
		for (itrlist = (*itr).second.begin(); itrlist != (*itr).second.end(); itrlist++) {
			if ((*itrlist).innerno == 1 && (*itrlist).wnd) {
				(*itrlist).wnd->GetWindowRect(rect);
				Move(((*itr).first<<16 | (*itrlist).innerno), (*itrlist).wnd, rect);
				break;
			}
		}
	}
}

/*============================================================================*/
/*! �E�B���h�E�A��

-# �C�ӂ̃E�B���h�E�̈ړ�

@param	group		�O���[�v�ԍ�(1-)
@param	pbase		�ړ��Ώۂ̃E�B���h�E
@param	rectbase	�ړ��Ώۂ̃E�B���h�E��`

@retval
*/
/*============================================================================*/
void CCustomSynchroWindow::Move(UINT group, CWnd* pbase, CRect rectbase)
{
	CRect rect, rc;
	rect = rectbase;

	GroupInfo search = GroupInfo(LOWORD(group), pbase);

	map< UINT, vector<GroupInfo> >::iterator itr;
	itr = mGroupWindowList.find(HIWORD(group));
	if (itr == mGroupWindowList.end()) {
		// �ΏۃO���[�v�����݂��Ȃ��̂ŉ������Ȃ�
		return;
	}
	vector<GroupInfo>::iterator itrLeft;
	itrLeft = std::find((*itr).second.begin(), (*itr).second.end(), search);
	if (itrLeft == (*itr).second.end()) {
		// �ΏۃE�B���h�E�����݂��Ȃ��̂ŉ������Ȃ�
		return;
	}

	// �ΏۃE�B���h�E�̍����ړ�
	vector<GroupInfo>::reverse_iterator itrRight;
	bool bfind = false;
	rect = rectbase;
	for (itrRight = (*itr).second.rbegin(); itrRight != (*itr).second.rend(); itrRight++) {
		if (bfind == true) {
			(*itrRight).wnd->GetWindowRect(rc);
			rect.right = rect.left + (GetSystemMetrics(SM_CXSIZEFRAME) * 3);
			rect.left = rect.right - rc.Width();
			(*itrRight).wnd->MoveWindow(rect.left, rect.top, rc.Width(), rc.Height());
			SetWindowPos((*itrRight).wnd->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE| SWP_NOACTIVATE);
			//rect.right -= rc.Width();
		}
		else {
			if ((*itrRight).wnd == pbase) {
				bfind = true;
			}
		}
	}

	// �ΏۃE�B���h�E�̉E���ړ�
	bfind = false;
	rect = rectbase;
	for (itrLeft = (*itr).second.begin(); itrLeft != (*itr).second.end(); itrLeft++) {
		if (bfind == true) {
			(*itrLeft).wnd->GetWindowRect(rc);
			rect.right -= (GetSystemMetrics(SM_CXSIZEFRAME) * 3);
			(*itrLeft).wnd->MoveWindow(rect.right, rect.top, rc.Width(), rc.Height());
			SetWindowPos((*itrLeft).wnd->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE| SWP_NOACTIVATE);
			rect.right += rc.Width();
		}
		else {
			if ((*itrLeft).wnd == pbase) {
				bfind = true;
			}
		}
	}
}

/*============================================================================*/
/*! �E�B���h�E�A��

-# �C�ӂ̃E�B���h�E�����

@param	group		�O���[�v�ԍ�(1-)
@param	pbase		�ړ��Ώۂ̃E�B���h�E

@retval
*/
/*============================================================================*/
void CCustomSynchroWindow::Close(UINT group, CWnd* pbase)
{
	map< UINT, vector<GroupInfo> >::iterator itr;
	itr = mGroupWindowList.find(HIWORD(group));
	if (itr == mGroupWindowList.end()) {
		// �ΏۃO���[�v�����݂��Ȃ��̂ŉ������Ȃ�
		return;
	}

	vector<GroupInfo>::iterator itrlist;
	for (itrlist = (*itr).second.begin(); itrlist != (*itr).second.end(); itrlist++) {
		if ((*itrlist).wnd != pbase) {
			(*itrlist).wnd->PostMessageW(WM_CLOSE, 0, 0);
		}
	}
}

/*============================================================================*/
/*! �E�B���h�E�A��

-# �C�ӂ̃E�B���h�E�̍폜

@param	group		�O���[�v�ԍ�(1-)
@param	pbase		�Ώۂ̃E�B���h�E

@retval
*/
/*============================================================================*/
void CCustomSynchroWindow::Clear(UINT group, CWnd* pbase/* = NULL*/)
{
	GroupInfo search = GroupInfo(LOWORD(group), pbase);

	if (group == 0) {
		// �S�č폜
		mGroupWindowList.clear();
		return;
	}

	map< UINT, vector<GroupInfo> >::iterator itr;
	itr = mGroupWindowList.find(HIWORD(group));
	if (itr != mGroupWindowList.end()) {
		if (pbase == NULL) {
			// �S�č폜����
			(*itr).second.clear();
		}
		else {
			vector<GroupInfo>::iterator itrlist;
			itrlist = std::find((*itr).second.begin(), (*itr).second.end(), search);
			if (itrlist != (*itr).second.end()) {
				(*itr).second.erase(itrlist);
			}
		}
	}
}

/*============================================================================*/
/*! �E�B���h�E�A��

-# �C�ӂ̃E�B���h�E����ʘA������Ă��邩�̊m�F

@param	group		�O���[�v�ԍ�(1-)
@param	pbase		�Ώۂ̃E�B���h�E

@retval
*/
/*============================================================================*/
BOOL CCustomSynchroWindow::IsSync(UINT group, CWnd* pbase)
{
	GroupInfo search = GroupInfo(LOWORD(group), pbase);

	map< UINT, vector<GroupInfo> >::iterator itr;
	itr = mGroupWindowList.find(HIWORD(group));
	if (itr != mGroupWindowList.end()) {
		vector<GroupInfo>::iterator itrlist;
		itrlist = std::find((*itr).second.begin(), (*itr).second.end(), search);
		if (itrlist != (*itr).second.end()) {
			return TRUE;
		}
	}
	return FALSE;
}

/*============================================================================*/
/*! �E�B���h�E�A��

-# ��ʘA�����̎擾

@param	group		�O���[�v�ԍ�(1-)

@retval
*/
/*============================================================================*/
int CCustomSynchroWindow::Size(UINT group)
{
	map< UINT, vector<GroupInfo> >::iterator itr;
	itr = mGroupWindowList.find(HIWORD(group));
	if (itr != mGroupWindowList.end()) {
		return (int)(*itr).second.size();
	}

	return 0;
}

/*============================================================================*/
/*! �E�B���h�E�A��

-# �C�ӂ̃E�B���h�E�̃O���[�v�ԍ��擾

@param	pbase		�ړ��Ώۂ̃E�B���h�E

@retval
*/
/*============================================================================*/
UINT CCustomSynchroWindow::Group(CWnd* pbase)
{
	map< UINT, vector<GroupInfo> >::iterator itr;
	for (itr = mGroupWindowList.begin(); itr != mGroupWindowList.end(); itr++) {
		vector<GroupInfo>::iterator itrlist;
		for (itrlist = (*itr).second.begin(); itrlist != (*itr).second.end(); itrlist++) {
			if ((*itrlist).wnd == pbase) {
				return (*itr).first;
			}
		}

	}
	return 0; // �O���[�v�Ȃ�
}
