#ifndef CERTIFY_H_
#define CERTIFY_H_



typedef struct {
    uint16_t cert_length;
    uint16_t pk_length;
    unsigned char *cert_data;
    unsigned char *pk_data;
} certify_t;

certify_t getCertificate();

#endif // CERTIFY_H_