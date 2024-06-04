#ifndef CONFIG_H
#define CONFIG_H

const char* ssid = "your-SSID";
const char* password = "your-PASSWORD";
const char* awsEndpoint = "your-endpoint.iot.your-region.amazonaws.com";
const char* deviceCert = R"KEY(
-----BEGIN CERTIFICATE-----
your-device-certificate
-----END CERTIFICATE-----
)KEY";

const char* privateKey = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
your-private-key
-----END RSA PRIVATE KEY-----
)KEY";

const char* rootCA = R"KEY(
-----BEGIN CERTIFICATE-----
your-root-CA
-----END CERTIFICATE-----
)KEY";

#endif
