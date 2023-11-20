#define MAX_HISTORY 16  // max length of history buffer

void history_init(void);
void saveToHistory(char* command);
int history(int historyId);