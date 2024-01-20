#include "io_tools.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/utils.h"
#include "include/random.h"
#define DT_DIR 4

static char public_CWD[80];
int History[1000];
int History_top = -1;

const char *commandToString(enum CommandsTable cmd)
{
    switch (cmd)
    {
    case METRO:
        return "metro";
    case IDOS:
        return "idos";
    case URAD:
        return "urad";
    case ODLOZ:
        return "odloz";
    case PLZEN:
        return "plzen";
    case DEFENESTRACE:
        return "defenestrace";
    case DEMONSTRACE:
        return "demonstrace";
    case REFLEX:
        return "reflex";
    case KAVARNA:
        return "kavarna";
    case IDNES:
        return "idnes";
    case MORAVA:
        return "morava";
    case TABERY:
        return "tabery";
    case TOMIO:
        return "tomio";
    default:
        return "unknown";
    }
}

bool is_directory_in_CWD(const char *f)
{
    char tmp_CWD[80];
    tmp_CWD[0] = '\0';
    strcpy(tmp_CWD, public_CWD);
    snprintf(tmp_CWD + strlen(tmp_CWD), 80 - strlen(tmp_CWD), "/%s", f);
    if (is_directory(tmp_CWD))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void show_help(enum CommandsTable type)
{
    switch (type)
    {
    case METRO:
        printf("metro - changes working directory\n");
        printf("Example usage: metro home\n");
        break;
    case IDOS:
        printf("idos - lists the files in CWD\n");
        printf("Arguments: pulnoc\n");
        printf("Example usage: idos\n");
        break;
    case URAD:
        printf("urad - help command, use fix to fix your issues\n");
        printf("Example usage -> urad fix \n");
        break;
    case ODLOZ:
        printf("odloz - creates(touches) a new file in CWD\n");
        printf("Example usage -> odloz new.txt\n");
        break;
    case PLZEN:
        printf("plzen - clears the screen (and the mind)\n");
        break;
    case DEFENESTRACE:
        printf("defenestrace - The remove command -> it removes a file\n");
        break;
    case DEMONSTRACE:
        printf("demonstrace - Command to write things into the stdout\n");
        break;
    case REFLEX:
        printf("reflex - used to write text into files -> use exit to stop and save\n");
        printf("Example usage: reflex new.txt\n");
        break;
    case KAVARNA:
        printf("kavarna - Creates a new directory in the CWD\n");
        break;
    case IDNES:
        printf("idnes - Outputs the console history\n");
        break;
    case TOMIO:
        puts("Well, that's complicated...");
        break;
    case TABERY:
        puts("tabery - writes a files contents to console");
        break;
    case MORAVA:
        puts("morava - exit command");
        break;
    default:
        raise("Failed to map command");
    }
}

void initialize_shell()
{
    assert(getcwd(public_CWD, sizeof(public_CWD)), "getcwd error");
}
void handle_metro(Command *cmd) // a command to change directory
{
    char tmp_CWD[80];
    tmp_CWD[0] = '\0';
    strcpy(tmp_CWD, public_CWD);
    snprintf(tmp_CWD + strlen(tmp_CWD), 80 - strlen(tmp_CWD), "/%s", cmd->args[1]);
    if (is_directory(tmp_CWD))
    {
        if (strcmp(cmd->args[1], "..") == 0) // to prevent cwd from stacking .. on top of one another -> prevent this: path/../folder/../../
        {
            int ln = strlen(public_CWD);
            int i = ln - 1;
            while (public_CWD[i] != '/')
            {
                i--;
            }
            public_CWD[i] = '\0';
        }
        else
        {
            strcpy(public_CWD, tmp_CWD);
        }
        assert(chdir(public_CWD) == 0, "Failed to change directory due to a system issue");
    }
    else
    {
        printf("%s is not a directory!\n", tmp_CWD);
    }
}
void handle_idos(Command *cmd) // a command that shows all the files in a directory
{

    DIR *d = opendir(public_CWD);
    struct dirent *dir_entry = readdir(d);
    assert(d != NULL, "Failed to open working directory");
    bool show_hidden = false;
    char *arg = cmd->args[0];
    for (int i = 0; i < MAX_ARGS; i++) // is there pulnoc amongst the arguments
    {
        if (strcmp("pulnoc", arg) == 0)
        {
            show_hidden = true;
        }
        arg = cmd->args[++i]; // next
    }
    while (dir_entry != NULL)
    {
        bool to_show = true;
        if (dir_entry->d_name[0] == '.')
        {
            if (show_hidden == false)
            {
                to_show = false;
            }
        }
        if (to_show)
        {
            if (dir_entry->d_type == DT_DIR)
            {
                printf("    %s/\n", dir_entry->d_name);
            }
            else
            {
                printf("    %s\n", dir_entry->d_name);
            }
        }
        dir_entry = readdir(d);
    }
    closedir(d);
    free(dir_entry);
}
void handle_urad(Command *cmd) // a help command
{
    bool fix = false;
    char *arg = cmd->args[0];
    for (int i = 0; i < MAX_ARGS; i++) // is 'fix' used?
    {
        if (strcmp("fix", arg) == 0)
        {
            fix = true;
        }
        arg = cmd->args[++i]; // next argument
    }
    if (fix)
    {
        show_help(History[History_top]);
    }
    else
    {
        printf("Arrived at the helping station:\n\n");
        printf("metro - changes working directory\n");
        printf("idos - lists the files in CWD\n");
        printf("odloz - creates(touches) a new file in CWD\n");
        printf("plzen - clears the screen (and the mind)\n");
        printf("reflex - used to write text into files -> use exit to stop and save\n");
        printf("defenestrace - The remove command -> it removes a file\n");
        printf("demonstrace - Command to write things into the stdout\n");
        printf("kavarna - Creates a new directory in the CWD\n");
        printf("idnes - Outputs history of commands\n");
        puts("morava - exit command");
        puts("tabery - writes a files contents to console");
        puts("tomio - mystery command");
        puts("urad - urad shows help messages -> urad fix for special treatment");
    }
}
void handle_odloz(Command *cmd) // creates(touches) a new blank file
{
    if (cmd->args[1][0] == '\0')
    {
        puts("Missing file name");
        return;
    }
    FILE *file;
    file = fopen(cmd->args[1], "w");
    assert(file != NULL, "File could not be created");
    fclose(file);
}
void handle_plzen(Command *cmd) // clears the screen
{
    for (int i = 0; i < 100; i++)
    {                  // Print 100 new lines
        putchar('\n'); // achieve a similar effect to clear. There is no better way to do this
    }
}
void handle_defenestrace(Command *cmd) // removes a file
{
    if (cmd->args[1][0] == '\0')
    {
        puts("Missing file name");
        return;
    }

    const char *name = cmd->args[1];
    if (file_exists(name) == false || is_directory_in_CWD(cmd->args[1]))
    {
        printf("%s is not an existing file\n", name);
        return;
    }
    if (remove(name) != 0)
    {
        printf("Failed to remove file");
    }
}
void handle_demonstrace(Command *cmd) // almost useless ...
{
    for (int i = 1; i < MAX_ARGS; i++)
    {
        if (strlen(cmd->args[i]) == 0)
        {
            return;
        }
        printf("Václavák: %s\n", cmd->args[i]);
    }
}
void handle_reflex(Command *cmd) // edits a created file
{
    if (cmd->args[1][0] == '\0')
    {
        puts("Missing file name");
        return;
    }
    if (file_exists(cmd->args[1]) == false)
    {
        puts("Please create a file first with odloz");
        return;
    }
    FILE *file;
    file = fopen(cmd->args[1], "w");
    char str_line[100];
    do
    {
        str_line[0] = '\0';
        printf("> "); // new line to write text on
        fgets(str_line, sizeof(str_line), stdin);
        if (strcmp(str_line, "exit\n") != 0)
        {
            fprintf(file, "%s", str_line);
        }
        else
        {
            break;
        }
        str_line[strlen(str_line) - 1] = '\0';
    } while (true);
    fclose(file);
}
void handle_kavarna(Command *cmd) // creates a new directory
{
    if (cmd->args[1][0] == '\0')
    {
        puts("Missing new directory name");
        return;
    }

    char *newdir = cmd->args[1];
    if (is_directory_in_CWD(newdir))
    {
        puts("A directory of this name already exists");
        return;
    }
    if (mkdir(newdir, 0700) != 0)
    {
        puts("Failed to create new directory");
    }
}
void handle_idnes(Command *cmd) // prints the history of the shell
{
    for (int i = above_0(History_top - 10); i <= History_top; i++)
    {
        printf("%s\n", commandToString(History[i]));
    }
}

void handle_tabery(Command *cmd) // prints the contents of a file
{
    if (cmd->args[1][0] == '\0')
    {
        puts("Missing target file name");
        return;
    }

    FILE *f = fopen(cmd->args[1], "r");

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    assert(f != NULL, "Failed to read file because of a system issue");

    while ((read = getline(&line, &len, f)) != -1)
    {
        printf(line);
    }

    free(line);
    fclose(f);
}
void handle_tomio()
{
    switch (randint(0, 2))
    {
    case 0:
        puts("JÁ MÁM 39 HOREČKY A... (Poslanecká sněmovna stále neudělala vůbec nic, protože má už 10 hodin proslov)");
        break;
    case 1:
        puts("JÁ ŽENY MILUJU...");
        break;
    case 2:
        puts(" afd sloh ");
        break;
    default:
        break;
    }
}
void handle_morava(Command *cmd)
{ // it cannot be handled
    exit(EXIT_SUCCESS);
}