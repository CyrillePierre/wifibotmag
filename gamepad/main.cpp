
#include <unistd.h>
#include "connection.hpp"

int main()
{
	Connection c("newton", "1337");
	
	unsigned char buf[9];
	while (c.read(buf, 9) > 0)
		write(1, buf, 9);

	return 0;
}
