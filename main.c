/*
 * Copyright (c) 2015-2016 Ken Bannister. All rights reserved.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       gcoap example
 *
 * @author      Ken Bannister <kb2ma@runbox.com>
 *
 * @}
 */

#include <stdio.h>
#include "msg.h"

#include "net/gcoap.h"
#include "shell.h"
#include "fmt.h"
#include "timex.h"
#include "xtimer.h"
/* Include lpsxxx headers */
#include "lpsxxx.h"
#include "lpsxxx_params.h"
#include "bmx280.h"

//#include "bmx280_params.h"
#include "mutex.h"

// extern int udp_cmd(int argc, char **argv);
// /* Declate lpsxxx_t sensor variable (globally) */
// static lpsxxx_t sensor;
// /* Declare _value variable (globally) */
// static uint16_t _value = 0;

// static ssize_t _encode_link(const coap_resource_t *resource, char *buf,
//                             size_t maxlen, coap_link_encoder_ctx_t *context);
// static ssize_t _riot_board_handler(coap_pkt_t* pdu, uint8_t *buf, size_t len, void *ctx);
// /* Declare cpu handler */
// static ssize_t _riot_cpu_handler(coap_pkt_t* pdu, uint8_t *buf, size_t len, void *ctx);
// /* Declare temperature handler */
// static ssize_t _temperature_handler(coap_pkt_t* pdu, uint8_t *buf, size_t len, void *ctx);
// /* Declare value handler */
// static ssize_t _value_handler(coap_pkt_t* pdu, uint8_t *buf, size_t len, void *ctx);

// /* CoAP resources. Must be sorted by path (ASCII order). */
// static const coap_resource_t _resources[] = {
//     { "/riot/board", COAP_GET, _riot_board_handler, NULL },
//     /* Add cpu resource */
//      { "/riot/cpu", COAP_GET, _riot_cpu_handler, NULL },
//     /* Add temperature resource */
//     { "/temperature", COAP_GET, _temperature_handler, NULL },
//     /* Add value resource */
//      { "/value", COAP_GET | COAP_PUT | COAP_POST, _value_handler, NULL },
// };





// static ssize_t _riot_board_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx)
// {
//     (void)ctx;
//     gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);
//     coap_opt_add_format(pdu, COAP_FORMAT_TEXT);
//     size_t resp_len = coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);

//     /* write the RIOT board name in the response buffer */
//     if (pdu->payload_len >= strlen(RIOT_BOARD)) {
//         memcpy(pdu->payload, RIOT_BOARD, strlen(RIOT_BOARD));
//         return resp_len + strlen(RIOT_BOARD);
//     }
//     else {
//         puts("gcoap_cli: msg buffer too small");
//         return gcoap_response(pdu, buf, len, COAP_CODE_INTERNAL_SERVER_ERROR);
//     }
// }

// static ssize_t _riot_cpu_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx)
// {
//   (void)ctx;
//     gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);
//     coap_opt_add_format(pdu, COAP_FORMAT_TEXT);
//     size_t resp_len = coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);

//     /* write the RIOT board name in the response buffer */
//     if (pdu->payload_len >= strlen(RIOT_CPU)) {
//         memcpy(pdu->payload, RIOT_CPU, strlen(RIOT_CPU));
//         return resp_len + strlen(RIOT_CPU);
//     }
//     else {
//         puts("gcoap_cli: msg buffer too small");
//         return gcoap_response(pdu, buf, len, COAP_CODE_INTERNAL_SERVER_ERROR);
//     }
// }

// static ssize_t _temperature_handler(coap_pkt_t *pdu, uint8_t *buf, 
//                                          size_t len, void *ctx)
// {
//     (void)ctx;

//     /* Implement temperature GET handler */
//     gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);
//     coap_opt_add_format(pdu, COAP_FORMAT_TEXT);
//     size_t resp_len = coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);

//     char response[32];
//     int16_t temp;
//     lpsxxx_read_temp(&sensor, &temp);
//     int temp_abs = temp / 100;
//     temp -= temp_abs * 100;
//     sprintf(response, "%2i.%02i°C",temp_abs, temp);

