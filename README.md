<!---
Copyright RiddleandCode
--->


# ANSI C driver for BigchainDB

This ANSI C driver enables developers to create, sign and send BigchainDB transactions to BigchainDB servers and has been used in several MCU based projects within RIDDLE&CODE.

**Please note**: This driver is written in C but is due to the C/C++ relation compatible with C++ by default. 


## Install


Clone the git repository and the submodules

```
$ git clone https://github.com/RiddleAndCode/bigchaindb-c-driver.git
$ git submodule update --init --recursive
```

Having done that, the compilation process can be started by running the cmake and make build process. 
Please ensure that you have cmake and make installed on your system:

```
mkdir build
cd build/
cmake ..
make
ctest
```


## Usage

The creation and signing of transaction on BigchainDB is simple. Inputs are taken and transferred to outputs. 
The inputs and outputs have to be prepared and defined. 


The test code of this project contains all of this aspects and can be used to create simple transactions. The support for more complex transactions is yet to come.

First the transaction is prepared (this is in most cases a very specific task, [tests/test_sig.c](https://github.com/RiddleAndCode/bigchaindb-c-driver/blob/master/tests/test_sig.c#L38)), thereafter
built, signed, fulfilled and serialized. 

Now, the transaction can be submitted to the BigchainDB nodes.

```
  /* inputs: operation, asset, metadata, base_pubkey(public key in base58)
  * output: tx
  * 'operation' can be either 'C' or 'T' for CREATE and TRANSFER respectively.
  * when 'operation' is CREATE then 'asset' can be arbitrary. (The keys on the JSON must be in alphabetical order)
  * but when 'operation' is TRANSFER then 'asset' must be the transaction id of the asset which is to be tranfered.
  */
  prepare_tx(BIGCHAIN_TX *tx, const char operation, char *asset, char *metadata , char *base_pubkey);
  
  /* inputs: tx, tx_id(same as 'asset' above, only needed for TRANSFER) , priv_key, pub_key(public key in hexadecimal base) , maxlen(maximum length of string)
  * output: json
  */
  fulfill_tx(BIGCHAIN_TX *tx, char *tx_id, uint8_t *priv_key, uint8_t *pub_key, uint8_t *json, uint16_t maxlen);

```


### Note
When building the metadata and asset objects take the following measures to make it work correctly:
1. No spaces in the json structure: ```{"key 1": "value a"}``` becomes ```'{"key 1":"value a"}'```
2. Keys should be alphabetically ordered: ```{"key 1":"value a","A key":"A value"}``` becomes ```{"A key":"A value","key 1":"value a"}```
3. No numbers fields, only strings ```{"key 1":2.3123}``` becomes ```{"key 1":"2.3123"}```




## BigchainDB Documentation

* [BigchainDB Server Quickstart](http://docs.bigchaindb.com/projects/server/en/latest/quickstart.html)
* [The Hitchhiker's Guide to BigchainDB](https://www.bigchaindb.com/developers/guide/)
* [HTTP API Reference](http://docs.bigchaindb.com/projects/server/en/latest/http-client-server-api.html)
* [All BigchainDB Documentation](https://docs.bigchaindb.com/en/latest/)



## Compatibility Matrix

The driver should be compatible with BigchainDB version 2.x but got only actively tested with BigchainDB v2.0.0b9. It might work with earlier versions too but hasn't been tested with them.

| BigchainDB Server | BigchainDB C driver |
| ----------------- |------------------------------|
| `2.x`             | `1.x`                      |


## Authors

The RIDDLE&CODE team.


## Support

Included code/APIs provided as is. 
If you find an issue with API, please report the issue in https://gitter.im/bigchaindb/bigchaindb. 
Help is always available at RIDDLE&CODE.
 

