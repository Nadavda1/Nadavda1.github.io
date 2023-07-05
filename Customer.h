#pragma once
#include "ShoppingCart.h"

typedef enum { eNotSort,eSortByName, eSortByShopTime, eSortByTotalSpend, eNofSortOption } eSortType;
static const char* typeString[eNofSortOption] = { "Leave unsorted", "Sort by name", "Sort by ShopTime", "Sort by total spend" };


typedef struct
{
	char*			name;
	int shopTimes;
	float totalSpend;
	ShoppingCart*	pCart;
}Customer;

int		initCustomer(Customer* pCustomer);
void	printCustomer(const Customer* pCustomer);
int		isCustomer(const Customer* pCust, const char* name);
void	pay(Customer* pCustomer);
void	freeCustomer(Customer* pCust);
int writeCustomerToFile(const Customer* pCust, FILE* fp);
int readCustomerToFile(Customer* pCust, FILE* fp);
int compareCustByName(const void* c1, const void* c2);
int compareCustByShopTime(const void* c1, const void* c2);
int compareCustByTotalSpend(const void* c1, const void* c2);
eSortType getSortType();