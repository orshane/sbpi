#ifndef CLI_H
#define CLI_H

int cli_install(const char *package);
int cli_remove(const char *package);
int cli_list(void);
int cli_sync(void);
int cli_update(const char *package);

#endif
