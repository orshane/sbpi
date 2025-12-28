#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"

void print_help() {
    printf("SBPI - Source Based Package Installer\n");
    printf("Usage: sbpi <command> [package]\n\n");
    printf("Commands:\n");
    printf("  sync                    - Sync port definitions from repository\n");
    printf("  list                    - List available ports\n");
    printf("  install <port>          - Install a port (e.g., editors/micro)\n");
    printf("  remove <port>           - Remove an installed port\n");
    printf("  update <port>           - Update a port\n");
    printf("  help                    - Show this help message\n\n");
    printf("Repository: https://github.com/orshane/SbpRepo\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }

    const char *command = argv[1];
    const char *package = (argc >= 3) ? argv[2] : NULL;

    if (strcmp(command, "install") == 0) {
        if (!package) { 
            printf("Specify a package to install.\n"); 
            printf("Example: sbpi install editors/micro\n");
            return 1; 
        }
        return cli_install(package);
    } else if (strcmp(command, "remove") == 0) {
        if (!package) { 
            printf("Specify a package to remove.\n"); 
            return 1; 
        }
        return cli_remove(package);
    } else if (strcmp(command, "list") == 0) {
        return cli_list();
    } else if (strcmp(command, "sync") == 0) {
        return cli_sync();
    } else if (strcmp(command, "update") == 0) {
        if (!package) { 
            printf("Specify a package to update.\n"); 
            return 1; 
        }
        return cli_update(package);
    } else if (strcmp(command, "help") == 0 || strcmp(command, "--help") == 0 || strcmp(command, "-h") == 0) {
        print_help();
        return 0;
    } else {
        printf("Unknown command: %s\n", command);
        printf("Use 'sbpi help' for usage information.\n");
        return 1;
    }

    return 0;
}
