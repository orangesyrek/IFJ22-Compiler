#include<stdio.h>
#include<stdlib.h>

#define FALSE 0
#define TRUE 1

/** Globální proměnná - příznak ošetření chyby. */
extern int error_flag;
/** Globální proměnná - indikuje, zda byla operace řešena. */
extern int solved;

/** Prvek dvousměrně vázaného seznamu. */
typedef struct DLLElement {
	/** Užitečná data. */
	char data[256];
	/** Ukazatel na předcházející prvek seznamu. */
	struct DLLElement *previousElement;
	/** Ukazatel na následující prvek seznamu. */
	struct DLLElement *nextElement;
} *DLLElementPtr;

/** Dvousměrně vázaný seznam. */
typedef struct {
	/** Ukazatel na první prvek seznamu. */
	DLLElementPtr firstElement;
	/** Ukazatel na aktuální prvek seznamu. */
	DLLElementPtr activeElement;
	/** Ukazatel na posledni prvek seznamu. */
	DLLElementPtr lastElement;
} DLList;

void DLL_Init( DLList * );

void DLL_Dispose( DLList * );

void DLL_InsertFirst( DLList *, char );

void DLL_InsertLast( DLList *, char );

void DLL_First( DLList * );

void DLL_Last( DLList * );

void DLL_GetFirst( DLList *, char * );

void DLL_GetLast( DLList *, char * );

void DLL_DeleteFirst( DLList * );

void DLL_DeleteLast( DLList * );

void DLL_DeleteAfter( DLList * );

void DLL_DeleteBefore( DLList * );

void DLL_InsertAfter( DLList *, char );

void DLL_InsertBefore( DLList *, char );

void DLL_GetValue( DLList *, char * );

void DLL_SetValue( DLList *, char );

void DLL_Next( DLList * );

void DLL_Previous( DLList * );

int DLL_IsActive( DLList * );
