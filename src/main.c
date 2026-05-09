#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <string.h>

static void scan_result_handler(struct net_mgmt_event_callback *cb,
                                uint32_t mgmt_event, struct net_if *iface)
{
    const struct wifi_scan_result *res = (const struct wifi_scan_result *)cb->info;
    
    if (res)
	{

        char ssid_str[33] = {0};
        uint8_t len = (res->ssid_length <= 32) ? res->ssid_length : 32;
        memcpy(ssid_str, res->ssid, len);

        // Dogrudan donanim uzerinden (UART) ekrana bas
        printk("[NODE DETECTED] SSID: %-20s | RSSI: %4d dBm | Channel: %2d\n", 
               ssid_str, res->rssi, res->channel);
    }
}


static void scan_done_handler(struct net_mgmt_event_callback *cb,
                              uint32_t mgmt_event, struct net_if *iface)
{
    printk("\n--- Scan Completed ---\n\n");
}

static struct net_mgmt_event_callback scan_res_cb;
static struct net_mgmt_event_callback scan_done_cb;

int main(void)
{
    struct net_if *iface = net_if_get_default();

    net_mgmt_init_event_callback(&scan_res_cb, (net_mgmt_event_handler_t)scan_result_handler, NET_EVENT_WIFI_SCAN_RESULT);
    net_mgmt_add_event_callback(&scan_res_cb);

    net_mgmt_init_event_callback(&scan_done_cb, (net_mgmt_event_handler_t)scan_done_handler, NET_EVENT_WIFI_SCAN_DONE);
    net_mgmt_add_event_callback(&scan_done_cb);

    printk("\nWating for Wi-Fi interface to be ready...\n");
    k_sleep(K_SECONDS(2)); 
    net_if_up(iface);
    k_sleep(K_MSEC(500)); 

    printk("\nScanning for Wi-Fi networks...\n");

    if (net_mgmt(NET_REQUEST_WIFI_SCAN, iface, NULL, 0)) {
        printk("Failed to start scan!\n");
        return -1;
    }

    while (1) {
        k_sleep(K_SECONDS(10));
    }

    return 0;
}