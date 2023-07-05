#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Supermarket.h"
#include "Product.h"
#include "Customer.h"
#include "Address.h"
#include "General.h"
#include "ShoppingCart.h"

int		initSuperMarket(SuperMarket* pMarket)
{
	int fileBCheck = 1;
	int fileTCheck = 1;
	if (!L_init(&(pMarket->productList)))
		return 0;

	if(!readSuperMarketToBFile(pMarket, "SuperMarket.bin"))
		fileBCheck = 0;

	if (!readCustomerToTFile(pMarket, "Customer.txt"))
		fileTCheck = 0;


	if (fileTCheck == 0)
	{
		pMarket->customerCount = 0;
		pMarket->customerArr = NULL;
	}
	else if (fileTCheck == 1 && fileBCheck == 0 )
	{
		printf("Only customer file successfully loaded\n");
	}
	if (fileBCheck == 0)
	{
		pMarket->proCount = 0;
		pMarket->name = getStrExactLength("Enter market name");
		return initAddress(&pMarket->location);
	}
	else {
		printf("SuperMarket successfully loaded from files\n");
	}
	return 1;
}

void	printSuperMarket(SuperMarket* pMarket)
{
	printf("Super Market Name: %s\t", pMarket->name);
	printf("Address: ");
	printAddress(&pMarket->location);
	printf("\n");
	printAllProducts(pMarket);
	printf("\n");
	printAllCustomers(pMarket);
}

int		addProduct(SuperMarket* pMarket)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket, barcode);
	if (pProd != NULL)
		updateProductCount(pProd);
	else
	{
		Product* pProd = (Product*)calloc(1, sizeof(Product));
		strcpy(pProd->barcode, barcode);
		initProductNoBarcode(pProd);
		insertProductToListSort(&pMarket->productList, pProd);
		pMarket->proCount++;
	}

	return 1;
}

void insertProductToListSort(LIST* productList, Product* pPro)
{
	NODE* tmp = &productList->head;
	while (tmp->next != NULL)
	{
		if (strcmp(pPro->barcode, ((Product*)(tmp->next->key))->barcode) < 0)
			break;
		tmp = tmp->next;
	}

	L_insert(tmp, pPro);
}

int		addCustomer(SuperMarket* pMarket)
{
	Customer cust;
	if (!initCustomer(&cust))
		return 0;

	if (isCustomerInMarket(pMarket, &cust))
	{
		printf("This customer already in market\n");
		free(cust.name);
		return 0;
	}

	pMarket->customerArr = (Customer*)realloc(pMarket->customerArr, (pMarket->customerCount + 1) * sizeof(Customer));
	if (!pMarket->customerArr)
	{
		free(cust.name);
		return 0;
	}

	pMarket->customerArr[pMarket->customerCount] = cust;
	pMarket->customerCount++;
	return 1;
}

int		isCustomerInMarket(SuperMarket* pMarket, Customer* pCust)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (strcmp(pMarket->customerArr[i].name, pCust->name) == 0)
			return 1;
	}
	return 0;
}

int	doShopping(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return 0;

	if (pCustomer->pCart == NULL)
	{
		pCustomer->pCart = (ShoppingCart*)malloc(sizeof(ShoppingCart));
		if (!pCustomer->pCart)
			return 0;
		initCart(pCustomer->pCart);
	}
	fillCart(pMarket, pCustomer->pCart);
	if (pCustomer->pCart->count == 0)
	{
		free(pCustomer->pCart);
		pCustomer->pCart = NULL;
	}else
		printf("---------- Shopping ended ----------\n");
	return 1;
}

Customer*	doPrintCart(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return NULL;
	if (pCustomer->pCart == NULL)
	{
		printf("Customer cart is empty\n");
		return NULL;
	}
	printShoppingCart(pCustomer->pCart);
	return pCustomer;
}

int	doPayment(SuperMarket* pMarket)
{
	Customer* pCustomer = doPrintCart(pMarket);
	if (!pCustomer)
		return 0;
	pay(pCustomer);
	return 1;
}

Customer*	getCustomerShopPay(SuperMarket* pMarket)
{
	if (pMarket->customerCount == 0)
	{
		printf("No customer listed to market\n");
		return NULL;
	}

	if (pMarket->proCount == 0)
	{
		printf("No products in market - cannot shop\n");
		return NULL;
	}

	Customer* pCustomer = getCustomerWhoShop(pMarket);
	if (!pCustomer)
	{
		printf("this customer not listed\n");
		return NULL;
	}

	return pCustomer;
}

