#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#define ALPHABET_SIZE 26
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')
// trie node
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];

    // isWordEnd is true if the node represents
    // end of a word
    bool isWordEnd;
};

struct TrieNode *getNode()
{
    struct TrieNode *pNode=(struct TrieNode*)malloc(sizeof(struct TrieNode));
    pNode->isWordEnd = false;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        pNode->children[i] = NULL;

    return pNode;
}

// If not present, inserts key into trie.  If the
// key is prefix of trie node, just marks leaf node
void insert(struct TrieNode *root,  char* key)
{
    struct TrieNode *pCrawl = root;

    for (int level = 0; level < strlen(key); level++)
    {
        int index = CHAR_TO_INDEX(key[level]);
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
    }

    // mark last node as leaf
    pCrawl->isWordEnd = true;
}

// Returns true if key presents in trie, else false
bool search(struct TrieNode *root, char* key)
{
    int length = strlen(key);
    struct TrieNode *pCrawl = root;
    for (int level = 0; level < length; level++)
    {
        int index = CHAR_TO_INDEX(key[level]);

        if (!pCrawl->children[index])
            return false;

        pCrawl = pCrawl->children[index];
    }

    return (pCrawl != NULL && pCrawl->isWordEnd);
}

// Returns 0 if current node has a child
// If all children are NULL, return 1.
bool isLastNode(struct TrieNode* root)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i])
            return 0;
    return 1;
}

// Recursive function to print auto-suggestions for given
// node.
void suggestionsRec(struct TrieNode* root, char* currPrefix)
{
    // found a string in Trie with the given prefix
    if (root->isWordEnd)
    {
        printf("%s\n",currPrefix);
    }

    // All children struct node pointers are NULL
    if (isLastNode(root))
        return;

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (root->children[i])
        { char a[2]; char j[100];
            // append current character to currPrefix string
	//	for(j=0; j<strlen(currPrefix); j++)
	//		a[j]=currPrefix[j];
       		strcpy(j, currPrefix);
		a[0]='a'+ i;
		a[1]='\0';
		strcat(j,a);
		// currPrefix.push_back(97 + i);

            // recur over the rest
            suggestionsRec(root->children[i], j);
        }
    }
}

// print suggestions for given query prefix.
int printAutoSuggestions(struct TrieNode* root, char* query)
{
    struct TrieNode* pCrawl = root;

    // Check if prefix is present and find the
    // the node (of last level) with last character
    // of given string.
    int level;
    int n = strlen(query);
    for (level = 0; level < n; level++)
    {
        int index = CHAR_TO_INDEX(query[level]);

        // no string in the Trie has this prefix
        if (!pCrawl->children[index])
            return 0;

        pCrawl = pCrawl->children[index];
    }

    // If prefix is present as a word.
    bool isWord = (pCrawl->isWordEnd == true);

    // If prefix is last node of tree (has no
    // children)
    bool isLast = isLastNode(pCrawl);

    // If prefix is present as a word, but
    // there is no subtree below the last
    // matching node.
    if (isWord && isLast)
    {
        printf("%s\n",query);
        return -1;
    }

   // If there are are nodes below last
    // matching character.
    if (!isLast)
    {
        char* prefix = query;
        suggestionsRec(pCrawl, prefix);
        return 1;
    }
}

int main()
{
    int swxx;
    struct TrieNode* root = getNode();
    char *str[7000];
    char *ptr;
    char *words[7000];
    char s[100];
    FILE * fp = fopen("newyes.txt", "r");
    fgets(str, 7000, fp);             // read characters
    ptr = strtok(str, "\n\r\n ");         // split our findings around the " "
    int i = 0;
    while(ptr != NULL)  // while there's more to the string
    {
        words[i]= ptr;
        i++;
        ptr = strtok(NULL, "\n\r\n "); // and keep splitting
    }
    fclose(fp);
    for(swxx=0;swxx<210;swxx++)
        insert(root,words[swxx]);
    printf("Enter string for suggestion: ");
    scanf("%s",s);
    printf("The suggestions are:\n");
    int comp = printAutoSuggestions(root, s);

    if (comp == -1)
        printf("No other strings found with this prefix\n");

    else if (comp == 0)
        printf("No string found with this prefix\n");

    return 0;
}
