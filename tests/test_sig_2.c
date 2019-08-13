#include "tests.h"

#define TEST_CREATE_ASSET	"\"data\":{\"bicycle\":{\"manufacturer\":\"bkfab\",\"serial_number\":\"abcd1234\"}}"
#define TEST_TRANSFER_ASSET "c81699a3713b36ac7b06b48bd0dbe2fb394428e1600d7e60c41207a3dae7ae53"
#define TEST_METADATA "\"planet\":\"earth\""
#define TEST_OPERATION_CREATE 'C'
#define TEST_OPERATION_TRANSFER 'T'
#define TEST_VERSION "2.0"
#define TEST_OWNER_BEFORE "6k17YAe4mJsqwPNgGj9tE1aMYLhpeqgJJivqaHeTTU5a"
#define TEST_INPUTS "\"inputs\":[{\"fulfillment\":null,\"fulfills\":null,\"owners_before\":[\"6k17YAe4mJsqwPNgGj9tE1aMYLhpeqgJJivqaHeTTU5a\"]}]"
#define TEST_OUTPUTS "\"outputs\":[{\"amount\":\"1\",\"condition\":{\"details\":{\"public_key\":\"6k17YAe4mJsqwPNgGj9tE1aMYLhpeqgJJivqaHeTTU5a\",\"type\":\"ed25519-sha-256\"},\"uri\":\"ni:///sha-256;Vta3W592Kt_Y2ljfHEDZLd4OCZPHLiHyCgjNNKrrNwo?fpt=ed25519-sha-256&cost=131072\"},\"public_keys\":[\"6k17YAe4mJsqwPNgGj9tE1aMYLhpeqgJJivqaHeTTU5a\"]}]"
#define TEST_DETAILS_PUBLIC_KEY "6k17YAe4mJsqwPNgGj9tE1aMYLhpeqgJJivqaHeTTU5a"
#define TEST_PUBLIC_KEY_0 "6k17YAe4mJsqwPNgGj9tE1aMYLhpeqgJJivqaHeTTU5a"
static const char* tx_json =
  "{\"asset\":{\"data\":{\"bicycle\":{\"manufacturer\":\"bkfab\",\"serial_number\":\"abcd1234\"}}},\"id\":null,\"inputs\":[{\"fulfillment\":null,\"fulfills\":null,\"owners_before\":[\"6k17YAe4mJsqwPNgGj9tE1aMYLhpeqgJJivqaHeTTU5a\"]}],\"metadata\":{\"planet\":\"earth\"},\"operation\":\"CREATE\",\"outputs\":[{\"amount\":\"1\",\"condition\":{\"details\":{\"public_key\":\"6k17YAe4mJsqwPNgGj9tE1aMYLhpeqgJJivqaHeTTU5a\",\"type\":\"ed25519-sha-256\"},\"uri\":\"ni:///sha-256;Vta3W592Kt_Y2ljfHEDZLd4OCZPHLiHyCgjNNKrrNwo?fpt=ed25519-sha-256&cost=131072\"},\"public_keys\":[\"6k17YAe4mJsqwPNgGj9tE1aMYLhpeqgJJivqaHeTTU5a\"]}],\"version\":\"2.0\"}";

static const char* C_tx_json =
  "{\"asset\":{\"data\":{\"bicycle\":{\"manufacturer\":\"bkfab\",\"serial_number\":\"abcd1234\"}}},\"id\":null,\"inputs\":[{\"fulfillment\":\"pGSAIFF_tz4ZAIVevbwN2MkUqGDA61cu-NZmETju56pw2KFGgUCmgJRLPtDZ-lDNorv6mS7lViZ5TkNI4lW_CSc0Ox3W3Gf4Bu7_kVV4t0QF7vUvArdrk49HEVvjGngWDIBZTC4N\",\"fulfills\":null,\"owners_before\":[\"6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9\"]}],\"metadata\":{\"planet\":\"earth\"},\"operation\":\"CREATE\",\"outputs\":[{\"amount\":\"1\",\"condition\":{\"details\":{\"public_key\":\"6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9\",\"type\":\"ed25519-sha-256\"},\"uri\":\"ni:///sha-256;Vta3W592Kt_Y2ljfHEDZLd4OCZPHLiHyCgjNNKrrNwo?fpt=ed25519-sha-256&cost=131072\"},\"public_keys\":[\"6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9\"]}],\"version\":\"2.0\"}";
