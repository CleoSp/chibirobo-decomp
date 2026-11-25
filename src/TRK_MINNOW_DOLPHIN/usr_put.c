#include "PowerPC_EABI_Support/MetroTRK/trk.h"

// MetroTRK declares this without a prototype, so the call has no varargs setup.
void OSReport();

/**
 * @note Address: N/A
 * @note Size: 0x58
 */
BOOL usr_putchar_serial(char c)
{
	char buf[2];
	BOOL connect = GetTRKConnected();

	buf[0] = c;
	buf[1] = '\0';

	SetTRKConnected(FALSE);
	OSReport(buf);
	SetTRKConnected(connect);
	return FALSE;
}

/**
 * @note Address: 0x8014BAF0
 * @note Size: 0x88
 */
BOOL usr_puts_serial(const char* msg)
{
	BOOL connect_ = FALSE;
	char c;

	while (!connect_ && (c = *msg++) != '\0') {
		connect_ = usr_putchar_serial(c);
	}
	return connect_;
}

/**
 * @note Address: 0x8014BAEC
 * @note Size: 0x4
 */
void usr_put_initialize(void)
{
}
