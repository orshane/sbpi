#ifndef PORT_H
#define PORT_H

// Load the commands from a port's Sbp file
int load_port(const char *port_path, char *fetch, char *build, char *install, char *uninstall, size_t size);

// Fetch, build, install, uninstall a port
int fetch_port(const char *port_path);
int build_port(const char *port_path);
int install_port(const char *port_path);
int uninstall_port(const char *port_path);

// Update a port (fetch + build + install)
int update_port(const char *port_path);

// List installed ports
int list_installed();

// Sync the repository from a URL
int sync_repo(const char *url);

#endif

