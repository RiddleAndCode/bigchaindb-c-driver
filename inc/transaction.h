#ifndef _BIGCHAIN_TX_H_
#define _BIGCHAIN_TX_H_
#define BDB_VERSION "2.0"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "json-maker.h"
#include "sha3.h"
#include "ed25519-donna/ed25519.h"

typedef struct {
  char amount[8];
  // FIXED CONDITION FOR EDCURVE
  char details_public_key[45];
  char public_keys[8][45];
  uint8_t num_public_keys;
} BIGCHAIN_OUTPUT;

typedef struct {
  char fulfillment[256];
  char fulfills[256];
  char owners_before[8][256];
  uint8_t num_owners;
} BIGCHAIN_INPUT;

typedef struct {
  char asset[256];
  char metadata[256];
  char operation[32];
  BIGCHAIN_OUTPUT outputs[8];
  uint8_t num_outputs;
  BIGCHAIN_INPUT inputs[8];
  uint8_t num_inputs;
  char version[8];
  char id[65];
} BIGCHAIN_TX;

void bigchain_fulfill_and_serialize(BIGCHAIN_TX *tx, uint8_t *json_tx, uint16_t maxlen, uint8_t *sig, uint8_t *pubkey);
void bigchain_sign_transaction(uint8_t *json_tx, uint16_t len, uint8_t *priv_key, uint8_t *pub_key, uint8_t *sig);
char* bigchain_build_json_inputs(BIGCHAIN_INPUT *inputs, uint8_t num_inputs, char *json_obj);
char* bigchain_build_json_outputs(BIGCHAIN_OUTPUT *outputs, uint8_t num_outputs, char *json_obj);
void bigchain_build_json_tx(BIGCHAIN_TX *tx, char *json_tx);
void prepare_tx(BIGCHAIN_TX *tx, const char operation, char *asset, char *metadata , char *base_pubkey );
void fulfill_tx(BIGCHAIN_TX *tx, char *tx_id, uint8_t *priv_key, uint8_t *pub_key, uint8_t *json, uint16_t maxlen);

#endif // _BIGCHAIN_TX_H_
