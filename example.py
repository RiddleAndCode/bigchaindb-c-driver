import json

import base58
import sha3
from cryptoconditions import Ed25519Sha256

from bigchaindb_driver.crypto import generate_keypair
from cryptoconditions import crypto
from binascii import unhexlify
import base58, base64
from collections import namedtuple

a = "c1cab23edf6065679dcbfbcbd17300384f7586bef4b7bda56d0366f707a91063c552f4a3471b3afd7506cf7a52c59965b9cfaaaf7934e0304535fd1838d76708"
b = base64.b64encode(unhexlify(a))
print(b)

def get_pair():
    prv = base58.b58encode(unhexlify("2470319889fed2139a6dd99ea3aef2d396e9e7475132337bdb04d47ca7d2028c"))
    # prv = base58.b58encode(unhexlify("92c9578337c55e4f0a706e1fb93a43e8c20bdf7dcdec473f52f4ab55e663dcb6"))
    # prv = base64.b58decode("uc+qr3k04DB\Nf0YONdnCA==\\000DhqC9A3ooLgD3i4V34H")
    # prv = "uc+qr3k04DBFNf0YONdnCA==\\000DhqC9A3ooLgD3i4V34H"
    sk = crypto.Ed25519SigningKey(key=prv)
    pk = sk.get_verifying_key().encode(encoding='base58')
    return prv, pk

def generate_keypair_2(seed=None):
    """Generates a cryptographic key pair.

    Args:
        seed (bytes): 32-byte seed for deterministic generation.
                      Defaults to `None`.
    Returns:
        :class:`~bigchaindb_driver.crypto.CryptoKeypair`: A
        :obj:`collections.namedtuple` with named fields
        :attr:`~bigchaindb_driver.crypto.CryptoKeypair.private_key` and
        :attr:`~bigchaindb_driver.crypto.CryptoKeypair.public_key`.

    """
    CryptoKeypair = namedtuple('CryptoKeypair', ('private_key', 'public_key'))

    return CryptoKeypair(
        *(k.decode() for k in get_pair()))


alice = generate_keypair_2()
print(alice.public_key)
operation = 'CREATE'

version = '2.0'

asset = {
    'data': {
        'bicycle': {
            'manufacturer': 'bkfab',
            'serial_number': 'abcd1234',
        },
    },
}

metadata = {'planet': 'earth'}

ed25519 = Ed25519Sha256(public_key=base58.b58decode(alice.public_key))

output = {
    'amount': '1',
    'condition': {
        'details': {
            'type': ed25519.TYPE_NAME,
            'public_key': base58.b58encode(ed25519.public_key).decode(),
        },
        'uri': ed25519.condition_uri,
    },
    'public_keys': (alice.public_key,),
}
outputs = (output,)

input_ = {
    'fulfillment': None,
    'fulfills': None,
    'owners_before': (alice.public_key,)
}
inputs = (input_,)

handcrafted_creation_tx = {
    'asset': asset,
    'metadata': metadata,
    'operation': operation,
    'outputs': outputs,
    'inputs': inputs,
    'version': version,
    'id': None,
}

message = json.dumps(
    handcrafted_creation_tx,
    sort_keys=True,
    separators=(',', ':'),
    ensure_ascii=False,
)

message = sha3.sha3_256(message.encode())
print(message.digest())
ed25519.sign(message.digest(), base58.b58decode(alice.private_key))

fulfillment_uri = ed25519.serialize_uri()
print(ed25519.serialize_binary())
handcrafted_creation_tx['inputs'][0]['fulfillment'] = fulfillment_uri

json_str_tx = json.dumps(
handcrafted_creation_tx,
sort_keys=True,
separators=(',', ':'),
ensure_ascii=False,
)

creation_txid = sha3.sha3_256(json_str_tx.encode()).hexdigest()

handcrafted_creation_tx['id'] = creation_txid

print(json.dumps(handcrafted_creation_tx,
sort_keys=True,
separators=(',', ':'),
ensure_ascii=False,))