#include "epod_purchase.h"


namespace ep
{
namespace purchase
{

int init() { return 1; }
void free() {}

void make(const char *pid) {}
int get_status(const char *pid) { return S_OK; }

} // purchase
} // namespace ep
