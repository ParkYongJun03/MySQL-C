# MySQL-C
MySQL을 연동한 C 프로젝트 
[선행학습](https://github.com/ParkYongJun03/MySQL)
- MySQL 설치
- WorkBench 설치
- SampleDB 테스트
- 쿼리문 실습

## MySQL-C
git만들때 꼭 저거 해주기 gitignore - VisualStudio

> ### MySQL - C Connector lib 설치 [MySQL Connector/C (Archived Versions) 링크](https://downloads.mysql.com/archives/c-c/)
> 
> Connector 설치 경로 `C:\DEV\Server\MySQL\MySQL Connector C 6.1`
> 
> C프로젝트(솔루션) 만들기 ->  프로젝트 속성페이지 ->
> 
> <img src="https://user-images.githubusercontent.com/83456300/174712762-f264847e-b0f0-4ea5-9aaa-35c729c50b0b.png" width="400">
>
>> 포함 디렉터리 : `C:\DEV\Server\MySQL\MySQL Connector C 6.1\include;`추가
>>  
>> 라이브러리 디렉터리 : `C:\DEV\Server\MySQL\MySQL Connector C 6.1\lib;`추가
>>
> 프로젝트 속성페이지 -> 명령줄 -> C/C++ -> `/source-charset:euc-kr /execution-charset:euc-kr ` 추가
> <img src="https://user-images.githubusercontent.com/83456300/174712340-57e67c53-90ff-456b-85ee-2d1682ff8536.png" width="400">
> 


> ### 폰트 설정
> <img src="https://user-images.githubusercontent.com/83456300/174712113-9a156f31-58ce-4b98-a274-472957635a50.png" width="500">
>
> `/source-charset:euc-kr /execution-charset:euc-kr`
>

> ### MySQL, C Connection
>> `C:\DEV\Server\MySQL\MySQL Connector C 6.1\lib` 에서 `libmysql.dll` `libmysql.lib`를 복사
>> 
>> `C:\Repository\MySQL-C\Portpolio\BookManagementApp` 에 붙여넣기
>> 
>> <img src="https://user-images.githubusercontent.com/83456300/174719555-a05f5ac1-aa8b-4823-a213-d0c706bea410.png" width="400">
>
> C 프로젝트에 가서 `libmysql.dll` `libmysql.lib` 프로젝트에 포함 -> 헤더에 추가된 것을 확인
> 소스파일에 second_main.c(이름 상관없음) 생성 -> 다른 이름으로 저장, 인코딩하여 저장???? 이거 


``` C
#pragma comment (lib, "libmysql.lib")	//My SQL 라이브러리 사용 선언
#define _WINSOCKAPI_

#include <stdio.h>
#include <stdlib.h>	//기본
#include <string.h>
#include <locale.h>
#include <Windows.h>

#include <my_global.h>	//DB용
#include <WinSock.h>	//네트워크 통신을 위한 라이브러리
#include <mysql.h>

#define mysqlip "127.0.0.1"		//	localhost, 192.168.0.21
#define port 3306
#define userid "root"
#define password ""
#define mysqlname "ems" // mysql schema명

int main() {
	// 시작
	MYSQL* conn;	//MYSQL*는 커넥션을 위해 사용하기 떄문에 변수명이 conn이다.
	MYSQL_RES* result;
	MYSQL_ROW row;

	int query_stat;
	unsigned int num_fields;
	unsigned long * lengths;
	int i = 0;

	setlocale(LC_ALL, "Korean");	//한국어 깨짐 방지용
	// MySQL 접속 시작!
	conn = mysql_init(NULL);	//MySQL 초기화
	mysql_real_connect(conn, mysqlip, userid, password, mysqlname, port, NULL, 0);

	if (conn == NULL) {
		fprintf(stderr, "연결오류! : %s\n", mysql_error(conn));
		return EXIT_FAILURE;
	}

	printf("연결성공!\n");
	//연결된 이후 처리!!!

	printf("서버버전 %s\n", conn->server_version);

	query_stat = mysql_query(conn, "SELECT * FROM books_info");
	if (query_stat != 0) {
		fprintf(stderr, "쿼리오류! : %s\n", mysql_error(conn));
		mysql_close(conn);
		return EXIT_FAILURE;
	}

	// 쿼리 결과를 화면출력
	result = mysql_store_result(conn);	//결과 정보객체 리턴
	num_fields = mysql_num_fields(result);	//컬럼 갯수
	while (row = mysql_fetch_row(result)) {
		lengths = mysql_fetch_lengths(result);
		for (i = 0; i < num_fields; i++)
		{
			printf("[%.*s] ", (int)lengths[i], row[i] ? row[i] : "NULL");
//			printf("[%.*s] ", row[i] ? row[i] : "NULL");
		}
		printf("\n");
	}

	//연결풀(Connection pool)
	mysql_close(conn);
	printf("연결종료\n");


	system("pause");
	return EXIT_SUCCESS;
}
```

