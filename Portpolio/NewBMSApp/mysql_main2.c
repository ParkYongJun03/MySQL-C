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
	char release_date[15];		//���� ���ڿ��� ���µ� �ڵ����� DATE�� �˾Ƽ� ��, NULL������ ����ؾ��Ѵ�.
	int pages;
	char company[GENRE_LENGTH];
	int price;
} book;
// DB ����
void connect_db(MYSQL*);
// å���� �Է�
int insert_book(MYSQL*);
// å���� ��ȸ
int display_books(MYSQL*);
// å���� ����
int update_book(MYSQL*);
// å ���� ����
int delete_book(MYSQL*);
// å ���� ã��
int search_book(MYSQL* );
// DB �ݱ�
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
		printf("���� ���� ���α׷�(v2.0)\n");
		printf("1. ������ȸ\n");
		printf("2. �����Է�\n");
		printf("3. ��������\n");
		printf("4. �����˻�\n");
		printf("5. ��������\n");
		printf("6. ���α׷� ����\n");
		printf("�Է� > ");
		scanf("%d", &input); rewind(stdin);
		switch (input) {
		case 1:
			ret = display_books(conn);
			if (ret == 100) {
				close_db(conn);
				printf("������������ ����Ǿ����ϴ�.");
				exit(1);
			}
			system("pause");
			break;
		case 2:
			ret = insert_book(conn);
			if (ret == 100) {
				close_db(conn);
				printf("������������ ����Ǿ����ϴ�.");
				exit(1);
			}
			system("pause");
			break;
		case 3:
			ret = update_book(conn);
			if (ret == 100) {
				close_db(conn);
				printf("������������ ����Ǿ����ϴ�.");
				exit(1);
			}
			system("pause");
			break;
		case 4:
			ret = search_book(conn);
			if (ret == 100) {
				close_db(conn);
				printf("������������ ����Ǿ����ϴ�.");
				exit(1);
			}
			system("pause");
			system("pause");
			break;
		case 5:
			ret = delete_book(conn);
			if (ret == 100) {
				close_db(conn);
				printf("������������ ����Ǿ����ϴ�.");
				exit(1);
			}
			system("pause");
			system("pause");
			break;
		default:
			display_books(conn);
			close_db(conn);
			printf("���α׷� ����");
			return;
			break;
		}
	}
	return;
}

void connect_db(MYSQL* conn) {

	mysql_real_connect(conn, mysqlip, loginusername, loginpassword, mysqlname, port, NULL, 0);
	if (conn == NULL) {
		fprintf(stderr, "�������! : %s\n", mysql_error(conn));
		return EXIT_FAILURE;
	}

	printf("���Ἲ��!\n");
	printf("�������� %s\n", conn->server_version);
}

void close_db(MYSQL* conn) {
	mysql_close(conn);
	printf("��������\n");
}
void print_header() {
	printf("[����]\t[å����]\t[����]\t[�帣]\t[������]\t[��������]\t[���ǻ�]\t[����]\n");
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
		fprintf(stderr, "��������! : %s\n", mysql_error(conn));
		mysql_close(conn);
		return 100;
	}

	result = mysql_store_result(conn);
	if (result->row_count == 0) {
		printf("����� �����Ͱ� �����ϴ�.\n");
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
	book insBook = { 0, }; //����ü ����/ �ʱ�ȭ
	int i, j = 0;
	char query[500] = { 0, };
	int res = 0;

	printf("å�̸� : ");
	res=scanf("%[^\n]s", insBook.title);
	rewind(stdin);
	// å �̸��� NOTNULL �Ӽ�
	if (res == 0) {
		printf("å�̸��� �����ϴ�. �޴��� �����մϴ�.");
		return 0;
	}

	printf("���ڸ� : ");
	res = scanf("%[^\n]s", insBook.writer);
	rewind(stdin);
	// å �̸��� NOTNULL �Ӽ�
	if (res == 0) {
		printf("���ڸ��� �����ϴ�. �޴��� �����մϴ�.");
		return 0;
	}

	printf("�帣 : ");
	scanf("%[^\n]s", insBook.genre);
	rewind(stdin);

	printf("������(ex, 2010-01-30) : ");
	scanf("%s", insBook.release_date);
	rewind(stdin);

	printf("��������(ex, 200) : ");
	scanf("%d", &insBook.pages);
	rewind(stdin);

	printf("���ǻ� : ");
	scanf("%[^\n]s", insBook.company);
	rewind(stdin);

	printf("����(ex, 15000) : ");
	scanf("%d", &insBook.price);
	rewind(stdin);

	sprintf(query,
		"insert into books_info (title, writer, genre, release_date, pages, company, price) \
         values ('%s', '%s', '%s', '%s', %d, '%s', %d)",
		insBook.title, insBook.writer, insBook.genre, insBook.release_date,
		insBook.pages, insBook.company, insBook.price);

	if (mysql_query(conn, query) != 0) {
		fprintf(stderr, "�Է¿���! : %s\n", mysql_error(conn));
		return 100;
	}
	printf("å ������ �ԷµǾ����ϴ�.");
	return 0;
}

