#define BUFFERSIZE 256
int num_digits(int check);
bool isnumber(char* target);
int ChunkWrite(int sock, char* const c_command);
int ChunkRead(int sock,char** arrived_command);
void perror_exit(const char* errormessage);
void perror2_exit(const char* errormessage, int err);