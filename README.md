
Install
----------
```
mkdir build
cd build/
cmake ..
make
ctest
```
## Note
When building the metadata and asset objects take the following measures:
1. No spaces in the json structure: ```{"key 1": "value a"}``` becomes ```'{"key 2":"value a"}'```
2. Keys should be alphabetically ordered: ```{"key 1":"value a","A key":"A value"}``` becomes ```{"A key":"A value","key 1":"value a"}```
3. No numbers fields, only strings ```{"key 1":2.3123}``` becomes ```{"key 1":"2.3123"}```


BigchainDB Documentation
------------------------------------
* `BigchainDB Server Quickstart`_
* `The Hitchhiker's Guide to BigchainDB`_
* `HTTP API Reference`_
* `All BigchainDB Documentation`_

Usage
----------
Example: Create a divisible asset for Alice who issues 10 token to Bob so that he can use her Game Boy.
Afterwards Bob spends 3 of these tokens.

If you want to send a transaction you need to `Determine the BigchainDB Root URL`_.


Compatibility Matrix
--------------------

+-----------------------+---------------------------+
| **BigchainDB Server** | **BigchainDB Driver**     |
+=======================+===========================+
| ``>= 2.0.0b7``        | ``0.6.2``                 |
+-----------------------+---------------------------+

License
--------
* `licenses`_ - open source & open content

Credits
-------

This package was initially created using Cookiecutter_ and the `audreyr/cookiecutter-pypackage`_ project template. Many BigchainDB developers have contributed since then.

.. _Documentation: https://docs.bigchaindb.com/projects/py-driver/
.. _pypi history: https://pypi.org/project/bigchaindb-driver/#history
.. _Quickstart: https://docs.bigchaindb.com/projects/py-driver/en/latest/quickstart.html
.. _BigchainDB Server Quickstart: https://docs.bigchaindb.com/projects/server/en/latest/quickstart.html
.. _The Hitchhiker's Guide to BigchainDB: https://www.bigchaindb.com/developers/guide/
.. _HTTP API Reference: https://docs.bigchaindb.com/projects/server/en/latest/http-client-server-api.html
.. _All BigchainDB Documentation: https://docs.bigchaindb.com/
.. _Determine the BigchainDB Root URL: https://docs.bigchaindb.com/projects/py-driver/en/latest/connect.html
.. _licenses: https://github.com/bigchaindb/bigchaindb-driver/blob/master/LICENSES.md
.. _Cookiecutter: https://github.com/audreyr/cookiecutter
.. _`audreyr/cookiecutter-pypackage`: https://github.com/audreyr/cookiecutter-pypackage
Transaction buider and signer.
