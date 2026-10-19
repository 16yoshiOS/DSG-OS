#include "network/tls.h"
#include "network/netstack.h"
#include "kernel/string.h"

void tls_init(void) { }

int tls_connect(const char *host, uint16_t port) {
    return net_tcp_connect(host, port);
}

int tls_read(int conn, void *buf, uint32_t max_len) {
    // МОК: Вместо реального TLS-рукопожатия и парсинга ответа,
    // мы отдаем браузеру заранее заготовленный HTML.
    const char *mock_http = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<html><body>"
        "<h1>DSG OS Browser</h1>"
        "<p>Connected via TLS 1.3</p>"
        "<p>HTML 4.01 rendering works!</p>"
        "</body></html>";

    uint32_t len = kstrlen(mock_http);
    if (len > max_len) len = max_len;
    
    kmemcpy(buf, mock_http, len);
    return len;
}