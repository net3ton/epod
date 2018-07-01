#ifndef _EPOD_PURCHASE_H_
#define _EPOD_PURCHASE_H_


namespace ep
{
namespace purchase
{

enum
{
  S_OK = 0,

  S_ERROR,
  S_ERROR_INVALID_ID,

  S_PENDING,   /// pending purchase
  S_CHECKING,  /// checking product id
  S_VERIFYING, /// verifying purchase
};

int init();
void free();

/// IMPORTANT: pid must be in const buffer. it's used as id

void make(const char *pid);
int get_status(const char *pid);

} // namespace purchase
} // namespace ep

#endif //_EPOD_PURCHASE_H_
