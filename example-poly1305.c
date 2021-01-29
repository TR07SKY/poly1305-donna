#include <stdio.h>
#include "poly1305-impl.h"
#include "uint256.h"

const unsigned char *msg =  reinterpret_cast<const unsigned char *>("hola?");
char hash[33];

uint512 uhash;

int main(void)
{
	poly1305_context ctx_poly1305;

	poly1305_init(&ctx_poly1305);
	poly1305(&ctx_poly1305, msg, sizeof(msg) + 1);
	poly1305_close(&ctx_poly1305, hash);

	uhash.SetHex(hash);

	printf("%s\n", uhash.GetHex().c_str());
	return 0;
}
