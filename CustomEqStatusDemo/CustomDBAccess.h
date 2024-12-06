#pragma once

/*
*******************************************************************************
* 設備監視名称情報
*******************************************************************************
*/
#define	CTLNAME_LEN	28	/* 設備制御名称最大長+1 *//* [Ver1.02] 20->28byte */
#define	EQNAME_LEN	12	/* 設備名称最大長+1 */
#define	CTLID_LEN	( CTLNAME_LEN + EQNAME_LEN )	/* 制御項目ID最大長+1 */
#define	OPTSTR_LEN		24								/* 制御項目定義 付加情報最大長+1 */
#define VECMAX			256		/* ＤＢ検索最大個数（パラメータ数も同意。） */
#define CTLNAME_ID      0x010001 /* 設備制御項目名称であることの識別ＩＤ */
#define	OBSNAME_ID		0x0101		/* 条件定義ＩＤ */

struct obsname_t{	/* このテーブルは全監視項目について存在する */
	long   l_id;	/* データｉｄ: 定数=OBSNAME_ID値 */
	long   l_len;	/* データ長  : sizeof( struct obsname_t ) */
	long   l_no;	/* 監視項目番号（コンパイラがシーケンシャルに付加する。） */
	char   sz_obsname[CTLID_LEN]; 	/* 監視項目ＩＤ: 設備名.監視項目名 */
	long   l_inflg; /* データ有効フラグ 1=有効  ＊１ */
	long   l_grmd;  /* ステータスフラグ 1=有効,2=時刻文字列 ＊１ */
	long   l_data;  /* 変換結果（整数）         ＊１ */
	double d_data;  /* 変換結果（実数）         ＊１ */
	char   sz_sts_name[64]; /* 変換結果ステータス文字列 */

	/* 以下は64bit整数精度対応拡張領域 */

	long double ld_data;/* 工学値データ64bit整数を格納できる精度 ＊１ */

	long   l_bitinfo;   /* 整数ビット数:整数抽出データのビット長:1..64
						==0:整数抽出ではない   ＊１ */
	long   l__rfu__;    /* 未使用：paddingバイト */

	long   l_cnvResType;	/* 変換結果タイプ（ビット並び） */
};
/*
*******************************************************************************
* 設備制御名称情報
*******************************************************************************
*/
struct ctlname_t {				/* このテーブルは全制御項目について存在する */
	long l_id;					/* データｉｄ: 定数=CTLNAME_ID値 */
	long l_len;					/* データ長  : sizeof( struct ctlname_t ) */
	long l_no;					/* 制御項目番号（コンパイラがシーケンシャルに付加する。） */
	long l_adr;					/* 設置位置情報:ユニットアドレス(3bit) */
	char sz_ctlname[CTLID_LEN];	/* 制御項目ＩＤ: 設備名.制御項目名 */
	char sz_optstr[OPTSTR_LEN];	/* 付加情報 */

	long l_eqno;				/* 設備番号（＊１） *//* Ver1.05 */
	long l_rfu;					/* パディング *//* Ver1.05 */
};
/*
*******************************************************************************
* 共有メモリ上のテーブル構造
*******************************************************************************
*/
struct res_t { /*共有メモリ上のテーブル構造*/
	long  l_id;
	long  l_len;
	char  buf[1];
};

/* ------------------------------------------------------------------------------------ */
/* ◆DBアクセスクラス                                                                   */
/* ------------------------------------------------------------------------------------ */
class CCustomDBAccess
{
public:
	CCustomDBAccess();
	~CCustomDBAccess();

public:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	vector<struct obsname_t *>			mDbAddrList;	// DBアドレスリスト
	vector<CString>						mEqDbList;		// DBリスト
	vector<CString>						mMonNameList;	// 監視名リスト
	vector<CString>						mCtrlNameList;	// 制御名リスト
	map<CString, __int64>				mCtrlAdrList;	// 制御アドレスリスト

public:

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
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

