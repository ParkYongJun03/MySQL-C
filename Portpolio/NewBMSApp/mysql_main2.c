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

#define TITLE_LENGTH 100
#define GENRE_LENGTH 50

typedef struct bookInfo {
	int id;
	char title[TITLE_LENGTH];
	char writer[TITLE_LENGTH];
	char genre[GENRE_LENGTH];
	char release_date[15];		//들어갈땐 문자열로 들어가는데 자동으로 DATE로 알아서 들어감, NULL까지는 고려해야한다.
	int pages;
	char company[GENRE_LENGTH];
	int price;
} book;
// DB 연결
void connect_db(MYSQL*);
// 책정보 입력
int insert_book(MYSQL*);
// 책정보 조회
int display_books(MYSQL*);
// 책정보 수정
int update_book(MYSQL*);
// 책 정보 삭제
int delete_book(MYSQL*);
// 책 정보 찾기
int search_book(MYSQL* );
// DB 닫기
void close_db(MYSQL*);



int main() {
	setlocale(LC_ALL, "Korean");
	int input = 0;
	MYSQL* conn;
	conn = mysql_init(NULL);
	int ret = 0;

	connect_db(conn);

	while (1) {
		system("cls");
		printf("도서 관리 프로그램(v2.0)\n");
		printf("1. 도서조회\n");
		printf("2. 도서입력\n");
		printf("3. 도서수정\n");
		printf("4. 도서검색\n");
		printf("5. 도서삭제\n");
		printf("6. 프로그램 종료\n");
		printf("입력 > ");
		scanf("%d", &input); rewind(stdin);
		switch (input) {
		case 1:
			ret = display_books(conn);
			if (ret == 100) {
				close_db(conn);
				printf("비정상적으로 종료되었습니다.");
				exit(1);
			}
			system("pause");
			break;
		case 2:
			ret = insert_book(conn);
			if (ret == 100) {
				close_db(conn);
				printf("비정상적으로 종료되었습니다.");
				exit(1);
			}
			system("pause");
			break;
		case 3:
			ret = update_book(conn);
			if (ret == 100) {
				close_db(conn);
				printf("비정상적으로 종료되었습니다.");
				exit(1);
			}
			system("pause");
			break;
		case 4:
			ret = search_book(conn);
			if (ret == 100) {
				close_db(conn);
				printf("비정상적으로 종료되었습니다.");
				exit(1);
			}
			system("pause");
			system("pause");
			break;
		case 5:
			ret = delete_book(conn);
			if (ret == 100) {
				close_db(conn);
				printf("비정상적으로 종료되었습니다.");
				exit(1);
			}
			system("pause");
			system("pause");
			break;
		default:
			display_books(conn);
			close_db(conn);
			printf("프로그램 종료");
			return;
			break;
		}
	}
	return;
}

void connect_db(MYSQL* conn) {

	mysql_real_connect(conn, mysqlip, loginusername, loginpassword, mysqlname, port, NULL, 0);
	if (conn == NULL) {
		fprintf(stderr, "연결오류! : %s\n", mysql_error(conn));
		return EXIT_FAILURE;
	}

	printf("연결성공!\n");
	printf("서버버전 %s\n", conn->server_version);
}

void close_db(MYSQL* conn) {
	mysql_close(conn);
	printf("연결종료\n");
}
void print_header() {
	printf("[순번]\t[책제목]\t[저자]\t[장르]\t[출판일]\t[페이지수]\t[출판사]\t[가격]\n");
	printf("==================================================================================\n");
}
int display_books(MYSQL* conn) {
	int i = 0;
	MYSQL_RES* result;
	MYSQL_ROW row;
	int query_stat;
	unsigned int num_fields;
	unsigned long* lengths;

	query_stat = mysql_query(conn, "SELECT * FROM books_info");
	if (query_stat != 0) {
		fprintf(stderr, "쿼리오류! : %s\n", mysql_error(conn));
		mysql_close(conn);
		return 100;
	}

	result = mysql_store_result(conn);
	if (result->row_count == 0) {
		printf("출력할 데이터가 없습니다.\n");
		return 0;
	}
	print_header();
	num_fields = mysql_num_fields(result);
	while (row = mysql_fetch_row(result)) {
		lengths = mysql_fetch_lengths(result);
		for (unsigned int i = 0; i < num_fields; i++)
		{
			printf("[%.*s] ", (int)lengths[i], row[i] ? row[i] : "NULL");
		}
		printf("\n");
	}
	return 0;
}

int insert_book(MYSQL* conn) {
	book insBook = { 0, }; //구조체 선언/ 초기화
	int i, j = 0;
	char query[500] = { 0, };
	int res = 0;

	printf("책이름 : ");
	res=scanf("%[^\n]s", insBook.title);
	rewind(stdin);
	// 책 이름은 NOTNULL 속성
	if (res == 0) {
		printf("책이름이 없습니다. 메뉴를 종료합니다.");
		return 0;
	}

	printf("저자명 : ");
	res = scanf("%[^\n]s", insBook.writer);
	rewind(stdin);
	// 책 이름은 NOTNULL 속성
	if (res == 0) {
		printf("저자명이 없습니다. 메뉴를 종료합니다.");
		return 0;
	}

	printf("장르 : ");
	scanf("%[^\n]s", insBook.genre);
	rewind(stdin);

	printf("출판일(ex, 2010-01-30) : ");
	scanf("%s", insBook.release_date);
	rewind(stdin);

	printf("페이지수(ex, 200) : ");
	scanf("%d", &insBook.pages);
	rewind(stdin);

	printf("출판사 : ");
	scanf("%[^\n]s", insBook.company);
	rewind(stdin);

	printf("가격(ex, 15000) : ");
	scanf("%d", &insBook.price);
	rewind(stdin);

	sprintf(query,
		"insert into books_info (title, writer, genre, release_date, pages, company, price) \
         values ('%s', '%s', '%s', '%s', %d, '%s', %d)",
		insBook.title, insBook.writer, insBook.genre, insBook.release_date,
		insBook.pages, insBook.company, insBook.price);

	if (mysql_query(conn, query) != 0) {
		fprintf(stderr, "입력오류! : %s\n", mysql_error(conn));
		return 100;
	}
	printf("책 정보가 입력되었습니다.");
	return 0;
}