//     /* write the temperature value in the response buffer */
//     if (pdu->payload_len >= strlen(response)) {
//         memcpy(pdu->payload, response, strlen(response));
//         return resp_len + strlen(response);
//     }
//     else {
//         puts("gcoap: msg buffer too small");
//         return gcoap_response(pdu, buf, len, COAP_CODE_INTERNAL_SERVER_ERROR);
//     }
// }

// static ssize_t _value_handler(coap_pkt_t* pdu, uint8_t *buf, size_t len, void *ctx)
// {
//     (void)ctx;

//     /* Implement value GET|PUT|POST handler */
//     /* read coap method type in packet */
//     unsigned method_flag = coap_method2flag(coap_get_code_detail(pdu));
//     switch(method_flag) {
//         case COAP_GET:
//             gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);
//             coap_opt_add_format(pdu, COAP_FORMAT_TEXT);
//             size_t resp_len = coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);

//             /* write the response buffer with the request count value */
//             resp_len += fmt_u16_dec((char *)pdu->payload, _value);
//             return resp_len;
    
//          case COAP_PUT:
//          case COAP_POST:
//              /* convert the payload to an integer and update the internal
//                 value */
//              if (pdu->payload_len <= 5) {
//                  char payload[6] = { 0 };
//                  memcpy(payload, (char *)pdu->payload, pdu->payload_len);
//                  _value = (uint16_t)strtoul(payload, NULL, 10);
//                  return gcoap_response(pdu, buf, len, COAP_CODE_CHANGED);
//              }
//              else {
//                  return gcoap_response(pdu, buf, len, COAP_CODE_BAD_REQUEST);
//              }
//      }
//      return 0;
// }

// static gcoap_listener_t _listener = {
//     &_resources[0],
//     ARRAY_SIZE(_resources),
//     _encode_link,
//     NULL,
//     NULL
// };

// /* Adds link format params to resource list */
// static ssize_t _encode_link(const coap_resource_t *resource, char *buf,
//                             size_t maxlen, coap_link_encoder_ctx_t *context) {
//     ssize_t res = gcoap_encode_link(resource, buf, maxlen, context);

//     return res;
// }

// void gcoap_cli_init(void)
// {
//     gcoap_register_listener(&_listener);
// }


// #define MAIN_QUEUE_SIZE (4)
// static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

// extern int my(void);
// extern int gcoap_cli_cmd(int argc, char **argv);
// extern void gcoap_cli_init(void);


// static const shell_command_t shell_commands[] = {
//     { "coap", "CoAP example", gcoap_cli_cmd },
//     { NULL, NULL, NULL }
// };

// int my(void)
// {
//     // Initialize networking
//     sock_udp_ep_t remote = {
//         .family = AF_INET6, // Use IPv6
//         .port = 5683, // CoAP default port
//     };

//     // Set the IPv6 address of the CoAP server
//     ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6[0], "2600:1f18:58cd:3f0c:3e0c:a46d:9262:24a9");

//     // Create a CoAP context
//     coap_init();

//     // Create a CoAP message
//     coap_pkt_t coap_pkt;
//     coap_pkt_init(&coap_pkt, NULL, 0, COAP_TYPE_CON, COAP_GET, coap_get_mid());

//     // Set the CoAP payload (your string data)
//     const char *payload = "Hello, CoAP!";
//     coap_set_payload(&coap_pkt, (uint8_t *)payload, strlen(payload));

//     // Send the CoAP message
//     if (coap_send_request(&remote, &coap_pkt) <= 0) {
//         printf("Failed to send CoAP request\n");
//         return -1;
//     }

//     // Wait for the response (optional)
//     coap_pkt_t coap_response;
//     sock_udp_recv(NULL, &coap_response, 1 * US_PER_SEC);

//     // Process the response if needed

//     return 0;
// }

#ifndef HOSTIP6
    #define HOSTIP6 "2600:1f18:58cd:3f0c:3e0c:a46d:9262:24a9"
#endif


//static bmx280_t bmp280;
static mutex_t bmp_lock = MUTEX_INIT;

static char bmp_stack[THREAD_STACKSIZE_MAIN];

coap_pkt_t coap_pkt;
uint8_t coap_buf[64];
sock_udp_ep_t remote = {.family = AF_INET6,
                        .port = 5863,
                        .netif = SOCK_ADDR_ANY_NETIF};


