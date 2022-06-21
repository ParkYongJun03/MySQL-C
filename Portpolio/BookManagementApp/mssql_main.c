#include <stdio.h>
#include <string.h>
#include <wtypes.h>	//wtypes.h를 추가
#include <sql.h>
#include <sqlext.h>
SQLHENV henv = NULL;	// DB 환경을 위한 구조체
SQLHDBC hdbc = NULL;	// DB 접속 구조체

// ODBC로 db접속
int ODBCConnect(SQLCHAR* dsn, SQLCHAR* userID, SQLCHAR* pwd) {
	SQLRETURN res;
	henv = NULL;
	hdbc = NULL;

	// SQL 환경 henv 값을 할당
	res = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	if (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
		// ODBC 버전에 따른 환경 속성 설정
		res = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
		if (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
			//커넥션 핸들 설정
			res = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);	//내 정보를 henv로 줄게 네 정보를 hdbc로 내놔
			if (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
				// 접속하기 위한 설정 먼저
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)10, 0);
				// 실제 접속(중요!)
				res = SQLConnect(hdbc, dsn, SQL_NTS, userID, SQL_NTS, pwd, SQL_NTS);
				if (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
					return 1; //접속 성공
				}
				SQLDisconnect(hdbc);
			}
			SQLFreeHandle(SQL_HANDLE_DBC, hdbc);

			hdbc = henv = NULL;

			return 0; // 접속 실패  flase
		}
	}
}
//DB 접속 헤제
int ODBCDisconnect() {
	if (hdbc) {
		SQLDisconnect(hdbc); //접속 종료
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	}
	if (henv) SQLFreeHandle(SQL_HANDLE_ENV, henv);
	hdbc = henv = NULL;
	return 1;
}
//데이터 가져오기
int db_fetch() {
	SQLRETURN rt;
	SQLHSTMT hstmt;
	SQLCHAR sql[256];	// 쿼리
	SQLLEN n;			// 쿼리를 위한 변수

	//데이터를 담기 위한 변수
	SQLINTEGER id;
	SQLCHAR title[100];
	SQLCHAR writer[100];
	SQLCHAR genre[45];
	SQLDATE release_date;
	SQLSMALLINT pages;
	SQLCHAR company[50];
	SQLDECIMAL price;

	if (hdbc == NULL || henv == NULL) return 0;

	rt = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	if (!(rt == SQL_SUCCESS || rt == SQL_SUCCESS_WITH_INFO)) return 0;

	strcpy(sql, "SELECT id, title, writer, genre, release_date, pages, company, price FROM books_info ");
	rt = SQLExecDirect(hstmt, sql, SQL_NTS);
	if (rt != SQL_SUCCESS) {
		printf("Cannot access query\n");
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		return 0;
	}
	while (1) {
		rt = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		if (rt == SQL_SUCCESS || rt == SQL_SUCCESS_WITH_INFO) {
			rt = SQLGetData(hstmt, 1, SQL_C_ULONG, &id, 0, &n);
			rt = SQLGetData(hstmt, 2, SQL_C_WCHAR, title, 100, &n);

			printf("%d, %s\n", id, title);
		}
		else if (rt == SQL_NO_DATA) {
			printf("No Data!! \n");
			break;
		}
		else {
			printf("Fail to Fetch Data \n");
			break;
		}
	}
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	return 1;
}
int main(void) {
	int ret = 0;
	printf("MSSQL 접속시작!\n");
	ret = ODBCConnect("BMS", "sa", "1234");
	if (ret == 1) printf("접속 성공 \n");

	ret = db_fetch();
	if (ret == 1)	printf("\n");

	ret = ODBCDisconnect();
	if (ret == 1) printf("접속 종료!");
	return 0;
}