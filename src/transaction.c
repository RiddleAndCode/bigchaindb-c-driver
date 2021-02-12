#include "memzero.h"
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

// copy from json maker
static char* chtoa(char* dest, char ch) {
    *dest   = ch;
    *++dest = '\0';
    return dest;
}

static char* atoesc(char* dest, const char *src) {
    for( ; *src != '\0'; ++dest, ++src ) {
        *dest = *src;
    }
    *dest = '\0';
    return dest;
}

static char* atoa(char* dest, const char * src) {
    for( ; *src != '\0'; ++dest, ++src )
        *dest = *src;
    *dest = '\0';
    return dest;
}

static char* strname(char* dest, const char * name) {
    dest = chtoa( dest, '\"' );
    if ( NULL != name ) {
        dest = atoa(dest, name);
        dest = atoa(dest, "\":\"");
    }
    return dest;
}

static char* bcdb_json_str(char* dest, char const* name, char const* value) {
    dest = strname(dest, name);
    dest = atoesc(dest, value);
    dest = atoa(dest, "\",");
    return dest;
}

void der_encode_fulfill(uint8_t *pub_key, uint8_t *sig, uint8_t *fulfill) {
  uint16_t offset = 4;

  fulfill[0] = 0xA4;
  fulfill[1] = 0x64;
  fulfill[2] = 0x80;
  fulfill[3] = 0x20;

  memcpy(fulfill + offset, pub_key, 32);
  offset += 32;

  fulfill[offset++] = 0x81;
  fulfill[offset++] = 0x40;
  memcpy(fulfill + offset, sig, 64);
}

void bigchain_fulfill(BIGCHAIN_TX *tx, uint8_t *sig, uint8_t *pub_key, uint8_t input_index) {
  char fulfillment[256] = {0};
  uint8_t der[256] = {0};

  der_encode_fulfill(pub_key, sig, der);
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

  memcpy(tx->inputs[input_index].fulfillment, fulfillment, strlen(fulfillment));
}

void bigchain_serialize(BIGCHAIN_TX *tx, uint8_t *json_tx, uint16_t maxlen) {
  memset(json_tx, 0, maxlen);
  bigchain_build_json_tx(tx, (char*)json_tx);

  uint8_t tx_id[32] = {0};
  sha3_256((const unsigned char *)json_tx, MIN(maxlen, strlen((char*)json_tx)), tx_id);

  for (uint8_t i = 0; i < 32; i++) {
    sprintf(tx->id + i * 2, "%02x", tx_id[i]);
  }

  memset(json_tx, 0, maxlen);
  bigchain_build_json_tx(tx, (char*)json_tx);
}

void bigchain_fulfill_and_serialize(BIGCHAIN_TX *tx, uint8_t *json_tx, uint16_t maxlen, uint8_t *sig, uint8_t *pub_key, uint8_t input_index) {
  bigchain_fulfill(tx, sig, pub_key, input_index);
  bigchain_serialize(tx, json_tx, maxlen);
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
  uint8_t der[38] = {0};
  uint8_t hash[32] = {0};
  char fingerp_base64[50] = {0};
  uint8_t pubkey[32] = {0};
  size_t binsz = 32 ;
  strcpy( uri, "ni:///sha-256;" );
  b58tobin(pubkey, &binsz, public_key_base58);

  der[0] = 0x30;
  der[1] = 0x22;
  der[2] = 0x80;
  der[3] = 0x20; //content-length = 32
  memcpy(der + 4, pubkey, 32);
  sha256_Raw(der, 36, hash);
  bintob64(fingerp_base64, hash, 32);
  uint8_t size = strlen(fingerp_base64);
  for (uint16_t i = 0; i < size; i++) {
    if (fingerp_base64[i] == '+')
      fingerp_base64[i] = '-';
    else if (fingerp_base64[i] == '/')
      fingerp_base64[i] = '_';
  }

  for (uint16_t i = size; i > 1; i--) {
    if (fingerp_base64[i] == '=')
      fingerp_base64[i] = '\0';
    else
      break;
  }
  memcpy(uri + 14, fingerp_base64, 43);
  strcat(uri, "?fpt=ed25519-sha-256&cost=131072");
  return uri;
}