static const char* T_tx_json =
  "{\"asset\":{\"id\":\"c81699a3713b36ac7b06b48bd0dbe2fb394428e1600d7e60c41207a3dae7ae53\"},\"id\":\"c81699a3713b36ac7b06b48bd0dbe2fb394428e1600d7e60c41207a3dae7ae53\",\"inputs\":[{\"fulfillment\":\"pGSAIFF_tz4ZAIVevbwN2MkUqGDA61cu-NZmETju56pw2KFGgUDQL6TudNolIANPFj6A47CiswYHYXsHE2wUrWKHkA2IsuVOjK0ajZXQOqWTWFEcXgF2F8cqqTjOiDeAUGPP_xQE\",\"fulfills\":{\"output_index\":0,\"transaction_id\":\"c81699a3713b36ac7b06b48bd0dbe2fb394428e1600d7e60c41207a3dae7ae53\"},\"owners_before\":[\"6k17YAe4mJsqwPNgGj9tE1aMYLhpeqgJJivqaHeTTU5a\"]}],\"metadata\":{\"planet\":\"earth\"},\"operation\":\"TRANSFER\",\"outputs\":[{\"amount\":\"1\",\"condition\":{\"details\":{\"public_key\":\"6k17YAe4mJsqwPNgGj9tE1aMYLhpeqgJJivqaHeTTU5a\",\"type\":\"ed25519-sha-256\"},\"uri\":\"ni:///sha-256;Vta3W592Kt_Y2ljfHEDZLd4OCZPHLiHyCgjNNKrrNwo?fpt=ed25519-sha-256&cost=131072\"},\"public_keys\":[\"6k17YAe4mJsqwPNgGj9tE1aMYLhpeqgJJivqaHeTTU5a\"]}],\"version\":\"2.0\"}";



char privkey[] = {'\x97', '\xd8', '\xda', '\x84', '\x95', '\xac', '\x9f', '\xad', '\x47', '\xf8', '\x2e', '\x37', '\x14', '\x14', '\x01', '\xbb', '\x2e', '\x44', '\x5a', '\xaf', '\x67', '\x3f', '\xed', '\x52', '\x52', '\x3c', '\x5e', '\x96', '\x03', '\x8b', '\xe9', '\xe2'};
char pubkey[]  = {'\x51', '\x7f', '\xb7', '\x3e', '\x19', '\x00', '\x85', '\x5e', '\xbd', '\xbc', '\x0d', '\xd8', '\xc9', '\x14', '\xa8', '\x60', '\xc0', '\xeb', '\x57', '\x2e', '\xf8', '\xd6', '\x66', '\x11', '\x38', '\xee', '\xe7', '\xaa', '\x70', '\xd8', '\xa1', '\x46'};
char base58_pubkey[] = "6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9";

void prepare_inputs(BIGCHAIN_INPUT *inputs, uint8_t *num_inputs) {
  memset(inputs, 0, sizeof(BIGCHAIN_INPUT));

  // Fill input struct
  memcpy(inputs[0].owners_before[0], TEST_OWNER_BEFORE, strlen(TEST_OWNER_BEFORE));
  inputs[0].num_owners = 1;
  *num_inputs = 1;
}

void prepare_outputs(BIGCHAIN_OUTPUT *outputs, uint8_t *num_outputs) {
  memset(outputs, 0, sizeof(BIGCHAIN_OUTPUT));

  // Fill output struct
  outputs[0].amount[0] = '1';
  memcpy(outputs[0].details_public_key, TEST_DETAILS_PUBLIC_KEY, strlen(TEST_DETAILS_PUBLIC_KEY));
  memcpy(outputs[0].public_keys[0], TEST_PUBLIC_KEY_0, strlen(TEST_PUBLIC_KEY_0));
  outputs[0].num_public_keys = 1;
  *num_outputs = 1;
}



