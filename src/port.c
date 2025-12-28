#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#define SBPI_DIR "/usr/local/var/sbpi"
#define REPO_DIR SBPI_DIR "/repo"
#define PORTS_DIR SBPI_DIR "/ports"
#define WORK_DIR SBPI_DIR "/work"
#define DISTFILES_DIR SBPI_DIR "/distfiles"

// Function declarations
static void ensure_dir(const char *path);
static int read_sbp_commands(const char *port_path, char *fetch, char *build, char *install, char *uninstall, size_t size);
static char* find_port_in_repo(const char *port_name);

static void ensure_dir(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0755);
    }
}

static int read_sbp_commands(const char *port_path, char *fetch, char *build, char *install, char *uninstall, size_t size) {
    char sbp_file[4096];
    snprintf(sbp_file, sizeof(sbp_file), "%s/Sbp", port_path);
    
    printf("Loading port from: %s\n", sbp_file);
    
    FILE *f = fopen(sbp_file, "r");
    if (!f) {
        printf("Cannot open Sbp file: %s\n", sbp_file);
        return -1;
    }

    // Initialize all commands
    fetch[0] = build[0] = install[0] = uninstall[0] = '\0';
    
    char line[4096];
    while (fgets(line, sizeof(line), f)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        if (strncmp(line, "FETCH_CMD=", 10) == 0) {
            strncpy(fetch, line + 10, size - 1);
            fetch[size - 1] = '\0';
        } else if (strncmp(line, "BUILD_CMD=", 10) == 0) {
            strncpy(build, line + 10, size - 1);
            build[size - 1] = '\0';
        } else if (strncmp(line, "INSTALL_CMD=", 12) == 0) {
            strncpy(install, line + 12, size - 1);
            install[size - 1] = '\0';
        } else if (strncmp(line, "UNINSTALL_CMD=", 14) == 0) {
            strncpy(uninstall, line + 14, size - 1);
            uninstall[size - 1] = '\0';
        }
    }
    
    fclose(f);
    
    // Check if we found all required commands
    if (fetch[0] == '\0' || build[0] == '\0' || install[0] == '\0') {
        printf("Missing required commands in Sbp file\n");
        return -1;
    }
    
    return 0;
}

static char* find_port_in_repo(const char *port_name) {
    static char port_path[4096];
    struct stat st;
    
    // 1. First check if port_name is a direct path in the repo
    snprintf(port_path, sizeof(port_path), "%s/repo/%s", REPO_DIR, port_name);
    
    if (stat(port_path, &st) == 0 && S_ISDIR(st.st_mode)) {
        // Check if Sbp file exists in this directory
        char sbp_file[4096];
        snprintf(sbp_file, sizeof(sbp_file), "%s/Sbp", port_path);
        if (stat(sbp_file, &st) == 0) {
            return port_path;
        }
    }
    
    // 2. Search for the port in the repository
    char find_cmd[4096];
    snprintf(find_cmd, sizeof(find_cmd), 
        "find \"%s/repo\" -type f -name \"Sbp\" 2>/dev/null | xargs grep -l \"%s\" 2>/dev/null | head -1", 
        REPO_DIR, port_name);
    
    FILE *fp = popen(find_cmd, "r");
    if (fp) {
        if (fgets(port_path, sizeof(port_path), fp)) {
            // Remove newline
            port_path[strcspn(port_path, "\n")] = 0;
            // Get directory path (remove "/Sbp" from end)
            char *sbp_pos = strstr(port_path, "/Sbp");
            if (sbp_pos) {
                *sbp_pos = '\0';
            }
            pclose(fp);
            return port_path;
        }
        pclose(fp);
    }
    
    // 3. Try to find by directory name
    snprintf(find_cmd, sizeof(find_cmd),
        "find \"%s/repo\" -type d -name \"*%s*\" 2>/dev/null | head -1",
        REPO_DIR, port_name);
    
    fp = popen(find_cmd, "r");
    if (fp) {
        if (fgets(port_path, sizeof(port_path), fp)) {
            port_path[strcspn(port_path, "\n")] = 0;
            pclose(fp);
            
            // Check if Sbp file exists
            char sbp_file[4096];
            snprintf(sbp_file, sizeof(sbp_file), "%s/Sbp", port_path);
            if (stat(sbp_file, &st) == 0) {
                return port_path;
            }
        }
        pclose(fp);
    }
    
    return NULL;
}

