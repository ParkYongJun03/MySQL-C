#pragma comment (lib, "libmysql.lib")	//My SQL 라이브러리 사용 선언

#include <stdio.h>
#include <stdlib.h>	//기본

#include <WinSock2.h>	//네트워크 통신을 위한 라이브러리
#include <my_global.h>	//DB용
#include <mysql.h>

void main() {
	printf("MySQL info = %s\n", mysql_get_client_info());

	system("pause");
	return EXIT_SUCCESS;
}