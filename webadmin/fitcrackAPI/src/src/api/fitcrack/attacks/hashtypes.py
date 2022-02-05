'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

hashtypes = [
    {
      "code": "900",
      "category": "Raw Hash",
      "name": "MD4",
    },
    {
      "code": "0",
      "category": "Raw Hash",
      "name": "MD5",
    },
    {
      "code": "100",
      "category": "Raw Hash",
      "name": "SHA1",
    },
    {
      "code": "1300",
      "category": "Raw Hash",
      "name": "SHA2-224",
    },
    {
      "code": "1400",
      "category": "Raw Hash",
      "name": "SHA2-256",
    },
    {
      "code": "10800",
      "category": "Raw Hash",
      "name": "SHA2-384",
    },
    {
      "code": "1700",
      "category": "Raw Hash",
      "name": "SHA2-512",
    },
    {
      "code": "17300",
      "category": "Raw Hash",
      "name": "SHA3-224",
    },
    {
      "code": "17400",
      "category": "Raw Hash",
      "name": "SHA3-256",
    },
    {
      "code": "17500",
      "category": "Raw Hash",
      "name": "SHA3-384",
    },
    {
      "code": "17600",
      "category": "Raw Hash",
      "name": "SHA3-512",
    },
    {
      "code": "6000",
      "category": "Raw Hash",
      "name": "RIPEMD-160",
    },
    {
      "code": "600",
      "category": "Raw Hash",
      "name": "BLAKE2b-512",
    },
    {
      "code": "11700",
      "category": "Raw Hash",
      "name": "GOST R 34.11-2012 (Streebog) 256-bit, big-endian",
    },
    {
      "code": "11800",
      "category": "Raw Hash",
      "name": "GOST R 34.11-2012 (Streebog) 512-bit, big-endian",
    },
    {
      "code": "6900",
      "category": "Raw Hash",
      "name": "GOST R 34.11-94",
    },
    {
      "code": "17010",
      "category": "Raw Hash",
      "name": "GPG (AES-128/AES-256 (SHA-1($pass)))",
    },
    {
      "code": "5100",
      "category": "Raw Hash",
      "name": "Half MD5",
    },
    {
      "code": "17700",
      "category": "Raw Hash",
      "name": "Keccak-224",
    },
    {
      "code": "17800",
      "category": "Raw Hash",
      "name": "Keccak-256",
    },
    {
      "code": "17900",
      "category": "Raw Hash",
      "name": "Keccak-384",
    },
    {
      "code": "18000",
      "category": "Raw Hash",
      "name": "Keccak-512",
    },
    {
      "code": "6100",
      "category": "Raw Hash",
      "name": "Whirlpool",
    },
    {
      "code": "10100",
      "category": "Raw Hash",
      "name": "SipHash",
    },
    {
      "code": "70",
      "category": "Raw Hash",
      "name": "md5(utf16le($pass))",
    },
    {
      "code": "170",
      "category": "Raw Hash",
      "name": "sha1(utf16le($pass))",
    },
    {
      "code": "1470",
      "category": "Raw Hash",
      "name": "sha256(utf16le($pass))",
    },
    {
      "code": "10870",
      "category": "Raw Hash",
      "name": "sha384(utf16le($pass))",
    },
    {
      "code": "1770",
      "category": "Raw Hash",
      "name": "sha512(utf16le($pass))",
    },
    {
      "code": "10",
      "category": "Raw Hash salted and/or iterated",
      "name": "md5($pass.$salt)",
    },
    {
      "code": "20",
      "category": "Raw Hash salted and/or iterated",
      "name": "md5($salt.$pass)",
    },
    {
      "code": "3800",
      "category": "Raw Hash salted and/or iterated",
      "name": "md5($salt.$pass.$salt)",
    },
    {
      "code": "3710",
      "category": "Raw Hash salted and/or iterated",
      "name": "md5($salt.md5($pass))",
    },
    {
      "code": "4110",
      "category": "Raw Hash salted and/or iterated",
      "name": "md5($salt.md5($pass.$salt))",
    },
    {
      "code": "4010",
      "category": "Raw Hash salted and/or iterated",
      "name": "md5($salt.md5($salt.$pass))",
    },
    {
      "code": "21300",
      "category": "Raw Hash salted and/or iterated",
      "name": "md5($salt.sha1($salt.$pass))",
    },
    {
      "code": "40",
      "category": "Raw Hash salted and/or iterated",
      "name": "md5($salt.utf16le($pass))",
    },
    {
      "code": "2600",
      "category": "Raw Hash salted and/or iterated",
      "name": "md5(md5($pass))",
    },
    {
      "code": "3910",
      "category": "Raw Hash salted and/or iterated",
      "name": "md5(md5($pass).md5($salt))",
    },
    {
      "code": "3500",
      "category": "Raw Hash salted and/or iterated",
      "name": "md5(md5(md5($pass)))",
    },
    {
      "code": "4400",
      "category": "Raw Hash salted and/or iterated",
      "name": "md5(sha1($pass))",
    },
    {
      "code": "20900",
      "category": "Raw Hash salted and/or iterated",
      "name": "md5(sha1($pass).md5($pass).sha1($pass))",
    },
    {
      "code": "21200",
      "category": "Raw Hash salted and/or iterated",
      "name": "md5(sha1($salt).md5($pass))",
    },
    {
      "code": "4300",
      "category": "Raw Hash salted and/or iterated",
      "name": "md5(strtoupper(md5($pass)))",
    },
    {
      "code": "30",
      "category": "Raw Hash salted and/or iterated",
      "name": "md5(utf16le($pass).$salt)",
    },
    {
      "code": "110",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha1($pass.$salt)",
    },
    {
      "code": "120",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha1($salt.$pass)",
    },
    {
      "code": "4900",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha1($salt.$pass.$salt)",
    },
    {
      "code": "4520",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha1($salt.sha1($pass))",
    },
    {
      "code": "24300",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha1($salt.sha1($pass.$salt))",
    },
    {
      "code": "140",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha1($salt.utf16le($pass))",
    },
    {
      "code": "19300",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha1($salt1.$pass.$salt2)",
    },
    {
      "code": "14400",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha1(CX)",
    },
    {
      "code": "4700",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha1(md5($pass))",
    },
    {
      "code": "4710",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha1(md5($pass).$salt)",
    },
    {
      "code": "21100",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha1(md5($pass.$salt))",
    },
    {
      "code": "18500",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha1(md5(md5($pass)))",
    },
    {
      "code": "4500",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha1(sha1($pass))",
    },
    {
      "code": "4510",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha1(sha1($pass).$salt)",
    },
    {
      "code": "5000",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha1(sha1($salt.$pass.$salt))",
    },
    {
      "code": "130",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha1(utf16le($pass).$salt)",
    },
    {
      "code": "1410",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha256($pass.$salt)",
    },
    {
      "code": "1420",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha256($salt.$pass)",
    },
    {
      "code": "22300",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha256($salt.$pass.$salt)",
    },
    {
      "code": "20720",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha256($salt.sha256($pass))",
    },
    {
      "code": "1440",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha256($salt.utf16le($pass))",
    },
    {
      "code": "20800",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha256(md5($pass))",
    },
    {
      "code": "20710",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha256(sha256($pass).$salt)",
    },
    {
      "code": "21400",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha256(sha256_bin($pass))",
    },
    {
      "code": "1430",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha256(utf16le($pass).$salt)",
    },
    {
      "code": "10810",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha384($pass.$salt)",
    },
    {
      "code": "10820",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha384($salt.$pass)",
    },
    {
      "code": "10840",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha384($salt.utf16le($pass))",
    },
    {
      "code": "10830",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha384(utf16le($pass).$salt)",
    },
    {
      "code": "1710",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha512($pass.$salt)",
    },
    {
      "code": "1720",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha512($salt.$pass)",
    },
    {
      "code": "1740",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha512($salt.utf16le($pass))",
    },
    {
      "code": "1730",
      "category": "Raw Hash salted and/or iterated",
      "name": "sha512(utf16le($pass).$salt)",
    },
    {
      "code": "50",
      "category": "Raw Hash authenticated",
      "name": "HMAC-MD5 (key = $pass)",
    },
    {
      "code": "60",
      "category": "Raw Hash authenticated",
      "name": "HMAC-MD5 (key = $salt)",
    },
    {
      "code": "150",
      "category": "Raw Hash authenticated",
      "name": "HMAC-SHA1 (key = $pass)",
    },
    {
      "code": "160",
      "category": "Raw Hash authenticated",
      "name": "HMAC-SHA1 (key = $salt)",
    },
    {
      "code": "1450",
      "category": "Raw Hash authenticated",
      "name": "HMAC-SHA256 (key = $pass)",
    },
    {
      "code": "1460",
      "category": "Raw Hash authenticated",
      "name": "HMAC-SHA256 (key = $salt)",
    },
    {
      "code": "1750",
      "category": "Raw Hash authenticated",
      "name": "HMAC-SHA512 (key = $pass)",
    },
    {
      "code": "1760",
      "category": "Raw Hash authenticated",
      "name": "HMAC-SHA512 (key = $salt)",
    },
    {
      "code": "11750",
      "category": "Raw Hash authenticated",
      "name": "HMAC-Streebog-256 (key = $pass), big-endian",
    },
    {
      "code": "11760",
      "category": "Raw Hash authenticated",
      "name": "HMAC-Streebog-256 (key = $salt), big-endian",
    },
    {
      "code": "11850",
      "category": "Raw Hash authenticated",
      "name": "HMAC-Streebog-512 (key = $pass), big-endian",
    },
    {
      "code": "11860",
      "category": "Raw Hash authenticated",
      "name": "HMAC-Streebog-512 (key = $salt), big-endian",
    },
    {
      "code": "11500",
      "category": "Raw Checksum",
      "name": "CRC32",
    },
    {
      "code": "27900",
      "category": "Raw Checksum",
      "name": "CRC32C",
    },
    {
      "code": "28000",
      "category": "Raw Checksum",
      "name": "CRC64Jones",
    },
    {
      "code": "18700",
      "category": "Raw Checksum",
      "name": "Java Object hashCode()",
    },
    {
      "code": "25700",
      "category": "Raw Checksum",
      "name": "MurmurHash",
    },
    {
      "code": "27800",
      "category": "Raw Checksum",
      "name": "MurmurHash3",
    },
    {
      "code": "14100",
      "category": "Raw Cipher, Known-plaintext attack",
      "name": "3DES (PT = $salt, key = $pass)",
    },
    {
      "code": "14000",
      "category": "Raw Cipher, Known-plaintext attack",
      "name": "DES (PT = $salt, key = $pass)",
    },
    {
      "code": "26401",
      "category": "Raw Cipher, Known-plaintext attack",
      "name": "AES-128-ECB NOKDF (PT = $salt, key = $pass)",
    },
    {
      "code": "26402",
      "category": "Raw Cipher, Known-plaintext attack",
      "name": "AES-192-ECB NOKDF (PT = $salt, key = $pass)",
    },
    {
      "code": "26403",
      "category": "Raw Cipher, Known-plaintext attack",
      "name": "AES-256-ECB NOKDF (PT = $salt, key = $pass)",
    },
    {
      "code": "15400",
      "category": "Raw Cipher, Known-plaintext attack",
      "name": "ChaCha20",
    },
    {
      "code": "14500",
      "category": "Raw Cipher, Known-plaintext attack",
      "name": "Linux Kernel Crypto API (2.4)",
    },
    {
      "code": "14900",
      "category": "Raw Cipher, Known-plaintext attack",
      "name": "Skip32 (PT = $salt, key = $pass)",
    },
    {
      "code": "11900",
      "category": "Generic KDF",
      "name": "PBKDF2-HMAC-MD5",
    },
    {
      "code": "12000",
      "category": "Generic KDF",
      "name": "PBKDF2-HMAC-SHA1",
    },
    {
      "code": "10900",
      "category": "Generic KDF",
      "name": "PBKDF2-HMAC-SHA256",
    },
    {
      "code": "12100",
      "category": "Generic KDF",
      "name": "PBKDF2-HMAC-SHA512",
    },
    {
      "code": "8900",
      "category": "Generic KDF",
      "name": "scrypt",
    },
    {
      "code": "400",
      "category": "Generic KDF",
      "name": "phpass",
    },
    {
      "code": "16100",
      "category": "Network Protocol",
      "name": "TACACS+",
    },
    {
      "code": "11400",
      "category": "Network Protocol",
      "name": "SIP digest authentication (MD5)",
    },
    {
      "code": "5300",
      "category": "Network Protocol",
      "name": "IKE-PSK MD5",
    },
    {
      "code": "5400",
      "category": "Network Protocol",
      "name": "IKE-PSK SHA1",
    },
    {
      "code": "25100",
      "category": "Network Protocol",
      "name": "SNMPv3 HMAC-MD5-96",
    },
    {
      "code": "25000",
      "category": "Network Protocol",
      "name": "SNMPv3 HMAC-MD5-96/HMAC-SHA1-96",
    },
    {
      "code": "25200",
      "category": "Network Protocol",
      "name": "SNMPv3 HMAC-SHA1-96",
    },
    {
      "code": "26700",
      "category": "Network Protocol",
      "name": "SNMPv3 HMAC-SHA224-128",
    },
    {
      "code": "26800",
      "category": "Network Protocol",
      "name": "SNMPv3 HMAC-SHA256-192",
    },
    {
      "code": "26900",
      "category": "Network Protocol",
      "name": "SNMPv3 HMAC-SHA384-256",
    },
    {
      "code": "27300",
      "category": "Network Protocol",
      "name": "SNMPv3 HMAC-SHA512-384",
    },
    {
      "code": "2500",
      "category": "Network Protocol",
      "name": "WPA-EAPOL-PBKDF2",
    },
    {
      "code": "2501",
      "category": "Network Protocol",
      "name": "WPA-EAPOL-PMK",
    },
    {
      "code": "22000",
      "category": "Network Protocol",
      "name": "WPA-PBKDF2-PMKID+EAPOL",
    },
    {
      "code": "22001",
      "category": "Network Protocol",
      "name": "WPA-PMK-PMKID+EAPOL",
    },
    {
      "code": "16800",
      "category": "Network Protocol",
      "name": "WPA-PMKID-PBKDF2",
    },
    {
      "code": "16801",
      "category": "Network Protocol",
      "name": "WPA-PMKID-PMK",
    },
    {
      "code": "7300",
      "category": "Network Protocol",
      "name": "IPMI2 RAKP HMAC-SHA1",
    },
    {
      "code": "10200",
      "category": "Network Protocol",
      "name": "CRAM-MD5",
    },
    {
      "code": "16500",
      "category": "Network Protocol",
      "name": "JWT (JSON Web Token)",
    },
    {
      "code": "19600",
      "category": "Network Protocol",
      "name": "Kerberos 5, etype 17, TGS-REP",
    },
    {
      "code": "19800",
      "category": "Network Protocol",
      "name": "Kerberos 5, etype 17, Pre-Auth",
    },
    {
      "code": "19700",
      "category": "Network Protocol",
      "name": "Kerberos 5, etype 18, TGS-REP",
    },
    {
      "code": "19900",
      "category": "Network Protocol",
      "name": "Kerberos 5, etype 18, Pre-Auth",
    },
    {
      "code": "7500",
      "category": "Network Protocol",
      "name": "Kerberos 5, etype 23, AS-REQ Pre-Auth",
    },
    {
      "code": "13100",
      "category": "Network Protocol",
      "name": "Kerberos 5, etype 23, TGS-REP",
    },
    {
      "code": "18200",
      "category": "Network Protocol",
      "name": "Kerberos 5, etype 23, AS-REP",
    },
    {
      "code": "5500",
      "category": "Network Protocol",
      "name": "NetNTLMv1 / NetNTLMv1+ESS",
    },
    {
      "code": "27000",
      "category": "Network Protocol",
      "name": "NetNTLMv1 / NetNTLMv1+ESS (NT)",
    },
    {
      "code": "5600",
      "category": "Network Protocol",
      "name": "NetNTLMv2",
    },
    {
      "code": "27100",
      "category": "Network Protocol",
      "name": "NetNTLMv2 (NT)",
    },
    {
      "code": "4800",
      "category": "Network Protocol",
      "name": "iSCSI CHAP authentication, MD5(CHAP)",
    },
    {
      "code": "8500",
      "category": "Operating System",
      "name": "RACF",
    },
    {
      "code": "6300",
      "category": "Operating System",
      "name": "AIX {smd5}",
    },
    {
      "code": "6700",
      "category": "Operating System",
      "name": "AIX {ssha1}",
    },
    {
      "code": "6400",
      "category": "Operating System",
      "name": "AIX {ssha256}",
    },
    {
      "code": "6500",
      "category": "Operating System",
      "name": "AIX {ssha512}",
    },
    {
      "code": "3000",
      "category": "Operating System",
      "name": "LM",
    },
    {
      "code": "19000",
      "category": "Operating System",
      "name": "QNX /etc/shadow (MD5)",
    },
    {
      "code": "19100",
      "category": "Operating System",
      "name": "QNX /etc/shadow (SHA256)",
    },
    {
      "code": "19200",
      "category": "Operating System",
      "name": "QNX /etc/shadow (SHA512)",
    },
    {
      "code": "15300",
      "category": "Operating System",
      "name": "DPAPI masterkey file v1",
    },
    {
      "code": "15900",
      "category": "Operating System",
      "name": "DPAPI masterkey file v2",
    },
    {
      "code": "7200",
      "category": "Operating System",
      "name": "GRUB 2",
    },
    {
      "code": "12800",
      "category": "Operating System",
      "name": "MS-AzureSync PBKDF2-HMAC-SHA256",
    },
    {
      "code": "12400",
      "category": "Operating System",
      "name": "BSDi Crypt, Extended DES",
    },
    {
      "code": "1000",
      "category": "Operating System",
      "name": "NTLM",
    },
    {
      "code": "9900",
      "category": "Operating System",
      "name": "Radmin2",
    },
    {
      "code": "5800",
      "category": "Operating System",
      "name": "Samsung Android Password/PIN",
    },
    {
      "code": "28100",
      "category": "Operating System",
      "name": "Windows Hello PIN/Password",
    },
    {
      "code": "13800",
      "category": "Operating System",
      "name": "Windows Phone 8+ PIN/password",
    },
    {
      "code": "2410",
      "category": "Operating System",
      "name": "Cisco-ASA MD5",
    },
    {
      "code": "9200",
      "category": "Operating System",
      "name": "Cisco-IOS $8$ (PBKDF2-SHA256)",
    },
    {
      "code": "9300",
      "category": "Operating System",
      "name": "Cisco-IOS $9$ (scrypt)",
    },
    {
      "code": "5700",
      "category": "Operating System",
      "name": "Cisco-IOS type 4 (SHA256)",
    },
    {
      "code": "2400",
      "category": "Operating System",
      "name": "Cisco-PIX MD5",
    },
    {
      "code": "8100",
      "category": "Operating System",
      "name": "Citrix NetScaler (SHA1)",
    },
    {
      "code": "22200",
      "category": "Operating System",
      "name": "Citrix NetScaler (SHA512)",
    },
    {
      "code": "1100",
      "category": "Operating System",
      "name": "Domain Cached Credentials (DCC), MS Cache",
    },
    {
      "code": "2100",
      "category": "Operating System",
      "name": "Domain Cached Credentials 2 (DCC2), MS Cache 2",
    },
    {
      "code": "7000",
      "category": "Operating System",
      "name": "FortiGate (FortiOS)",
    },
    {
      "code": "26300",
      "category": "Operating System",
      "name": "FortiGate256 (FortiOS256)",
    },
    {
      "code": "125",
      "category": "Operating System",
      "name": "ArubaOS",
    },
    {
      "code": "501",
      "category": "Operating System",
      "name": "Juniper IVE",
    },
    {
      "code": "22",
      "category": "Operating System",
      "name": "Juniper NetScreen/SSG (ScreenOS)",
    },
    {
      "code": "15100",
      "category": "Operating System",
      "name": "Juniper/NetBSD sha1crypt",
    },
    {
      "code": "26500",
      "category": "Operating System",
      "name": "iPhone passcode (UID key + System Keybag)",
    },
    {
      "code": "122",
      "category": "Operating System",
      "name": "macOS v10.4, macOS v10.5, macOS v10.6",
    },
    {
      "code": "1722",
      "category": "Operating System",
      "name": "macOS v10.7",
    },
    {
      "code": "7100",
      "category": "Operating System",
      "name": "macOS v10.8+ (PBKDF2-SHA512)",
    },
    {
      "code": "3200",
      "category": "Operating System",
      "name": "bcrypt $2*$, Blowfish (Unix)",
    },
    {
      "code": "500",
      "category": "Operating System",
      "name": "md5crypt, MD5 (Unix), Cisco-IOS $1$ (MD5)",
    },
    {
      "code": "1500",
      "category": "Operating System",
      "name": "descrypt, DES (Unix), Traditional DES",
    },
    {
      "code": "7400",
      "category": "Operating System",
      "name": "sha256crypt $5$, SHA256 (Unix)",
    },
    {
      "code": "1800",
      "category": "Operating System",
      "name": "sha512crypt $6$, SHA512 (Unix)",
    },
    {
      "code": "24600",
      "category": "Database Server",
      "name": "SQLCipher",
    },
    {
      "code": "131",
      "category": "Database Server",
      "name": "MSSQL (2000)",
    },
    {
      "code": "132",
      "category": "Database Server",
      "name": "MSSQL (2005)",
    },
    {
      "code": "1731",
      "category": "Database Server",
      "name": "MSSQL (2012, 2014)",
    },
    {
      "code": "24100",
      "category": "Database Server",
      "name": "MongoDB ServerKey SCRAM-SHA-1",
    },
    {
      "code": "24200",
      "category": "Database Server",
      "name": "MongoDB ServerKey SCRAM-SHA-256",
    },
    {
      "code": "12",
      "category": "Database Server",
      "name": "PostgreSQL",
    },
    {
      "code": "11100",
      "category": "Database Server",
      "name": "PostgreSQL CRAM (MD5)",
    },
    {
      "code": "3100",
      "category": "Database Server",
      "name": "Oracle H: Type (Oracle 7+)",
    },
    {
      "code": "112",
      "category": "Database Server",
      "name": "Oracle S: Type (Oracle 11+)",
    },
    {
      "code": "12300",
      "category": "Database Server",
      "name": "Oracle T: Type (Oracle 12+)",
    },
    {
      "code": "7401",
      "category": "Database Server",
      "name": "MySQL $A$ (sha256crypt)",
    },
    {
      "code": "11200",
      "category": "Database Server",
      "name": "MySQL CRAM (SHA1)",
    },
    {
      "code": "200",
      "category": "Database Server",
      "name": "MySQL323",
    },
    {
      "code": "300",
      "category": "Database Server",
      "name": "MySQL4.1/MySQL5",
    },
    {
      "code": "8000",
      "category": "Database Server",
      "name": "Sybase ASE",
    },
    {
      "code": "8300",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "DNSSEC (NSEC3)",
    },
    {
      "code": "25900",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "KNX IP Secure - Device Authentication Code",
    },
    {
      "code": "16400",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "CRAM-MD5 Dovecot",
    },
    {
      "code": "1411",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "SSHA-256(Base64), LDAP {SSHA256}",
    },
    {
      "code": "1711",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "SSHA-512(Base64), LDAP {SSHA512}",
    },
    {
      "code": "24900",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "Dahua Authentication MD5",
    },
    {
      "code": "10901",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "RedHat 389-DS LDAP (PBKDF2-HMAC-SHA256)",
    },
    {
      "code": "15000",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "FileZilla Server >= 0.9.55",
    },
    {
      "code": "12600",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "ColdFusion 10+",
    },
    {
      "code": "1600",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "Apache $apr1$ MD5, md5apr1, MD5 (APR)",
    },
    {
      "code": "141",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "Episerver 6.x < .NET 4",
    },
    {
      "code": "1441",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "Episerver 6.x >= .NET 4",
    },
    {
      "code": "1421",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "hMailServer",
    },
    {
      "code": "101",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "nsldap, SHA-1(Base64), Netscape LDAP SHA",
    },
    {
      "code": "111",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "nsldaps, SSHA-1(Base64), Netscape LDAP SSHA",
    },
    {
      "code": "7700",
      "category": "Enterprise Application Software (EAS)",
      "name": "SAP CODVN B (BCODE)",
    },
    {
      "code": "7701",
      "category": "Enterprise Application Software (EAS)",
      "name": "SAP CODVN B (BCODE) from RFC_READ_TABLE",
    },
    {
      "code": "7800",
      "category": "Enterprise Application Software (EAS)",
      "name": "SAP CODVN F/G (PASSCODE)",
    },
    {
      "code": "7801",
      "category": "Enterprise Application Software (EAS)",
      "name": "SAP CODVN F/G (PASSCODE) from RFC_READ_TABLE",
    },
    {
      "code": "10300",
      "category": "Enterprise Application Software (EAS)",
      "name": "SAP CODVN H (PWDSALTEDHASH) iSSHA-1",
    },
    {
      "code": "133",
      "category": "Enterprise Application Software (EAS)",
      "name": "PeopleSoft",
    },
    {
      "code": "13500",
      "category": "Enterprise Application Software (EAS)",
      "name": "PeopleSoft PS_TOKEN",
    },
    {
      "code": "21500",
      "category": "Enterprise Application Software (EAS)",
      "name": "SolarWinds Orion",
    },
    {
      "code": "21501",
      "category": "Enterprise Application Software (EAS)",
      "name": "SolarWinds Orion v2",
    },
    {
      "code": "24",
      "category": "Enterprise Application Software (EAS)",
      "name": "SolarWinds Serv-U",
    },
    {
      "code": "8600",
      "category": "Enterprise Application Software (EAS)",
      "name": "Lotus Notes/Domino 5",
    },
    {
      "code": "8700",
      "category": "Enterprise Application Software (EAS)",
      "name": "Lotus Notes/Domino 6",
    },
    {
      "code": "9100",
      "category": "Enterprise Application Software (EAS)",
      "name": "Lotus Notes/Domino 8",
    },
    {
      "code": "26200",
      "category": "Enterprise Application Software (EAS)",
      "name": "OpenEdge Progress Encode",
    },
    {
      "code": "20600",
      "category": "Enterprise Application Software (EAS)",
      "name": "Oracle Transportation Management (SHA256)",
    },
    {
      "code": "4711",
      "category": "Enterprise Application Software (EAS)",
      "name": "Huawei sha1(md5($pass).$salt)",
    },
    {
      "code": "20711",
      "category": "Enterprise Application Software (EAS)",
      "name": "AuthMe sha256",
    },
    {
      "code": "22400",
      "category": "Full-Disk Encryption (FDE)",
      "name": "AES Crypt (SHA256)",
    },
    {
      "code": "27400",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VMware VMX (PBKDF2-HMAC-SHA1 + AES-256-CBC)",
    },
    {
      "code": "14600",
      "category": "Full-Disk Encryption (FDE)",
      "name": "LUKS",
    },
    {
      "code": "13711",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt RIPEMD160 + XTS 512 bit",
    },
    {
      "code": "13712",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt RIPEMD160 + XTS 1024 bit",
    },
    {
      "code": "13713",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt RIPEMD160 + XTS 1536 bit",
    },
    {
      "code": "13741",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt RIPEMD160 + XTS 512 bit + boot-mode",
    },
    {
      "code": "13742",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt RIPEMD160 + XTS 1024 bit + boot-mode",
    },
    {
      "code": "13743",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt RIPEMD160 + XTS 1536 bit + boot-mode",
    },
    {
      "code": "13751",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt SHA256 + XTS 512 bit",
    },
    {
      "code": "13752",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt SHA256 + XTS 1024 bit",
    },
    {
      "code": "13753",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt SHA256 + XTS 1536 bit",
    },
    {
      "code": "13761",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt SHA256 + XTS 512 bit + boot-mode",
    },
    {
      "code": "13762",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt SHA256 + XTS 1024 bit + boot-mode",
    },
    {
      "code": "13763",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt SHA256 + XTS 1536 bit + boot-mode",
    },
    {
      "code": "13721",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt SHA512 + XTS 512 bit",
    },
    {
      "code": "13722",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt SHA512 + XTS 1024 bit",
    },
    {
      "code": "13723",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt SHA512 + XTS 1536 bit",
    },
    {
      "code": "13771",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt Streebog-512 + XTS 512 bit",
    },
    {
      "code": "13772",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt Streebog-512 + XTS 1024 bit",
    },
    {
      "code": "13773",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt Streebog-512 + XTS 1536 bit",
    },
    {
      "code": "13781",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt Streebog-512 + XTS 512 bit + boot-mode",
    },
    {
      "code": "13782",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt Streebog-512 + XTS 1024 bit + boot-mode",
    },
    {
      "code": "13783",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt Streebog-512 + XTS 1536 bit + boot-mode",
    },
    {
      "code": "13731",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt Whirlpool + XTS 512 bit",
    },
    {
      "code": "13732",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt Whirlpool + XTS 1024 bit",
    },
    {
      "code": "13733",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt Whirlpool + XTS 1536 bit",
    },
    {
      "code": "23900",
      "category": "Full-Disk Encryption (FDE)",
      "name": "BestCrypt v3 Volume Encryption",
    },
    {
      "code": "16700",
      "category": "Full-Disk Encryption (FDE)",
      "name": "FileVault 2",
    },
    {
      "code": "27500",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VirtualBox (PBKDF2-HMAC-SHA256 & AES-128-XTS)",
    },
    {
      "code": "27600",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VirtualBox (PBKDF2-HMAC-SHA256 & AES-256-XTS)",
    },
    {
      "code": "20011",
      "category": "Full-Disk Encryption (FDE)",
      "name": "DiskCryptor SHA512 + XTS 512 bit",
    },
    {
      "code": "20012",
      "category": "Full-Disk Encryption (FDE)",
      "name": "DiskCryptor SHA512 + XTS 1024 bit",
    },
    {
      "code": "20013",
      "category": "Full-Disk Encryption (FDE)",
      "name": "DiskCryptor SHA512 + XTS 1536 bit",
    },
    {
      "code": "22100",
      "category": "Full-Disk Encryption (FDE)",
      "name": "BitLocker",
    },
    {
      "code": "12900",
      "category": "Full-Disk Encryption (FDE)",
      "name": "Android FDE (Samsung DEK)",
    },
    {
      "code": "8800",
      "category": "Full-Disk Encryption (FDE)",
      "name": "Android FDE <= 4.3",
    },
    {
      "code": "18300",
      "category": "Full-Disk Encryption (FDE)",
      "name": "Apple File System (APFS)",
    },
    {
      "code": "6211",
      "category": "Full-Disk Encryption (FDE)",
      "name": "TrueCrypt RIPEMD160 + XTS 512 bit",
    },
    {
      "code": "6212",
      "category": "Full-Disk Encryption (FDE)",
      "name": "TrueCrypt RIPEMD160 + XTS 1024 bit",
    },
    {
      "code": "6213",
      "category": "Full-Disk Encryption (FDE)",
      "name": "TrueCrypt RIPEMD160 + XTS 1536 bit",
    },
    {
      "code": "6241",
      "category": "Full-Disk Encryption (FDE)",
      "name": "TrueCrypt RIPEMD160 + XTS 512 bit + boot-mode",
    },
    {
      "code": "6242",
      "category": "Full-Disk Encryption (FDE)",
      "name": "TrueCrypt RIPEMD160 + XTS 1024 bit + boot-mode",
    },
    {
      "code": "6243",
      "category": "Full-Disk Encryption (FDE)",
      "name": "TrueCrypt RIPEMD160 + XTS 1536 bit + boot-mode",
    },
    {
      "code": "6221",
      "category": "Full-Disk Encryption (FDE)",
      "name": "TrueCrypt SHA512 + XTS 512 bit",
    },
    {
      "code": "6222",
      "category": "Full-Disk Encryption (FDE)",
      "name": "TrueCrypt SHA512 + XTS 1024 bit",
    },
    {
      "code": "6223",
      "category": "Full-Disk Encryption (FDE)",
      "name": "TrueCrypt SHA512 + XTS 1536 bit",
    },
    {
      "code": "6231",
      "category": "Full-Disk Encryption (FDE)",
      "name": "TrueCrypt Whirlpool + XTS 512 bit",
    },
    {
      "code": "6232",
      "category": "Full-Disk Encryption (FDE)",
      "name": "TrueCrypt Whirlpool + XTS 1024 bit",
    },
    {
      "code": "6233",
      "category": "Full-Disk Encryption (FDE)",
      "name": "TrueCrypt Whirlpool + XTS 1536 bit",
    },
    {
      "code": "12200",
      "category": "Full-Disk Encryption (FDE)",
      "name": "eCryptfs",
    },
    {
      "code": "10400",
      "category": "Document",
      "name": "PDF 1.1 - 1.3 (Acrobat 2 - 4)",
    },
    {
      "code": "10410",
      "category": "Document",
      "name": "PDF 1.1 - 1.3 (Acrobat 2 - 4), collider #1",
    },
    {
      "code": "10420",
      "category": "Document",
      "name": "PDF 1.1 - 1.3 (Acrobat 2 - 4), collider #2",
    },
    {
      "code": "10500",
      "category": "Document",
      "name": "PDF 1.4 - 1.6 (Acrobat 5 - 8)",
    },
    {
      "code": "25400",
      "category": "Document",
      "name": "PDF 1.4 - 1.6 (Acrobat 5 - 8) - user and owner pass",
    },
    {
      "code": "10600",
      "category": "Document",
      "name": "PDF 1.7 Level 3 (Acrobat 9)",
    },
    {
      "code": "10700",
      "category": "Document",
      "name": "PDF 1.7 Level 8 (Acrobat 10 - 11)",
    },
    {
      "code": "9400",
      "category": "Document",
      "name": "MS Office 2007",
    },
    {
      "code": "9500",
      "category": "Document",
      "name": "MS Office 2010",
    },
    {
      "code": "9600",
      "category": "Document",
      "name": "MS Office 2013",
    },
    {
      "code": "25300",
      "category": "Document",
      "name": "MS Office 2016 - SheetProtection",
    },
    {
      "code": "9700",
      "category": "Document",
      "name": "MS Office <= 2003 $0/$1, MD5 + RC4",
    },
    {
      "code": "9710",
      "category": "Document",
      "name": "MS Office <= 2003 $0/$1, MD5 + RC4, collider #1",
    },
    {
      "code": "9720",
      "category": "Document",
      "name": "MS Office <= 2003 $0/$1, MD5 + RC4, collider #2",
    },
    {
      "code": "9810",
      "category": "Document",
      "name": "MS Office <= 2003 $3, SHA1 + RC4, collider #1",
    },
    {
      "code": "9820",
      "category": "Document",
      "name": "MS Office <= 2003 $3, SHA1 + RC4, collider #2",
    },
    {
      "code": "9800",
      "category": "Document",
      "name": "MS Office <= 2003 $3/$4, SHA1 + RC4",
    },
    {
      "code": "18400",
      "category": "Document",
      "name": "Open Document Format (ODF) 1.2 (SHA-256, AES)",
    },
    {
      "code": "18600",
      "category": "Document",
      "name": "Open Document Format (ODF) 1.1 (SHA-1, Blowfish)",
    },
    {
      "code": "16200",
      "category": "Document",
      "name": "Apple Secure Notes",
    },
    {
      "code": "23300",
      "category": "Document",
      "name": "Apple iWork",
    },
    {
      "code": "6600",
      "category": "Password Manager",
      "name": "1Password, agilekeychain",
    },
    {
      "code": "8200",
      "category": "Password Manager",
      "name": "1Password, cloudkeychain",
    },
    {
      "code": "9000",
      "category": "Password Manager",
      "name": "Password Safe v2",
    },
    {
      "code": "5200",
      "category": "Password Manager",
      "name": "Password Safe v3",
    },
    {
      "code": "6800",
      "category": "Password Manager",
      "name": "LastPass + LastPass sniffed",
    },
    {
      "code": "13400",
      "category": "Password Manager",
      "name": "KeePass 1 (AES/Twofish) and KeePass 2 (AES)",
    },
    {
      "code": "23400",
      "category": "Password Manager",
      "name": "Bitwarden",
    },
    {
      "code": "16900",
      "category": "Password Manager",
      "name": "Ansible Vault",
    },
    {
      "code": "26000",
      "category": "Password Manager",
      "name": "Mozilla key3.db",
    },
    {
      "code": "26100",
      "category": "Password Manager",
      "name": "Mozilla key4.db",
    },
    {
      "code": "23100",
      "category": "Password Manager",
      "name": "Apple Keychain",
    },
    {
      "code": "11600",
      "category": "Archive",
      "name": "7-Zip",
    },
    {
      "code": "12500",
      "category": "Archive",
      "name": "RAR3-hp",
    },
    {
      "code": "23800",
      "category": "Archive",
      "name": "RAR3-p (Compressed)",
    },
    {
      "code": "23700",
      "category": "Archive",
      "name": "RAR3-p (Uncompressed)",
    },
    {
      "code": "13000",
      "category": "Archive",
      "name": "RAR5",
    },
    {
      "code": "17220",
      "category": "Archive",
      "name": "PKZIP (Compressed Multi-File)",
    },
    {
      "code": "17200",
      "category": "Archive",
      "name": "PKZIP (Compressed)",
    },
    {
      "code": "17225",
      "category": "Archive",
      "name": "PKZIP (Mixed Multi-File)",
    },
    {
      "code": "17230",
      "category": "Archive",
      "name": "PKZIP (Mixed Multi-File Checksum-Only)",
    },
    {
      "code": "17210",
      "category": "Archive",
      "name": "PKZIP (Uncompressed)",
    },
    {
      "code": "20500",
      "category": "Archive",
      "name": "PKZIP Master Key",
    },
    {
      "code": "20510",
      "category": "Archive",
      "name": "PKZIP Master Key (6 byte optimization)",
    },
    {
      "code": "23001",
      "category": "Archive",
      "name": "SecureZIP AES-128",
    },
    {
      "code": "23002",
      "category": "Archive",
      "name": "SecureZIP AES-192",
    },
    {
      "code": "23003",
      "category": "Archive",
      "name": "SecureZIP AES-256",
    },
    {
      "code": "13600",
      "category": "Archive",
      "name": "WinZip",
    },
    {
      "code": "18900",
      "category": "Archive",
      "name": "Android Backup",
    },
    {
      "code": "24700",
      "category": "Archive",
      "name": "Stuffit5",
    },
    {
      "code": "13200",
      "category": "Archive",
      "name": "AxCrypt 1",
    },
    {
      "code": "13300",
      "category": "Archive",
      "name": "AxCrypt 1 in-memory SHA1",
    },
    {
      "code": "23500",
      "category": "Archive",
      "name": "AxCrypt 2 AES-128",
    },
    {
      "code": "23600",
      "category": "Archive",
      "name": "AxCrypt 2 AES-256",
    },
    {
      "code": "14700",
      "category": "Archive",
      "name": "iTunes backup < 10.0",
    },
    {
      "code": "14800",
      "category": "Archive",
      "name": "iTunes backup >= 10.0",
    },
    {
      "code": "8400",
      "category": "Forums, CMS, E-Commerce",
      "name": "WBB3 (Woltlab Burning Board)",
    },
    {
      "code": "2612",
      "category": "Forums, CMS, E-Commerce",
      "name": "PHPS",
    },
    {
      "code": "121",
      "category": "Forums, CMS, E-Commerce",
      "name": "SMF (Simple Machines Forum) > v1.1",
    },
    {
      "code": "3711",
      "category": "Forums, CMS, E-Commerce",
      "name": "MediaWiki B type",
    },
    {
      "code": "4521",
      "category": "Forums, CMS, E-Commerce",
      "name": "Redmine",
    },
    {
      "code": "24800",
      "category": "Forums, CMS, E-Commerce",
      "name": "Umbraco HMAC-SHA1",
    },
    {
      "code": "11",
      "category": "Forums, CMS, E-Commerce",
      "name": "Joomla < 2.5.18",
    },
    {
      "code": "13900",
      "category": "Forums, CMS, E-Commerce",
      "name": "OpenCart",
    },
    {
      "code": "11000",
      "category": "Forums, CMS, E-Commerce",
      "name": "PrestaShop",
    },
    {
      "code": "16000",
      "category": "Forums, CMS, E-Commerce",
      "name": "Tripcode",
    },
    {
      "code": "7900",
      "category": "Forums, CMS, E-Commerce",
      "name": "Drupal7",
    },
    {
      "code": "4522",
      "category": "Forums, CMS, E-Commerce",
      "name": "PunBB",
    },
    {
      "code": "2811",
      "category": "Forums, CMS, E-Commerce",
      "name": "MyBB 1.2+, IPB2+ (Invision Power Board)",
    },
    {
      "code": "2611",
      "category": "Forums, CMS, E-Commerce",
      "name": "vBulletin < v3.8.5",
    },
    {
      "code": "2711",
      "category": "Forums, CMS, E-Commerce",
      "name": "vBulletin >= v3.8.5",
    },
    {
      "code": "25600",
      "category": "Forums, CMS, E-Commerce",
      "name": "bcrypt(md5($pass)) / bcryptmd5",
    },
    {
      "code": "25800",
      "category": "Forums, CMS, E-Commerce",
      "name": "bcrypt(sha1($pass)) / bcryptsha1",
    },
    {
      "code": "21",
      "category": "Forums, CMS, E-Commerce",
      "name": "osCommerce, xt:Commerce",
    },
    {
      "code": "18100",
      "category": "One-Time Password",
      "name": "TOTP (HMAC-SHA1)",
    },
    {
      "code": "2000",
      "category": "Plaintext",
      "name": "STDOUT",
    },
    {
      "code": "99999",
      "category": "Plaintext",
      "name": "Plaintext",
    },
    {
      "code": "21600",
      "category": "Framework",
      "name": "Web2py pbkdf2-sha512",
    },
    {
      "code": "10000",
      "category": "Framework",
      "name": "Django (PBKDF2-SHA256)",
    },
    {
      "code": "124",
      "category": "Framework",
      "name": "Django (SHA-1)",
    },
    {
      "code": "12001",
      "category": "Framework",
      "name": "Atlassian (PBKDF2-HMAC-SHA1)",
    },
    {
      "code": "19500",
      "category": "Framework",
      "name": "Ruby on Rails Restful-Authentication",
    },
    {
      "code": "27200",
      "category": "Framework",
      "name": "Ruby on Rails Restful Auth (one round, no sitekey)",
    },
    {
      "code": "20200",
      "category": "Framework",
      "name": "Python passlib pbkdf2-sha512",
    },
    {
      "code": "20300",
      "category": "Framework",
      "name": "Python passlib pbkdf2-sha256",
    },
    {
      "code": "20400",
      "category": "Framework",
      "name": "Python passlib pbkdf2-sha1",
    },
    {
      "code": "24410",
      "category": "Private Key",
      "name": "PKCS#8 Private Keys (PBKDF2-HMAC-SHA1 + 3DES/AES)",
    },
    {
      "code": "24420",
      "category": "Private Key",
      "name": "PKCS#8 Private Keys (PBKDF2-HMAC-SHA256 + 3DES/AES)",
    },
    {
      "code": "15500",
      "category": "Private Key",
      "name": "JKS Java Key Store Private Keys (SHA1)",
    },
    {
      "code": "22911",
      "category": "Private Key",
      "name": "RSA/DSA/EC/OpenSSH Private Keys ($0$)",
    },
    {
      "code": "22921",
      "category": "Private Key",
      "name": "RSA/DSA/EC/OpenSSH Private Keys ($6$)",
    },
    {
      "code": "22931",
      "category": "Private Key",
      "name": "RSA/DSA/EC/OpenSSH Private Keys ($1, $3$)",
    },
    {
      "code": "22941",
      "category": "Private Key",
      "name": "RSA/DSA/EC/OpenSSH Private Keys ($4$)",
    },
    {
      "code": "22951",
      "category": "Private Key",
      "name": "RSA/DSA/EC/OpenSSH Private Keys ($5$)",
    },
    {
      "code": "23200",
      "category": "Instant Messaging Service",
      "name": "XMPP SCRAM PBKDF2-SHA1",
    },
    {
      "code": "22600",
      "category": "Instant Messaging Service",
      "name": "Telegram Desktop < v2.1.14 (PBKDF2-HMAC-SHA1)",
    },
    {
      "code": "24500",
      "category": "Instant Messaging Service",
      "name": "Telegram Desktop >= v2.1.14 (PBKDF2-HMAC-SHA512)",
    },
    {
      "code": "22301",
      "category": "Instant Messaging Service",
      "name": "Telegram Mobile App Passcode (SHA256)",
    },
    {
      "code": "23",
      "category": "Instant Messaging Service",
      "name": "Skype",
    },
    {
      "code": "26600",
      "category": "Cryptocurrency Wallet",
      "name": "MetaMask Wallet",
    },
    {
      "code": "21000",
      "category": "Cryptocurrency Wallet",
      "name": "BitShares v0.x - sha512(sha512_bin(pass))",
    },
    {
      "code": "11300",
      "category": "Cryptocurrency Wallet",
      "name": "Bitcoin/Litecoin wallet.dat",
    },
    {
      "code": "16600",
      "category": "Cryptocurrency Wallet",
      "name": "Electrum Wallet (Salt-Type 1-3)",
    },
    {
      "code": "21700",
      "category": "Cryptocurrency Wallet",
      "name": "Electrum Wallet (Salt-Type 4)",
    },
    {
      "code": "21800",
      "category": "Cryptocurrency Wallet",
      "name": "Electrum Wallet (Salt-Type 5)",
    },
    {
      "code": "12700",
      "category": "Cryptocurrency Wallet",
      "name": "Blockchain, My Wallet",
    },
    {
      "code": "15200",
      "category": "Cryptocurrency Wallet",
      "name": "Blockchain, My Wallet, V2",
    },
    {
      "code": "18800",
      "category": "Cryptocurrency Wallet",
      "name": "Blockchain, My Wallet, Second Password (SHA256)",
    },
    {
      "code": "25500",
      "category": "Cryptocurrency Wallet",
      "name": "Stargazer Stellar Wallet XLM",
    },
    {
      "code": "16300",
      "category": "Cryptocurrency Wallet",
      "name": "Ethereum Pre-Sale Wallet, PBKDF2-HMAC-SHA256",
    },
    {
      "code": "15600",
      "category": "Cryptocurrency Wallet",
      "name": "Ethereum Wallet, PBKDF2-HMAC-SHA256",
    },
    {
      "code": "15700",
      "category": "Cryptocurrency Wallet",
      "name": "Ethereum Wallet, SCRYPT",
    },
    {
      "code": "22500",
      "category": "Cryptocurrency Wallet",
      "name": "MultiBit Classic .key (MD5)",
    },
    {
      "code": "27700",
      "category": "Cryptocurrency Wallet",
      "name": "MultiBit Classic .wallet (scrypt)",
    },
    {
      "code": "22700",
      "category": "Cryptocurrency Wallet",
      "name": "MultiBit HD (scrypt)",
    },
]

def getHashById(code):
  for hash in hashtypes:
    if hash['code'] == code:
      return hash
      break
  return None
