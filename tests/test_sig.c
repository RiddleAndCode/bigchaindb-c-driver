#include "tests.h"

#define TEST_CREATE_ASSET	"{\"asset\":{\"data\":{\"bicycle\":{\"manufacturer\":\"bkfab\",\"serial_number\":\"abcd1234\"}}}}"
#define TEST_ASSET_ID "c81699a3713b36ac7b06b48bd0dbe2fb394428e1600d7e60c41207a3dae7ae53"
#define TEST_TRANSFER_ASSET "{\"asset\":{\"id\":\"c81699a3713b36ac7b06b48bd0dbe2fb394428e1600d7e60c41207a3dae7ae53\"}}"
#define TEST_METADATA "{\"metadata\":{\"planet\":\"earth\"}}"

#define TEST_OPERATION_CREATE 'C'
#define TEST_OPERATION_TRANSFER 'T'
#define TEST_VERSION "2.0"
#define TEST_OWNER_BEFORE "6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9"
#define TEST_INPUTS_CREATE "\"inputs\":[{\"fulfillment\":null,\"fulfills\":null,\"owners_before\":[\"6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9\"]}]"
#define TEST_INPUTS_TRANSFER "\"inputs\":[{\"fulfillment\":null,\"fulfills\":{\"output_index\":0,\"transaction_id\":\"c81699a3713b36ac7b06b48bd0dbe2fb394428e1600d7e60c41207a3dae7ae53\"},\"owners_before\":[\"6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9\"]}]"
#define TEST_OUTPUTS "\"outputs\":[{\"amount\":\"1\",\"condition\":{\"details\":{\"public_key\":\"6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9\",\"type\":\"ed25519-sha-256\"},\"uri\":\"ni:///sha-256;WVm8YmcTjv05Osmho-Hc7o6N2Hg0YvgsKdaidCaRb0Q?fpt=ed25519-sha-256&cost=131072\"},\"public_keys\":[\"6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9\"]}]"
#define TEST_DETAILS_PUBLIC_KEY "6k17YAe4mJsqwPNgGj9tE1aMYLhpeqgJJivqaHeTTU5a"

static const char* C_tx_json_0 =
  "{\"asset\":{\"data\":{\"bicycle\":{\"manufacturer\":\"bkfab\",\"serial_number\":\"abcd1234\"}}},\"id\":null,\"inputs\":[{\"fulfillment\":null,\"fulfills\":null,\"owners_before\":[\"6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9\"]}],\"metadata\":{\"planet\":\"earth\"},\"operation\":\"CREATE\",\"outputs\":[{\"amount\":\"1\",\"condition\":{\"details\":{\"public_key\":\"6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9\",\"type\":\"ed25519-sha-256\"},\"uri\":\"ni:///sha-256;WVm8YmcTjv05Osmho-Hc7o6N2Hg0YvgsKdaidCaRb0Q?fpt=ed25519-sha-256&cost=131072\"},\"public_keys\":[\"6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9\"]}],\"version\":\"2.0\"}";

static const char* C_tx_json =
  "{\"asset\":{\"data\":{\"bicycle\":{\"manufacturer\":\"bkfab\",\"serial_number\":\"abcd1234\"}}},\"id\":\"c81699a3713b36ac7b06b48bd0dbe2fb394428e1600d7e60c41207a3dae7ae53\",\"inputs\":[{\"fulfillment\":\"pGSAIFF_tz4ZAIVevbwN2MkUqGDA61cu-NZmETju56pw2KFGgUCmgJRLPtDZ-lDNorv6mS7lViZ5TkNI4lW_CSc0Ox3W3Gf4Bu7_kVV4t0QF7vUvArdrk49HEVvjGngWDIBZTC4N\",\"fulfills\":null,\"owners_before\":[\"6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9\"]}],\"metadata\":{\"planet\":\"earth\"},\"operation\":\"CREATE\",\"outputs\":[{\"amount\":\"1\",\"condition\":{\"details\":{\"public_key\":\"6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9\",\"type\":\"ed25519-sha-256\"},\"uri\":\"ni:///sha-256;WVm8YmcTjv05Osmho-Hc7o6N2Hg0YvgsKdaidCaRb0Q?fpt=ed25519-sha-256&cost=131072\"},\"public_keys\":[\"6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9\"]}],\"version\":\"2.0\"}";
