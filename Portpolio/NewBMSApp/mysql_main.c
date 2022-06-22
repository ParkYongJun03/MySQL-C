#pragma comment (lib, "libmysql.lib")

#include <winsock.h>
#include <windows.h>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define mysqlip "127.0.0.1"
#define port 3306
#define loginusername "bms"
#define loginpassword "1234"
#define mysqlname "bms"

int main() {
	MYSQL* conn;
	//MYSQL_RES* result;
	//MYSQL_ROW row;
	//int query_stat;

	//unsigned int num_fields;
	//unsigned long* lengths;

	//int i = 0;

	//setlocale(LC_ALL, "Korean");
	conn = mysql_init(NULL);
	// 한글이 안나오면 써라
//	mysql_options(conn, MYSQL_SET_CHARSET_NAME, "euckr");
//	mysql_options(conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");
	mysql_real_connect(conn, mysqlip, loginusername, loginpassword, mysqlname, port, NULL, 0);

	if (conn == NULL) {
		fprintf(stderr, "연결오류! : %s\n", mysql_error(conn));
		return EXIT_FAILURE;
	}

	printf("연결성공!\n");
	printf("서버버전 %s\n", conn->server_version);

	//query_stat = mysql_query(conn, "SELECT * FROM books_info");
	//if (query_stat != 0) {
	//	fprintf(stderr, "쿼리오류! : %s\n", mysql_error(conn));
	//	mysql_close(conn);
	//	return EXIT_FAILURE;
	//}

	//result = mysql_store_result(conn);
	//num_fields = mysql_num_fields(result);
	//while (row = mysql_fetch_row(result)) {
	//	lengths = mysql_fetch_lengths(result);
	//	for (i = 0; i < num_fields; i++)
	//	{
	//		printf("[%.*s] ", (int)lengths[i], row[i] ? row[i] : "NULL");
	//	}
	//	printf("\n");
	//}

	mysql_close(conn);
	printf("연결종료\n");
	system("pause");
	return EXIT_SUCCESS;
}
