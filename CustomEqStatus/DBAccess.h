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

class CDBAccess
{
public:
	CDBAccess();
	virtual ~CDBAccess();

public:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	// �q���ŗL���DB
	char* m_eqsatdb_data;
	vector<CString> m_satnamelist;
	map<CString, __int64> m_satadrlist;

	// �ݔ�����DB
	char* m_eqctldb_data;
	vector<CString> m_ctlnamelist;
	map<CString, __int64> m_ctladrlist;

	// �Ď��f�[�^���ڃ��X�g
	char* m_eqmondb_data;
	vector<CString> m_monnamelist;

	// �Ď��f�[�^�i��M�Ď��f�[�^�j
	map<UINT, map<CString, obsname_t>> m_mondatalist;
	//	vector<CEngValCnv::obsname_t> m_mondatalist[4];
	//	CEngValCnv::obsname_t	mObsName[4][sizeof(CEngValCnv::obsname_t)];
	//	deque<char*> mObList;

	// ���f�B�l�X�`�F�b�N����
	map<CString, int> m_rdychktimelist;
	// �v�掞��
	map<CString, int> m_plntimelist;

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	int eqsat_db_read();		// �q���ŗL���DB�擾
	int eqctl_db_read();		// �ݔ�����DB�擾
	int eqmon_db_read();		// �ݔ�����DB�擾
	int rdychk_read();			// ���f�B�l�X�`�F�b�N���Ԏ擾
	int plntime_read();			// �v�掞�Ԏ擾

	int db_read(char* db_file_nam, char *cp_loadadr_p);

	int ud_adr_to_sattbl(vector<CString>& snamelist);
	int ud_adr_to_ctltbl(vector<CString>& list);
	int ud_adr_to_montbl(vector<CString>& list);

	int ud_adr_to_resvec(char* base_p, long l_id, long l_limitid, long l_endoftblid, char* vec[], int i_argc);
	int ud_macname_to_ststbl(char *base_p, char *psz_macname, char *tbl[], char *vec[], int i_argc);
	int ud_macadr_to_ststbl(char *base_p, char *tbl[], char *vec[], int i_argc);
	int ud_macstsadr_to_exectbl(char *base_p, char *tbl[], char *vec[], int i_argc);
	int ud_macsts_to_exectbl(char *base_p, char *psz_param, char *tbl[], char *vec[], int i_argc);

	vector<CString>&		GetSatNameList() { return m_satnamelist; }
	map<CString, __int64>&	GetSatAdrList() { return m_satadrlist; }

	vector<CString>&		GetCtlNameList() { return m_ctlnamelist; }
	map<CString, __int64>&	GetCtlAdrList() { return m_ctladrlist; }

	vector<CString>&		GetMonNameList() { return m_monnamelist; }

protected:

};
