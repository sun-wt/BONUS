
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <libcli.h>

int32_t cmd_hello(struct cli_def *cli, char *command, char *argv[], int argc) 
{
    cli_print(cli, "hello world");
    return 0;
}
int32_t cmd_test(struct cli_def *cli, char *command, char *argv[], int argc) 
{
	cli_print(cli, "called %s with %s\r\n", __FUNCTION__, command);
	return CLI_OK;
}

int32_t cmd_set(struct cli_def *cli, char *command, char *argv[], int argc) 
{
  if (argc < 2) 
	{
    cli_print(cli, "Specify a variable to set\r\n");
    return CLI_OK;
  }
  cli_print(cli, "Setting %s to %s\r\n", argv[0], argv[1]);
  return CLI_OK;
}

int main(void) 
{
    struct cli_def *cli;
    int32_t sockfd, acceptfd;
    struct sockaddr_in saddr, raddr;
    unsigned int on = 1;
    unsigned int rlen = sizeof(struct sockaddr_in);

    /*初始化*/
	  cli = cli_init(); 
	  cli_set_hostname(cli, "usr");//名稱決定
	  cli_set_banner(cli, "Hello!");//顯示Hello給連線者

		/*身分證明*/
	  cli_allow_user(cli, "fred", "nerk");
	  cli_allow_user(cli, "foo", "bar");

		/*設定開始*/
    cli_register_command(cli, NULL, "hello", cmd_hello, PRIVILEGE_UNPRIVILEGED, MODE_ANY, NULL);
		cli_register_command(cli, NULL, "test", cmd_test, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, NULL);
	  cli_register_command(cli, NULL, "set", cmd_set, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, NULL);
		
		/*Socket*/
    if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1) 
		{
         perror("socket");
         exit(-1);
    }

		saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(12345);
    
    memset(&raddr, 0, sizeof(raddr));
    raddr.sin_family=AF_INET;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0) 
		{
        perror("setsockopt");
        exit(-1);
    }
    if(bind(sockfd, (struct sockaddr*)&saddr, sizeof(saddr)) == -1) 
		{
         perror("bind");
         exit(-1);
    }
    if(listen(sockfd, 12) != 0) 
		{
         perror("listen");
         exit(-1);
    }
		/*執行*/
    while((acceptfd=accept(sockfd, (struct sockaddr*)&raddr, &rlen)) > 0) 
		{    
         while(cli_loop(cli, acceptfd) == 0);
    }
		/*關閉*/
		cli_done()
    return 0;
}
