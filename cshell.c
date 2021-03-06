#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


// parse the command line and return in an array of tokens with a trailing (char*)0
// example
//   line="ls -al foo"
//   return={"ls", "-al", "foo", (char*)0}
char** getCommandLine(const char *line)
{
    char **ppCmd = NULL;
    int cTokens = 0;
    const char delims[] = " \t\n";

    // first count the number of tokens
    // note that strtok will write on your original line buffer, 
    // so we use a temp duplicated copy, don't forget to free it
    char *dupLine = strdup(line);
    if (dupLine)
    {
        char *pToken = strtok(dupLine, delims);
        while (pToken != NULL)
        {
            cTokens++;
            pToken = strtok(NULL, delims);
        }

        // free the temp copy
        free(dupLine);
    }

    // allocate memory, add one token for the trailing '(char*)0'
    ppCmd = malloc((cTokens+1) * sizeof(char*));
    if (ppCmd)
    {
        int iToken = 0;
        char *pToken = strtok(line, delims);
        while (pToken != NULL)
        {
            ppCmd[iToken] = pToken;
            iToken++;
            pToken = strtok(NULL, delims);
        }

        // set the trailing '(char*)0'
        ppCmd[iToken] = (char*)0;
    }

    return ppCmd;
}

int doCommand(char **ppCmd)
{

    pid_t pID;
            
    // is it an internal command "exit" or "cd"?
    if (!strcmp(ppCmd[0], "exit"))
    {
        // add code here
        exit(pID);

    }
    else if (!strcmp(ppCmd[0], "cd"))
    {
        // add code here

        if (chdir(ppCmd[1]) == NULL) {
        	chdir(getenv("HOME"));
        }
        else {
        	if (chdir(ppCmd[1]) == -1) {
        		printf(" %s: no such directory\n", ppCmd[1]);
        	}
        }

    }
    else
    {
        // execute the external command
        pID = fork();
        if (pID == 0)
        {
            // child process
            // add code here: change execlp to execvp to include command line arguments
            execvp(ppCmd[0], ppCmd);

            // this following instruction will only happens if the exec failed
            printf("exec failed\n");
            
            // add code here: what do you do if the child process failed to lunch the command?
            //      for example, user types "lkasjdhflakjh" which is an invalid command
            printf("%s is an invalid command\n", ppCmd[0]);
        }
        else if (pID < 0)
        {
            // fork failed
            printf("fork failed\n");
            return 2;
        }
        else
        {
        	int status;
            // this is the parent process
            // wait for child process to finish
            // add code here: use waitpid system call
            waitpid(pID, &status, WNOHANG);

        }
    }

    return 0;
}

// main function 
int main(int argc, char* argv[])
{
    size_t n;
    char *line = NULL;
    int i;
    int result = 0;

    // add code here: loop till result is non-zero
    while (result == 0) {
	    char *dupLine = NULL;
	    printf("IT344Shell> ");
	    ssize_t cc = (ssize_t)getline(&line, &n, stdin);
	    if (line)
	    {
	        char **ppCmd = getCommandLine(line);
	        if (!ppCmd)
	        {
	            // getCommandLine failed to allocate memory
	            // add code here: exit the loop
	            break;
	        }
	       		// add code here: what if user just hit enter at the prompt without typing a command

	        if (ppCmd[0] == NULL) {
	        	continue;
	        }

	        result = doCommand(ppCmd);
	        free(ppCmd);

	    } // end if
	}

    // free the auto allocated line buffer
    if (line)
        free(line);

    return result;
}
