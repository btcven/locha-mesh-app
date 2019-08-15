/**
 * @file certify.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-07-31
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 */

#ifndef CERTIFY_H_
#define CERTIFY_H_

typedef struct {
    uint16_t cert_length;
    uint16_t pk_length;
    unsigned char *cert_data;
    unsigned char *pk_data;
} certify_t;

/**
 * @brief Get the Certificate object
 * 
 * @return certify_t 
 */
certify_t getCertificate();

#endif // CERTIFY_H_