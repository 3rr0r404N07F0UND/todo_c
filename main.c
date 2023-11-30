#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  MYSQL *conn = mysql_init(NULL);

  if (conn == NULL) {
    printf("mysql_init() failed\n");

    return 1;
  }

  if (mysql_real_connect(conn, "localhost", "root", "", "todo_c", 3306, NULL, 0) == NULL ||
      mysql_options(conn, MYSQL_SET_CHARSET_NAME, "utf8mb4") != 0) {
    printf("mysql_real_connect() failed\n");
    printf("%s\n", mysql_error(conn));
    mysql_close(conn);

    return 1;
  }

  printf("----------------------------------------------\n");
  printf("Connected to MySQL server!\n");
  printf("TODO 프로그램이 정상적으로 실행되었습니다.\n");
  printf("----------------------------------------------\n");

  while (true) {
    int work_type;
    printf("\n0: 종료\n");
    printf("1: 조회\n");
    printf("2: 추가\n");
    printf("3: 수정\n");
    printf("4: 삭제\n\n");
    printf("작업을 선택하세요> ");
    scanf("%d", &work_type);

    if (work_type > 4 || work_type == 0 || work_type < 0) {
      break;
    }

    if (work_type == 1) {
      mysql_query(conn, "SELECT * FROM todo_table");

      MYSQL_RES *result = mysql_store_result(conn);
      MYSQL_ROW row;

      printf("----------------------------------------------\n");

      while ((row = mysql_fetch_row(result))) {
        for (register int i = 0; i < mysql_num_fields(result); i++) {
          printf("%s ", row[i]);
        }
        printf("\n");
      }

      mysql_free_result(result);

      printf("----------------------------------------------\n");
    } else if (work_type == 2) {
      char *title = (char *)calloc(255, sizeof(char));
      char *contents = (char *)calloc(65535, sizeof(char));
      char *query = (char *)calloc(66000, sizeof(char));

      if (title == NULL && contents == NULL && query == NULL) {
        printf("메모리 할당 실패\n");

        break;
      }

      printf("제목> ");
      scanf("%255s", title);

      printf("내용> ");
      scanf("%65535s", contents);

      sprintf(query, "INSERT INTO todo_table (title, contents) VALUES ('%s', '%s')", title, contents);
      if (mysql_query(conn, query) == 0) {
        printf("\n추가 성공\n");
        printf("----------------------------------------------\n");
      } else {
        printf("\n오류!\n");
        printf("%s\n", mysql_error(conn));
        printf("----------------------------------------------\n");
      }

      free(title);
      free(contents);
      free(query);
    } else if (work_type == 3) {
      int todo_number;
      char *query = (char *)calloc(66000, sizeof(char));

      if (query == NULL) {
        printf("메모리 할당 실패\n");

        break;
      }

      printf("번호> ");
      scanf("%d", &todo_number);

      sprintf(query, "SELECT * FROM todo_table WHERE ID = %d", todo_number);
      mysql_query(conn, query);

      MYSQL_RES *result = mysql_store_result(conn);
      if (mysql_num_rows(result)) {
        char *title = (char *)calloc(255, sizeof(char));
        char *contents = (char *)calloc(65535, sizeof(char));

        if (title == NULL && contents == NULL) {
          printf("메모리 할당 실패\n");

          break;
        }

        printf("제목> ");
        scanf("%255s", title);

        printf("내용> ");
        scanf("%65535s", contents);

        sprintf(query, "UPDATE todo_table SET title = %s, contents = %s WHERE ID = %d", title, contents, todo_number);
        if (mysql_query(conn, query) == 0) {
          printf("\n수정 성공\n");
          printf("----------------------------------------------\n");
        } else {
          printf("\n오류!\n");
          printf("%s\n", mysql_error(conn));
          printf("----------------------------------------------\n");
        }

        free(title);
        free(contents);
      } else {
        printf("번호를 다시 확인하세요.");
      }
      free(query);
    } else if (work_type == 4) {
      int todo_number;
      char *query = (char *)calloc(66000, sizeof(char));

      if (query == NULL) {
        printf("메모리 할당 실패\n");

        break;
      }

      printf("번호> ");
      scanf("%d", &todo_number);

      sprintf(query, "SELECT * FROM todo_table WHERE ID = %d", todo_number);
      mysql_query(conn, query);

      MYSQL_RES *result = mysql_store_result(conn);
      if (mysql_num_rows(result)) {
        char check_y[3];
        printf("정말로 삭제하겠습니까? [y/N]> ");
        scanf("%s", check_y);
        if (strcmp(check_y, "y") == 0 || strcmp(check_y, "Y") == 0) {
          sprintf(query, "DELETE FROM todo_table WHERE ID = %d", todo_number);
          if (mysql_query(conn, query) == 0) {
            printf("\n삭제 성공\n");
            printf("----------------------------------------------\n");
          } else {
            printf("\n오류!\n");
            printf("%s\n", mysql_error(conn));
            printf("----------------------------------------------\n");
          }
        }
      }
    }
  }

  printf("프로그램 종료\n");
  mysql_close(conn);

  return 0;
}