void	printAllProducts(SuperMarket* pMarket)
{
	printf("There are %d products\n", pMarket->proCount);
	printf("%-20s %-10s\t", "Name", "Barcode");
	printf("%-20s %-10s %s\n", "Type", "Price", "Count In Stoke");
	printf("--------------------------------------------------------------------------------\n");
	L_print(&pMarket->productList,printProduct);
}

void	printAllCustomers(const SuperMarket* pMarket)
{
	printf("There are %d listed customers\n", pMarket->customerCount);
	generalArrayFunction((void*)pMarket->customerArr, pMarket->customerCount, sizeof(Customer), printCustomer);
}


Customer* getCustomerWhoShop(SuperMarket* pMarket)
{
	printAllCustomers(pMarket);
	char name[MAX_STR_LEN];
	getsStrFixSize(name, sizeof(name), "Who is shopping? Enter cutomer name\n");

	Customer* pCustomer = FindCustomerByName(pMarket, name);

	return pCustomer;
}


void fillCart(SuperMarket* pMarket, ShoppingCart* pCart)
{
	printAllProducts(pMarket);
	char op;
	while (1)
	{
		printf("Do you want to shop for a product? y/Y, anything else to exit!!\t");
		do {
			scanf("%c", &op);
		} while (isspace(op));
		getchar(); //clean the enter
		if (op != 'y' && op != 'Y')
			break;
		int count;
		Product* pProd = getProductAndCount(pMarket, &count);
		if (pProd)
		{
			if (!addItemToCart(pCart, pProd->barcode, pProd->price, count))
			{
				printf("Error adding item\n");
				return;
			}
			pProd->count -= count; //item bought!!!
		}
	}
}

Product* getProductAndCount(SuperMarket* pMarket, int* pCount)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket, barcode);
	if (pProd == NULL)
	{
		printf("No such product\n");
		return NULL;
	}

	if (pProd->count == 0)
	{
		printf("This product out of stock\n");
		return NULL;
	}

	int count;
	do {
		printf("How many items do you want? max %d\n", pProd->count);
		scanf("%d", &count);
	} while (count <= 0 || count > pProd->count);
	*pCount = count;
	return pProd;
}

void	printProductByType(SuperMarket* pMarket)
{
	if (pMarket->proCount == 0)
	{
		printf("No products in market\n");
		return;
	}
	eProductType	type = getProductType();
	int count = 0;
	NODE* tmp = &pMarket->productList.head;
	while (tmp->next != NULL)
	{
		if ((((Product*)(tmp->next->key))->type) == type)
		{
			printProduct(tmp->next->key);
			count++;
		}
		tmp = tmp->next;
	}
	if (count == 0)
		printf("There are no product of type %s in market %s\n", getProductTypeStr(type), pMarket->name);
}

Product* getProductFromUser(SuperMarket* pMarket, char* barcode)
{
	getBorcdeCode(barcode);
	return getProductByBarcode(pMarket, barcode);
}

void	freeMarket(SuperMarket* pMarket)
{
	free(pMarket->name);
	freeAddress(&pMarket->location);
	L_free(&pMarket->productList, freeProduct);
	generalArrayFunction((void*)pMarket->customerArr, pMarket->customerCount, sizeof(Customer), freeCustomer);
	free(pMarket->customerArr);
}

Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode)
{

	Product temp = { 0 };
	strcpy(temp.barcode, barcode);

	NODE* tmp;
	tmp = L_find(pMarket->productList.head.next, &temp, compareProduct);
	if (tmp != NULL)
		return tmp->key;
	
	return NULL;
}

Customer* FindCustomerByName(SuperMarket* pMarket, const char* name)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (isCustomer(&pMarket->customerArr[i], name))
			return &pMarket->customerArr[i];
	}
	return  NULL;
}

void sortCustomerByType(SuperMarket* pMarket)
{
	if (pMarket->customerCount == 0)
	{
		printf("Theres no customer!!\n");
		return;
	}
	pMarket->sortOpt = getSortType();
	if (pMarket->sortOpt == eNotSort)
	{
		printf("sort doesnt change\n");
		return;
	}
	else if (pMarket->sortOpt == eSortByName)
	{
		qsort(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareCustByName);
	}
	else if (pMarket->sortOpt == eSortByShopTime)
	{
		qsort(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareCustByShopTime);
	}
	else if (pMarket->sortOpt == eSortByTotalSpend)
	{
		qsort(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareCustByTotalSpend);
	}
	printf("Customer list are sorted!\n");
	return;
}