int update_book(MYSQL* conn) {
	book udBook = { 0, }; //����ü ����/ �ʱ�ȭ
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
	scanf("%d", &udBook.id);
	rewind(stdin);

	sprintf(query,
		"SELECT title, writer, genre, release_date, pages, company, price FROM books_info \
          WHERE id = %d", udBook.id);

	query_stat = mysql_query(conn, query);
	if (query_stat != 0) {
		fprintf(stderr, "��������! : %s\n", mysql_error(conn));
		mysql_close(conn);
		return 100;
	}

	printf("��������\n");
	print_header();
	result = mysql_store_result(conn);
	if (result->row_count == 0) {
		printf("������ �����Ͱ� �����ϴ�.\n");
		return 0;
	}
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
	scanf("%[^\n]s", udBook.title);
	rewind(stdin);

	printf("���� ���ڸ�(�ʼ�) : ");
	scanf("%[^\n]s", udBook.writer);
	rewind(stdin);

	printf("���� �帣 : ");
	scanf("%[^\n]s", udBook.genre);
	rewind(stdin);

	printf("���� ���� : ");
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
		fprintf(stderr, "�������� ����! : %s\n", mysql_error(conn));
		return 100;
	}
	printf("å������ �����Ǿ����ϴ�.\n");
	return 0;
}

int delete_book(MYSQL* conn) {
	book delBook = { 0, }; //����ü ����/ �ʱ�ȭ
	char query[500] = { 0, };
	int query_stat = 0;
	MYSQL_RES* result;
	result = mysql_store_result(conn);
	printf("������ å��ȣ �Է� : ");
	scanf("%d", &delBook.id);

	sprintf(query,
		"DELETE FROM books_info WHERE id = %d", delBook.id);

	query_stat = mysql_query(conn, query);
	if (query_stat != 0) {
		fprintf(stderr, "��������!. : %s\n", mysql_error(conn));
		mysql_close(conn);
		return 100;
	}

	result = mysql_store_result(conn);
	if (result->row_count == 0) {
		printf("������ �����Ͱ� �����ϴ�.\n");
		return 0;
	}
	printf("å������ �����Ǿ����ϴ�.\n");
}
int search_book(MYSQL* conn) {
	book scBook = { 0, }; //����ü ����/ �ʱ�ȭ
	int i = 0;
	char query[500] = { 0, };
	MYSQL_RES* result;
	MYSQL_ROW row;
	int query_stat = 0;
	unsigned int num_fields;
	unsigned long* lengths;


	printf("�˻��� �Է� : ");
	scanf("%[^\n]s", scBook.title);
	rewind(stdin);

	sprintf(query,
		"SELECT id, title, writer, genre, release_date, pages, company, price FROM books_info \
          WHERE title LIKE '%%%s%%'", scBook.title);

	query_stat = mysql_query(conn, query);
	if (query_stat != 0) {
		fprintf(stderr, "��������! : %s\n", mysql_error(conn));
		mysql_close(conn);
		return 100;
	}

	result = mysql_store_result(conn);
	if (result->row_count == 0) {
		printf("�˻��� �����Ͱ� �����ϴ�.\n");
		return 0;
	}
	printf("�˻�����\n");
	print_header();
	result = mysql_store_result(conn);
	num_fields = mysql_num_fields(result);
	while (row = mysql_fetch_row(result)) { // 1����
		lengths = mysql_fetch_lengths(result);
		for (i = 0; i < num_fields; i++)
		{
			printf("[%.*s]\t", (int)lengths[i], row[i] ? row[i] : "NULL");
		}
		printf("\n\n");
		rewind(stdin);
	}
}
