#include "transaction.h"

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#define DEFAULT_URI "ni:///sha-256;WVm8YmcTjv05Osmho-Hc7o6N2Hg0YvgsKdaidCaRb0Q?fpt=ed25519-sha-256&cost=131072"
#define FULFILL_PREFIX "\"output_index\":0,\"transaction_id\":\""
#define FULFILL_PREFIX_LENGTH 35
#define TX_ID_LENGTH 64
#define ASSET_ID "\"id\":\""
#define ASSET_ID_LENGTH 6

void der_encode_fulfill(uint8_t *pubkey, uint8_t *sig, uint8_t *fulfill) {
  uint16_t offset = 4;

  fulfill[0] = 0xA4;
  fulfill[1] = 0x64;
  fulfill[2] = 0x80;
  fulfill[3] = 0x20;

  memcpy(fulfill + offset, pubkey, 32);
  offset += 32;

  fulfill[offset++] = 0x81;
  fulfill[offset++] = 0x40;
  memcpy(fulfill + offset, sig, 64);
}

void bigchain_fulfill_and_serialize(BIGCHAIN_TX *tx, uint8_t *json_tx, uint16_t maxlen, uint8_t *sig, uint8_t *pubkey) {
  char fulfillment[256] = {0};
  uint8_t der[256] = {0};

  der_encode_fulfill(pubkey, sig, der);

  bintob64(fulfillment, der, 4 + 32 + 2 + 64);

  uint16_t size = strlen(fulfillment);
  for (uint16_t i = 0; i < size; i++) {
    if (fulfillment[i] == '+')
      fulfillment[i] = '-';
    else if (fulfillment[i] == '/')
      fulfillment[i] = '_';
  }

  for (uint16_t i = size; i > 1; i--) {
    if (fulfillment[i] == '=')
      fulfillment[i] = '\0';
    else
      break;
  }

  memcpy(tx->inputs[0].fulfillment, fulfillment, strlen(fulfillment));

  memset(json_tx, 0, maxlen);
  bigchain_build_json_tx(tx, json_tx);

  uint8_t tx_id[32] = {0};
  sha3_256((const unsigned char *)json_tx, MIN(maxlen, strlen(json_tx)), tx_id);

  for (uint8_t i = 0; i < 32; i++) {
    sprintf(tx->id + i * 2, "%02x", tx_id[i]);
  }

  memset(json_tx, 0, maxlen);
  bigchain_build_json_tx(tx, json_tx);
}

/*
 * Takes a json string, hashes it sha3_256 and signs it with ed25519.
 */
void bigchain_sign_transaction(uint8_t *json_tx, uint16_t len, uint8_t *priv_key, uint8_t *pub_key, uint8_t *sig) {
  uint8_t hash[32] = {0};
  sha3_256((const unsigned char *)json_tx, len, hash);
  ed25519_sign(hash, 32, priv_key, pub_key, sig);
}

char *bigchain_build_condition_uri(char *public_key_base58, char *uri) {
  uint8_t der[256] = {0};

  der_encode_fulfill(pubkey, sig, der);

  bintob64(fulfillment, der, 4 + 32 + 2 + 64);
}

char *bigchain_build_json_outputs(BIGCHAIN_OUTPUT *outputs, uint8_t num_outputs, char *json_obj) {
  char *p = json_obj;
  p = json_arrOpen(p, "outputs");
  for (uint8_t i = 0; i < num_outputs; i++) {
    p = json_objOpen(p, NULL);
    p = json_str(p, "amount", outputs[i].amount);
    p = json_objOpen(p, "condition");

    p = json_objOpen(p, "details");
    p = json_str(p, "public_key", outputs[i].details_public_key);
    p = json_str(p, "type", "ed25519-sha-256");
    p = json_objClose(p);

    p = json_str(p, "uri", DEFAULT_URI);
    // p = json_str(p, "uri", bigchain_build_condition_uri(uri_str , outputs[i].details_public_key);
    p = json_objClose(p);

    p = json_arrOpen(p, "public_keys");
    for (uint8_t j = 0; j < outputs[i].num_public_keys; j++) {
      p = json_str(p, NULL, outputs[i].public_keys[j]);
    }
    p = json_arrClose(p);

    p = json_objClose(p);
  }
  p = json_arrClose(p);
  return p;
}

