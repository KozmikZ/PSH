#ifndef SHELL_H
#define SHELL_H
#define MAX_COMMAND_KEYWORD_LENGTH 20
#define MAX_ARGS 5

enum CommandsTable
{
    IDOS,         // idos
    METRO,        // ls
    URAD,        // help
    ODLOZ,         // touch
    PLZEN,        // clear
    DEFENESTRACE, // rm
    DEMONSTRACE,  // echo
    REFLEX,       // text editing
    KAVARNA,      // mkdir
    IDNES,        // history
    TABERY,       // cat
    MORAVA,        // exit
    TOMIO, // This command does absolutely nothing
};

typedef struct
{
    char cmd[MAX_COMMAND_KEYWORD_LENGTH];
    char args[MAX_ARGS][MAX_COMMAND_KEYWORD_LENGTH];
} Command;

void shell();
void handle_command(enum CommandsTable commandType, Command *cmd);

#endif