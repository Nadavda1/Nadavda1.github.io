#pragma once

typedef enum { eFruitVegtable, eFridge, eFrozen, eShelf, eNofProductType } eProductType;
static const char* typeStr[eNofProductType] = { "Fruit Vegtable", "Fridge", "Frozen", "Shelf" };

#define NAME_LENGTH 20
#define BARCODE_LENGTH 7

typedef struct
{
	char			name[NAME_LENGTH + 1];
	char			barcode[BARCODE_LENGTH + 1];
	eProductType	type;
	float			price;
	int				count;
}Product;

void	initProduct(Product* pProduct);
void	initProductNoBarcode(Product* pProduct);
void	initProductName(Product* pProduct);
void	printProduct(const void* p1);
void	getBorcdeCode(char* code);
int		isProduct(const Product* pProduct, const char* barcode);
int compareProduct(const void* p1, const void* p2);
eProductType getProductType();
const char*	getProductTypeStr(eProductType type);
int writeProToBFile(FILE* fp, Product* pP);
int readProToBFile(FILE* fp, Product* pP);
void	updateProductCount(Product* pProduct);
void	freeProduct(Product* pProduct);
