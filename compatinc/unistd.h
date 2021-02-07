#ifdef _MSC_VER
#include <io.h>
#include <direct.h>
typedef int mode_t;
#define S_ISREG(x) (x & _S_IFREG)
#define S_ISFIFO(x) (0)
#define S_ISDIR(x) (x & _S_IFDIR)
#define S_IWUSR _S_IWRITE
#define S_IRUSR _S_IREAD
#define PATH_MAX _MAX_PATH
#endif
