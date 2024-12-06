#pragma once

/*
*******************************************************************************
* �ݔ��Ď����̏��
*******************************************************************************
*/
#define	CTLNAME_LEN	28	/* �ݔ����䖼�̍ő咷+1 *//* [Ver1.02] 20->28byte */
#define	EQNAME_LEN	12	/* �ݔ����̍ő咷+1 */
#define	CTLID_LEN	( CTLNAME_LEN + EQNAME_LEN )	/* ���䍀��ID�ő咷+1 */
#define	OPTSTR_LEN		24								/* ���䍀�ڒ�` �t�����ő咷+1 */
#define VECMAX			256		/* �c�a�����ő���i�p�����[�^�������ӁB�j */
#define CTLNAME_ID      0x010001 /* �ݔ����䍀�ږ��̂ł��邱�Ƃ̎��ʂh�c */
#define	OBSNAME_ID		0x0101		/* ������`�h�c */

struct obsname_t{	/* ���̃e�[�u���͑S�Ď����ڂɂ��đ��݂��� */
	long   l_id;	/* �f�[�^����: �萔=OBSNAME_ID�l */
	long   l_len;	/* �f�[�^��  : sizeof( struct obsname_t ) */
	long   l_no;	/* �Ď����ڔԍ��i�R���p�C�����V�[�P���V�����ɕt������B�j */
	char   sz_obsname[CTLID_LEN]; 	/* �Ď����ڂh�c: �ݔ���.�Ď����ږ� */
	long   l_inflg; /* �f�[�^�L���t���O 1=�L��  ���P */
	long   l_grmd;  /* �X�e�[�^�X�t���O 1=�L��,2=���������� ���P */
	long   l_data;  /* �ϊ����ʁi�����j         ���P */
	double d_data;  /* �ϊ����ʁi�����j         ���P */
	char   sz_sts_name[64]; /* �ϊ����ʃX�e�[�^�X������ */

	/* �ȉ���64bit�������x�Ή��g���̈� */

	long double ld_data;/* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x ���P */

	long   l_bitinfo;   /* �����r�b�g��:�������o�f�[�^�̃r�b�g��:1..64
						==0:�������o�ł͂Ȃ�   ���P */
	long   l__rfu__;    /* ���g�p�Fpadding�o�C�g */

	long   l_cnvResType;	/* �ϊ����ʃ^�C�v�i�r�b�g���сj */
};
/*
*******************************************************************************
* �ݔ����䖼�̏��
*******************************************************************************
*/
struct ctlname_t {				/* ���̃e�[�u���͑S���䍀�ڂɂ��đ��݂��� */
	long l_id;					/* �f�[�^����: �萔=CTLNAME_ID�l */
	long l_len;					/* �f�[�^��  : sizeof( struct ctlname_t ) */
	long l_no;					/* ���䍀�ڔԍ��i�R���p�C�����V�[�P���V�����ɕt������B�j */
	long l_adr;					/* �ݒu�ʒu���:���j�b�g�A�h���X(3bit) */
	char sz_ctlname[CTLID_LEN];	/* ���䍀�ڂh�c: �ݔ���.���䍀�ږ� */
	char sz_optstr[OPTSTR_LEN];	/* �t����� */

	long l_eqno;				/* �ݔ��ԍ��i���P�j *//* Ver1.05 */
	long l_rfu;					/* �p�f�B���O *//* Ver1.05 */
};
/*
*******************************************************************************
* ���L��������̃e�[�u���\��
*******************************************************************************
*/
struct res_t { /*���L��������̃e�[�u���\��*/
	long  l_id;
	long  l_len;
	char  buf[1];
};

/* ------------------------------------------------------------------------------------ */
/* ��DB�A�N�Z�X�N���X                                                                   */
/* ------------------------------------------------------------------------------------ */
class CCustomDBAccess
{
public:
	CCustomDBAccess();
	~CCustomDBAccess();

public:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	vector<struct obsname_t *>			mDbAddrList;	// DB�A�h���X���X�g
	vector<CString>						mEqDbList;		// DB���X�g
	vector<CString>						mMonNameList;	// �Ď������X�g
	vector<CString>						mCtrlNameList;	// ���䖼���X�g
	map<CString, __int64>				mCtrlAdrList;	// ����A�h���X���X�g

public:

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	vector<CString>&	GetMonNameList() { return mMonNameList; }
	vector<CString>&	GetCtrlNameList() { return mCtrlNameList; }
	vector<CString>&	GetEqList() { return mEqDbList; }

	int		eq_db_read(CString filename);
	int		eqmon_db_read(CString strSharePath, CString strStationName);
	int		eqctl_db_read(CString strSharePath, CString strStationName);

protected:
	int		ud_adr_to_montbl(vector<CString>& list, char* pbuf);
	int		ud_adr_to_ctltbl(vector<CString>& list, map<CString, __int64>& adrlist, char* pbuf);
	int		ud_adr_to_resvec(char* base_p, long l_id, long l_limitid, long l_endoftblid, char* vec[], int i_argc);
	int		db_read(char* db_file_nam, char *cp_loadadr_p);

protected:
	long ds_htonl(long val)
	{
#if IS_BIG_ENDIA
		return htonl(val);
#else
		return val;
#endif
	}

};

