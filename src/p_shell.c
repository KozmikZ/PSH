#include "p_shell.h"
#include "commands.c"
#include "include/vector.c"
void handle_command(enum CommandsTable commandType, Command *cmd)
{
    switch (commandType)
    {
    case METRO:
        handle_metro(cmd);
        break;
    case IDOS:
        handle_idos(cmd);
        break;
    case URAD:
        handle_urad(cmd);
        break;
    case ODLOZ:
        handle_odloz(cmd);
        break;
    case PLZEN:
        handle_plzen(cmd);
        break;
    case DEFENESTRACE:
        handle_defenestrace(cmd);
        break;
    case DEMONSTRACE:
        handle_demonstrace(cmd);
        break;
    case REFLEX:
        handle_reflex(cmd);
        break;
    case KAVARNA:
        handle_kavarna(cmd);
        break;
    case IDNES: 
        handle_idnes(cmd);
        break;
    case TABERY:
        handle_tabery(cmd);
        break;
    case MORAVA:
        handle_morava(cmd);
        break;
    case TOMIO:
        handle_tomio(cmd);
        break;
    default:
        raise("Failed to map command");
    }
    History[++History_top] = commandType;
}

Command *parse_command_str(char *str) // parses a command string into command and arguments
{
    Command *command = (Command *)malloc(sizeof(Command));
    for (int i = 0; i < MAX_ARGS; i++)
    { // clearing the memory of unspecified behaviour
        command->args[i][0] = '\0';
    }
    command->cmd[0] = '\0';
    Vector *split_vec = createNewVector(1);
    int tmp_index = 0;
    char *part = (char *)malloc(sizeof(char) * MAX_COMMAND_KEYWORD_LENGTH);
    memset(part, 0, MAX_COMMAND_KEYWORD_LENGTH);
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == ' ')
        {
            if (split_vec->top == 4)
            {
                printf("Maximum number of arguments reached... stopping");
                break;
            }
            part[tmp_index] = '\0';
            push(split_vec, part);
            part = (char *)malloc(sizeof(char) * MAX_COMMAND_KEYWORD_LENGTH);
            memset(part, 0, MAX_COMMAND_KEYWORD_LENGTH);
            tmp_index = 0;
            continue;
        }

        part[tmp_index] = str[i];
        tmp_index++;
    }
    push(split_vec, part);
    for (int i = 0; i <= split_vec->top; i++)
    {
        strcpy(command->args[i], split_vec->items[i]);
    }
    strcpy(command->cmd, command->args[0]);
    freeVector(split_vec);
    return command;
}

void execute_command(Command *cmd) // executes a given parsed command
{

    for (int i = 0; i <= TOMIO; i++)
    {
        enum CommandsTable type = i;
        if (strcmp(commandToString(type), cmd->cmd) == 0)
        {

            handle_command(type, cmd);
            return;
        }
    }
    printf("Unknown command - %s, use urad for help\n", cmd->cmd);
}

void trim_newline(char *str) // removes the last character in a string if it is a \n
{
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
    }
}

void shell()
{
    initialize_shell();
    while (true)
    { 
        printf("psh:default_user:%s/> ", public_CWD);
        char Str_Command[100];
        fgets(Str_Command, sizeof(Str_Command), stdin);
        trim_newline(Str_Command);
        Command *to_command = parse_command_str(Str_Command);
        execute_command(to_command);
        free(to_command);
    }
}
int main()
{

    shell();
}