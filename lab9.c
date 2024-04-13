//Alexander Moreno
//Lab Assignment 9
//4-12-2024

#include <stdio.h>
#include <stdlib.h>

// RecordType.
struct RecordType
{
    int id;
    char name;
    int order;
    struct RecordType* next; //pointer for separate chaining.
};

// HashType
#define HASH_SIZE 37 //adjust the hash size based on your needs.

struct HashType
{
    struct RecordType* hashArray[HASH_SIZE];
};

// Compute the hash function
int hash(int x)
{
    return x % HASH_SIZE; //hashing.
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    if (inFile == NULL)
    {
        printf("Error opening file: %s\n", inputFileName);
        exit(EXIT_FAILURE);
    }

    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        //implement parse data block.
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
            pRecord->next = NULL; //initialize next pointer.
        }

        fclose(inFile);
    }

    return dataSz;
}

// Insert a record into the hash table (maintaining sorted order within each index)
void insertRecord(struct HashType* pHashArray, struct RecordType* pRecord)
{
    int index = hash(pRecord->id);
    struct RecordType* current = pHashArray->hashArray[index];
    struct RecordType* prev = NULL;

    //find the correct position to insert the record.
    while (current != NULL && current->id < pRecord->id)
    {
        prev = current;
        current = current->next;
    }

    //insert the record.
    if (prev == NULL)
    {
        pRecord->next = pHashArray->hashArray[index];
        pHashArray->hashArray[index] = pRecord;
    }
    else
    {
        pRecord->next = prev->next;
        prev->next = pRecord;
    }
}

// Display records in the hash structure
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    printf("\n");

    for (int i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i); //print index.

        struct RecordType* currentRecord = pHashArray->hashArray[i];

        while (currentRecord != NULL)
        {
            printf("%d, %c, %d", currentRecord->id, currentRecord->name, currentRecord->order);
            currentRecord = currentRecord->next;

            if (currentRecord != NULL)
            {
                printf(" -> ");
            }
        }

        printf("\n");
    }
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

int main(void)
{
    struct RecordType* pRecords;
    struct HashType hashTable = {{NULL}}; //initialize hashArray to NULL.
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printf("Number of records read: %d\n", recordSz);
    if (recordSz > 0) 
	{
        printRecords(pRecords, recordSz);

        //insert records into the hash table.
        for (int i = 0; i < recordSz; ++i)
        {
            insertRecord(&hashTable, &pRecords[i]);
        }

        printf("Hash Table after insertion:\n");
        displayRecordsInHash(&hashTable, HASH_SIZE);
    } 
	else 
	{
        printf("No records read.\n");
    }

    return 0;
}