int update_book(MYSQL* conn) {
	book udBook = { 0, }; //구조체 선언/ 초기화
	int i = 0;
	char query[500] = { 0, };
	MYSQL_RES* result;
	MYSQL_ROW row;
	int query_stat = 0;
	unsigned int num_fields;
	unsigned long* lengths;

	char title[TITLE_LENGTH];
	char writer[TITLE_LENGTH];
	char genre[GENRE_LENGTH];
	char release_date[15];
	char pages[15];
	char company[GENRE_LENGTH];
	char price[10];

	printf("수정할 책번호 입력 : ");
	scanf("%d", &udBook.id);
	rewind(stdin);

	sprintf(query,
		"SELECT title, writer, genre, release_date, pages, company, price FROM books_info \
          WHERE id = %d", udBook.id);

	query_stat = mysql_query(conn, query);
	if (query_stat != 0) {
		fprintf(stderr, "쿼리오류! : %s\n", mysql_error(conn));
		mysql_close(conn);
		return 100;
	}

	printf("기존정보\n");
	print_header();
	result = mysql_store_result(conn);
	if (result->row_count == 0) {
		printf("수정할 데이터가 없습니다.\n");
		return 0;
	}
	num_fields = mysql_num_fields(result);
	while (row = mysql_fetch_row(result)) { // 1건임
		lengths = mysql_fetch_lengths(result);
		for (i = 0; i < num_fields; i++)
		{
			printf("[%.*s] ", (int)lengths[i], row[i] ? row[i] : "NULL");
		}
		printf("\n");
		rewind(stdin);
	}

	printf("변경 책이름(필수) : ");
	scanf("%[^\n]s", udBook.title);
	rewind(stdin);

	printf("변경 저자명(필수) : ");
	scanf("%[^\n]s", udBook.writer);
	rewind(stdin);

	printf("변경 장르 : ");
	scanf("%[^\n]s", udBook.genre);
	rewind(stdin);

	printf("변경 가격 : ");
	scanf("%d", &udBook.price);
	rewind(stdin);

	sprintf(query,
		"UPDATE books_info SET \
		        title = '%s' \
		      , writer = '%s' \
		      , genre = '%s' \
		      , price = '%d' \
          WHERE id = %d",
		udBook.title, udBook.writer, udBook.genre, udBook.price, udBook.id);

	if (mysql_query(conn, query) != 0) {
		fprintf(stderr, "수정쿼리 오류! : %s\n", mysql_error(conn));
		return 100;
	}
	printf("책정보가 수정되엇습니다.\n");
	return 0;
}

int delete_book(MYSQL* conn) {
	book delBook = { 0, }; //구조체 선언/ 초기화
	char query[500] = { 0, };
	int query_stat = 0;
	MYSQL_RES* result;
	result = mysql_store_result(conn);
	printf("삭제할 책번호 입력 : ");
	scanf("%d", &delBook.id);

	sprintf(query,
		"DELETE FROM books_info WHERE id = %d", delBook.id);

	query_stat = mysql_query(conn, query);
	if (query_stat != 0) {
		fprintf(stderr, "쿼리오류!. : %s\n", mysql_error(conn));
		mysql_close(conn);
		return 100;
	}

	result = mysql_store_result(conn);
	if (result->row_count == 0) {
		printf("삭제할 데이터가 없습니다.\n");
		return 0;
	}
	printf("책정보가 삭제되엇습니다.\n");
}
int search_book(MYSQL* conn) {
	book scBook = { 0, }; //구조체 선언/ 초기화
	int i = 0;
	char query[500] = { 0, };
	MYSQL_RES* result;
	MYSQL_ROW row;
	int query_stat = 0;
	unsigned int num_fields;
	unsigned long* lengths;


	printf("검색명 입력 : ");
	scanf("%[^\n]s", scBook.title);
	rewind(stdin);

	sprintf(query,
		"SELECT id, title, writer, genre, release_date, pages, company, price FROM books_info \
          WHERE title LIKE '%%%s%%'", scBook.title);

	query_stat = mysql_query(conn, query);
	if (query_stat != 0) {
		fprintf(stderr, "쿼리오류! : %s\n", mysql_error(conn));
		mysql_close(conn);
		return 100;
	}

	result = mysql_store_result(conn);
	if (result->row_count == 0) {
		printf("검색할 데이터가 없습니다.\n");
		return 0;
	}
	printf("검색정보\n");
	print_header();
	result = mysql_store_result(conn);
	num_fields = mysql_num_fields(result);
	while (row = mysql_fetch_row(result)) { // 1건임
		lengths = mysql_fetch_lengths(result);
		for (i = 0; i < num_fields; i++)
		{
			printf("[%.*s]\t", (int)lengths[i], row[i] ? row[i] : "NULL");
		}
		printf("\n\n");
		rewind(stdin);
	}
}
