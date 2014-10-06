#ifndef SERIAL_H_
#define SERIAL_H_

void serial_init();
void serial_send(char *s, uint32_t len);
void serial_send_string(char *s);

#endif
