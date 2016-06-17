#include <iostream>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

using std::vector;
using std::string;
using cmd_list = vector<vector<string>>;

cmd_list get_commands() {
    string s;
    vector<string> cmd;
    cmd_list lst;
    while(std::cin >> s) {
        if(s == "|") {
            if(cmd.size() > 0)
                lst.push_back(cmd);
            cmd.clear();
        } else {
            cmd.push_back(s);
        }
    }
    if(cmd.size() > 0)
        lst.push_back(cmd);

    return lst;
}

void execute(const cmd_list& lst) {
    int p_out = STDIN_FILENO;

    for(int cmd_i =0; cmd_i<lst.size(); ++cmd_i) {
        const cmd_list::value_type& cmd = lst[cmd_i];

        char** params = new char*[cmd.size()+1];
        for(int i =0; i< cmd.size(); ++i)
            params[i] = strdup(cmd[i].c_str());
        params[cmd.size()] = NULL;

        int pfd[2];
        pipe(pfd);

        if(cmd_i != lst.size() - 1) {
            if(!fork()) {
                close(STDOUT_FILENO);
                if (p_out != STDIN_FILENO)
                    close(STDIN_FILENO);
                dup2(p_out, STDIN_FILENO);
                dup2(pfd[1], STDOUT_FILENO);
                close(p_out);
                close(pfd[0]);
                close(pfd[1]);
                execvp(params[0],params);
            }
        } else {
            if (p_out != STDIN_FILENO)
                close(STDIN_FILENO);
            dup2(p_out, STDIN_FILENO);
            close(p_out);
            close(pfd[0]);
            close(pfd[1]);
            execvp(params[0],params);
        }
        
        for(int i=0; i< cmd.size(); ++i)
            free(params[i]);
        delete params;
        p_out = pfd[0];
        close(pfd[1]);
    }

    int status;
    while(waitpid(-1, &status, WNOHANG) != -1);
}

int main() {
    cmd_list lst = get_commands();
    execute(lst);

    return 0;
}
