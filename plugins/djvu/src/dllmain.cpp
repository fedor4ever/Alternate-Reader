#include <ecom/implementationproxy.h>
#include "CDJVUDocument.h"
#include "plugin_uids.hrh"
// Define the interface UIDs
const TImplementationProxy ImplementationTable[] = 
{
	IMPLEMENTATION_PROXY_ENTRY(IMPLEMENTATION_PROXY_ENTRY_UID, CDJVUDocument::NewL)
};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
{
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
}
