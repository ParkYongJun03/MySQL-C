#pragma comment (lib, "libmysql.lib")

#include <winsock.h>
#include <windows.h>
#include <mysql.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	char release_date[15];
	int pages;
	char company[GENRE_LENGTH];
	int price;
} book;

// å���� �Է�
void insert_book(MYSQL*, book*);
// å���� ��ȸ
void display_books(MYSQL*);
// å���� ����
void update_book(MYSQL*, book*);

int main() {
	setlocale(LC_ALL, "Korean");

	MYSQL* conn;
	MYSQL_RES* result;
	MYSQL_ROW row;
	int query_stat;
	unsigned int num_fields;
	unsigned long* lengths;

	book myBook;

	conn = mysql_init(NULL);
	mysql_real_connect(conn, mysqlip, loginusername, loginpassword, mysqlname, port, NULL, 0);

	if (conn == NULL) {
		fprintf(stderr, "�������! : %s\n", mysql_error(conn));
		return EXIT_FAILURE;
	}

	printf("���Ἲ��!\n");
	printf("�������� %s\n", conn->server_version);

	display_books(conn);
	//insert_book(conn, &myBook);
	update_book(conn, &myBook);
	display_books(conn);

	mysql_close(conn);
	printf("��������\n");
	system("pause");

	return EXIT_SUCCESS;
}

void display_books(MYSQL* conn) {
	int i = 0;
	MYSQL_RES* result;
	MYSQL_ROW row;
	int query_stat;
	unsigned int num_fields;
	unsigned long* lengths;

	query_stat = mysql_query(conn, "SELECT * FROM books_info");
	if (query_stat != 0) {
		fprintf(stderr, "��������! : %s\n", mysql_error(conn));
		mysql_close(conn);
		return EXIT_FAILURE;
	}

	result = mysql_store_result(conn);
	num_fields = mysql_num_fields(result);
	while (row = mysql_fetch_row(result)) {
		lengths = mysql_fetch_lengths(result);
		for (i = 0; i < num_fields; i++)
		{
			printf("[%.*s] ", (int)lengths[i], row[i] ? row[i] : "NULL");
		}
		printf("\n");
	}

}

void update_book(MYSQL* conn, book* book) {
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

	printf("������ å��ȣ �Է� : ");
	scanf("%d", &book->id);

	sprintf(query,
		"SELECT title, writer, genre, release_date, pages, company, price FROM books_info \
          WHERE id = %d", book->id);

	query_stat = mysql_query(conn, query);
	if (query_stat != 0) {
		fprintf(stderr, "��������! : %s\n", mysql_error(conn));
		mysql_close(conn);
		return EXIT_FAILURE;
	}

	printf("��������\n");
	result = mysql_store_result(conn);
	num_fields = mysql_num_fields(result);
	while (row = mysql_fetch_row(result)) { // 1����
		lengths = mysql_fetch_lengths(result);
		for (i = 0; i < num_fields; i++)
		{
			printf("[%.*s] ", (int)lengths[i], row[i] ? row[i] : "NULL");
		}
		printf("\n");
		rewind(stdin);
	}

	printf("���� å�̸�(�ʼ�) : ");
	scanf("%[^\n]s", book->title);
	rewind(stdin);

	printf("���� ���ڸ�(�ʼ�) : ");
	scanf("%[^\n]s", book->writer);
	rewind(stdin);

	printf("���� �帣 : ");
	scanf("%[^\n]s", book->genre);
	rewind(stdin);

	sprintf(query,
		"UPDATE books_info SET \
		        title = '%s' \
		      , writer = '%s' \
		      , genre = '%s' \
          WHERE id = %d",
		book->title, book->writer, book->genre, book->id);

	if (mysql_query(conn, query) != 0) {
		fprintf(stderr, "�Է¿���! : %s\n", mysql_error(conn));
	}
}

void insert_book(MYSQL* conn, book* book) {
	int i, j = 0;
	char query[500] = { 0, };

	printf("å�̸�(�ʼ�) : ");
	scanf("%[^\n]s", book->title);
	rewind(stdin);

	printf("���ڸ�(�ʼ�) : ");
	scanf("%[^\n]s", book->writer);
	rewind(stdin);

	printf("�帣 : ");
	scanf("%[^\n]s", book->genre);
	rewind(stdin);

	printf("������(�ʼ�, ex, 2010-01-30) : ");
	scanf("%s", book->release_date);
	rewind(stdin);

	printf("��������(�ʼ�, ex, 200) : ");
	scanf("%d", &book->pages);
	rewind(stdin);
	printf("���ǻ� : ");
	scanf("%[^\n]s", book->company);
	rewind(stdin);

	printf("����(�ʼ�, ex, 15000) : ");
	scanf("%d", &book->price);
	rewind(stdin);

	sprintf(query,
		"INSERT INTO books_info (title, writer, genre, release_date, pages, company, price) \
         VALUES ('%s', '%s', '%s', '%s', %d, '%s', %d)",
		book->title, book->writer, book->genre, book->release_date,
		book->pages, book->company, book->price);

	if (mysql_query(conn, query) != 0) {
		fprintf(stderr, "�Է¿���! : %s\n", mysql_error(conn));
	}
}