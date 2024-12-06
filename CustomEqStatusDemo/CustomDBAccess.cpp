#include "stdafx.h"
#include "CustomDBAccess.h"

#include "sys/stat.h"
#include <ctime>
#include <locale.h>

#include <iostream>
#include <fstream>
#include <mlang.h>

/* ------------------------------------------------------------------------------------ */
/* ��DB�A�N�Z�X�N���X                                                                   */
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

-# �q���ŗL���DB�Ǎ�����

@param	filename	DB�t�@�C����
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
		//DB���i�[�̈��������
		memset(pbuf, 0, mFileSize);

		//DB�t�@�C����ǂ�ł���
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

-# �q���ŗL���DB�Ǎ�����

@param	strSharePath	���L�t�H���_����
@param	strStationName	�X�e�[�V��������
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
		//DB���i�[�̈��������
		memset(pbuf, 0, mFileSize);

		//DB�t�@�C����ǂ�ł���
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

-# �ݔ�����DB�Ǎ�����

@param	strSharePath	���L�t�H���_����
@param	strStationName	�X�e�[�V��������
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
		//DB���i�[�̈��������
		memset(pbuf, 0, mFileSize);

		//DB�t�@�C����ǂ�ł���
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

-# ��������̉q���ŗL���c�a����A�e�q����������̐擪�A�h���X��
-# �z��Ɋi�[���ĕԂ��B

@param	list	�i�[�̈�
@param	pbuf	�f�[�^�x�[�X�o�b�t�@
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
	�q�����擾
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

-# ��������̐ݔ�����DB����A�e�q����������̐擪�A�h���X��
-# �z��Ɋi�[���ĕԂ��B

@param	list	�i�[�̈�
@param	pbuf	�f�[�^�x�[�X�o�b�t�@
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
	�q�����擾
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

-# �X�̐ݔ�������A�q����`���A�܂��́A�ݔ�����`�̐擪�A�h���X����A
-# �f�[�^�h�c�l �̈�v����f�[�^�\���̐擪�A�h���X�����߂�B
-# �i�ݔ�������C�q���ŗL���C���Ɠ����̃f�[�^�\�������e�[�u���̌�����
-# �g����ėp�̃��[�`���j
-# ud_adr_to_res()�Ƃ̈Ⴂ�́A�T���Ă���f�[�^�h�c�Ɉ�v����e�[�u��������
-# ���݂��Ă���ꍇ�́A�����̃e�[�u���̐擪�A�h���X��z��Ɋi�[ ���邱��
-# �ɂ���B

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

-# DB�t�@�C���̓Ǎ�

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

	unsigned long ul_top_pos; /*�t�@�C���̐擪���烁�������[�h���e�̐擪�܂ł̃o�C�g��*/
	long   l_idnum = 0;
	FILE *fp = NULL;
	char *fname = db_file_nam;

	/*�t�@�C�����Ƃ݂Ȃ�*/
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
		/*#!HEAD:�����邩�ǂ����ǂ�ł݂�*/
		return(500);
	}
	if (!strncmp(sz_work, "#!HEAD:", 7))
	{
		/*#!HEAD:��ǂݔ�΂�*/
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
		/*�t�@�C���̐擪����S���e��ǂݍ���*/
		ul_top_pos = 0; /*�t�@�C���̐擪���烁�����Ƀ��[�h������e�̐擪�܂ł̃o�C�g��*/
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
	l_alldatasize -= ul_top_pos; /*#!HEAD:�̕����̃o�C�g����␳*/
	fclose(fp); /*�t�@�C����close����*/

	return 0;
}