static void *bmp_thread(void *arg)
{
    (void)arg;

    while (1) {
        /* Acquire the mutex here */
        mutex_lock(&bmp_lock);
        /* Read the temperature values here */
        int16_t temp = 0;
        temp = 39; // add tempb get here
        printf("Temperature: %i.%u C\n",(temp/100),(temp%100));


        /* Send temperature as coap message */
        size_t len;
       gcoap_req_init(&coap_pkt, coap_buf, sizeof(coap_buf), COAP_METHOD_POST, "/api/createRecord");
       len = coap_opt_finish(&coap_pkt, COAP_OPT_FINISH_PAYLOAD);
       char msg[32]={'\0'};
       sprintf(msg,"%02i.%02u",(temp/100),(temp%100));
       strcpy((char*)coap_pkt.payload,msg);
       len+=strlen(msg);
       printf("in 2");
       
       int res= gcoap_req_send(coap_buf, len, &remote, NULL, NULL);
       printf("%d",res);


        /* Release the mutex here */
        mutex_unlock(&bmp_lock);
        xtimer_sleep(10);
    }

    return 0;
}

static void _bmp280_usage(char *cmd){
    printf("usage: %s <temperature|pressure>\n", cmd);
}

static int bmp280_handler(int argc, char *argv[]){
    (void) argc;
    if (!strcmp(argv[1],"temperature")){
        int16_t temp = 0;
        //temp = bmx280_read_temperature(&bmp280);
        temp = 40;
        printf("Temperature: %i.%u C\n",(temp/100),(temp%100));
    }
    else if (!strcmp(argv[1],"pressure")){
        uint32_t pres = 0;
        //pres = bmx280_read_pressure(&bmp280);
        pres = 100;
        printf("Pressure: %li hPa\n",pres);
    }else{
        _bmp280_usage(argv[0]);
        return -1;
    }
    return 0;
}
static void _bmp280_thread_usage(char *cmd){
    printf("usage: %s <start|stop>\n", cmd);
}

static int bmp280_thread_handler(int argc, char *argv[]){
    (void) argc;
    if (!strcmp(argv[1],"start")){
        mutex_unlock(&bmp_lock);
    }else if (!strcmp(argv[1],"stop")){
        mutex_trylock(&bmp_lock);
    }else{
        _bmp280_thread_usage(argv[0]);
        return -1;
    }
    return 0;
}

static const shell_command_t commands[] = {
    {"bmp","Read the BMP280 values", bmp280_handler},
    {"bmc","Continuesly reads BMP280 temperature", bmp280_thread_handler},
    { NULL, NULL, NULL}
};


#define MAIN_QUEUE_SIZE (4)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];


// int main(void)
// {

//     /* Initialize and enable the lps331ap device */
//     if (lpsxxx_init(&sensor, &lpsxxx_params[0]) != LPSXXX_OK) {
//         puts("LPS331AP initialization failed");
//         return 1;
//     }
//     lpsxxx_enable(&sensor);
    
//     /* for the thread running the shell */
//     msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
//     gcoap_cli_init();
//     //my();
    
//     puts("gcoap example app");

//     /* start shell */
//     puts("All up, running the shell now");
//     char line_buf[SHELL_DEFAULT_BUFSIZE];
//     my();
//     shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

//     /* should never be reached */
//     return 0;
// }

int main(void)
{

    // /* Initialize the bmp280 sensor here */
    // int err = bmx280_init(&bmp280, bmx280_params);
    // if(err==BMX280_OK){
    //     printf("BMP280 initialized succesfully\n");
    // }else if(err==BMX280_ERR_BUS){
    //     printf("BMP280 BUS ERROR\n");
    // }else if(err==BMX280_ERR_NODEV){
    //     printf("BMP280 device not found\n");
    // }

    printf("Coap target is: %s",HOSTIP6);
    ipv6_addr_t addr;
    ipv6_addr_from_str(&addr, HOSTIP6);
    memcpy(&remote.addr.ipv6[0], &addr.u8[0], sizeof(addr.u8));

    thread_create(bmp_stack, sizeof(bmp_stack), THREAD_PRIORITY_MAIN - 1,
                  0, bmp_thread, NULL, "bmp280");
    
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    /* Everything is ready, let's start the shell now */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}