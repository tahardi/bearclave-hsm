# Bearclave: Hardware Security Modules

Hardware Security Modules (HSMs) are physical devices that manage cryptographic
keys and perform operations like encryption and signing. These devices are
separate from the computer(s) that use them and are built to resist physical 
tampering and attacks. They expose an interface defined by the Public-Key
Cryptography Standard #11 (PKCS #11), which defines functions for managing
keys and performing cryptographic operations.

This repository contains a software implementation of the PKCS #11 interface
that can be used as a stand-in for testing code that interacts with HSMs.
