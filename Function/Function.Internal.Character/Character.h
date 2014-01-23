namespace Character
{

	TCHAR IntToHexChar(int x);

	int HexCharToInt(TCHAR hex);

	char * UnicodeToANSI( const wchar_t* str );

	wchar_t * UTF8ToUnicode( const char* str );

	char * UnicodeToUTF8( const wchar_t* str ) ;
	/*¿í×Ö·û×ª»»Îª¶à×Ö·ûUnicode - ANSI*/ 
	char* w2m(const wchar_t* wcs) ;
	/*¶à×Ö·û×ª»»Îª¿í×Ö·ûANSI - Unicode*/ 
	wchar_t* m2w(const char* mbs) ;

	/*char* ANSIToUTF8(const char* str) 
	{ 
	return UnicodeToUTF8(ANSIToUnicode(str)); 
	}*/ 

	char* UTF8ToANSI(const char* str) ;

	BOOL ANSIToUnicode(LPCTSTR str,LPWSTR wszStr );
}