## c-bigchaindb-tx

Transaction buider and signer.

```
mkdir build
cd build/
cmake ..
make
ctest
```
## Note
When building the metadata and asset objects take the following measures:
1. No spaces in the json structure: '{"key 1": "value a"}' becomes '{"key 2":"value a"}
2. Keys should be alphabetically ordered: '{"key 1":"value a","A key":"A value"}' becomes {"A key":"A value","key 1":"value a"}'
3. No numbers fields, only strings '{"key 1": 2.3123}' becomes '{"key 1":"2.3123"}'
