#pragma once

struct GroupInfo
{
	UINT	innerno;	// 内部番号
	CWnd*	wnd;		// ウィンドウハンドル

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
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	map< UINT, vector<GroupInfo> > mGroupWindowList;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
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