void findCustomer(const SuperMarket* pMarket)
{
	Customer c = { 0 };

	if (pMarket->customerCount == 0)
	{
		printf("Theres no customer!!\n");
		return;
	}

	if (pMarket->sortOpt <= eNotSort || pMarket->sortOpt > 3)
	{
		printf("list doent sort cant search!\n");
		return;
	}
	else if (pMarket->sortOpt == eSortByName)
	{
		char name[255];
		printf("Enter customer name: ");
		myGets(name, 255);
		c.name = name;
		Customer* cust = bsearch(&c, pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareCustByName);
		if(cust == NULL)
			printf("Customer was not found!\n");
		else {
			printCustomer(cust);
		}
	}
	else if (pMarket->sortOpt == eSortByShopTime)
	{
		int s;
		printf("Enter shop time: ");
		scanf("%d", &s);
		c.shopTimes = s;
		Customer* cust = bsearch(&c, pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareCustByShopTime);
		if (cust == NULL)
			printf("Customer was not found!\n");
		else {
			printCustomer(cust);
		}
	}
	else if (pMarket->sortOpt == eSortByTotalSpend)
	{
		float t;
		printf("Enter total spend: ");
		scanf("%f", &t);
		c.totalSpend = t;
		Customer* cust = bsearch(&c, pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareCustByTotalSpend);
		if (cust == NULL)
			printf("Customer was not found!\n");
		else {
			printCustomer(cust);
		}
	}

}
int writeSuperMarketToBFile(SuperMarket* pSuper, const char* fileName)
{
	FILE* fp;
	int len = (int)strlen(pSuper->name) + 1;

	fp = fopen(fileName, "wb");
	if (!fp)
	{
		printf("Error open Super file to write!\n");
		return 0;
	}
	fwrite(&len, sizeof(int), 1, fp);
	if (fwrite(pSuper->name, sizeof(char), len, fp) != len)
	{
		fclose(fp);
		return 0;
	}
	if (writeAddressToBFile(fp, &pSuper->location) != 1)
	{
		fclose(fp);
		return 0;
	}
	fwrite(&pSuper->proCount, sizeof(int), 1, fp);
	NODE* pNode = pSuper->productList.head.next;
	while (pNode != NULL)
	{
		if (!writeProToBFile(fp, pNode->key))
		{
			fclose(fp);
			return 0;
		}
		pNode = pNode->next;
	}
	fclose(fp);
	return 1;
}
int readSuperMarketToBFile( SuperMarket* pSuper, const char* fileName)
{
	FILE* fp = fopen(fileName, "rb");
	if (!fp)
		return 0;
	int len;
	if (fread(&len, sizeof(int), 1, fp) != 1)
		return 0;
	pSuper->name = (char*)malloc(len * sizeof(char));
	if (!pSuper->name)
		return 0;
	if (fread(pSuper->name, sizeof(char), len, fp) != len)
	{
		fclose(fp);
		free(pSuper->name);
		return 0;
	}

	if (readAddressToBFile(fp,&pSuper->location)!= 1)
	{
		fclose(fp);
		free(pSuper->name);
		return 0;
	}
	if (fread(&pSuper->proCount, sizeof(int), 1, fp) != 1)
	{
		return 0;
	}
	Product* pP;
	for (int i = 0; i < pSuper->proCount; i++)
	{
		pP = (Product*)malloc(sizeof(Product));
		if (!pP)
			return 0;

		if (!readProToBFile(fp, pP))
		{
			fclose(fp);
			return 0;
		}
		if (pP != NULL)
		{
			insertProductToListSort(&pSuper->productList, pP);
		}
	}
	fclose(fp);
	return 1;
}
int writeCustomerArrToTFile(const SuperMarket* pSuper, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "w");
	if (!fp)
		return 0;

	fprintf(fp, "%d\n", pSuper->customerCount);

	for (int i = 0; i < pSuper->customerCount; i++)
		writeCustomerToFile(&pSuper->customerArr[i], fp);

	fclose(fp);
	return 1;
}
Customer* readCustomerToTFile(SuperMarket* pSuper, const char* fileName)
{
	pSuper->customerArr = NULL;
	FILE* fp = fopen(fileName, "r");
	if (!fp)
		return NULL;
	if (fscanf(fp, "%d", &(pSuper->customerCount)) != 1)
	{	
		fclose(fp);
		return NULL;
	}

	pSuper->customerArr = (Customer*)malloc(pSuper->customerCount * sizeof(Customer));
	if (!pSuper->customerArr)
	{
		fclose(fp);
		return NULL;
	}
	for (int i = 0; i < pSuper->customerCount; i++)
	{
		if (!readCustomerToFile(&pSuper->customerArr[i], fp))
		{
			free(pSuper->customerArr);
			fclose(fp);
			return NULL;
		}
	}
	fclose(fp);
	return pSuper->customerArr;
}
