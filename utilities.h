#ifndef _TREE
#define _TREE

// Tree data structure

typedef unsigned char byte;
typedef struct LinkedList_NodeChain LinkedList_NodeChain;
typedef struct FileData FileData;
typedef struct Node Node;
void InsertNode(Node* parent, Node* node);
Node* CreateNode(FileData* data, Node* parent, char name[256]);
FileData* CreateFileData(int Type, byte Data);
int NodeCount(Node* node);
void ShowTree(Node* node, int level);
void LookAtChildrenOf(Node* node);
char* GetName(Node* node);
Node* GetParent(Node* node);
LinkedList_NodeChain* GetFirstChild(Node* node);
char* GetNameLLNC(LinkedList_NodeChain* llnc);
Node* GetNodeValue(LinkedList_NodeChain* chain);
LinkedList_NodeChain* GetNext(LinkedList_NodeChain* llnc);

int StartsWith(const char *pre, const char *str);

#endif