#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
// Function prototypes
void executeCommand(const char *command);
void listAllFilesAndDirectories();
void createNewFile(int fileChoice);
void deleteExistingFile();
void renameFile();
void editFileContent();
void searchFile();
void viewFileContent();
void sortFileContent();
void displayFileDetails();
int main()
{
    int i = 0;

    printf("==============================================================");
    printf("--------------------File Management Project--------------------");
    printf("==============================================================");
    printf("Welcome, the main menu is given below: ");
    while (i < 100)
    {
        int opt1;
        printf("\nOptions:\n");
        printf("1. List all files and directories\n");
        printf("2. Create a new file\n");
        printf("3. Delete an existing file\n");
        printf("4. Rename a file\n");
        printf("5. Edit file content\n");
        printf("6. Search for a file\n");
        printf("7. Display file details\n");
        printf("8. View file content\n");
        printf("9. Sort file content\n");
        printf("10. Write content to file \n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &opt1);
        switch (opt1)
        {
        case 1:
            listAllFilesAndDirectories();
            break;
        case 2:
            createNewFile(1); // Assuming .c file for simplicity
            break;
        case 3:
            deleteExistingFile();
            break;
        case 4:
            renameFile();
            break;
        case 5:
            editFileContent();
            break;
        case 6:
            searchFile();
            break;
        case 7:
            displayFileDetails();
            break;
        case 8:
            viewFileContent();
            break;
        case 9:
            sortFileContent();
            break;
        case 10:
            // writeFileContent();
            break;
        case 0:
            printf("Good Bye..\nSuccessfully Exit\n");
            return 0;
        default:
            printf("Invalid Input..Try again....\n");
        }
        i++;
    }
    return 0;
}
// Function Definitions
void executeCommand(const char *command)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    { // Child process
        execl("/bin/sh", "sh", "-c", command, (char *)NULL);
        perror("Exec failed");
        exit(EXIT_FAILURE);
    }
    else
    { // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            fprintf(stderr, "Command failed with exit status %d\n", WEXITSTAT(status));
            exit(EXIT_FAILURE);
        }
    }
}
void listAllFilesAndDirectories()
{
    printf("List all files and directories here..\n");
    printf("Showing all files and directories....\n");
    printf("Loading..\n");
    // Simulating sleep
    // sleep(3);
    printf("-------------------------------OutPut-------------------------\n");
    executeCommand("ls");
    printf(" \n");
}
void createNewFile(int fileChoice)
{
    char filename[50];
    const char *extensions[] = {".c", ".sh", ".txt"};
    const char *fileType = extensions[fileChoice - 1];
    printf("Enter File Name without %s Extension\n", fileType);
    scanf("%s", filename);
    char command[100];
    sprintf(command, "touch \"%s%s\"", filename, fileType);
    printf("-------------------------------OutPut-------------------------\n");
    executeCommand(command);
    printf("File Created Successfully\n \n");
}
void deleteExistingFile()
{
    char fileName[50];
    printf("Enter name of File you want to Delete!\n");
    printf("Note: Please Enter the full Name with Extension.\n");
    scanf("%s", fileName);
    printf("-------------------------------OutPut-------------------------\n");
    if (unlink(fileName) == 0)
    {
        printf("Successfully Deleted.\n \n");
    }
    else
    {
        printf("File Does not Exist..Try again\n \n");
    }
}
void renameFile()
{
    char oldName[50], newName[50];
    printf("Enter Old Name of File with Extension..\n");
    scanf("%s", oldName);
    printf("Checking for file...\n");
    if (access(oldName, F_OK) == 0)
    {
        printf("Ok File Exists.\n");
        printf("Now Enter New Name for file with Extension\n");
        scanf("%s", newName);
        char command[300];
        sprintf(command, "mv \"%s\" \"%s\"", oldName, newName);
        executeCommand(command);
        printf("Successfully Renamed.\n");
        printf("Now Your File Exists with %s Name\n", newName);
    }
    else
    {
        printf("%s does not exist..Try again with the correct filename.\n", oldName);
    }
    printf(" \n");
}
void Edit_File()
{
    char detailFile[50];
    printf("Enter File Name to Edit : ");
    scanf("%s", detailFile);
    printf("------OutPut------n");
    printf("Checking for file..\n");
    if (access(detailFile, F_OK) == 0)
    {
        char command[100];
        sprintf(command, "nano \"%s\"", detailFile);
        if (system(command) == -1)
        {
            printf("Error executing the command\n");
        }
    }
    else
        printf("%s File does not exist..Try again\n", detailFile);
    printf(" \n");
}

