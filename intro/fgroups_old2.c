/*
 *   fgroups.c
 *
 *   Jake Jarvis (jjarvi01)
 *   Tam Luong   (tluong04)
 *
 *   COMP 40
 *   Homework 1 Part B
 *   1/23/15
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <atom.h>
#include <string.h>
#include <table.h>
#include <list.h>

void print_group(const void *key, void **value, void *cl);
void print_name(void **x, void *cl);

int main(int argc, char *argv[])
{
        /* compiler makes us use all the variables */
        (void)argc;
        (void)argv;

        /* create a new table that will store all the key (fingerprint) and
         * value (name) pairs */
        Table_T table = Table_new(0, NULL, NULL);

        /* buffer that will be used to iterate through each line of input */
        char buffer[2048];

        while (fgets(buffer, sizeof(buffer), stdin)) {
                if (buffer[0] == ' ' || buffer[0] == '\t') {  
                        /* line starts with whitespace, no good */
                        fprintf(stderr, "BAD INPUT: \"%s\"\n%s\n\n", buffer, 
                                        "Starts with whitespace.");
                } else if (buffer[0] != '\n') {    /* skip this line if it's 
                                                    * just a newline */

                        /* invariant: if there's no space or tab on this line,
                         * these will always equal -1 */
                        int space_index = -1;
                        int tab_index = -1;

                        /* search line for first instance of whitespace */
                        if (strchr(buffer, ' ') != NULL)
                                space_index = strchr(buffer, ' ') - buffer;
                        if (strchr(buffer, '\t') != NULL)
                                tab_index = strchr(buffer, '\t') - buffer;

                        /* which one occurs first, if at all? */
                        int whitespace_index = 0;
                        if (space_index > 0 && tab_index > 0) {
                                if (space_index < tab_index) /* space first */
                                        whitespace_index = space_index;
                                else                         /* tab first */
                                        whitespace_index = tab_index;
                        } else if (space_index > 0) {        /* space only */
                                whitespace_index = space_index;
                        } else if (tab_index > 0) {          /* tab only */
                                whitespace_index = tab_index;
                        } else { /* if there's no space or tab then there's 
                                  * only one element (a fingerprint). fail! */
                                fprintf(stderr, "BAD INPUT: \"%s\"\n%s\n\n",
                                                buffer, "No name.");
                        }

                        /* store the fingerprint in own variable for later */
                        char *fingerprint = malloc(512);
                        memcpy(fingerprint, &buffer[0], whitespace_index);
                        fingerprint[whitespace_index] = '\0';

                        /* the rest of the line is the name... */
                        char *name_untrimmed = malloc(512);
                        int name_length = sizeof(buffer) - whitespace_index;
                            /* name_length isn't correct yet, it still includes
                             * leading & trailing whitespace */
                        memcpy(name_untrimmed, &buffer[whitespace_index],
                               name_length);


                        /* ...but we have to trim the whitespace on both ends
                         * start with leading whitespace: */
                        int first_index = 0;
                        while (isspace(name_untrimmed[first_index]))
                                first_index++;

                        /* now trim trailing whitespace: */
                        int last_index = strlen(name_untrimmed);
                        while (isspace(name_untrimmed[last_index])
                           || name_untrimmed[last_index] == '\0')
                                last_index--;

                        /* replace name with trimmed version & terminate */
                        name_length = last_index - first_index + 1;
                        char *name = malloc(512);
                        memcpy(name, &name_untrimmed[first_index], name_length);
                        name[name_length] = '\0';



                        // DEBUG
                        /*printf("line: %s\n", buffer);
                        printf("index of space: %i\n", space_index);
                        printf("index of tab: %i\n", tab_index);
                        printf("whitespace index: %i\n", whitespace_index);
                        printf("first character index: %i\n", first_index);
                        printf("last character index: %i\n", last_index);
                        
                        printf("fingerprint: %s\n", fingerprint);
                        printf("name: %s\n", name);
                        
                        printf("\n\n");*/



                        /* the fingerprint must be converted into an atom now
                         * because we need to compare them to each other */
                        const char *fingerprint_atom = Atom_string(fingerprint);

                        /* retrieve the existing list of names from the table
                         * corresponding to the fingerprint we're looking at
                         * for this line. (there might not be one yet.) */
                        List_T name_list = Table_get(table, fingerprint_atom);

                        /* push the name to the top of the list we received and
                         * overwrite the old list */
                        name_list = List_push(name_list, name);

                        /* put the modified list back into the table under the
                         * fingerprint key */
                        Table_put(table, fingerprint_atom, name_list);
                }
        }
        
        if (ferror(stdin))    /* something went wrong with reading stdin */
                fprintf(stderr, "BAD INPUT: Error reading stdin\n");

        /* print the groups */
        Table_map(table, print_group, NULL);

        /* all done with the table, free up memory */
        Table_free(&table);

        return 0;
}

void print_group(const void *key, void **value, void *cl)
{
        /* compiler makes us use all the variables */
        (void)key;
        (void)cl;

        /* if there's only one name to this fingerprint, don't print anything */
        if (List_length(*value) > 1) {
                List_map(*value, print_name, NULL);
                printf("\n"); /* print an extra newline at the end of the group */
        }

        /* all done with this pair, free up memory */
        List_free(*value);
}

void print_name(void **x, void *cl)
{
        /* compiler makes us use all the variables */
        (void)cl;

        /* x argument is actually a string of the name */
        char *name = *x;

        printf("%s\n", name);
}
