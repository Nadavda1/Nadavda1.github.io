#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Customer.h"
#include "General.h"

int	initCustomer(Customer* pCustomer)
{

	do {
		pCustomer->name = getStrExactLength("Enter customer name\n");
		if (!pCustomer->name)
			return 0;
	} while (checkEmptyString(pCustomer->name));

	pCustomer->shopTimes = 0;
	pCustomer->totalSpend = 0;
	pCustomer->pCart = NULL;
	return 1;
}

void	printCustomer(const Customer* pCustomer)
{
	printf("Name: %s, number of purchases: %d, total spend: %.2f ", pCustomer->name,pCustomer->shopTimes,pCustomer->totalSpend);

	if (pCustomer->pCart == NULL)
		printf("Shopping cart is empty!\n");
	else
	{
		printf("Doing shopping now!!!\n");
		//printShoppingCart(pCustomer->pCart);
	}
}


void	pay(Customer* pCustomer)
{
	if (!pCustomer->pCart)
		return;
	printf("---------- Cart info and bill for %s ----------\n", pCustomer->name);
	printShoppingCart(pCustomer->pCart);
	printf("!!! --- Payment was recived!!!! --- \n");
	pCustomer->shopTimes++;
	pCustomer->totalSpend += getTotalPrice(pCustomer->pCart);
	freeShoppingCart(pCustomer->pCart);
	free(pCustomer->pCart);
	pCustomer->pCart = NULL;
}




int isCustomer(const Customer* pCust, const char* name)
{
	if (strcmp(pCust->name, name) == 0)
		return 1;
	return 0;
}

void	freeCustomer(Customer* pCust)
{
	if (pCust->pCart)
		pay(pCust); //will free every thing
	free(pCust->name);
}

int compareCustByName(const void* c1, const void* c2)
{
	const Customer* pCust1 = (Customer*)c1;
	const Customer* pCust2 = (Customer*)c2;
	return strcmp(pCust1->name, pCust2->name);
}
int compareCustByShopTime(const void* c1, const void* c2)
{
	const Customer* pCust1 = (Customer*)c1;
	const Customer* pCust2 = (Customer*)c2;
	return (pCust1->shopTimes - pCust2->shopTimes);
}
int compareCustByTotalSpend(const void* c1, const void* c2)
{
	const Customer* pCust1 = (Customer*)c1;
	const Customer* pCust2 = (Customer*)c2;
	return ((int)pCust1->totalSpend - (int)pCust2->totalSpend);
}

eSortType getSortType()
{
	int option;
	printf("\n\n");
	do {
		printf("Please enter one of the following types\n");
		for (int i = 0; i < eNofSortOption; i++)
			printf("%d for %s\n", i, typeString[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofSortOption);
	getchar();
	return (eSortType)option;
}

int writeCustomerToFile(const Customer* pCust, FILE* fp)
{
	if (!pCust)
		return 0;
	fprintf(fp, "%s\n", pCust->name);
	fprintf(fp, "%d ", pCust->shopTimes);
	fprintf(fp, "%.2f\n", pCust->totalSpend);

	return 1;
}
int readCustomerToFile(Customer* pCust, FILE* fp)
{
		char tmp[255];
	if (fscanf(fp, "%s %d %f", tmp, &pCust->shopTimes, &pCust->totalSpend) != 3)
		return 0;
	pCust->name = _strdup(tmp);
	if (!pCust->name)
		return 0;
	pCust->pCart = NULL;
	return 1;
}
