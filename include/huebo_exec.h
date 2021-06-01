#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

#ifdef __cplusplus
extern "C"
{
#endif
extern void exec_fork(const char *__cmd,char *const* __cmd_arglist/*[]*/) __THROW;
extern void exec_fork(const char *__cmd,char *const* __cmd_arglist/*[]*/){
	int return_value;
	//char *cmd = __cmd;
	//char *cmd_argslist[] = __cmd_arglist[];
	if((fork())== -1){
		perror("Making Child Process");
		exit(EXIT_FAILURE);
	}
	else{
		return_value = execvp(__cmd,__cmd_arglist);
		if(return_value == -1){
			perror("Error");
			exit(EXIT_FAILURE);
		}
	}
}

#ifdef __cplusplus
extern "C"
}
#endif 
