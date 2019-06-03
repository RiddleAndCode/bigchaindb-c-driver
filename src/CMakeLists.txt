
file(GLOB SOURCES "*.c")
file(GLOB JSON_SOURCES "../libs/json-maker/*.c")
file(GLOB TREZOR_CRYPTO_ED255 "../libs/trezor-crypto/ed25519-donna/*.c")
file(GLOB TREZOR_CRYPTO "../libs/trezor-crypto/*.c")
file(GLOB BASE64 "../libs/base64/base64.c")


add_library(cbig STATIC
    ${SOURCES}
    ${TREZOR_CRYPTO}
    ${TREZOR_CRYPTO_ED255}
    ${JSON_SOURCES}
    ${BASE64}
)
 
target_include_directories(cbig PUBLIC
    ../inc
    ../libs/trezor-crypto/
    ../
    ../libs/json-maker
    ../libs/base64
)