#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <atom.h>
#include <string.h>

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        
        char buffer[512];
        
        while(fgets(buffer, sizeof(buffer), stdin)) {
                if(buffer[0] == ' ') {
                        fprintf(stderr, "BAD INPUT: \"%s\"\n%s\n\n", buffer, 
                                        "Starts with whitespace.");
                } else if(buffer[0] != '\n') {    // skip this line if it's 
                                                  // just a newline
                        
                        // if there's no space or tab, these will equal -1
                        int space_index = -1;
                        int tab_index = -1;
                        
                        // search line for first instance of whitespace
                        if(strchr(buffer, ' ') != NULL)
                                space_index = strchr(buffer, ' ') - buffer;
                        if(strchr(buffer, '\t') != NULL)
                                tab_index = strchr(buffer, '\t') - buffer;
                        
                        // which one occurs first, if at all?
                        int whitespace_index = 0;
                        if(  (space_index < tab_index && space_index > 0)
                          || (space_index > tab_index && tab_index < 0)   )
                                whitespace_index = space_index; // space first
                        else if( (tab_index < space_index && tab_index > 0)
                              || (tab_index > space_index && space_index < 0) )
                                whitespace_index = tab_index;   // tab first
                        else if(space_index < 0 && tab_index < 0)
                                fprintf(stderr, "BAD INPUT: \"%s\"\n%s\n\n",
                                                buffer, "No name.");
                                // if there's no space or tab then there's only
                                // one element (a fingerprint). fail!
                        
                        // store the fingerprint in its own variable for later
                        char fingerprint[512];
                        memcpy(fingerprint, &buffer[0], whitespace_index);
                        fingerprint[whitespace_index] = '\0';
                        
                        // the rest of the line is the name...
                        char *name = malloc(512);
                        memcpy(name, &buffer[whitespace_index], sizeof(buffer) - whitespace_index);
                        
                        // ...but we have to trim the whitespace on both ends
                        // start with leading whitespace:
                        int first_char_index = 0;
                        while(isspace(name[first_char_index]))
                                first_char_index++;
                        
                        // now trim trailing whitespace:
                        int last_char_index = strlen(name);
                        while(isspace(name[last_char_index]) || name[last_char_index] == '\0')
                                last_char_index--;
                        
                        // replace name string with trimmed version & terminate
                        int name_length = last_char_index - first_char_index + 1;
                        memcpy(name, &name[first_char_index], name_length);
                        name[name_length] = '\0';
                        
                        
                        
                        // DEBUG
                        //printf("line: %s\n", buffer);
                        /*printf("index of space: %i\n", space_index);
                        printf("index of tab: %i\n", tab_index);
                        printf("whitespace index: %i\n", whitespace_index);
                        printf("first character index: %i\n", first_char_index);
                        printf("last character index: %i\n", last_char_index);*/
                        
                        printf("fingerprint: %s\n", fingerprint);
                        printf("name: %s\n", name);
                        
                        printf("\n\n");
                        
                }
        }
        
        if(ferror(stdin))
                fprintf(stderr, "BAD INPUT: Error reading stdin\n");

        return 0;
                
                
                
                
                //printf("\n");
                

           /*     printf("length: %i\n", (int)sizeof(buffer));
                int total_length = (int)sizeof(buffer);

                int space_index = strchr(buffer, ' ') - buffer;
                printf("index of space: %i\n", space_index);

                int tab_index = strchr(buffer, '\t') - buffer;
                printf("index of tab: %i\n", tab_index);*/


           //     int test = strchr(buffer, ' ');

                
             //   fingerprint[space_index] = '\0';
                
             //   char fingerprint[512];
             //   memcpy(fingerprint, &buffer[0], space_index);

     /*           char name[512];
                memcpy(name, &buffer[space_index], total_length);
                name[sizeof(name)] = '\0';
*/
             //   name = trim(name);
                
        /*        if(strcmp(name[sizeof(name)-1], "\n")) {
                        printf("there is a newline at the end\n");
                }*/
                
                
                
                // boolean false until non-whitespace character found
         /*       int leading_whitespace_trimmed = 0;
                int first_char_index = 0;
                while(leading_whitespace_trimmed == 0) {
                        if(name[first_char_index] == ' ' || name[first_char_index] == '\t') {
                                first_char_index++;
                        } else {
                                leading_whitespace_trimmed = 1;
                        }
                }
                
                memcpy(name, &name[first_char_index], total_length);
                
                
                
                
                int trailing_whitespace_trimmed = 0;
                int last_char_index = 0;
                while(trailing_whitespace_trimmed == 0) {
                        if(name[last_char_index] == ' ' || name[last_char_index] == '\t' || name[last_char_index] == '\n') {
                                last_char_index--;
                        } else {
                                trailing_whitespace_trimmed = 1;
                        }
                }*/
                
                
                
           /*     char *trimmed;
                trimmed = *name;

                unsigned int i;
                unsigned int j;

                    for (i = 0, j = 0; i < sizeof(name); i++, j++)          
                    {
                        if(*name[i] != ' ')                           
                            trimmed[j] = name[i];                     
                        else
                            j--;                                     
                    }
                    trimmed[j] = 0;*/
                
                
                
                

                
           //     printf("name: %sxxx\n", name);






}

/*char[512]* trim(char*s[512])
{
    char *end = s + strlen(s)-1;
    while(*s && isspace(*s))
        *s++ = 0;
    while(isspace(*end))
        *end-- = 0;
    return s;
}*/