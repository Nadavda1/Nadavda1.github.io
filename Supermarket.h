#pragma once
#include "Address.h"
#include "Customer.h"
#include "Product.h"
#include "ShoppingCart.h"
#include "listGen.h"


typedef struct
{
	char*		name;
	Address		location;
	Customer*	customerArr;
	eSortType sortOpt;
	int			customerCount;
	LIST	productList;
	int			proCount;
}SuperMarket;


int		initSuperMarket(SuperMarket* pMarket);
void	printSuperMarket( SuperMarket* pMarket);
int		addProduct(SuperMarket* pMarket);
int		addCustomer(SuperMarket* pMarket);
int		isCustomerInMarket(SuperMarket* pMarket, Customer* pCust);
int		doShopping(SuperMarket* pMarket);
Customer* doPrintCart(SuperMarket* pMarket);
int		doPayment(SuperMarket* pMarket);
Customer*		getCustomerShopPay(SuperMarket* pMarket);
void fillCart(SuperMarket* pMarket, ShoppingCart* pCart);
void	printProductByType(SuperMarket* pMarket);
void	printAllProducts(SuperMarket* pMarket);
void	printAllCustomers(const SuperMarket* pMarket);
void insertProductToListSort(LIST* productList, Product* pPro);
Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode);
Product* getProductFromUser(SuperMarket* pMarket, char* barcode);
Customer* FindCustomerByName(SuperMarket* pMarket, const char* name);
Customer* getCustomerWhoShop(SuperMarket* pMarket);
Product* getProductAndCount(SuperMarket* pMarket, int* pCount);
void sortCustomerByType(SuperMarket* pMarket);
void findCustomer(const SuperMarket* pMarket);
int writeSuperMarketToBFile(SuperMarket* pSuper, const char* fileName);
int readSuperMarketToBFile(SuperMarket* pSuper, const char* fileName);
int writeCustomerArrToTFile(const SuperMarket* pSuper, const char* fileName);
Customer* readCustomerToTFile(SuperMarket* pSuper, const char* fileName);
void	freeMarket(SuperMarket* pMarket);