static const char* T_tx_json =
  "{\"asset\":{\"id\":\"c81699a3713b36ac7b06b48bd0dbe2fb394428e1600d7e60c41207a3dae7ae53\"},\"id\":\"5123d5c81b2af437b7ef86c74a151ac586e63d7d8070ec48775343af2ae454c7\",\"inputs\":[{\"fulfillment\":\"pGSAIFF_tz4ZAIVevbwN2MkUqGDA61cu-NZmETju56pw2KFGgUDQL6TudNolIANPFj6A47CiswYHYXsHE2wUrWKHkA2IsuVOjK0ajZXQOqWTWFEcXgF2F8cqqTjOiDeAUGPP_xQE\",\"fulfills\":{\"output_index\":0,\"transaction_id\":\"c81699a3713b36ac7b06b48bd0dbe2fb394428e1600d7e60c41207a3dae7ae53\"},\"owners_before\":[\"6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9\"]}],\"metadata\":{\"planet\":\"earth\"},\"operation\":\"TRANSFER\",\"outputs\":[{\"amount\":\"1\",\"condition\":{\"details\":{\"public_key\":\"6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9\",\"type\":\"ed25519-sha-256\"},\"uri\":\"ni:///sha-256;WVm8YmcTjv05Osmho-Hc7o6N2Hg0YvgsKdaidCaRb0Q?fpt=ed25519-sha-256&cost=131072\"},\"public_keys\":[\"6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9\"]}],\"version\":\"2.0\"}";



char privkey[] = {'\x80', '\x8c', '\xeb', '\x78', '\x8f', '\x3e', '\x2b', '\xe7', '\x67', '\x7a', '\xd6', '\x5f', '\x96', '\x93', '\xb2', '\x3a', '\x64', '\xd7', '\x00', '\xb3', '\x4b', '\xcc', '\xad', '\xdd', '\x03', '\xf2', '\x2d', '\x3e', '\x32', '\x35', '\xf1', '\x1d'};
char pubkey[]  = {'\x51', '\x7f', '\xb7', '\x3e', '\x19', '\x00', '\x85', '\x5e', '\xbd', '\xbc', '\x0d', '\xd8', '\xc9', '\x14', '\xa8', '\x60', '\xc0', '\xeb', '\x57', '\x2e', '\xf8', '\xd6', '\x66', '\x11', '\x38', '\xee', '\xe7', '\xaa', '\x70', '\xd8', '\xa1', '\x46'};
char base58_pubkey[] = "6V8ycJdv7kPiXpAhCgk6YPrmc35yMnCCvxP4YnGzvhp9";

void prepare_inputs(char operation, BIGCHAIN_INPUT *inputs, uint8_t *num_inputs) {
  memset(inputs, 0, sizeof(BIGCHAIN_INPUT));

  // Fill input struct
  memcpy(inputs[0].owners_before[0], TEST_OWNER_BEFORE, strlen(TEST_OWNER_BEFORE));
  if(operation == 'T') {
    inputs[0].fulfills.output_index = 0;
    strcpy(inputs[0].fulfills.transaction_id, "c81699a3713b36ac7b06b48bd0dbe2fb394428e1600d7e60c41207a3dae7ae53");
  } else if(operation != 'C') {
    return;
  }
  inputs[0].num_owners = 1;
  *num_inputs = 1;
}

void prepare_outputs(BIGCHAIN_OUTPUT *outputs, uint8_t *num_outputs) {
  memset(outputs, 0, sizeof(BIGCHAIN_OUTPUT));

  // Fill output struct
  outputs[0].amount[0] = '1';
  memcpy(outputs[0].details_public_key, TEST_OWNER_BEFORE, strlen(TEST_OWNER_BEFORE));
  memcpy(outputs[0].public_keys[0], TEST_OWNER_BEFORE, strlen(TEST_OWNER_BEFORE));
  outputs[0].num_public_keys = 1;
  *num_outputs = 1;
}



void test_sig(void) {
  uint8_t hash[32] = {0}, sig[640] = {0};

  // Hashing 256
  sha3_256((const unsigned char*)C_tx_json_0, strlen(C_tx_json_0), hash);
  uint8_t cmp_hash[] = {'\x4a','\x48','\xc5','\x7b','\xb2','\x02','\xf7','\x00','\x40','\xfe','\x84','\xf2','\x51','\x6b','\xd7','\x0c','\x1d','\x66','\x40','\x12','\xac','\x71','\x8f','\x01','\xb0','\x1a','\x26','\x37','\x69','\x26','\x18','\x03'};
  TEST_ASSERT_EQUAL_INT8_ARRAY(cmp_hash, hash, 32);

  // ED25519 signing
  uint8_t cmp_sig[] = {'\xa6','\x80','\x94','\x4b','\x3e','\xd0','\xd9','\xfa','\x50','\xcd','\xa2','\xbb','\xfa','\x99','\x2e','\xe5','\x56','\x26','\x79','\x4e','\x43','\x48','\xe2','\x55','\xbf','\x9','\x27','\x34','\x3b','\x1d','\xd6','\xdc','\x67','\xf8','\x06','\xee','\xff','\x91','\x55','\x78','\xb7','\x44','\x5','\xee','\xf5','\x2f','\x2','\xb7','\x6b','\x93','\x8f','\x47','\x11','\x5b','\xe3','\x1a','\x78','\x16','\x0c','\x80','\x59','\x4c','\x2e','\x0d'};
  ed25519_sign(hash, 32, (const unsigned char*) privkey, (const unsigned char*) pubkey, sig);
  TEST_ASSERT_EQUAL_INT8_ARRAY(cmp_sig, sig, 64);

  bigchain_sign_transaction((uint8_t*)C_tx_json_0, strlen(C_tx_json_0), (uint8_t*)privkey, (uint8_t*)pubkey, sig);
  TEST_ASSERT_EQUAL_INT8_ARRAY(cmp_sig, sig, 64);
}

