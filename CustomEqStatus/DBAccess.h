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

class CDBAccess
{
public:
	CDBAccess();
	virtual ~CDBAccess();

public:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	// 衛星固有情報DB
	char* m_eqsatdb_data;
	vector<CString> m_satnamelist;
	map<CString, __int64> m_satadrlist;

	// 設備制御DB
	char* m_eqctldb_data;
	vector<CString> m_ctlnamelist;
	map<CString, __int64> m_ctladrlist;

	// 監視データ項目リスト
	char* m_eqmondb_data;
	vector<CString> m_monnamelist;

	// 監視データ（受信監視データ）
	map<UINT, map<CString, obsname_t>> m_mondatalist;
	//	vector<CEngValCnv::obsname_t> m_mondatalist[4];
	//	CEngValCnv::obsname_t	mObsName[4][sizeof(CEngValCnv::obsname_t)];
	//	deque<char*> mObList;

	// レディネスチェック時間
	map<CString, int> m_rdychktimelist;
	// 計画時間
	map<CString, int> m_plntimelist;

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	int eqsat_db_read();		// 衛星固有情報DB取得
	int eqctl_db_read();		// 設備制御DB取得
	int eqmon_db_read();		// 設備制御DB取得
	int rdychk_read();			// レディネスチェック時間取得
	int plntime_read();			// 計画時間取得

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
