//      DLL_Init ........... inicializace seznamu před prvním použitím
//      DLL_Dispose ........ zrušení všech prvků seznamu
//      DLL_InsertFirst .... vložení prvku na začátek seznamu
//      DLL_InsertLast ..... vložení prvku na konec seznamu
//      DLL_First .......... nastavení aktivity na první prvek
//      DLL_Last ........... nastavení aktivity na poslední prvek
//      DLL_GetFirst ....... vrací hodnotu prvního prvku
//      DLL_GetLast ........ vrací hodnotu posledního prvku
//      DLL_DeleteFirst .... zruší první prvek seznamu
//      DLL_DeleteLast ..... zruší poslední prvek seznamu
//      DLL_DeleteAfter .... ruší prvek za aktivním prvkem
//      DLL_DeleteBefore ... ruší prvek před aktivním prvkem
//      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu
//      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu
//      DLL_GetValue ....... vrací hodnotu aktivního prvku
//      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou
//      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu
//      DLL_Next ........... posune aktivitu na další prvek seznamu
//      DLL_IsActive ....... zjišťuje aktivitu seznamu
#include "dll.h"

int error_flag;
int solved;

void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}
//DLL_Init ........... inicializace seznamu před prvním použitím
void DLL_Init( DLList *list ) {
    list->firstElement = NULL;
    list->activeElement = NULL;
    list->lastElement = NULL;
}
//DLL_Dispose ........ zrušení všech prvků seznamu
void DLL_Dispose( DLList *list ) {
    DLLElementPtr tmp, iter;

    iter = list->firstElement;
    while (iter) {
        tmp = iter->nextElement;
        free(iter);
        iter = tmp;
    }

    DLL_Init(list);
}
//DLL_InsertFirst .... vložení prvku na začátek seznamu
void DLL_InsertFirst( DLList *list, char data ) {
    DLLElementPtr new;

    new = calloc(1, sizeof *new);
    if (!new) {
        DLL_Error();
        return;
    }

    if (!list->firstElement) {
        list->lastElement = new;
    } else {
        list->firstElement->previousElement = new;
        new->nextElement = list->firstElement;
    }

    new->data = data;
    list->firstElement = new;
}
//DLL_InsertLast ..... vložení prvku na konec seznamu
void DLL_InsertLast( DLList *list, char data ) {
    DLLElementPtr new;

    new = calloc(1, sizeof *new);
    if (!new) {
        DLL_Error();
        return;
    }

    if (!list->lastElement) {
        /* list is empty */
        list->firstElement = new;
    } else {
        list->lastElement->nextElement = new;
        new->previousElement = list->lastElement;
    }

    new->data = data;
    list->lastElement = new;
}
//DLL_First .......... nastavení aktivity na první prvek
void DLL_First( DLList *list ) {
    list->activeElement = list->firstElement;
}
//DLL_Last ........... nastavení aktivity na poslední prvek
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}
//DLL_GetFirst ....... vrací hodnotu prvního prvku
void DLL_GetFirst( DLList *list, char *dataPtr ) {
	if (!list->firstElement) {
        DLL_Error();
        return;
    }

    *dataPtr = list->firstElement->data;
}
//DLL_GetLast ........ vrací hodnotu posledního prvku
void DLL_GetLast( DLList *list, char *dataPtr ) {
	if (!list->lastElement) {
        DLL_Error();
        return;
    }

    *dataPtr = list->lastElement->data;
}
//DLL_DeleteFirst .... zruší první prvek seznamu
void DLL_DeleteFirst( DLList *list ) {
	DLLElementPtr tmp;

    if (!list->firstElement) {
        /* empty list */
        return;
    }

    if (list->firstElement == list->activeElement) {
        /* deactivate the list */
        list->activeElement = NULL;
    }

    tmp = list->firstElement;
    if (list->firstElement == list->lastElement) {
        /* it is the only element in the list */
        list->firstElement = NULL;
        list->lastElement = NULL;
    } else {
        list->firstElement = list->firstElement->nextElement;
        list->firstElement->previousElement = NULL;
    }
    free(tmp);
}
//DLL_DeleteLast ..... zruší poslední prvek seznamu
void DLL_DeleteLast( DLList *list ) {
    DLLElementPtr tmp;

    if (!list->lastElement) {
        return;
    }

    if (list->lastElement == list->activeElement) {
        list->activeElement = NULL;
    }

    tmp = list->lastElement;
    if (list->firstElement == list->lastElement) {
        list->firstElement = NULL;
        list->lastElement = NULL;
    } else {
        list->lastElement = list->lastElement->previousElement;
        list->lastElement->nextElement = NULL;
    }
    free(tmp);
}
//DLL_DeleteAfter .... ruší prvek za aktivním prvkem
void DLL_DeleteAfter( DLList *list ) {
    DLLElementPtr tmp;

    if ((!list->activeElement) || (list->activeElement == list->lastElement)) {
        /* list is either not active or the active element is the last */
        return;
    }

    tmp = list->activeElement->nextElement->nextElement;
    free(list->activeElement->nextElement);

    list->activeElement->nextElement = tmp;
    if (tmp) {
        tmp->previousElement = list->activeElement;
    } else {
        list->lastElement = list->activeElement;
    }
}
//DLL_DeleteBefore ... ruší prvek před aktivním prvkem
void DLL_DeleteBefore( DLList *list ) {
    DLLElementPtr tmp;

    if ((!list->activeElement) || (list->activeElement == list->firstElement)) {
        /* list is either not active or the active element is the first */
        return;
    }

    tmp = list->activeElement->previousElement->previousElement;
    free(list->activeElement->previousElement);

    list->activeElement->previousElement = tmp;
    if (tmp) {
        tmp->nextElement = list->activeElement;
    } else {
        list->firstElement = list->activeElement;
    }
}
//DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu
void DLL_InsertAfter( DLList *list, char data ) {
    DLLElementPtr new;

    if (!list->activeElement) {
        return;
    }

    new = malloc(sizeof *new);
    if (!new) {
        DLL_Error();
        return;
    }

    new->data = data;

    if (list->activeElement == list->lastElement) {
        list->lastElement = new;
        new->nextElement = NULL;
    } else {
        new->nextElement = list->activeElement->nextElement;
        list->activeElement->nextElement->previousElement = new;
    }

    new->previousElement = list->activeElement;
    list->activeElement->nextElement = new;
}
//DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu
void DLL_InsertBefore( DLList *list, char data ) {
	DLLElementPtr new;

    if (!list->activeElement) {
        return;
    }

    new = malloc(sizeof *new);
    if (!new) {
        DLL_Error();
        return;
    }

    new->data = data;

    if (list->activeElement == list->firstElement) {
        list->firstElement = new;
        new->previousElement = NULL;
    } else {
        new->previousElement = list->activeElement->previousElement;
        list->activeElement->previousElement->nextElement = new;
    }

    new->nextElement = list->activeElement;
    list->activeElement->previousElement = new;
}
//DLL_GetValue ....... vrací hodnotu aktivního prvku
void DLL_GetValue( DLList *list, char *dataPtr ) {
    if (!list->activeElement) {
        DLL_Error();
        return;
    }

    *dataPtr = list->activeElement->data;
}
//DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou
void DLL_SetValue( DLList *list, char data ) {
    if (!list->activeElement) {
        return;
    }

    list->activeElement->data = data;
}
//DLL_Next ........... posune aktivitu na další prvek seznamu
void DLL_Next( DLList *list ) {
    if (!list->activeElement) {
        return;
    }

    list->activeElement = list->activeElement->nextElement;
}
//DLL_Previous ....... posune aktivitu na předchozí prvek seznamu
void DLL_Previous( DLList *list ) {
	if (!list->activeElement) {
        return;
    }

    list->activeElement = list->activeElement->previousElement;
}
//DLL_IsActive ....... zjišťuje aktivitu seznamu
int DLL_IsActive( DLList *list ) {
	return list->activeElement ? 1 : 0;
}