void test_sig(void) {
  uint8_t hash[32] = {0}, sig[640] = {0};

  // Hashing 256
  sha3_256((const unsigned char*)tx_json, strlen(tx_json), hash);
  uint8_t cmp_hash[] = {'\xbb','\xb7','\xdf','\xd5','\x1b','\xe3','\xbb','\x66','\x91','\x80','\xf4','\x8b','\x37','\xf1','\x86','\x98','\xb8','\x2c','\xfc','\x40','\xea','\x4c','\xa4','\x95','\x2a','\x1c','\x82','\xeb','\x11','\xff','\x5f','\xb5'};
  TEST_ASSERT_EQUAL(0, memcmp(cmp_hash, hash, 32));

  // ED25519 signing
  uint8_t cmp_sig[] = {'\x8d','\x31','\xa2','\x89','\xe9','\xa5','\x74','\xdd','\xa7','\x0d','\x06','\xa7','\x18','\x09','\xf7','\xc4','\x58','\x07','\x0d','\x41','\xd8','\x71','\x4b','\x5a','\x3b','\x6d','\xe6','\xfa','\x22','\xca','\x71','\x7e','\xdd','\x18','\x9a','\x0b','\xf6','\xfa','\x85','\x25','\x38','\x50','\x1e','\x4f','\xa2','\x9e','\x3f','\xae','\x70','\x2e','\xcf','\xd2','\x39','\x22','\xab','\x2d','\x16','\x80','\x20','\x0f','\xd4','\xef','\x99','\x02'};
  ed25519_sign(hash, 32, (const unsigned char*) privkey, (const unsigned char*) pubkey, sig);
  TEST_ASSERT_EQUAL(0, memcmp(cmp_sig, sig, 64));

  bigchain_sign_transaction((uint8_t*)tx_json, strlen(tx_json), (uint8_t*)privkey, (uint8_t*)pubkey, sig);
  TEST_ASSERT_EQUAL(0, memcmp(cmp_sig, sig, 64));
}

void  test_bigchain_build_json_outputs(void) {
  BIGCHAIN_OUTPUT outputs[1];
  char outputs_json[512] = {0};
  uint8_t num = 0;
  prepare_outputs(outputs, &num );

  // Builds output json from structure
  bigchain_build_json_outputs(outputs, 1, outputs_json);
  TEST_ASSERT_EQUAL(0, memcmp(TEST_OUTPUTS, outputs_json, strlen(TEST_OUTPUTS)));

}

void test_bigchain_build_json_inputs(void) {
  BIGCHAIN_INPUT inputs[1];
  char inputs_json[256] = {0};
  uint8_t num = 0;

  prepare_inputs(inputs, &num);

  // Builds input json from structure
  bigchain_build_json_inputs(inputs, 1, inputs_json);
  TEST_ASSERT_EQUAL(0, memcmp(TEST_INPUTS, inputs_json, strlen(TEST_INPUTS)));

}

void test_bigchain_build_json_tx(void) {
  BIGCHAIN_TX tx;
  char json[6000] = {0};

  memset(&tx, 0, sizeof(BIGCHAIN_TX));
  prepare_test_tx(&tx);
  bigchain_build_json_tx(&tx, json);

  TEST_ASSERT_EQUAL(0, memcmp(tx_json, json, 572));

  char sig[128] = {0};
  bigchain_sign_transaction((uint8_t*)json, strlen(json), (uint8_t*)privkey, (uint8_t*)pubkey,  (uint8_t*)sig);
  bigchain_fulfill_and_serialize(&tx, (uint8_t*)json, 6000, (uint8_t*)sig, (uint8_t*)pubkey);

  // Base64
  char fullfil[] = "pGSAIFF_tz4ZAIVevbwN2MkUqGDA61cu-NZmETju56pw2KFGgUDwNZHVN27d1oQuUrAF3ZRz3sn0bE0Cat1oScQN-ZGQpMW8uzT363yR6FCjb4tbLNC32OdHyMGZqCgkiCjCed0H";
  uint8_t transaction_id[] = "78dde9669c5fe68e1470aca7ebb70ea5f526d9ee96597922b4ccca2388bcdf41";

  TEST_ASSERT_EQUAL(0, memcmp(tx.inputs->fulfillment, fullfil, 137));
  TEST_ASSERT_EQUAL(0, memcmp(tx.id, transaction_id, 64));

}

void test_bigchain_create_tx() {
    BIGCHAIN_TX tx;
    char json[3000] = {0};
    memset(&tx, 0, sizeof(BIGCHAIN_TX));
    prepare_tx(&tx, TEST_OPERATION_CREATE , TEST_CREATE_ASSET , TEST_METADATA , base58_pubkey );
    fulfill_tx(&tx, 0, privkey, pubkey, json, 3000 );
    TEST_ASSERT_EQUAL(0, memcmp(C_tx_json, json, 3000));
}

void test_bigchain_transfer_tx() {
    BIGCHAIN_TX tx;
    char json[3000] = {0};
    memset(&tx, 0, sizeof(BIGCHAIN_TX));
    prepare_tx(&tx, TEST_OPERATION_TRANSFER , TEST_TRANSFER_ASSET , TEST_METADATA , base58_pubkey ) ;
    fulfill_tx(&tx, 0, privkey, pubkey, json, 3000 );
    TEST_ASSERT_EQUAL(0, memcmp(T_tx_json, json, 3000));
}


int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_sig);
  RUN_TEST(test_bigchain_create_tx);
  RUN_TEST(test_bigchain_transfer_tx);
  return UNITY_END();
}
