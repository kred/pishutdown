#ifndef SERIAL_H_
#define SERIAL_H_

void serial_check();
void serial_send(char *s, uint32_t len);
void serial_send_string(char *s);
void serial_ping();
void serial_shutdown();

#endif