void  test_bigchain_build_json_outputs(void) {
  BIGCHAIN_OUTPUT outputs[1];
  char outputs_json[512] = {0};
  uint8_t num = 0;
  prepare_outputs(outputs, &num );

  // Builds output json from structure
  bigchain_build_json_outputs(outputs, 1, outputs_json);
  // TEST_ASSERT_EQUAL(0, memcmp(TEST_OUTPUTS, outputs_json, strlen(TEST_OUTPUTS)));
  TEST_ASSERT_EQUAL_STRING_LEN(TEST_OUTPUTS, outputs_json, strlen(TEST_OUTPUTS));
}

void test_bigchain_build_json_inputs(void) {
  BIGCHAIN_INPUT inputs[2];
  char inputs_json_CREATE[256] = {0};
  char inputs_json_TRANSFER[256] = {0};
  uint8_t num = 0;

  prepare_inputs(TEST_OPERATION_CREATE, &inputs[0], &num );
  prepare_inputs(TEST_OPERATION_TRANSFER, &inputs[1], &num );

  // Builds input json from structure
  bigchain_build_json_inputs(&inputs[0], 1, inputs_json_CREATE);
  bigchain_build_json_inputs(&inputs[1], 1, inputs_json_TRANSFER);

  TEST_ASSERT_EQUAL_STRING_LEN(TEST_INPUTS_CREATE, inputs_json_CREATE, strlen(TEST_INPUTS_CREATE) );
  TEST_ASSERT_EQUAL_STRING_LEN(TEST_INPUTS_TRANSFER, inputs_json_TRANSFER, strlen(TEST_INPUTS_TRANSFER) );
}

void test_bigchain_create_tx() {
  BIGCHAIN_TX tx;
  char json[3000] = {0};
  memset(&tx, 0, sizeof(BIGCHAIN_TX));
  prepare_tx(&tx, TEST_OPERATION_CREATE, TEST_CREATE_ASSET, TEST_METADATA, base58_pubkey );
  fulfill_tx(&tx, 0, privkey, pubkey, json, 3000 , 0);
  TEST_ASSERT_EQUAL_STRING_LEN(C_tx_json, json, sizeof(C_tx_json));
}

void test_bigchain_transfer_tx() {
  BIGCHAIN_TX tx;
  char json[2000] = {0};
  memset(&tx, 0, sizeof(BIGCHAIN_TX));
  prepare_tx(&tx, TEST_OPERATION_TRANSFER, TEST_TRANSFER_ASSET, TEST_METADATA, base58_pubkey ) ;
  fulfill_tx(&tx, TEST_ASSET_ID, privkey, pubkey, json, 2000, 0);
  TEST_ASSERT_EQUAL_STRING_LEN(T_tx_json, json, sizeof(T_tx_json));
}

void test_b58tobin() {
  size_t binsz = 32 ;
  uint8_t pubkey_test[32] = {0};
  b58tobin( pubkey_test, &binsz, base58_pubkey);
  memcmp( pubkey, pubkey_test, 32 );
  //TEST_ASSERT_EQUAL( 0, memcmp( pubkey , pubkey_test , 32 ) );
  TEST_ASSERT_EQUAL_INT8_ARRAY( pubkey, pubkey_test, 32 );
}

void test_bigchain_parse_json() {
  BIGCHAIN_TX tx;
  char json[3000] = {0};
  memset(&tx, 0, sizeof(BIGCHAIN_TX));
  memcpy(json, C_tx_json, strlen(C_tx_json));
  bigchain_parse_json(json, &tx);
  TEST_ASSERT_EQUAL_STRING("c81699a3713b36ac7b06b48bd0dbe2fb394428e1600d7e60c41207a3dae7ae53", tx.id);
  // TODO: add more assertion
}

int main(void) {
  test_b58tobin();
  UNITY_BEGIN();
  RUN_TEST(test_sig);
  RUN_TEST(test_bigchain_build_json_inputs);
  RUN_TEST(test_bigchain_build_json_outputs);
  RUN_TEST(test_bigchain_create_tx);
  RUN_TEST(test_bigchain_transfer_tx);
  RUN_TEST(test_b58tobin);
  RUN_TEST(test_bigchain_parse_json);
  return UNITY_END();
}
