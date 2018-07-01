#include "epod_purchase.h"
#include <map>

#import <UIKit/UIKit.h>
#import <StoreKit/StoreKit.h>

int is_purchases_enabled()
{
  return [[[UIDevice currentDevice] systemVersion] floatValue] >= 3.0;
}

typedef std::map<const char *, int> PurchaseInfo;
typedef PurchaseInfo::iterator PurchaseInfoIter;

PurchaseInfo m_status;

typedef std::map<const char *, NSData *> VerifyInfo;
typedef VerifyInfo::iterator VerifyInfoIter;

VerifyInfo m_verifies;

PurchaseVerifier *verifier = nil;


///////////////////////////////////////////////////////////////////////////////


@interface TransactionsObserver : NSObject<SKPaymentTransactionObserver>
{
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;

@end

@implementation TransactionsObserver

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
  for (SKPaymentTransaction *trans in transactions)
  {
    const char *prod_id = [trans.payment.productIdentifier UTF8String];

    PurchaseInfoIter it = m_status.find(prod_id);
    if (it == m_status.end())
      continue;

    switch (trans.transactionState)
    {
      case SKPaymentTransactionStatePurchasing:
        NSLog(@"[PURCHASE] Purchase pending: '%s'", prod_id);
        break;

      case SKPaymentTransactionStateRestored:
        NSLog(@"[PURCHASE] Purchase restored");
      case SKPaymentTransactionStatePurchased:
        NSLog(@"[PURCHASE] Purchase complete: '%s'", prod_id);

        (*it).second = S_VERIFYING;
        [[SKPaymentQueue defaultQueue] finishTransaction:trans];

        verify_purchase(prod_id, trans.transactionReceipt);
        break;

      case SKPaymentTransactionStateFailed:
        NSLog(@"[PURCHASE] Purchase failed: '%s'. Error = '%@', code = %d, domain = '%@'",
          prod_id, trans.error.localizedDescription, trans.error.code, trans.error.domain);

        (*it).second = S_ERROR;
        [[SKPaymentQueue defaultQueue] finishTransaction:trans];
        break;
    }
  }
}

@end


///////////////////////////////////////////////////////////////////////////////


