//******************************************************************************
// @file	LogTraceEx.h
// @brief	ログ出力クラス
// @date	2012/02/15
// @author	SCC
//******************************************************************************
#pragma once

// CHtmlLog コマンド ターゲット

using namespace std;

//!< 定義
#ifdef UNICODE
#define STRLEN  wcslen
#else
#define STRLEN  strlen
#endif // !UNICODE

static const CString	_LogHeadString[]={
	_T("Time"), /*_T("Source"), */_T("Class"), _T("Method"), _T("Description"), _T("Details"),
};

// 拡張子
static const CString	LOG_FILE_EXTENSION_HTML	= _T(".html");
static const CString	LOG_FILE_EXTENSION_TEXT	= _T(".log");
static const CString	LOG_FILE_EXTENSION_XML	= _T(".xml");
static const CString	LOG_FILE_XSL			= _T("xmlview.xsl");

// 最大バッファサイズ
static const DWORD		LOG_BUFFER_MAX			= (1024 * 1024);     // 1MByte

static const CString	LOG_TITLE				= _T("Application Logging Report");

//!< HTMLタグ
static const CString	LOG_HTML_HEADER_1		= _T("<HTML>\r\n<HEAD><TITLE>");
static const CString	LOG_HTML_HEADER_2		= _T("</TITLE></HEAD>\r\n<BODY>\r\n\r\n<TABLE BORDERCOLOR=#a0a0ff CELLSPACING=0 CELLPADDING=1 BORDER=2>\r\n");
static const CString	LOG_HTML_HEADER_3		= _T("<TH BGCOLOR=#e0e0ff><FONT COLOR=#FFFFFF>%s</FONT></TH>\r\n");
static const CString	LOG_HTML_FOOTER			= _T("</TABLE>\r\n</BODY>\r\n</HTML>");

//!< XMLタグ
static const CString	LOG_XML_HEADER_1		= _T("<?xml version=\"1.0\" encoding=\"shift_jis\"?>\r\n");
static const CString	LOG_XML_HEADER_2		= _T("<?xml:stylesheet type=\"text/xsl\" href=\"xmlview.xsl\"?>\r\n");
static const CString	LOG_XML_HEADER_3		= _T("<LogFile>\r\n");
static const CString	LOG_XML_FOOTER			= _T("</LogFile>\r\n");

