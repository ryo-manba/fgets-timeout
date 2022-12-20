#include <stdio.h>

#define BUF_SIZE 50
char buf[BUF_SIZE];

int main(void)
{
  printf("input > ");
  fgets(buf, BUF_SIZE, stdin);
  printf("output: %s\n", buf);
  return 0;
}
