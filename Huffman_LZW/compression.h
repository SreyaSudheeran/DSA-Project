typedef struct comp_DictNode {
    int value; 
    int prefix; 
    int character; 
    struct comp_DictNode *next;
}comp_DictNode;

extern struct comp_DictNode *dictionary, *tail;

void Dinit();
void appendNode(struct comp_DictNode *node);
void destroy();
int lookup(int prefix, int character);
void Dict_Add(int prefix, int character, int value);
void writeBinary(int fd2, int code);
void compress(int fd1, int fd2);
#define dictionarySize 4095