//!< XML用スタイルシートタグ
static const CString	LOG_XSL_HEADER_1		= _T("<?xml version=\"1.0\" ?>\r\n");
static const CString	LOG_XSL_HEADER_2		= _T("<xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">\r\n");
static const CString	LOG_XSL_HEADER_3		= _T("<xsl:template match=\"/\">\r\n\r\n");
static const CString	LOG_XSL_HEADER_4		= _T("<html>\r\n <head>\r\n  <title>%s</title>\r\n </head>\r\n <body>\r\n  <h1>%s</h1>\r\n\r\n");
static const CString	LOG_XSL_FOOTER			= _T(" </body>\r\n</html>\r\n\r\n</xsl:template>\r\n</xsl:stylesheet>\r\n");

typedef struct _stLogLevelStr{
    TCHAR*  header;
    TCHAR*  footer;
    TCHAR*  name;
    TCHAR*  mark;
    TCHAR*  headerxml;
    TCHAR*  footerxml;
} stLogLevelStr;
static const stLogLevelStr LogLevelStr[]=
{
    { _T("<FONT COLOR=\"Blue\">"),    _T("</FONT>"),     _T("Information"),		_T("[I] "),	_T("<tr bgcolor=\"#ffffff\" style=\"color:Blue\">"),		_T("</tr>") },
    { _T("<FONT COLOR=\"Orange\">"),  _T("</FONT>"),     _T("Warning    "),		_T("[W] "),	_T("<tr bgcolor=\"#ffffff\" style=\"color:Orange\">"),		_T("</tr>") },
    { _T("<FONT COLOR=\"Red\">"),     _T("</FONT>"),     _T("Error      "),		_T("[E] "),	_T("<tr bgcolor=\"#ffffff\" style=\"color:Red\">"),			_T("</tr>") },
    { _T("<FONT COLOR=\"Magenta\">"), _T("</FONT>"),     _T("Fatal      "),		_T("[F] "),	_T("<tr bgcolor=\"#ffffff\" style=\"color:Magenta\">"),		_T("</tr>") },
    { _T("<FONT COLOR=\"Black\">"),   _T("</FONT>"),     _T("Debug      "),		_T("[D] "),	_T("<tr bgcolor=\"#ffffff\" style=\"color:Black\">"),		_T("</tr>") },
	{ _T("<FONT COLOR=\"Green\">"),   _T("</FONT>"),     _T("Detail     "),     _T("[T] "), _T("<tr bgcolor=\"#ffffff\" style=\"color:Green\">"),       _T("</tr>") },
};

namespace nLogEx {
	///< ログ出力形式
	enum eLogType {
		text,		// テキスト形式
		html,		// HTML形式
		xml,		// XML形式
	};
	///< ログ出力レベル
	enum eLogLevel {
		info,		// 情報
		warning,	// 警告
		error,		// エラー
		fatal,		// 致命的
		debug,		// デバッグ
		detail,		// 詳細
		none,
	};
};

class CLogDateTime
{
public:
    ///// コンストラクタ，デストラクタ
    CLogDateTime();
    CLogDateTime(const CString& inDt);
    ~CLogDateTime();

    // MilliSec と足し算
    void addMilliSec(const long inAdd);
    // 比較演算
	bool operator < (CLogDateTime& inDateTime) const;
    bool operator > (CLogDateTime& inDateTime) const;
    bool operator <= (CLogDateTime& inDateTime) const;
    bool operator >= (CLogDateTime& inDateTime) const;
    // DataTime_t 型で取得
    // 後で実装 operator DateTime_t();
    CString getDateTime() const;              // YYYYMMDDhhmmsscc
    CString getDateTimeMSOrder() const;       // YYYYMMDDhhmmssccc
    CString getDateTimeSecondOrder() const;   // YYYYMMDDhhmmss
    CString getDateTimeForLogFile() const;    // YYYYMMDD_hhmmss
    CString getDateTimeForLogStamp() const;   // YYYY/MM/DD hh:mm:ss.ccc
    unsigned long getTime100msOrder() const;


    CString getDateTimeForLog() {return getDateTimeForLogFile(); }

    // System Time 型で取得
    SYSTEMTIME& getSystemTime() {return mSystemTime;}

private:
    // 実質の本体
    SYSTEMTIME mSystemTime;

    // Private
    long _subSystemTime(const SYSTEMTIME& s1, const SYSTEMTIME& s2) const;
};

class CLogTraceEx
{
public:
	CLogTraceEx();
	virtual ~CLogTraceEx();

	static bool	Create( const TCHAR* pFilePath, const TCHAR* pFileName, const DWORD dwLogLevel, const DWORD dwLogType);
	static void	Delete();
	static void	Close();
	static void	Write(const TCHAR* inSource, const TCHAR* inClass, const TCHAR* inMethod, const TCHAR* inDescription, const TCHAR* inDetails, const DWORD inLevel);

private:
    static CCriticalSection	mSection;			//!< 書き込み用クリティカルセクション
	static CStdioFile		mFile;				//!< ログファイル
	static CStdioFile		mFileXSL;			//!< XML用のスタイルシート
	static CString			mFilePath;			//!< ログファイルパス※終端￥
	static CString			mFileName;			//!< ログファイル名
	static CString			mBuffer;			//!< 書き込みbuffer
    static DWORD			mFileSize;			//!< ファイルサイズ上限
    static DWORD			mBufferWriteLimit;	//!< buffer書き込み閾値

	static bool				mLogEnable;			//!< ログ出力可能かのフラグ
    static DWORD			mLogLevel;			//!< ログ出力対象ログレベル
    static DWORD			mLogType;			//!< ログ出力タイプ

	static bool	_open();
	static void	_close();
	static void	_createXSL();
	static void	_makeHtmlLine(CString& outLine, const TCHAR* inSource, const TCHAR* inClass, const TCHAR* inMethod, const TCHAR* inDescription, const TCHAR* inDetails, const DWORD inLevel);
	static void	_makeXmlLine(CString& outLine, const TCHAR* inSource, const TCHAR* inClass, const TCHAR* inMethod, const TCHAR* inDescription, const TCHAR* inDetails, const DWORD inLevel);
	static void	_makeTextLine(CString& outLine, const TCHAR* inSource, const TCHAR* inClass, const TCHAR* inMethod, const TCHAR* inDescription, const TCHAR* inDetails, const DWORD inLevel);
	static void	_write(const CString& inLine, bool inForce=false);
	static void	_writetext(CStdioFile& file, const CString& buf);
};