void editFileContent()
{
    char editFile[50];
    printf("Enter File Name with Extension : ");
    scanf("%s", editFile);
    printf("-------------------------------OutPut-------------------------\n");
    printf("Checking for file..\n");
    if (access(editFile, F_OK) == 0)
    {
        int fd = open(editFile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if (fd == -1)
        {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
        char buffer[4096];
        ssize_t bytesRead;
        while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0)
        {
            write(STDOUT_FILENO, buffer, bytesRead);
        }
        printf("Editing file...\n");
        // Clear the file content
        if (ftruncate(fd, 0) == -1)
        {
            perror("Error truncating file");
            exit(EXIT_FAILURE);
        }
        // Move the file cursor to the beginning
        if (lseek(fd, 0, SEEK_SET) == -1)
        {
            perror("Error seeking file");
            exit(EXIT_FAILURE);
        }
        printf("Enter new content. Press Ctrl+D to save and exit.\n");
        while (1)
        {
            char inputBuffer[4096];
            ssize_t inputSize = read(STDIN_FILENO, inputBuffer,
                                     sizeof(inputBuffer));
            if (inputSize == 0)
            {
                break; // Ctrl+D pressed
            }
            if (write(fd, inputBuffer, inputSize) == -1)
            {
                perror("Error writing to file");
                exit(EXIT_FAILURE);
            }
        }
        close(fd);
        printf("File edited successfully.\n");
    }
    else
    {
        printf("%s File does not exist..Try again.\n", editFile);
    }
}
void searchFile()
{
    char searchFile[50];
    printf("Enter File Name with Extension to search: ");
    scanf("%s", searchFile);
    printf("-------------------------------OutPut-------------------------\n");
    if (access(searchFile, F_OK) == 0)
    {
        printf("Searching for %s File\n", searchFile);
        printf("File Found.\n");
        char command[100];
        sprintf(command, "find /home -name \"%s\"", searchFile);
        executeCommand(command);
        printf(" \n");
    }
    else
        printf("File Does not Exist..Try again.\n\n");
}
void viewFileContent()
{
    char viewFile[50];
    printf("Enter File Name : ");
    scanf("%s", viewFile);
    printf("-------------------------------OutPut-------------------------\n");
    if (access(viewFile, F_OK) == 0)
    {
        int fd = open(viewFile, O_RDONLY);
        if (fd == -1)
        {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
        char buffer[4096];
        ssize_t bytesRead;
        while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0)
        {
            write(STDOUT_FILENO, buffer, bytesRead);
        }
        close(fd);
    }
    else
        printf("%s does not exist\n", viewFile);

    printf(" \n");
}
void sortFileContent()
{
    char sortFile[50];
    printf("Enter File Name with Extension to sort :");
    scanf("%s", sortFile);
    printf("-------------------------------OutPut-------------------------\n");
    if (access(sortFile, F_OK) == 0)
    {
        printf("Sorting File Content..\n");
        char command[100];
        sprintf(command, "sort \"%s\"", sortFile);
        executeCommand(command);
    }
    else
        printf("%s File does not exist..Try again.\n", sortFile);

    printf(" \n");
}
void displayFileDetails()
{
    char detailFile[50];
    printf("Enter File Name with Extension to see Detail : ");
    scanf("%s", detailFile);
    printf("-------------------------------OutPut-------------------------\n");
    printf("Checking for file..\n");
    if (access(detailFile, F_OK) == 0)
    {
        printf("Loading Properties..\n");
        char command[100];
        sprintf(command, "stat \"%s\"", detailFile);
        executeCommand(command);
    }
    else
        printf("%s File does not exist..Try again\n", detailFile);

    printf(" \n");
}