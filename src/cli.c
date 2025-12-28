#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port.h"

#define SBPI_REPO_URL "https://github.com/orshane/SbpRepo.git"

int cli_install(const char *package) {
    if (!package) {
        printf("Error: No port specified\n");
        return 1;
    }

    char fetch[4096], build[4096], install[4096], uninstall[4096];
    
    printf("Loading port: %s\n", package);
    
    // Pass the package name directly (e.g., "editors/micro")
    if (load_port(package, fetch, build, install, uninstall, sizeof(fetch)) != 0) {
        printf("Failed to load port: %s\n", package);
        printf("Make sure you've run 'sbpi sync' to download port definitions.\n");
        return 1;
    }

    printf("\n=== Installing %s ===\n", package);
    printf("Fetch command: %s\n", fetch);
    printf("Build command: %s\n", build);
    printf("Install command: %s\n", install);
    
    printf("\n1. Fetching...\n");
    if (system(fetch) != 0) {
        printf("Failed to fetch port: %s\n", package);
        return 1;
    }

    printf("\n2. Building...\n");
    if (system(build) != 0) {
        printf("Failed to build port: %s\n", package);
        return 1;
    }

    printf("\n3. Installing...\n");
    if (system(install) != 0) {
        printf("Failed to install port: %s\n", package);
        return 1;
    }

    printf("\n%s installed successfully!\n", package);
    return 0;
}

int cli_remove(const char *package) {
    if (!package) {
        printf("Error: No port specified\n");
        return 1;
    }

    char fetch[4096], build[4096], install[4096], uninstall[4096];
    
    printf("Loading port: %s\n", package);
    
    // Pass package name directly
    if (load_port(package, fetch, build, install, uninstall, sizeof(fetch)) != 0) {
        printf("Failed to load port: %s\n", package);
        printf("Make sure the port is installed or repository is synced.\n");
        return 1;
    }

    printf("\n=== Removing %s ===\n", package);
    printf("Uninstall command: %s\n", uninstall);
    
    printf("\nUninstalling...\n");
    if (system(uninstall) != 0) {
        printf("Failed to uninstall port: %s\n", package);
        return 1;
    }

    printf("\n%s removed successfully!\n", package);
    return 0;
}

int cli_update(const char *package) {
    if (!package) {
        printf("Error: No port specified\n");
        return 1;
    }

    char fetch[4096], build[4096], install[4096], uninstall[4096];
    
    printf("Loading port: %s\n", package);
    
    // First load the port to get commands
    if (load_port(package, fetch, build, install, uninstall, sizeof(fetch)) != 0) {
        printf("Failed to load port: %s\n", package);
        printf("Make sure you've run 'sbpi sync' to download port definitions.\n");
        return 1;
    }

    printf("\n=== Updating %s ===\n", package);
    printf("Fetch command: %s\n", fetch);
    printf("Build command: %s\n", build);
    printf("Install command: %s\n", install);
    
    printf("\n1. Fetching (update)...\n");
    if (system(fetch) != 0) {
        printf("Failed to fetch port update: %s\n", package);
        return 1;
    }

    printf("\n2. Building (update)...\n");
    if (system(build) != 0) {
        printf("Failed to build port update: %s\n", package);
        return 1;
    }

    printf("\n3. Installing (update)...\n");
    if (system(install) != 0) {
        printf("Failed to install port update: %s\n", package);
        return 1;
    }

    printf("\n%s updated successfully!\n", package);
    return 0;
}

int cli_list(void) {
    return list_installed();
}

int cli_sync(void) {
    printf("Syncing repository...\n");
    if (sync_repo(SBPI_REPO_URL) != 0) {
        printf("Failed to sync repository.\n");
        return 1;
    }
    printf("Sync complete.\n");
    return 0;
}