@interface ProductRequestObserver: NSObject<SKProductsRequestDelegate>
{
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response;
- (void)request:(SKRequest *)request didFailWithError:(NSError *)error;

@end

@implementation ProductRequestObserver

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response 
{
  NSLog(@"[PURCHASE] Product ids checking...");

  NSArray *products = response.products;
  for (int i=0; i<[products count]; i++)
  {
    SKProduct *prod = [products objectAtIndex:i];
    const char *prod_id = [prod.productIdentifier UTF8String];

    PurchaseInfoIter it = m_status.find(prod_id);
    if (it != m_status.end() && (*it).second == S_CHECKING)
    {
      (*it).second == S_PENDING;

      NSLog(@"[PURCHASE] Product id checked: '%s'", (*it).first);

      if ([SKPaymentQueue canMakePayments])
      {
        SKPayment *payment = [SKPayment paymentWithProductIdentifier:[NSString stringWithUTF8String:prod_id]];
        [[SKPaymentQueue defaultQueue] addPayment:payment];
      }
      else
      {
        (*it).second == S_ERROR;
        NSLog(@"[PURCHASE] ERROR. Purchases are disabled!");
      }
    }
  }

  NSArray *invalidIds = response.invalidProductIdentifiers;
  for (int i=0; i<[invalidIds count]; i++)
  {
    const char *prod_id = [[invalidIds objectAtIndex:i] UTF8String];

    PurchaseInfoIter it = m_status.find(prod_id);
    if (it != m_status.end() && (*it).second == S_CHECKING)
    {
      (*it).second == S_ERROR_INVALID_ID;

      NSLog(@"[PURCHASE] Product id is invalid: '%s'", (*it).first);
    }
  }
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error 
{
  NSLog(@"[PURCHASE] ERROR. Failed to check product ids!");

  for (PurchaseInfoIter it=m_status.begin(); it!=m_status.end(); ++it)
  {
    if ((*it).second == S_CHECKING)
      (*it).second = S_ERROR_INVALID_ID;
  }
}

@end


///////////////////////////////////////////////////////////////////////////////


@interface NSData (Base64Encoding)

- (NSString*)base64Encode;

@end

@implementation NSData (Base64Encoding)

- (NSString*)base64Encode
{
  static char table [] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

  NSInteger length = [self length];
  NSMutableData* data = [NSMutableData dataWithLength:((length + 2) / 3) * 4];
  uint8_t* input = (uint8_t*)[self bytes];
  uint8_t* output = (uint8_t*)data.mutableBytes;

  for (NSInteger i = 0; i < length; i += 3)
  {
    NSInteger value = 0;
    for (NSInteger j = i; j < (i + 3); ++j)
    {
      value <<= 8;

      if (j < length)
      {
        value |= (0xff & input[j]);
      }
    }

    NSInteger index = (i / 3) * 4;
    output[index + 0] =                    table[(value >> 18) & 0x3f];
    output[index + 1] =                    table[(value >> 12) & 0x3f];
    output[index + 2] = (i + 1) < length ? table[(value >> 6) & 0x3f] : '=';
    output[index + 3] = (i + 2) < length ? table[(value >> 0) & 0x3f] : '=';
  }

  return [[[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding] autorelease];
}

@end


void verify_purchase(const char *prod_id, NSData *receipt)
{
  [receipt retain];
  m_verifies[prod_id] = receipt;

  make_next_verification();
}

void on_verified(const char *prod_id, int is_ok)
{
  VerifyInfoIter vit = m_verifies.find(prod_id);
  if (vit != m_verifies.end())
  {
    [(*vit).second release];
    m_verifies.erase(vit);
  }
  else
    NSLog(@"[PURCHASE] ERROR. No such product id in verifies!");

  PurchaseInfoIter pit = m_status.find(prod_id);
  if (pit != m_status.end() && (*pit).second == S_VERIFYING)
  {
    NSLog(@"[PURCHASE] Purchase made: %s", prod_id);
    m_status.erase(pit);
  }
  else
    NSLog(@"[PURCHASE] ERROR. No such product id in pendings!");

  NSLog(@"[PURCHASE] Product verification finished: %s", prod_id);

  make_next_verification();
}

void make_next_verification()
{
  if (verifier.inProccess)
    return;

  if (m_verifies.empty())
    return;

  VerifyInfoIter vit = m_verifies.begin();
  [verifier verify:(*vit).second product:(*vit).first];
}


@interface PurchaseVerifier: NSObject
{
@private
  const char *m_productId;
  NSMutableData *m_data;

  BOOL m_inProccess;
}

- (id)init;

- (void)verify:(NSData *)receipt product:(const char *)product_id;
- (void)finish:(int)res withMsg:(NSString *)msg;

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response;
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data;
- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error;
- (void)connectionDidFinishLoading:(NSURLConnection *)connection;

@property(assign) BOOL inProccess;

@end


@implementation PurchaseVerifier

@synthesize inProccess = m_inProccess;

- (id)init
{
  self = [super init];

  m_data = [[NSMutableData alloc] init];
  m_inProccess = FALSE;
  return self;
}

- (void)verify:(NSData *)receipt product:(const char *)product_id
{
  self.inProccess = TRUE;

  m_productId = product_id;
  [m_data setLength:0];

  NSString *receiptAsString = [receipt base64Encode];
  NSDictionary *receiptDictionary = [[NSDictionary alloc] initWithObjectsAndKeys:receiptAsString, @"receipt-data", nil];

  NSError *jsonError = nil;
  NSData *receiptAsData = [NSJSONSerialization dataWithJSONObject:receiptDictionary options:NSJSONWritingPrettyPrinted error:&jsonError];

  if (receiptAsData == nil || jsonError != nil)
  {
    [self finish:0 withMsg:@"[PURCHASE] Verification FAILED! (cannot convert receipt to json)"];
    return;
  }

  NSURL *urlVerify = [NSURL URLWithString:@"https://buy.itunes.apple.com/verifyReceipt"];

  NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:urlVerify cachePolicy:NSURLRequestReloadIgnoringLocalAndRemoteCacheData timeoutInterval:30.0];
  [request setHTTPMethod:@"POST"];
  [request setHTTPBody:receiptAsData];

  [[NSURLConnection alloc] initWithRequest:request delegate:self];
}

- (void)finish:(int)res withMsg:(NSString *)msg
{
  NSLog(msg);
  [m_data setLength:0];

  self.inProccess = FALSE;

  on_verified(m_productId, res);
}


- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
  [m_data setLength:0];
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
  [m_data appendData:data];
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
  [connection release];

  [self finish:1 withMsg:@"[PURCHASE] Verification OK! (error connecting verification server)"];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
  [connection release];

  NSError *jsonError = nil;
  NSDictionary *response = [NSJSONSerialization JSONObjectWithData:m_data options:0 error:&jsonError];

  if (response == nil || jsonError != nil)
  {
    [self finish:0 withMsg:@"[PURCHASE] Verification FAILED! (wrong received data)"];
    return;
  }

  NSNumber *status = [response objectForKey:@"status"];
  NSDictionary *receipt = [response objectForKey:@"receipt"];

  if (status == nil || receipt == nil)
  {
    [self finish:0 withMsg:@"[PURCHASE] Verification FAILED! (wrong json)"];
    return;
  }

  if ([status integerValue] != 0)
  {
    [self finish:0 withMsg:@"[PURCHASE] Verification FAILED! (status != 0)"];
    return;
  }

  [self finish:1 withMsg:@"[PURCHASE] Verification OK!"];
}

@end


///////////////////////////////////////////////////////////////////////////////


namespace ep
{
namespace purchase
{

TransactionsObserver *transCb = nil;
ProductRequestObserver *prodsCb = nil;


int init()
{
  m_status.clear();

  if (!is_purchases_enabled())
    return 0;

  transCb = [[TransactionsObserver alloc] init];
  prodsCb = [[ProductRequestObserver alloc] init];
  verifier = [[PurchaseVerifier alloc] init];

  if (transCb)
    [[SKPaymentQueue defaultQueue] addTransactionObserver:transCb];

  return 1;
}


void free()
{
  if (transCb)
    [transCb release];
  if (prodsCb)
    [prodsCb release];
  if (verifier)
    [verifier release];

  transCb = nil;
  prodsCb = nil;
  verifier = nil;
}


void make(const char *pid)
{
  if (!pid || !*pid || !transCb || !prodsCb)
    return;

  NSLog(@"[PURCHASE] make '%s'", pid);

  m_status[pid] = S_CHECKING;

  SKProductsRequest *request = [[SKProductsRequest alloc] initWithProductIdentifiers:[NSSet setWithObject: [NSString stringWithUTF8String:pid]]];
  request.delegate = prodsCb;
  [request start];
}


int get_status(const char *pid)
{
  PurchaseInfoIter it = m_status.find(pid);
  if (it != m_status.end())
    return (*it).second;

  return S_OK;
}

} // namespace purchase
} // namespace ep
