/*
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <mysql.h>
#include <my_global.h>

WINDOW *pad;
MYSQL *con;

static int mrow, mcol;
static char *host, *user, *pass;

void quit(void)
{
  // close MySQL connection
  mysql_close(con);
  // end curses
  endwin();
}


int main(int argc, char **argv)
{
  // initialize curses
  initscr();
  atexit(quit);
  clear();
  cbreak();
  keypad(stdscr, TRUE);
  start_color();

  // get terminal size
  getmaxyx(stdscr, mrow, mcol);

  // open MySQL connection
  host = "localhost";
  user = "root";
  pass = "8$oP.4L!";
  con = mysql_init(NULL);
  if (con == NULL)
  {
    exit(1);
  }
  if (mysql_real_connect(con, host, user, pass, NULL, 0, NULL, 0) == NULL)
  {
    mysql_close(con);
    exit(1);
  }

  // Cursor off
  curs_set(0);

  // Get SQL Data
  char *query = "SELECT IF((SELECT name FROM zeiterf.stdjobs WHERE nr = zeiterf.wtime.job) != '', (SELECT name FROM zeiterf.stdjobs WHERE nr = zeiterf.wtime.job), job), IF(usr != '', (SELECT vname FROM zeiterf.user WHERE nr = zeiterf.wtime.usr), NULL), IF(usr != '', (SELECT nname FROM zeiterf.user WHERE nr = zeiterf.wtime.usr), NULL), SEC_TO_TIME(SUM(TIME_TO_SEC(tout) - TIME_TO_SEC(tin))) FROM zeiterf.wtime WHERE tout != 0 AND job != '...ENDE' GROUP BY job , usr WITH ROLLUP;";
  if (mysql_query(con, query))
  {
    exit (1);
  }

  MYSQL_RES *result = mysql_store_result(con);
  MYSQL_ROW *row;
  int num_fields = mysql_num_fields(result);
  int rowcount = mysql_num_rows(result);

  // create pad
  pad = newpad (rowcount + 1, mcol);

  // col titles
  wprintw(pad, "Auftrag \tName \t\t\tZeit\n");

  while (row = mysql_fetch_row(result))
  {
    int i = 0;

    for (i = 0; i < num_fields; i++)
    {
      wprintw(pad, "%s\t", row[i]); 
    }

    wprintw(pad, "\n");
  }

  mysql_free_result(result);

  // Show content of pad
  int mypadpos = 0;
  prefresh(pad, mypadpos, mcol, 0, 0, mrow, mcol);

  // wait for exit key
  int ch;
  while((ch = wgetch(pad)) != 'q')
  {
    switch (ch)
    {
      case KEY_UP:
        if (mypadpos >= 0)
    {
      mypadpos--;
    }
    prefresh(pad, mypadpos, mcol, 0, 0, mrow, mcol);
    break;
      case KEY_DOWN:
    if (mypadpos <= rowcount+1)
    {
      mypadpos++;
    }
    prefresh(pad, mypadpos, mcol, 0, 0, mrow-1, mcol);
    break;
     }
  }

  // remove window
  delwin(pad);
  clear();
  refresh();

  return (0);
}
*/