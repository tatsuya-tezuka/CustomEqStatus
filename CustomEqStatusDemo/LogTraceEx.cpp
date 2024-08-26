// LogTrace.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LogTraceEx.h"

#include <ctime>
#include <locale.h>

CCriticalSection	CLogTraceEx::mSection;			//!< 書き込み用クリティカルセクション
CStdioFile			CLogTraceEx::mFile;				//!< ログファイル
CStdioFile			CLogTraceEx::mFileXSL;			//!< XML用のスタイルシート
CString				CLogTraceEx::mFilePath;			//!< ログファイルパス※終端￥
CString				CLogTraceEx::mFileName;			//!< ログファイル名
CString				CLogTraceEx::mBuffer;				//!< 書き込みbuffer
DWORD				CLogTraceEx::mFileSize;			//!< ファイルサイズ上限
DWORD				CLogTraceEx::mBufferWriteLimit;	//!< buffer書き込み閾値
bool				CLogTraceEx::mLogEnable;			//!< ログ出力可能かのフラグ
DWORD				CLogTraceEx::mLogLevel;			//!< ログ出力対象ログレベル
DWORD				CLogTraceEx::mLogType;			//!< ログ出力タイプ

////////////////////////////////////////////////////////////////////////////////
// CLogDateTime
////////////////////////////////////////////////////////////////////////////////
/*============================================================================*/
/*! ログ時間処理
    コンストラクタ
    	
    @param[in]
    	
    @retval なし
*/
/*============================================================================*/
CLogDateTime::CLogDateTime()
{
    // Local Time の取得
    GetLocalTime(&mSystemTime);
}

/*============================================================================*/
/*! ログ時間処理
    コンストラクタ
    	
    @param[in]	inDt	指定時間
    	
    @retval なし
*/
/*============================================================================*/
CLogDateTime::CLogDateTime(const CString& inDt)
{
    // 変換用バッファ (Performance UP)
    CString buffer;
    buffer = inDt;
    // フェールセーフで，必要文字長以上にしておく
    //                  YYYYMMDDhhmmssccc
    const TCHAR* zero = _T("00000000000000000");
    buffer += zero;

    // 数値への変換の為，NULLを挿入していく
    //   ずれるので後ろから入れる。最後尾は既に入っているので入れない
    buffer.Insert((long)STRLEN(_T("YYYYMMDDhhmmssccc")), _T("\0"));
    buffer.Insert((long)STRLEN(_T("YYYYMMDDhhmmss")), _T("\0"));
    buffer.Insert((long)STRLEN(_T("YYYYMMDDhhmm")), _T("\0"));
    buffer.Insert((long)STRLEN(_T("YYYYMMDDhh")), _T("\0"));
    buffer.Insert((long)STRLEN(_T("YYYYMMDD")), _T("\0"));
    buffer.Insert((long)STRLEN(_T("YYYYMM")), _T("\0"));
    buffer.Insert((long)STRLEN(_T("YYYY")), _T("\0"));

    // 変換用ポインタ
    TCHAR* ptr;
    ptr = (TCHAR*)(LPCTSTR)buffer;

    // Year
    mSystemTime.wYear = (unsigned short)_wtoi(ptr);
	ptr += wcslen(_T("YYYY\0"))+1;
    // Month
    mSystemTime.wMonth = (unsigned short)_wtoi(ptr);
    ptr += wcslen(_T("MM\0"))+1;
    // Day
    mSystemTime.wDay = (unsigned short)_wtoi(ptr);
    ptr += wcslen(_T("DD\0"))+1;
    // Hour
    mSystemTime.wHour = (unsigned short)_wtoi(ptr);
    ptr += wcslen(_T("hh\0"))+1;
    // Minute
    mSystemTime.wMinute = (unsigned short)_wtoi(ptr);
    ptr += wcslen(_T("mm\0"))+1;
    // Second
    mSystemTime.wSecond = (unsigned short)_wtoi(ptr);
    ptr += wcslen(_T("ss\0"))+1;
    // mSecond
    mSystemTime.wMilliseconds = (unsigned short)_wtoi(ptr);

    // mSystemTimeの正当性チェック
    if( (mSystemTime.wYear<1970 || mSystemTime.wYear>3000) || 
        (mSystemTime.wMonth<1   || mSystemTime.wMonth>12) || 
        (mSystemTime.wDay<1     || mSystemTime.wDay>31) ){

        // 正当性チェックでエラーになったので、現在時刻に置き換える。
        GetLocalTime( &mSystemTime );
    }
}

