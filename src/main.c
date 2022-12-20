#include <stdio.h>

#define BUF_SIZE 50
char buf[BUF_SIZE];

char *tfgets(char *str, int size, FILE *stream);

int main(void)
{
  if (tfgets(buf, BUF_SIZE, stdin) == NULL)
    puts("timeout!!");
  else
    puts(buf);
  return 0;
}
