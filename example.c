#include <stdio.h>
#include <libcli.h>

int cmd_test(struct cli_def *cli, char *command, char *argv[], int argc) 
{
	cli_print(cli, "called %s with %s\r\n", __FUNCTION__, command);
	return CLI_OK;
}

int cmd_set(struct cli_def *cli, char *command, char *argv[], int argc) 
{
    if (argc < 2) 
	{
        cli_print(cli, "Specify a variable to set\r\n");
        return CLI_OK;
    }
    cli_print(cli, "Setting %s to %s\r\n", argv[0], argv[1]);
    return CLI_OK;
}

int main(int argc, char *argv[]) 
{
	struct sockaddr_in info;
    struct cli_command *c;
    struct cli_def *cli;
    int on = 1, x, s;
	/*初始化*/
    cli = cli_init(); 
    cli_set_hostname(cli, "usr");//名稱決定
    cli_set_banner(cli, "Hello!");//顯示Hello給連線者
	/*身分證明*/
    cli_allow_user(cli, "fred", "nerk");
    cli_allow_user(cli, "foo", "bar");
	/*設定開始*/
    cli_register_command(cli, NULL, "ex1", cmd_test, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, NULL);
    cli_register_command(cli, NULL, "ex2", NULL, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, NULL);
    cli_register_command(cli, NULL, "ex3", cmd_set, PRIVILEGE_PRIVILEGED, MODE_EXEC, NULL);
	// 會形成兩命令 ex4ex5 , ex4ex6
    c = cli_register_command(cli, NULL, "ex4", NULL, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, NULL);
    cli_register_command(cli, c, "ex5", cmd_test, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, NULL);
    cli_register_command(cli, c, "ex6", cmd_test, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Show the counters that the system uses");
	//Socket部分
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)); 
    bzero(&info,sizeof(info));
    info.sin_family = AF_INET;
    info.sin_addr.s_addr = htonl(INADDR_ANY);
    info.sin_port = htons(12345);
    bind(s, (struct sockaddr *)&info, sizeof(info));

    listen(s, 50);
    while ((x = accept(s, NULL, 0))) //若x符合套接字，則迴圈持續
	{
		/*執行*/
        cli_loop(cli, x); 
        close(x);
    }
    /*關閉*/
    cli_done(cli);
    return 0;
}