int load_port(const char *port_name, char *fetch, char *build, char *install, char *uninstall, size_t size) {
    // Check if the repository exists
    char repo_path[4096];
    snprintf(repo_path, sizeof(repo_path), "%s/repo", REPO_DIR);
    
    struct stat st;
    if (stat(repo_path, &st) == -1) {
        printf("Repository not found. Run 'sbpi sync' first.\n");
        return -1;
    }
    
    printf("Looking for port: %s\n", port_name);
    
    // Find the port in the repository
    char *port_path = find_port_in_repo(port_name);
    if (!port_path) {
        printf("Port '%s' not found in repository.\n", port_name);
        
        // List available ports
        printf("\nAvailable ports:\n");
        char list_cmd[4096];
        snprintf(list_cmd, sizeof(list_cmd), 
            "find \"%s/repo\" -type f -name \"Sbp\" 2>/dev/null | "
            "sed 's|.*/repo/||' | sed 's|/Sbp||' | sort", 
            REPO_DIR);
        system(list_cmd);
        
        return -1;
    }
    
    printf("Found port at: %s\n", port_path);
    return read_sbp_commands(port_path, fetch, build, install, uninstall, size);
}

int fetch_port(const char *port_name) {
    char fetch[4096], build[4096], install[4096], uninstall[4096];
    char *port_path = find_port_in_repo(port_name);
    
    if (!port_path) {
        printf("Port '%s' not found\n", port_name);
        return -1;
    }
    
    if (read_sbp_commands(port_path, fetch, build, install, uninstall, sizeof(fetch)) != 0) {
        return -1;
    }
    
    printf("Fetching: %s\n", fetch);
    return system(fetch);
}

int build_port(const char *port_name) {
    char fetch[4096], build[4096], install[4096], uninstall[4096];
    char *port_path = find_port_in_repo(port_name);
    
    if (!port_path) {
        printf("Port '%s' not found\n", port_name);
        return -1;
    }
    
    if (read_sbp_commands(port_path, fetch, build, install, uninstall, sizeof(fetch)) != 0) {
        return -1;
    }
    
    printf("Building: %s\n", build);
    return system(build);
}

int install_port(const char *port_name) {
    char fetch[4096], build[4096], install[4096], uninstall[4096];
    char *port_path = find_port_in_repo(port_name);
    
    if (!port_path) {
        printf("Port '%s' not found\n", port_name);
        return -1;
    }
    
    if (read_sbp_commands(port_path, fetch, build, install, uninstall, sizeof(fetch)) != 0) {
        return -1;
    }
    
    printf("Installing: %s\n", install);
    return system(install);
}

int uninstall_port(const char *port_name) {
    char fetch[4096], build[4096], install[4096], uninstall[4096];
    char *port_path = find_port_in_repo(port_name);
    
    if (!port_path) {
        printf("Port '%s' not found\n", port_name);
        return -1;
    }
    
    if (read_sbp_commands(port_path, fetch, build, install, uninstall, sizeof(fetch)) != 0) {
        return -1;
    }
    
    printf("Uninstalling: %s\n", uninstall);
    return system(uninstall);
}

int update_port(const char *port_name) {
    printf("Updating port: %s\n", port_name);
    
    // Update is fetch + build + install
    if (fetch_port(port_name) != 0) {
        printf("Failed to fetch port: %s\n", port_name);
        return -1;
    }
    
    if (build_port(port_name) != 0) {
        printf("Failed to build port: %s\n", port_name);
        return -1;
    }
    
    if (install_port(port_name) != 0) {
        printf("Failed to install port: %s\n", port_name);
        return -1;
    }
    
    return 0;
}