char *bigchain_build_json_outputs(BIGCHAIN_OUTPUT *outputs, uint8_t num_outputs, char *json_obj) {
  char *p = json_obj;
  p = json_arrOpen(p, "outputs");
  char uri_str[90] = {0};

  for (uint8_t i = 0; i < num_outputs; i++) {
    p = json_objOpen(p, NULL);
    p = json_str(p, "amount", outputs[i].amount);
    p = json_objOpen(p, "condition");
    p = json_objOpen(p, "details");
    // p = bcdb_json_str(p, "public_key", outputs[i].details_public_key);
    // p = bcdb_json_str(p, "type", "ed25519-sha-256");
    p = json_str(p, "public_key", outputs[i].details_public_key);
    p = json_str(p, "type", "ed25519-sha-256");
    p = json_objClose(p);

    // p = bcdb_json_str(p, "uri", bigchain_build_condition_uri(outputs[i].details_public_key, uri_str));
    p = json_str(p, "uri", bigchain_build_condition_uri(outputs[i].details_public_key, uri_str));
    p = json_objClose(p);

    p = json_arrOpen(p, "public_keys");
    for (uint8_t j = 0; j < outputs[i].num_public_keys; j++) {
      // p = bcdb_json_str(p, NULL, outputs[i].public_keys[j]);
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
      // p = bcdb_json_str(p, "fulfillment", inputs[i].fulfillment);
      p = json_str(p, "fulfillment", inputs[i].fulfillment);
    } else {
      p = json_null(p, "fulfillment");
    }

    if (inputs[i].fulfills[0] != '\0') {
      p = json_objOpen(p, "fulfills");
      // p = atoa(p, &(inputs[i].fulfills));
      p = atoa(p, (const char*)(inputs[i].fulfills));
      p = json_objClose(p);
    } else {
      p = json_null(p, "fulfills");
    }

    p = json_arrOpen(p, "owners_before");
    for (uint8_t j = 0; j < inputs[i].num_owners; j++) {
      p = json_str(p, NULL, inputs[i].owners_before[j]);
      // p = bcdb_json_str(p, NULL, inputs[i].owners_before[j]);
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
    // p = bcdb_json_str(p, "id", tx->id);
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
  // p = bcdb_json_str(p, "operation", tx->operation);
  p = json_str(p, "operation", tx->operation);

  // OUTPUTS
  p = bigchain_build_json_outputs(tx->outputs, tx->num_outputs, p);

  // VERSION
  // p = bcdb_json_str(p, "version", tx->version);
  p = json_str(p, "version", tx->version);
  p = json_objClose(p);
  p = json_end(p);
}

/* 
 * prepare_tx
 * 'operation'can be either 'C' or 'T' for CREATE and TRANSFER respectively.
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
    memcpy(tx->inputs[0].fulfills, FULFILL_PREFIX, FULFILL_PREFIX_LENGTH );
    memcpy(tx->inputs[0].fulfills + FULFILL_PREFIX_LENGTH, asset, TX_ID_LENGTH );
    memcpy(tx->inputs[0].fulfills + FULFILL_PREFIX_LENGTH + TX_ID_LENGTH, "\"\0", 2);
    memcpy(tx->asset, ASSET_ID, ASSET_ID_LENGTH);
    memcpy(tx->asset + ASSET_ID_LENGTH, asset, TX_ID_LENGTH );
    memcpy(tx->asset + ASSET_ID_LENGTH + TX_ID_LENGTH, "\"\0", 2);
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


/* 
*  No spaces in the json structure: {"key 1": "value a"} becomes '{"key 1":"value a"}'
*  Keys should be alphabetically ordered: {"key 1":"value a","A key":"A value"} becomes {"A key":"A value","key 1":"value a"}
*  No numbers fields, only strings {"key 1":2.3123} becomes {"key 1":"2.3123"}
*/

/* 
* fulfill_tx
* inputs: tx, tx_id(same as 'asset' above, only needed for TRANSFER) , priv_key, pub_key(public key in hexadecimal base) , maxlen(maximum length of string)
* output: json
*/
void fulfill_tx(BIGCHAIN_TX *tx, char *tx_id, uint8_t *priv_key, uint8_t *pub_key, uint8_t *json, uint16_t maxlen, uint8_t input_index) {
  uint8_t sig[140] = {0};
  bigchain_build_json_tx(tx, (char*)json);
  if (strcmp(tx->operation, "TRANSFER") == 0) {
  // if (!memcmp(tx->operation, "TRANSFER", 8)) {
    // For TRANSFER the json string must be concatenated with the input tx_id and the output_index
    strcat((char*)json, tx_id);
    strcat((char*)json, "0");
  }

  bigchain_sign_transaction((uint8_t *)json, strlen((char*)json), (uint8_t *)priv_key, (uint8_t *)pub_key, (uint8_t *)sig);
  bigchain_fulfill_and_serialize(tx, (uint8_t *)json, maxlen, (uint8_t *)sig, (uint8_t *)pub_key, input_index);
}

/*
*  partial_fulfill_tx
*  useful to create fulfillement for each input with potentially different key pair
* 
*  for (uint8_t i = 0; i < tx->num_inputs; i++) {
*    uint8_t sig[140] = {0};
*    bigchain_sign_transaction((uint8_t *)json, strlen((char*)json), (uint8_t *)priv_key[i], (uint8_t *)pub_key[i], (uint8_t *)sig)
*    partial_fulfill_tx(tx, sig, pubkey[i], i);
*  }
*
*/
void partial_fulfill_tx(BIGCHAIN_TX *tx, char *tx_id, uint8_t *priv_key, uint8_t *pub_key, uint8_t *json, uint16_t maxlen, uint8_t input_index) {
  uint8_t sig[140] = {0};
  bigchain_build_json_tx(tx, (char*)json);
  if (strcmp(tx->operation, "TRANSFER") == 0) {
    // For TRANSFER the json string must be concatenated with the input tx_id and the output_index
    strcat((char*)json, tx_id);
    strcat((char*)json, "0");
  }
  bigchain_sign_transaction((uint8_t *)json, strlen((char*)json), (uint8_t *)priv_key, (uint8_t *)pub_key, (uint8_t *)sig);
  bigchain_fulfill(tx, sig, pub_key, input_index);
}

/* 
* bigchain_parse_json
* Handles transaction prepared by JS-bigchain-driver
*
*/
int bigchain_parse_json_inputs(const json_t* json_obj, BIGCHAIN_INPUT *inputs) 
{
  const json_t *inputs_field = json_getProperty(json_obj, "inputs");
  if (!inputs_field || JSON_ARRAY != json_getType(inputs_field)) {
    // puts("the inputs list property is not found.");
    return 0;
  }

  memset(inputs, 0, sizeof(BIGCHAIN_INPUT));
  uint8_t i = 0;
  const json_t *input;
  for(input = json_getChild(inputs_field); input != 0; input = json_getSibling(input) ) {
    if (JSON_OBJ == json_getType(input)) {
      // FULFILLEMENT
      const char* fulfillment = json_getPropertyValue(input, "fulfillment");
      if (fulfillment) {
        memcpy(inputs[i].fulfillment, fulfillment, strlen(fulfillment));
      }
      // FULFILLS
      const char* fulfills = json_getPropertyValue(input, "fulfills");
      if (fulfills) {
        memcpy(inputs[i].fulfills, fulfills, strlen(fulfills));
      }

      // json_t const* fulfills_obj = json_getProperty(input, "fulfills");
      // if (!fulfills_obj || JSON_OBJ != json_getType(fulfills_obj)) {
      //   puts("Error, the fulfills property is not found.");
      //   return 0;
      // }
      // const char* transaction_id = json_getPropertyValue(fulfills_obj, "transaction_id");
      // const char* output_index = json_getPropertyValue(fulfills_obj, "output_index");
      // if (transaction_id && output_index) {
      //   memcpy(inputs[0].fulfills, FULFILL_PREFIX, FULFILL_PREFIX_LENGTH );
      //   memcpy(inputs[0].fulfills + FULFILL_PREFIX_LENGTH, transaction_id, TX_ID_LENGTH );
      //   memcpy(inputs[0].fulfills + FULFILL_PREFIX_LENGTH + TX_ID_LENGTH, "\"\0", 2);
      //   memcpy(inputs[0].fulfills + FULFILL_PREFIX_LENGTH + TX_ID_LENGTH, output_index, 1);
      // } 

      // num_owners
      const json_t *num_owners_key = json_getProperty(input, "num_owners");
      if (!num_owners_key || JSON_INTEGER != json_getType(num_owners_key) ) {
        puts("Error, the num_owners property is not found.");
        return 0;
      }
      int const num_owners = (int)json_getInteger(num_owners_key);
      inputs[i].num_owners = num_owners;

      // owners_before
      const json_t *owners_before = json_getProperty(input, "owners_before");
      if (!owners_before || JSON_ARRAY != json_getType(owners_before) ) {
        puts("Error, the owners_before list property is not found.");
        return 0;
      }
      uint8_t j = 0;
      const json_t *owner_before_key;
      for (owner_before_key = json_getChild(owners_before); owner_before_key != 0; owner_before_key = json_getSibling(owner_before_key) ) {
        if (JSON_TEXT == json_getType(owner_before_key)) {
          const char* owner_before = json_getValue(owner_before_key);
          memcpy(inputs[i].owners_before[j], owner_before, strlen(owner_before));
        }
        j++;
      }
    }
    i++;
  }
  return i;
}


int bigchain_parse_json_outputs(const json_t* json_obj, BIGCHAIN_OUTPUT *outputs) 
{
  const json_t *outputs_field = json_getProperty(json_obj, "outputs");
  if (!outputs_field || JSON_ARRAY != json_getType(outputs_field)) {
    // puts("the outputs list property is not found.");
    return 0;
  }
  
  memset(outputs, 0, sizeof(BIGCHAIN_OUTPUT));
  uint8_t i = 0;
  const json_t *output;
  for (output = json_getChild(outputs_field); output != 0; output = json_getSibling(output)) {
    if (JSON_OBJ == json_getType(output)) {
      // AMOUNT
      const char* amount = json_getPropertyValue(output, "amount");
      if (amount) {
        memcpy(outputs[i].amount, amount, strlen(amount));
      }

      // CONDITION
      const json_t *condition = json_getProperty(output, "condition");
      if (!condition || JSON_OBJ != json_getType(condition)) {
        puts("Error, the condition property is not found.");
        return 0;
      }

      // condition->details
      const json_t *details = json_getProperty(condition, "details");
      if (!details || JSON_OBJ != json_getType(details)) {
        puts("Error, the details property is not found.");
        return 0;
      }
      // const char* details_type = json_getPropertyValue(details, "type");
      // if (details_type) {
      //   memcpy(outputs[i].details_type, details_type, strlen(details_type));
      // }    
      const char* details_public_key = json_getPropertyValue(details, "public_key");
      if (details_public_key) {
        memcpy(outputs[i].details_public_key, details_public_key, strlen(details_public_key));
      }       

      // condition->uri
      // const char* uri = json_getPropertyValue(details, "uri");
      // if (uri) {
      //   memcpy(outputs[i].condition.uri, uri, strlen(uri));
      // }    

      // num_public_keys
      const json_t *num_public_keys_key = json_getProperty(output, "num_public_keys");
      if (!num_public_keys_key || JSON_INTEGER != json_getType(num_public_keys_key) ) {
        puts("Error, the num_public_keys property is not found.");
        return 0;
      }
      int const num_public_keys = (int)json_getInteger(num_public_keys_key);
      outputs[i].num_public_keys = num_public_keys;

      // public_keys
      const json_t *public_keys = json_getProperty(output, "public_keys");
      if (!public_keys || JSON_ARRAY != json_getType(public_keys)) {
        puts("Error, the public_keys list property is not found.");
        return 0;
      }
      uint8_t j = 0;
      const json_t *public_key_key;
      for (public_key_key = json_getChild(public_keys); public_key_key != 0; public_key_key = json_getSibling(public_key_key) ) {
        if (JSON_TEXT == json_getType(public_key_key)) {
          const char* public_key = json_getValue(public_key_key);
          memcpy(outputs[i].public_keys[j], public_key, strlen(public_key));
        }
        j++;
      }
    }
    i++;
  }
  return i;
}

static char* bigchain_parse_object(const json_t *json, char *str, const char *name) 
{
  const jsonType_t type = json_getType(json);
  char *p = str;
  if (type != JSON_OBJ && type != JSON_ARRAY) {
    // puts("Input is not an object.");
    return p;
  }

  if (type == JSON_OBJ) {
    if ( NULL != name ) {
      p = json_objOpen(p, NULL);
    } else {
      p = json_objOpen(p, name);
    }
  } else {
    if ( NULL != name ) {
      p = json_arrOpen(p, NULL);
    } else {
      p = json_arrOpen(p, name);
    }
  }

  const json_t *child;
  for (child = json_getChild(json); child != 0; child = json_getSibling(child)) {
    jsonType_t propertyType = json_getType(child);
    const char* child_name = json_getName(child);
    // if (child_name) {
    //   printf(" \"%s\": ", child_name);
    // }
    if (propertyType == JSON_OBJ || propertyType == JSON_ARRAY) {
      p = bigchain_parse_object(child, p, child_name);
      // char child_str[200];
      // bigchain_parse_object(child, child_str, NULL);
      // p = json_objOpen(p, child_name);
      // p = atoa(p, child_str);
      // p = json_objClose(p);
    } else {
      char const* value = json_getValue(child);
      if (value) {
        if (value[0] != '\0') {
          p = json_str(p, child_name, value);
        } else {
          p = json_null(p, child_name);
        }
      }
    }
  }

  if (type == JSON_OBJ) {
    p = json_objClose(p);
  } else {
    p = json_arrClose(p);
  }
  return p;
}

int bigchain_parse_asset(const json_t *json_obj, char* asset) 
{
  const json_t *asset_field = json_getProperty(json_obj, "asset");
  const jsonType_t type = json_getType(asset_field);
  if (type != JSON_OBJ && type != JSON_ARRAY) {
    puts("Error, the asset property is not found.");
    return 0;
  }
  char* p = bigchain_parse_object(asset_field, asset, NULL);
  p = json_end(p);
  return p - asset;
}

int bigchain_parse_metadata(const json_t *json_obj, char* metadata) 
{
  const json_t *metadata_field = json_getProperty(json_obj, "metadata");
  const jsonType_t type = json_getType(metadata_field);
  if (type != JSON_OBJ && type != JSON_ARRAY) {
    puts("Error, the metadata property is not found.");
    return 0;
  }
  char* p = bigchain_parse_object(metadata_field, metadata, NULL);
  p = json_end(p);
  return p - metadata;
}

// TODO: add max fields, max asset and metadata sizes 
int bigchain_parse_json(char* json_tx, BIGCHAIN_TX *tx) {
    memzero(tx, sizeof(BIGCHAIN_TX));
    json_t mem[128];
    const json_t *json_obj = json_create((char*)json_tx, mem, sizeof mem / sizeof *mem);
    if (!json_obj) {
      puts("Error json create.");
      return 0;
    }
    
    // ID
    const char* id = json_getPropertyValue(json_obj, "id");
    if (id) {
      memcpy(tx->id, id, strlen(id));
      // puts("Error, the id property is not found.");
      // return 0;
    }	

    // VERSION
    const char* version = json_getPropertyValue(json_obj, "version");
    if (!version) {
      puts("Error, the version property is not found.");
      return 0;
    }	
    // TODO: validate version
    memcpy(tx->version, version, strlen(version));

    // OPERATION
    const char* operation = json_getPropertyValue(json_obj, "operation");
    if (!operation) {
      puts("Error, the operation property is not found.");
      return 0;
    }	
    // TODO: validate operation
    memcpy(tx->operation, operation, strlen(operation));

    // ASSET
    // TODO: how to set buffer size ?
    char asset[1024];
    int len = bigchain_parse_asset(json_obj, asset);
    if (len >= sizeof asset) {
      fprintf(stderr, "%s%d%s%d\n", "Error. Len: ", len, " Max: ", (int)sizeof asset - 1 );
      return 0;
    }
    if (strcmp(operation, "CREATE") == 0) {
      memcpy(tx->asset, asset, strlen(asset));
    } else if (strcmp(operation, "TRANSFER") == 0) {
      const json_t *asset_field = json_getProperty(json_obj, "asset");
      const char* asset_id = json_getPropertyValue(asset_field, "id");
      memcpy(tx->asset, ASSET_ID, ASSET_ID_LENGTH);
      memcpy(tx->asset + ASSET_ID_LENGTH, asset_id, TX_ID_LENGTH );
      memcpy(tx->asset + ASSET_ID_LENGTH + TX_ID_LENGTH, "\"\0", 2);
    }

    // METADATA
    // TODO: how to set buffer size ?
    char metadata[1024];
    len = bigchain_parse_metadata(json_obj, metadata);
    if (len >= sizeof asset) {
      fprintf(stderr, "%s%d%s%d\n", "Error. Len: ", len, " Max: ", (int)sizeof metadata - 1 );
      return 0;
    }
    memcpy(tx->metadata, metadata, strlen(metadata));

    // INPUTS
    int num_inputs = bigchain_parse_json_inputs(json_obj, tx->inputs);
    tx->num_inputs = num_inputs;

    // OUTPUTS
    int num_outputs = bigchain_parse_json_outputs(json_obj, tx->outputs);
    tx->num_outputs = num_outputs;

    return 1;
}

