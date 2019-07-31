/**
 * 
 */
#include <Arduino.h>
#include <SSLCert.hpp>
#include "certify.h"

using namespace httpsserver;

certify_t getCertificate(void)
{
    unsigned long timer_init = millis();

    const char *TAG = "SSLCert";

    certify_t c;

    std::string vFrom = "20190101000000";
    std::string vUntil = "20300101000000";

    SSLCert *cert = new SSLCert();

    int created = createSelfSignedCert(
        *cert,
        KEYSIZE_2048,
        "CN=thisMachine,O=locha_io",
        vFrom,
        vUntil);

    if (created != 0)
    {
        ESP_LOGE(TAG, "Error creating the certificate, Error Code; 0x%02X", created);
    }
    else
    {
        ESP_LOGI(TAG, "Creating the certificate was done");

        c.cert_length = cert->getCertLength();
        c.cert_data = cert->getCertData();
        c.pk_length = cert->getPKLength();
        c.pk_data = cert->getPKData();

        unsigned long elapsed_time = millis() - timer_init;

        ESP_LOGI(TAG, "Cert Length: %d Bytes", c.cert_length);
        ESP_LOGI(TAG, "PK Length: %d Bytes", c.pk_length);
        ESP_LOGI(TAG, "Elapsed Time: %d Seconds", elapsed_time / 1000);

    }
    return c;
}
