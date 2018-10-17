#include "Interactive.h"

int input(char *s,int length);

void Interactive::run()
{
    char *buffer;
    size_t bufsize = 132;
    size_t characters;
    buffer = (char *)GC_MALLOC(bufsize * sizeof(char));
    if( buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }
    printf("Type something: ");
    characters = getline(&buffer,&bufsize,stdin);
    printf("%zu characters were read.\n",characters);
    printf("You typed: '%s'\n",buffer);
    Runner r;
    r.compile_run_string(buffer,true);
}