int sync_repo(const char *url) {
    ensure_dir(SBPI_DIR);
    ensure_dir(REPO_DIR);
    
    char cmd[4096];
    char repo_path[4096];
    snprintf(repo_path, sizeof(repo_path), "%s/repo", REPO_DIR);
    
    printf("Syncing repository: %s\n", url);
    
    struct stat st;
    if (stat(repo_path, &st) == 0 && S_ISDIR(st.st_mode)) {
        // Try to update with --allow-unrelated-histories
        printf("Updating repository...\n");
        snprintf(cmd, sizeof(cmd), 
            "cd \"%s\" && git pull origin main --allow-unrelated-histories --rebase 2>&1", 
            repo_path);
    } else {
        // Clone fresh
        printf("Cloning repository...\n");
        snprintf(cmd, sizeof(cmd), 
            "git clone --quiet --depth 1 \"%s\" \"%s\" 2>&1", 
            url, repo_path);
    }
    
    // Execute the command
    FILE *fp = popen(cmd, "r");
    if (fp) {
        char output[4096];
        int success = 1;
        while (fgets(output, sizeof(output), fp)) {
            printf("%s", output);
            // Check for specific errors
            if (strstr(output, "fatal:")) {
                success = 0;
            }
        }
        int result = pclose(fp);
        
        if (result == 0 && success) {
            printf("Repository synced successfully.\n");
            return 0;
        } else {
            // If update failed, try clean clone
            printf("Update failed, trying clean clone...\n");
            snprintf(cmd, sizeof(cmd), "rm -rf \"%s\"", repo_path);
            system(cmd);
            
            snprintf(cmd, sizeof(cmd), 
                "git clone --quiet --depth 1 \"%s\" \"%s\" 2>&1", 
                url, repo_path);
                
            result = system(cmd);
            if (result == 0) {
                printf("Clean clone successful.\n");
                return 0;
            }
            return -1;
        }
    }
    
    return -1;
}int list_installed() {
    printf("Installed ports:\n");
    
    // Check if ports directory exists
    struct stat st;
    if (stat(PORTS_DIR, &st) == -1) {
        printf("No ports installed yet.\n");
        return 0;
    }
    
    char cmd[4096];
    snprintf(cmd, sizeof(cmd), "find \"%s\" -type f -name \"Sbp\" 2>/dev/null", PORTS_DIR);
    
    FILE *fp = popen(cmd, "r");
    if (fp) {
        char line[4096];
        int count = 0;
        while (fgets(line, sizeof(line), fp)) {
            // Extract port name from path
            line[strcspn(line, "\n")] = 0;
            char *port_name = strrchr(line, '/');
            if (port_name) {
                printf("  %s\n", port_name + 1);
                count++;
            }
        }
        pclose(fp);
        
        if (count == 0) {
            printf("No ports installed.\n");
        } else {
            printf("Total: %d ports installed.\n", count);
        }
    }
    
    return 0;
}

int list_available() {
    char repo_path[4096];
    snprintf(repo_path, sizeof(repo_path), "%s/repo", REPO_DIR);
    
    struct stat st;
    if (stat(repo_path, &st) == -1) {
        printf("Repository not synced. Run 'sbpi sync' first.\n");
        return -1;
    }
    
    printf("Available ports in repository:\n");
    
    char cmd[4096];
    snprintf(cmd, sizeof(cmd), 
        "find \"%s\" -type f -name \"Sbp\" 2>/dev/null | "
        "sed 's|.*/repo/||' | sed 's|/Sbp||' | sort | "
        "while read port; do echo \"  $port\"; done", 
        REPO_DIR);
    
    FILE *fp = popen(cmd, "r");
    if (fp) {
        char line[4096];
        int count = 0;
        while (fgets(line, sizeof(line), fp)) {
            printf("%s", line);
            count++;
        }
        pclose(fp);
        
        if (count == 0) {
            printf("  No ports found in repository.\n");
        } else {
            printf("\nTotal: %d ports available.\n", count);
        }
    }
    
    return 0;
}