char *bigchain_build_json_inputs(BIGCHAIN_INPUT *inputs, uint8_t num_inputs, char *json_obj) {
  char *p = json_obj;
  p = json_arrOpen(p, "inputs");
  for (uint8_t i = 0; i < num_inputs; i++) {
    p = json_objOpen(p, NULL);
    if (inputs[i].fulfillment[0] != '\0') {
      p = json_str(p, "fulfillment", inputs[i].fulfillment);
    } else {
      p = json_null(p, "fulfillment");
    }

    if (inputs[i].fulfills[0] != '\0') {
      p = json_objOpen(p, "fulfills");
      p = atoa(p, &(inputs[i].fulfills));
      p = json_objClose(p);
    } else {
      p = json_null(p, "fulfills");
    }

    p = json_arrOpen(p, "owners_before");
    for (uint8_t j = 0; j < inputs[i].num_owners; j++) {
      p = json_str(p, NULL, inputs[i].owners_before[j]);
    }
    p = json_arrClose(p);
    p = json_objClose(p);
  }
  p = json_arrClose(p);
  return p;
}

void bigchain_build_json_tx(BIGCHAIN_TX *tx, char *json_tx) {
  char *p = json_tx;
  p = json_objOpen(p, NULL);

  // ASSET
  p = json_objOpen(p, "asset");
  p = atoa(p, tx->asset);
  p = json_objClose(p);

  // ID
  if (tx->id[0] != '\0') {
    p = json_str(p, "id", tx->id);
  } else {
    p = json_null(p, "id");
  }

  // INPUTS
  p = bigchain_build_json_inputs(tx->inputs, tx->num_inputs, p);

  // METADA
  p = json_objOpen(p, "metadata");
  p = atoa(p, tx->metadata);
  p = json_objClose(p);

  // OPERATION
  p = json_str(p, "operation", tx->operation);

  // OUTPUTS
  p = bigchain_build_json_outputs(tx->outputs, tx->num_outputs, p);

  // VERSION
  p = json_str(p, "version", tx->version);
  p = json_objClose(p);
  p = json_end(p);
}

/* 'operation'can be either 'C' or 'T' for CREATE and TRANSFER respectively.
 * when 'operation' is CREATE then 'asset' can be arbitrary. (The keys on the JSON must be in alphabetical order)
 * but when 'operation' is TRANSFER then 'asset' must be the transaction id of the asset which is to be tranfered.
 */
void prepare_tx(BIGCHAIN_TX *tx, const char operation, char *asset, char *metadata, char *base_pubkey) {
  // Fill input struct
  memset(tx->inputs, 0, sizeof(BIGCHAIN_INPUT));
  memcpy(tx->inputs[0].owners_before[0], base_pubkey, strlen(base_pubkey));
  tx->inputs[0].num_owners = 1;
  tx->num_inputs = 1;

  if (operation == 'C') {
    memcpy(tx->operation, "CREATE", strlen("CREATE"));
    memcpy(tx->asset, asset, strlen(asset));
  } else if (operation == 'T') {
    memcpy(tx->operation, "TRANSFER", strlen("TRANSFER"));
    memcpy(tx->inputs[0].fulfills, FULFILL_PREFIX , FULFILL_PREFIX_LENGTH );
    memcpy(tx->inputs[0].fulfills + FULFILL_PREFIX_LENGTH , asset , TX_ID_LENGTH );
    memcpy(tx->inputs[0].fulfills + FULFILL_PREFIX_LENGTH + TX_ID_LENGTH , "\"\0", 2);
    memcpy(tx->asset, ASSET_ID , ASSET_ID_LENGTH);
    memcpy(tx->asset + ASSET_ID_LENGTH, asset, TX_ID_LENGTH );
    memcpy(tx->asset + ASSET_ID_LENGTH + TX_ID_LENGTH , "\"\0", 2);
  }

  memcpy(tx->metadata, metadata, strlen(metadata));
  memcpy(tx->version, BDB_VERSION, strlen(BDB_VERSION));

  // Fill output struct
  memset(tx->outputs, 0, sizeof(BIGCHAIN_OUTPUT));
  tx->outputs[0].amount[0] = '1';
  memcpy(tx->outputs[0].details_public_key, base_pubkey, strlen(base_pubkey));
  memcpy(tx->outputs[0].public_keys[0], base_pubkey, strlen(base_pubkey));
  tx->outputs[0].num_public_keys = 1;
  tx->num_outputs = 1;
}

void fulfill_tx(BIGCHAIN_TX *tx, char *tx_id, uint8_t *priv_key, uint8_t *pub_key, uint8_t *json, uint16_t maxlen) {
  uint8_t sig[140] = {0};
  bigchain_build_json_tx(tx, json);
  if (!memcmp(tx->operation, "TRANSFER", 8)) {
    // For TRANSFER the json string must be concatenated with the input tx_id and the output_index
    strcat(json, tx_id);
    strcat(json, "0");
  }
  bigchain_sign_transaction((uint8_t *)json, strlen(json), (uint8_t *)priv_key, (uint8_t *)pub_key, (uint8_t *)sig);
  bigchain_fulfill_and_serialize(tx, (uint8_t *)json, maxlen, (uint8_t *)sig, (uint8_t *)pub_key);
}