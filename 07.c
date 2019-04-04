#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *substrings[5] = {"the", "and", "or", "not", "Ph'nglui mglw'nafh Cthulhu R'lyeh wgah'nagl fhtagn"};

print_usage() {
    fprintf(stderr, "use: 07.exe FILE\n");
    fprintf(stderr, "\nFILE is the file for which the substrings will be searched\n");
}


int count_substrings(char *sub, char *file, char *buffer) { 
    int sub_len = strlen(sub);

    FILE *f = fopen(file, "r");
    fgets(buffer, 1024, f);
    fclose(f);

    int buffer_len = strlen(buffer); 
    int count = 0; 
    
	int i;
    for (i = 0; i <= buffer_len - sub_len; i++) {
        int j; 
        for (j = 0; j < sub_len; j++) 
            if (buffer[i+j] != sub[j]) 
                break; 
   
        if (j == sub_len)   
        { 
           count++; 
           j = 0; 
        } 
    } 
    return count; 
} 

int main(int argc, char *argv[]) {
    if(argc < 2) {
        print_usage();
        exit(0);
    }

    printf("Main parent starting forking: ");
    pid_t pid;
    int child_no;
    int my_substring;
    for(child_no = 1; child_no <= 5; child_no++) {
        pid = fork();
        if(pid < 0) {
            fprintf(stderr, "Error while forking child number %d\n", child_no);
        }

        if(pid == 0) {
            break;
        }
    }

    if(pid == 0) {
        int my_substring = child_no-1;
        char buffer[1024];
		int niceness = nice(child_no * 2);
        printf("I am child no. %d (PID %d, niceness %d)\n    found \"%s\" %d times\n",
            child_no,
            getpid(),
			niceness,
            substrings[my_substring],
            count_substrings(substrings[my_substring], argv[1], buffer));
        
        exit(0);
    }

    for(child_no = 1; child_no <= 5; child_no++) {
        wait(NULL);
    }

    printf("Main parent exits\n");

    return 0;
}