/*============================================================================*/
/*! ログ時間処理
    デストラクタ
    	
    @param[in]
    	
    @retval なし
*/
/*============================================================================*/
CLogDateTime::~CLogDateTime(void)
{
}

//============================================================================
/// @brief	日時の比較演算を行います
/// @param
/// @return
//============================================================================
bool CLogDateTime::operator < (CLogDateTime& inDateTime) const
{
    return (_subSystemTime(mSystemTime, inDateTime.getSystemTime()) < 0);
}
bool CLogDateTime::operator > (CLogDateTime& inDateTime) const
{
    return (_subSystemTime(mSystemTime, inDateTime.getSystemTime()) > 0);
}
bool CLogDateTime::operator <= (CLogDateTime& inDateTime) const
{
    return (_subSystemTime(mSystemTime, inDateTime.getSystemTime()) <= 0);
}
bool CLogDateTime::operator >= (CLogDateTime& inDateTime) const
{
    return (_subSystemTime(mSystemTime, inDateTime.getSystemTime()) >= 0);
}
//============================================================================
/// @brief	ミリ秒単位の足し算
/// @param	prm1	inAdd	: ミリ秒
/// @return
//============================================================================
void CLogDateTime::addMilliSec(long inAdd)
{
    // うるう年とかの計算が面倒なので，既存の CTimeクラスに計算させる
    //   CTimeでは，msecが計算できないので，そこだけ外だし。
    //   もっと良い方法あるかな～。

    // MSEC計算桁上がり／下がり保存用
    long carrySec, milliSec;

    ///// msec は自前計算
    milliSec = mSystemTime.wMilliseconds;
    milliSec += inAdd; // nemoto
    // msecを 0~999 に補正して，桁上がり，下がりを算出
    if(milliSec >= 0) {
        for(carrySec = 0; milliSec >= 1000; carrySec++, milliSec -= 1000);
    }
    else {
        for(carrySec = 0; milliSec < 0; carrySec--, milliSec += 1000);
    }

    // mSystemTimeの正当性チェック
    if( (mSystemTime.wYear<1970 || mSystemTime.wYear>3000) || 
        (mSystemTime.wMonth<1   || mSystemTime.wMonth>12) || 
        (mSystemTime.wDay<1     || mSystemTime.wDay>31) ){
        // エラー時は、何もしない
    }
    else{
        // 計算用CTimeインスタンス作成と同時に計算
        CTime t(mSystemTime.wYear, mSystemTime.wMonth, mSystemTime.wDay, mSystemTime.wHour, mSystemTime.wMinute, mSystemTime.wSecond + carrySec);
        // System Time に戻す
        t.GetAsSystemTime(mSystemTime);
        mSystemTime.wMilliseconds = (unsigned short)milliSec;
    }
}
//============================================================================
/// @brief	DateTime_t 型で取得
/// @param
/// @return	CString	日時文字列
//============================================================================
CString CLogDateTime::getDateTime() const
{ 
    CString str;
    //str.Preallocate(64);
    str.Format(_T("%04d%02d%02d%02d%02d%02d%02d\0"), mSystemTime.wYear, mSystemTime.wMonth, mSystemTime.wDay, mSystemTime.wHour, mSystemTime.wMinute, mSystemTime.wSecond, mSystemTime.wMilliseconds / 10);
    return str;
}
//============================================================================
/// @brief	ミリ秒で取得
/// @param
/// @return	CString	日時文字列
//============================================================================
CString CLogDateTime::getDateTimeMSOrder() const
{ 
    CString str;
    //str.Preallocate(64);
    str.Format(_T("%04d%02d%02d%02d%02d%02d%03d\0"), mSystemTime.wYear, mSystemTime.wMonth, mSystemTime.wDay, mSystemTime.wHour, mSystemTime.wMinute, mSystemTime.wSecond, mSystemTime.wMilliseconds);
    return str;
}
//============================================================================
/// @brief	秒までの形で取得
/// @param
/// @return	CString	日時文字列
//============================================================================
CString CLogDateTime::getDateTimeSecondOrder() const
{ 
    CString str;
    //str.Preallocate(64);
    str.Format(_T("%04d%02d%02d%02d%02d%02d\0"), mSystemTime.wYear, mSystemTime.wMonth, mSystemTime.wDay, mSystemTime.wHour, mSystemTime.wMinute, mSystemTime.wSecond);
    return str;
}
//============================================================================
/// @brief	ログファイル名用の日時で取得
/// @param
/// @return	CString	日時文字列
//============================================================================
CString CLogDateTime::getDateTimeForLogFile() const
{ 
    CString str;
    //str.Preallocate(64);
    str.Format(_T("%04d%02d%02d_%02d%02d%02d\0"), mSystemTime.wYear, mSystemTime.wMonth, mSystemTime.wDay, mSystemTime.wHour, mSystemTime.wMinute, mSystemTime.wSecond);
    return str;
}
//============================================================================
/// @brief	ログ用のタイムスタンプ日時で取得
/// @param
/// @return	CString	日時文字列
//============================================================================
CString CLogDateTime::getDateTimeForLogStamp() const
{ 
    CString str;
    //str.Preallocate(64);
    str.Format(_T("%04d/%02d/%02d %02d:%02d:%02d.%03d\0"), mSystemTime.wYear, mSystemTime.wMonth, mSystemTime.wDay, mSystemTime.wHour, mSystemTime.wMinute, mSystemTime.wSecond, mSystemTime.wMilliseconds);
    return str;
}
//============================================================================
/// @brief	100ms Orderの時間を取得
/// @param
/// @return	CString	日時文字列
//============================================================================
unsigned long CLogDateTime::getTime100msOrder() const
{
    return ((((mSystemTime.wHour * 60) + mSystemTime.wMinute) * 60) + mSystemTime.wSecond) * 10 + (mSystemTime.wMilliseconds / 100);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////// Private

//============================================================================
/// @brief	System Time の計算
/// @param	prm1	s1	: SYSTEMTIME時間
/// @param	prm2	s2	: SYSTEMTIME時間
/// @return	long	計算結果
//============================================================================
long CLogDateTime::_subSystemTime(const SYSTEMTIME& s1, const SYSTEMTIME& s2) const {
    // 計算用バッファ
    unsigned long days1, days2, msecs1, msecs2;

    // それぞれの日単位での値を計算する
    days1 = (s1.wYear * 13 + s1.wMonth) * 32 + s1.wDay;
    days2 = (s2.wYear * 13 + s2.wMonth) * 32 + s2.wDay;
    // それぞれの msec 単位での値を計算する
    msecs1 = ((s1.wHour * 60 + s1.wMinute) * 60 + s1.wSecond) * 1000 + s1.wMilliseconds;
    msecs2 = ((s2.wHour * 60 + s2.wMinute) * 60 + s2.wSecond) * 1000 + s2.wMilliseconds;

    // まずは日以上で比較
    if(days1 > days2) {
        return 1;
    }
    else if(days1 < days2) {
        return -1;
    }
    // 日以上で差が無かったら，msecで比較する
    else {
        if(msecs1 > msecs2) {
            return 1;
        }
        else if(msecs1 < msecs2) {
            return -1;
        }
        else {
            return 0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// CLogTraceEx
////////////////////////////////////////////////////////////////////////////////
/*============================================================================*/
/*! ログトレース関連
    コンストラクタ
    	
    @param[in]
    	
    @retval なし
*/
/*============================================================================*/
CLogTraceEx::CLogTraceEx()
{
	mLogEnable = false;
}

/*============================================================================*/
/*! ログトレース関連
    デストラクタ
    	
    @param[in]
    	
    @retval なし
*/
/*============================================================================*/
CLogTraceEx::~CLogTraceEx()
{
    if(mFile.m_hFile != CFile::hFileNull){
		_write(_T(""), true);
		Close();
	}
}

// CLogTrace メンバー関数

//============================================================================
/// @brief	ログファイルの作成及び初期化
/// @param	prm1	pFilePath	: ログ出力用ファイルパス名（終端￥）
/// @param	prm2	pFileName	: ログ出力用ファイル名（拡張子なし）
/// @param	prm3	dwLogLevel	: ログ出力用レベル
/// @param	prm4	dwLogType	: ログ出力タイプ
/// @return	bool	成功した場合はtrueを返す。それ以外はfalseを返す
//============================================================================
bool CLogTraceEx::Create( const TCHAR* pFilePath, const TCHAR* pFileName, const DWORD dwLogLevel, const DWORD dwLogType)
{
	mLogLevel = dwLogLevel;
	mLogType = dwLogType;
	mFilePath = pFilePath;
	mFileName = pFileName;
    mFileSize = 1024 * 1024 * 10;
	mBufferWriteLimit = LOG_BUFFER_MAX * 8 / 10;
	mLogEnable = false;

	if(mLogLevel >= nLogEx::none)
		return false;

	if(_open() == false)
		return false;

	mLogEnable = true;
	return true;
}
void CLogTraceEx::Delete()
{
	mLogEnable = false;
}
//============================================================================
/// @brief	ログファイルのクローズ
/// @param
/// @return
//============================================================================
void CLogTraceEx::Close()
{
	_write(_T(""), true);
	_close();
}
//============================================================================
/// @brief	ログファイルの書き込み
/// @param	prm1	inSource		: ソースファイル名
/// @param	prm2	inClass			: クラス名
/// @param	prm3	inMethod		: 関数名
/// @param	prm4	inDescription	: 説明
/// @param	prm5	inDetails		: 詳細
/// @param	prm6	inLevel			: ログレベル
/// @return
//============================================================================
void CLogTraceEx::Write(const TCHAR* inSource, const TCHAR* inClass, const TCHAR* inMethod, const TCHAR* inDescription, const TCHAR* inDetails, const DWORD inLevel)
{
    mSection.Lock();

    try {
        // Log Level 確認
        if(mLogLevel >= inLevel){
            if(mLogLevel >= nLogEx::none) {
			    mSection.Unlock();
				return;
            }

            // 1 line 作成
            CString line;

			switch(mLogType){
				case	nLogEx::html:
					_makeHtmlLine(line, inSource, inClass, inMethod, inDescription, inDetails, inLevel);
					break;
				case	nLogEx::text:
					_makeTextLine(line, inSource, inClass, inMethod, inDescription, inDetails, inLevel);
					break;
				case	nLogEx::xml:
					_makeXmlLine(line, inSource, inClass, inMethod, inDescription, inDetails, inLevel);
					break;
			}

			// 改行の追加
            line += _T("\r\n");

            //### Log 書き込み判断 ###//
            _write(line, true);
        }
    }
    catch(...) {
    }

    mSection.Unlock();
}
//============================================================================
/// @brief	ログファイルの作成及び初期化
/// @param	prm1	pFileName		: ログ出力用ファイル名（拡張子なし）
/// @param	prm2	dwLogLevel		: ログ出力用レベル
/// @return bool	成功した場合はtrueを返す。それ以外はfalseを返す
//============================================================================
bool CLogTraceEx::_open()
{
	// Log File の Open と，確認
    if(mFile.m_hFile == CFile::hFileNull) {
        // FILE OPEN
        CFileException e;
        // Pathの作成
        CString path;

		// 現在日時の取得
	    CLogDateTime dt;
		path.Format(_T("%s%s_%s"), (LPCTSTR)mFilePath, (LPCTSTR)mFileName, (LPCTSTR)dt.getDateTimeForLogFile());

		switch(mLogType){
			case	nLogEx::html:	path += LOG_FILE_EXTENSION_HTML;	break;
			case	nLogEx::text:	path += LOG_FILE_EXTENSION_TEXT;	break;
			case	nLogEx::xml:	path += LOG_FILE_EXTENSION_XML;		break;
		}

		// File Open
        mFile.Open(path, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyWrite, &e);
        if(e.m_cause != CFileException::none) {
            return false;
        }

		// ヘッダー出力
		CString title;
		int i;
		switch(mLogType){
			case	nLogEx::html:
				title.Format(_T("%s%s%s"), (LPCTSTR)LOG_HTML_HEADER_1, (LPCTSTR)LOG_TITLE, (LPCTSTR)LOG_HTML_HEADER_2);
				_writetext(mFile, title);
				// ヘッダータイトル出力
 				for(i=0; i<sizeof(_LogHeadString) / sizeof(_LogHeadString[0]); i++){
					title.Format(LOG_HTML_HEADER_3, _LogHeadString[i]);
					_writetext(mFile, title);
				}
				break;
			case	nLogEx::text:
				break;
			case	nLogEx::xml:
				_createXSL();
				title.Format(_T("%s%s%s"), (LPCTSTR)LOG_XML_HEADER_1, (LPCTSTR)LOG_XML_HEADER_2, (LPCTSTR)LOG_XML_HEADER_3);
				_writetext(mFile, title);
				break;
		}

		mBuffer.Empty();
	}

	return true;
}
//============================================================================
/// @brief	ログファイルのクローズ
/// @param
/// @return
//============================================================================
void CLogTraceEx::_close()
{
	try
	{
		// Log Open しているときだけ
		if(mFile.m_hFile != CFile::hFileNull) {
			// フッター出力
			CString title;
			switch(mLogType){
				case	nLogEx::html:
					title.Format(_T("%s"), (LPCTSTR)LOG_HTML_FOOTER);
					_writetext(mFile, title);
					break;
				case	nLogEx::text:
					break;
				case	nLogEx::xml:
					title.Format(_T("%s"), (LPCTSTR)LOG_XML_FOOTER);
					_writetext(mFile, title);
					break;
			}

			// ファイルクローズ
			mFile.Flush();
			mFile.Close();
		}
	}
	catch (CFileException* e)
	{
		_close();
		e->Delete();
	}
	catch (CMemoryException* e)
	{
		_close();
		e->Delete();
	}
	catch (...)
	{
		_close();
	}
}
//============================================================================
/// @brief	XML用のスタイルシートの作成
/// @param
/// @return
//============================================================================
void CLogTraceEx::_createXSL()
{
/*
http://www.envinfo.uee.kyoto-u.ac.jp/user/susaki/envinfo/xml_if.html
*/

	int	i,j;
	// Log File の Open と，確認
    if(mFileXSL.m_hFile == CFile::hFileNull) {
        // FILE OPEN
        CFileException e;
        // Pathの作成
        CString path;

		path.Format(_T("%s%s"), (LPCTSTR)mFilePath, (LPCTSTR)LOG_FILE_XSL);

		// File Open
        mFileXSL.Open(path, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e);
        if(e.m_cause != CFileException::none) {
            return;
        }

		// ヘッダー出力
		CString title;
		title.Format(_T("%s%s%s"), (LPCTSTR)LOG_XSL_HEADER_1, (LPCTSTR)LOG_XSL_HEADER_2, (LPCTSTR)LOG_XSL_HEADER_3);
		_writetext(mFileXSL, title);
		title.Format(LOG_XSL_HEADER_4, LOG_TITLE, LOG_TITLE);
		_writetext(mFileXSL, title);
		// テーブル定義
		title.Format(_T("  <table border=\"0\" cellspacing=\"1\" cellpadding=\"0\">\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("   <tr>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("    <td bgcolor=\"#dddddd\">\r\n"));	// ボーダーカラー
		_writetext(mFileXSL, title);
		title.Format(_T("     <table border=\"0\" cellpadding=\"3\" cellspacing=\"1\">\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("      <tr>\r\n"));
		_writetext(mFileXSL, title);
 		for(i=0; i<sizeof(_LogHeadString) / sizeof(_LogHeadString[0]); i++){
			title.Format(_T("       <td bgcolor=\"#ffffff\"><b>%s</b></td>\r\n"), (LPCTSTR)_LogHeadString[i]);
			_writetext(mFileXSL, title);
		}
		title.Format(_T("      </tr>\r\n"));
		_writetext(mFileXSL, title);
		// 条件分岐
		title.Format(_T("      <xsl:for-each select=\"LogFile/LogItem\">\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("        <xsl:choose>\r\n"));
		_writetext(mFileXSL, title);
		// ログレベルの個数分処理する
		for(i=0; i<nLogEx::none; i++){
			title.Format(_T("          <xsl:when test=\"contains(Description, '%s')\">\r\n"), LogLevelStr[i].mark);
			_writetext(mFileXSL, title);
			title.Format(_T("            %s\r\n"), LogLevelStr[i].headerxml);
			_writetext(mFileXSL, title);
 			for(j=0; j<sizeof(_LogHeadString) / sizeof(_LogHeadString[0]); j++){
				title.Format(_T("            <td valign=\"top\"><nobr><xsl:value-of select=\"%s\" /></nobr></td>\r\n"), (LPCTSTR)_LogHeadString[j]);
				_writetext(mFileXSL, title);
			}
			title.Format(_T("            %s\r\n"), LogLevelStr[i].footerxml);
			_writetext(mFileXSL, title);
			title.Format(_T("          </xsl:when>\r\n"));
			_writetext(mFileXSL, title);
		}
		// 上記以外の場合の定義
		title.Format(_T("          <xsl:otherwise>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("            <tr bgcolor=\"#ffffff\" style=\"color:Black\">\r\n"));
		_writetext(mFileXSL, title);
 		for(i=0; i<sizeof(_LogHeadString) / sizeof(_LogHeadString[0]); i++){
			title.Format(_T("            <td valign=\"top\"><nobr><xsl:value-of select=\"%s\" /></nobr></td>\r\n"), (LPCTSTR)_LogHeadString[i]);
			_writetext(mFileXSL, title);
		}
		title.Format(_T("            </tr>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("          </xsl:otherwise>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("        </xsl:choose>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("      </xsl:for-each>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("     </table>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("    </td>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("   </tr>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("  </table>\r\n"));
		_writetext(mFileXSL, title);

		title.Format(_T("%s"), (LPCTSTR)LOG_XSL_FOOTER);
		_writetext(mFileXSL, title);

		// ファイルクローズ
        mFileXSL.Flush();
        mFileXSL.Close();
	}
}
//============================================================================
/// @brief	HTML用タグの作成
/// @param	prm1	outLine			: タグ文字列
/// @param	prm2	inSource		: ソースファイル名
/// @param	prm3	inClass			: クラス名
/// @param	prm4	inMethod		: 関数名
/// @param	prm5	inDescription	: 説明
/// @param	prm6	inDetails		: 詳細
/// @param	prm7	inLevel			: ログレベル
/// @return
//============================================================================
void CLogTraceEx::_makeHtmlLine(CString& outLine, const TCHAR* inSource, const TCHAR* inClass, const TCHAR* inMethod, const TCHAR* inDescription, const TCHAR* inDetails, const DWORD inLevel)
{
    CLogDateTime dt;

    // \t を <BR>に変換
    CString convDetails = inDetails;
    convDetails.Replace(_T("\t"), _T("<BR>"));
    // Tableに出力
    CString info;
    outLine.Format(_T("<TR BGCOLOR=#FFFFFF><TD NOWRAP><FONT SIZE=-2>%s%s%s</FONT></TD><TD NOWRAP>%s%s%s</TD><TD NOWRAP>%s%s%s</TD><TD NOWRAP>%s%s%s%s</TD><TD COLSPAN=2 NOWRAP>%s%s%s</TD></TR>\r\n"),
		LogLevelStr[inLevel].header, (LPCTSTR)dt.getDateTimeForLogStamp(), LogLevelStr[inLevel].footer,
        LogLevelStr[inLevel].header, inClass, LogLevelStr[inLevel].footer,
        LogLevelStr[inLevel].header, inMethod, LogLevelStr[inLevel].footer,
        LogLevelStr[inLevel].header, LogLevelStr[inLevel].mark, inDescription, LogLevelStr[inLevel].footer,
		LogLevelStr[inLevel].header, (LPCTSTR)convDetails, LogLevelStr[inLevel].footer);
}
//============================================================================
/// @brief	XML用タグの作成
/// @param	prm1	outLine			: タグ文字列
/// @param	prm2	inSource		: ソースファイル名
/// @param	prm3	inClass			: クラス名
/// @param	prm4	inMethod		: 関数名
/// @param	prm5	inDescription	: 説明
/// @param	prm6	inDetails		: 詳細
/// @param	prm7	inLevel			: ログレベル
/// @return
//============================================================================
void CLogTraceEx::_makeXmlLine(CString& outLine, const TCHAR* inSource, const TCHAR* inClass, const TCHAR* inMethod, const TCHAR* inDescription, const TCHAR* inDetails, const DWORD inLevel)
{
    CLogDateTime dt;

    // Tableに出力
    CString info;
    outLine.Format(_T("  <LogItem>\r\n    <%s>%s</%s>\r\n    <%s>%s</%s>\r\n    <%s>%s</%s>\r\n    <%s>%s%s</%s>\r\n    <%s>%s</%s>\r\n  </LogItem>\r\n"),
		(LPCTSTR)_LogHeadString[0], (LPCTSTR)dt.getDateTimeForLogStamp(), (LPCTSTR)_LogHeadString[0],
		(LPCTSTR)_LogHeadString[1], inClass, (LPCTSTR)_LogHeadString[1],
		(LPCTSTR)_LogHeadString[2], inMethod, (LPCTSTR)_LogHeadString[2],
		(LPCTSTR)_LogHeadString[3], LogLevelStr[inLevel].mark, inDescription, (LPCTSTR)_LogHeadString[3],
		(LPCTSTR)_LogHeadString[4], inDetails, (LPCTSTR)_LogHeadString[4]);
}
//============================================================================
/// @brief	TEXT用タグの作成
/// @param	prm1	outLine			: タグ文字列
/// @param	prm2	inSource		: ソースファイル名
/// @param	prm3	inClass			: クラス名
/// @param	prm4	inMethod		: 関数名
/// @param	prm5	inDescription	: 説明
/// @param	prm6	inDetails		: 詳細
/// @param	prm7	inLevel			: ログレベル
/// @return
//============================================================================
void CLogTraceEx::_makeTextLine(CString& outLine, const TCHAR* inSource, const TCHAR* inClass, const TCHAR* inMethod, const TCHAR* inDescription, const TCHAR* inDetails, const DWORD inLevel)
{
    CLogDateTime dt;

    outLine.Format(_T("%s\t%s[%s::%s]\t(%s)\t%s\t%s"),
		(LPCTSTR)dt.getDateTimeForLogStamp(),
        inSource, inClass, inMethod, LogLevelStr[inLevel].name, inDescription, inDetails);
}
//============================================================================
/// @brief	ログの書き込み
/// @param	prm1	inLine			: タグ文字列
/// @param	prm2	inForce			: 強制書き込み
/// @return
//============================================================================
void CLogTraceEx::_write(const CString& inLine, bool inForce)
{
	if(mLogEnable == false)
		return;

	if(mFile.m_hFile == CFile::hFileNull) {
		if(_open() == false)
			return;
	}

	try
	{
		mBuffer += inLine;
		// log 書き込み判断
		if(((DWORD)mBuffer.GetLength() >= mBufferWriteLimit || inForce == true)){
			_writetext(mFile, mBuffer);
			mBuffer.Empty();
			//mBuffer.Preallocate( LOG_BUFFER_MAX );

			// 最大サイズチェック
			// 最大サイズを超えたら一旦Closeして次回再作成する
			if(mFile.GetLength() > mFileSize) {
				_close();
			}
		}
	}
	catch (CFileException* e)
	{
		mBuffer.Empty();
		_close();
		e->Delete();
	}
	catch (CMemoryException* e)
	{
		mBuffer.Empty();
		_close();
		e->Delete();
	}
	catch (...)
	{
		mBuffer.Empty();
		_close();
	}
}
//============================================================================
/// @brief	ログの書き込み
/// @param	prm1	inLine			: タグ文字列
/// @param	prm2	inForce			: 強制書き込み
/// @return
//============================================================================
void CLogTraceEx::_writetext(CStdioFile& file, const CString& buf)
{
	try
	{
		//_wsetlocale(LC_ALL,_T("japanese")); 
		CString conv = buf;
		conv.Replace(_T("\r\n"), _T("\n"));
		file.WriteString(conv);
	}
	catch (CFileException* e)
	{
		_close();
		e->Delete();
	}
	catch (CMemoryException* e)
	{
		_close();
		e->Delete();
	}
	catch (...)
	{
		_close();
	}
}
