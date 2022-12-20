#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/wait.h>
#include <unistd.h>

#define TIMEOUT 5
static sigjmp_buf env;

static void handler(int sig)
{
  wait(NULL);
  // sigsetjmpが1を返す
  siglongjmp(env, 1);
}

char *tfgets(char *s, int size, FILE *stream)
{
  pid_t pid;
  char *str = NULL;

  // SIGCHLDを補足するシグナルハンドラを設定する
  signal(SIGCHLD, handler);

  // 子プロセス -> pid = 0
  // 親プロセス -> pid = 子プロセスのID
  if ((pid = fork()) == 0)
  {
    /// 子プロセス
    sleep(TIMEOUT);
    exit(0);
  }
  else
  {
    /// 親プロセス

    // 子プロセスによってsiglongjmpが実行された場合は0を返さない
    if (sigsetjmp(env, 1) == 0)
    {
      str = fgets(s, size, stream);
      kill(pid, SIGKILL);
      // siglongjmpが呼ばれるまで待つ
      pause();
    }
    return str;
  }
}
