#pragma once

struct GroupInfo
{
	UINT	innerno;	// �����ԍ�
	CWnd*	wnd;		// �E�B���h�E�n���h��

	GroupInfo()
		: innerno(0)
		, wnd(NULL)
	{}

	GroupInfo(UINT no, CWnd* p)
		: innerno(no)
		, wnd(p)
	{}

	bool operator==(const GroupInfo info)
	{
		bool hit = false;
		if (innerno != 0) {
			hit = innerno == info.innerno;
		}
		else {
			hit = wnd == info.wnd;
		}
		return hit;
	}

	bool operator!=(const GroupInfo info)
	{
		bool hit = false;
		if (innerno != 0) {
			hit = innerno == info.innerno;
		}
		else {
			hit = wnd == info.wnd;
		}
		return !hit;
	}

	bool operator<(const GroupInfo info)
	{
		return innerno < info.innerno;
	}
};

class CCustomSynchroWindow
{
public:
	CCustomSynchroWindow();
	~CCustomSynchroWindow();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	map< UINT, vector<GroupInfo> > mGroupWindowList;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	Set(UINT group, CWnd* pbase);
	void	Start();
	void	Move(UINT group, CWnd* pbase, CRect rectbase);
	void	Close(UINT group, CWnd* pbase);
	void	Clear(UINT group, CWnd* pbase = NULL);
	BOOL	IsSync(UINT group, CWnd* pbase);
	int		Size(UINT group);
	UINT	Group(CWnd* pbase);

protected:

	/* ------------------------------------------------------------------------------------ */
};

