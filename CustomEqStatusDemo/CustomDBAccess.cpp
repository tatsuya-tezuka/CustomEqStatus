#include "stdafx.h"
#include "CustomDBAccess.h"

#include "sys/stat.h"
#include <ctime>
#include <locale.h>

#include <iostream>
#include <fstream>
#include <mlang.h>

/* ------------------------------------------------------------------------------------ */
/* ◆DBアクセスクラス                                                                   */
/* ------------------------------------------------------------------------------------ */
CCustomDBAccess::CCustomDBAccess()
{
	mDbAddrList.clear();
	mMonNameList.clear();
	mCtrlNameList.clear();
	mEqDbList.clear();
}

CCustomDBAccess::~CCustomDBAccess()
{
	vector<struct obsname_t *>::iterator itr;
	for (itr = mDbAddrList.begin(); itr != mDbAddrList.end(); itr++){
		delete (*itr);
	}
	mDbAddrList.clear();
	mMonNameList.clear();
	mCtrlNameList.clear();
	mEqDbList.clear();
}

/*============================================================================*/
/*! CCustomDBAccess

-# 衛星固有情報DB読込処理

@param	filename	DBファイル名
@retval

*/
/*============================================================================*/
int CCustomDBAccess::eq_db_read(CString filename)
{
	try{
		HANDLE hFile = CreateFileA(CStringA(filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE){
			return -1;
		}

		DWORD mFileSize = GetFileSize(hFile, NULL) + 100;

		CloseHandle(hFile);

		char *pbuf = new char[mFileSize];
		//DB情報格納領域を初期化
		memset(pbuf, 0, mFileSize);

		//DBファイルを読んでくる
		if (db_read(CStringA(filename).GetBuffer(), pbuf) != 0){
			delete[] pbuf;
			return -1;
		}

		mEqDbList.clear();
		mCtrlAdrList.clear();
		if (filename.Find(_T("eqmon")) > 0){
			ud_adr_to_montbl(mEqDbList, pbuf);
		}
		if (filename.Find(_T("eqctl")) > 0){
			ud_adr_to_ctltbl(mEqDbList, mCtrlAdrList, pbuf);
		}

		delete[] pbuf;
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}

/*============================================================================*/
/*! CCustomDBAccess

-# 衛星固有情報DB読込処理

@param	strSharePath	共有フォルダ名称
@param	strStationName	ステーション名称
@retval

*/
/*============================================================================*/
int CCustomDBAccess::eqmon_db_read(CString strSharePath, CString strStationName)
{
	CString filename = strSharePath + _T("\\") + strStationName + _T("\\") + _T("ctrl\\db\\ld\\eqmon.db.obj");

	try{
		HANDLE hFile = CreateFileA(CStringA(filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE){
			return -1;
		}

		DWORD mFileSize = GetFileSize(hFile, NULL) + 100;

		CloseHandle(hFile);

		char *pbuf = new char[mFileSize];
		//DB情報格納領域を初期化
		memset(pbuf, 0, mFileSize);

		//DBファイルを読んでくる
		if (db_read(CStringA(filename).GetBuffer(), pbuf) != 0){
			delete[] pbuf;
			return -1;
		}

		mMonNameList.clear();
		ud_adr_to_montbl(mMonNameList, pbuf);

		delete[] pbuf;
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}

/*============================================================================*/
/*! CCustomDBAccess

-# 設備制御DB読込処理

@param	strSharePath	共有フォルダ名称
@param	strStationName	ステーション名称
@retval

*/
/*============================================================================*/
int CCustomDBAccess::eqctl_db_read(CString strSharePath, CString strStationName)
{
	CString filename = strSharePath + _T("\\") + strStationName + _T("\\") + _T("ctrl\\db\\ld\\eqctl.db.obj");

	try{
		HANDLE hFile = CreateFileA(CStringA(filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE){
			return -1;
		}
		DWORD mFileSize = GetFileSize(hFile, NULL) + 100;

		CloseHandle(hFile);

		char *pbuf = new char[mFileSize];
		//DB情報格納領域を初期化
		memset(pbuf, 0, mFileSize);

		//DBファイルを読んでくる
		if (db_read(CStringA(filename).GetBuffer(), pbuf) != 0){
			delete[] pbuf;
			return -1;
		}

		mCtrlNameList.clear();
		ud_adr_to_ctltbl(mCtrlNameList, mCtrlAdrList, pbuf);

		delete[] pbuf;
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}

/*============================================================================*/
/*! CCustomDBAccess

-# メモリ上の衛星固有情報ＤＢから、各衛星名文字列の先頭アドレスを
-# 配列に格納して返す。

@param	list	格納領域
@param	pbuf	データベースバッファ
@retval

*/
/*============================================================================*/
int CCustomDBAccess::ud_adr_to_montbl(vector<CString>& list, char* pbuf)
{
	char   *vec[VECMAX * 100];
	int i_ret;
	int i;

	i_ret = ud_adr_to_resvec((char *)(pbuf)+sizeof(long)+sizeof(long), OBSNAME_ID, NULL, NULL, vec, VECMAX * 100);

	/*
	衛星名取得
	*/
	for (i = 0; i < i_ret; i++)
	{
		CString str = CString(((struct obsname_t *)vec[i])->sz_obsname);
		list.push_back(str);
	}
	return (i_ret);
}

/*============================================================================*/
/*! CCustomDBAccess

-# メモリ上の設備制御DBから、各衛星名文字列の先頭アドレスを
-# 配列に格納して返す。

@param	list	格納領域
@param	pbuf	データベースバッファ
@retval

*/
/*============================================================================*/
int CCustomDBAccess::ud_adr_to_ctltbl(vector<CString>& list, map<CString, __int64>& adrlist, char* pbuf)
{
	char   *vec[VECMAX * 10];
	int i_ret;
	int i;

	i_ret = ud_adr_to_resvec((char *)(pbuf)+sizeof(long)+sizeof(long), CTLNAME_ID, NULL, NULL, vec, VECMAX * 10);

	/*
	衛星名取得
	*/
	for (i = 0; i < i_ret; i++)
	{
		CString str = CString(((struct ctlname_t *)vec[i])->sz_ctlname);
		list.push_back(str);
		adrlist[str] = *(__int64*)&vec[i];
	}

	return (i_ret);
}

/*============================================================================*/
/*! CCustomDBAccess

-# 個々の設備制御情報、衛星定義情報、または、設備情報定義の先頭アドレスから、
-# データＩＤ値 の一致するデータ構造の先頭アドレスを求める。
-# （設備制御情報，衛星固有情報，等と同等のデータ構造を持つテーブルの検索に
-# 使える汎用のルーチン）
-# ud_adr_to_res()との違いは、探しているデータＩＤに一致するテーブルが複数
-# 存在している場合は、これらのテーブルの先頭アドレスを配列に格納 すること
-# にある。

@param	list
@retval
*/
/*============================================================================*/
int CCustomDBAccess::ud_adr_to_resvec(char* base_p, long l_id, long l_limitid, long l_endoftblid, char* vec[], int i_argc)
{
	struct res_t *p;
	int    n = 0;

	for (p = (struct res_t *)base_p; p->l_id != NULL; p = (struct res_t *)((char*)p + p->l_len))
	{
		if (p->l_len == 0)
		{
			break;
		}
		if (p->l_id == l_id)
		{
			vec[n] = (char *)p;
			n++;
			if (n == i_argc)
			{
				break;
			}
		}
		else if ((p->l_id == l_endoftblid) || (p->l_id == l_limitid))
		{
			break;
		}
	}

	return n;
}

/*============================================================================*/
/*! CCustomDBAccess

-# DBファイルの読込

@param
@retval

*/
/*============================================================================*/
int CCustomDBAccess::db_read(char* db_file_nam, char *cp_loadadr_p)
{
	int errno;
	struct stat stbuf;
	int    i_readbyte;
	long   l_alldatasize = 0, l_datasize;
	char   sz_work[256] = { 0 };

	unsigned long ul_top_pos; /*ファイルの先頭からメモリロード内容の先頭までのバイト数*/
	long   l_idnum = 0;
	FILE *fp = NULL;
	char *fname = db_file_nam;

	/*ファイル名とみなす*/
	if (stat(fname, &stbuf) == -1)
	{
		return(500);
	}

	l_datasize = stbuf.st_size;


	if (l_idnum == 0)
	{
		l_alldatasize += l_datasize;
	}
	else
	{
		l_alldatasize += (l_datasize * l_idnum);
	}

	errno_t ans;
	/*cp_loadadr_p = cp_loadadr;*/
	if ((ans = fopen_s(&fp, fname, "rb")) != 0 || fp == NULL)
	{
		return(500);
	}

	if (fread(sz_work, sizeof(char), sizeof(sz_work), fp) == -1)
	{
		/*#!HEAD:があるかどうか読んでみる*/
		return(500);
	}
	if (!strncmp(sz_work, "#!HEAD:", 7))
	{
		/*#!HEAD:を読み飛ばす*/
		if ((ul_top_pos = (unsigned long)strchr(sz_work, '\n')) == NULL)
		{
			return(500);
		}
		else
		{
			ul_top_pos = (ul_top_pos - (unsigned long)&sz_work[0]) + 1;
		}
	}
	else
	{
		/*ファイルの先頭から全内容を読み込む*/
		ul_top_pos = 0; /*ファイルの先頭からメモリにロードする内容の先頭までのバイト数*/
	}
	if (fseek(fp, ul_top_pos, SEEK_SET) == -1)
	{
		return(500);
	}
	for (;;)
	{
		i_readbyte = (int)fread(cp_loadadr_p, sizeof(char), (l_datasize - ul_top_pos), fp);
		if (i_readbyte <= 0)
		{
			break;
		}
		if (i_readbyte == (l_datasize - ul_top_pos))
		{
			break;
		}
		l_datasize -= i_readbyte;

		cp_loadadr_p = cp_loadadr_p + i_readbyte;
	}
	cp_loadadr_p = cp_loadadr_p + (l_datasize - ul_top_pos);
	l_alldatasize -= ul_top_pos; /*#!HEAD:の部分のバイト数を補正*/
	fclose(fp); /*ファイルをcloseする*/

	return 0;
}
