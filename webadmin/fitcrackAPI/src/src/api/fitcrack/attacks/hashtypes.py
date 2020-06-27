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
      "code": "5100",
      "category": "Raw Hash",
      "name": "Half MD5",
    },
    {
      "code": "18700",
      "category": "Raw Hash",
      "name": "Java Object hashCode()",
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
      "code": "21400",
      "category": "Raw Hash",
      "name": "sha256(sha256_bin($pass))",
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
      "code": "21000",
      "category": "Raw Hash",
      "name": "BitShares v0.x - sha512(sha512_bin(pass))",
    },
    {
      "code": "10",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5($pass.$salt)",
    },
    {
      "code": "20",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5($salt.$pass)",
    },
    {
      "code": "3800",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5($salt.$pass.$salt)",
    },
    {
      "code": "3710",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5($salt.md5($pass))",
    },
    {
      "code": "4110",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5($salt.md5($pass.$salt))",
    },
    {
      "code": "4010",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5($salt.md5($salt.$pass))",
    },
    {
      "code": "21300",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5($salt.sha1($salt.$pass))",
    },
    {
      "code": "40",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5($salt.utf16le($pass))",
    },
    {
      "code": "2600",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5(md5($pass))",
    },
    {
      "code": "3910",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5(md5($pass).md5($salt))",
    },
    {
      "code": "4400",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5(sha1($pass))",
    },
    {
      "code": "20900",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5(sha1($pass).md5($pass).sha1($pass))",
    },
    {
      "code": "21200",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5(sha1($salt).md5($pass))",
    },
    {
      "code": "4300",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5(strtoupper(md5($pass)))",
    },
    {
      "code": "30",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5(utf16le($pass).$salt)",
    },
    {
      "code": "110",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1($pass.$salt)",
    },
    {
      "code": "120",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1($salt.$pass)",
    },
    {
      "code": "4900",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1($salt.$pass.$salt)",
    },
    {
      "code": "4520",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1($salt.sha1($pass))",
    },
    {
      "code": "140",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1($salt.utf16le($pass))",
    },
    {
      "code": "19300",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1($salt1.$pass.$salt2)",
    },
    {
      "code": "14400",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1(CX)",
    },
    {
      "code": "4700",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1(md5($pass))",
    },
    {
      "code": "4710",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1(md5($pass).$salt)",
    },
    {
      "code": "21100",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1(md5($pass.$salt))",
    },
    {
      "code": "18500",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1(md5(md5($pass)))",
    },
    {
      "code": "4500",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1(sha1($pass))",
    },
    {
      "code": "130",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1(utf16le($pass).$salt)",
    },
    {
      "code": "1410",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha256($pass.$salt)",
    },
    {
      "code": "1420",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha256($salt.$pass)",
    },
    {
      "code": "22300",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha256($salt.$pass.$salt)",
    },
    {
      "code": "1440",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha256($salt.utf16le($pass))",
    },
    {
      "code": "20800",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha256(md5($pass))",
    },
    {
      "code": "20710",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha256(sha256($pass).$salt)",
    },
    {
      "code": "1430",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha256(utf16le($pass).$salt)",
    },
    {
      "code": "1710",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha512($pass.$salt)",
    },
    {
      "code": "1720",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha512($salt.$pass)",
    },
    {
      "code": "1740",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha512($salt.utf16le($pass))",
    },
    {
      "code": "1730",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha512(utf16le($pass).$salt)",
    },
    {
      "code": "19500",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "Ruby on Rails Restful-Authentication",
    },
    {
      "code": "50",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-MD5 (key = $pass)",
    },
    {
      "code": "60",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-MD5 (key = $salt)",
    },
    {
      "code": "150",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-SHA1 (key = $pass)",
    },
    {
      "code": "160",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-SHA1 (key = $salt)",
    },
    {
      "code": "1450",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-SHA256 (key = $pass)",
    },
    {
      "code": "1460",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-SHA256 (key = $salt)",
    },
    {
      "code": "1750",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-SHA512 (key = $pass)",
    },
    {
      "code": "1760",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-SHA512 (key = $salt)",
    },
    {
      "code": "11750",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-Streebog-256 (key = $pass), big-endian",
    },
    {
      "code": "11760",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-Streebog-256 (key = $salt), big-endian",
    },
    {
      "code": "11850",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-Streebog-512 (key = $pass), big-endian",
    },
    {
      "code": "11860",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-Streebog-512 (key = $salt), big-endian",
    },
    {
      "code": "11500",
      "category": "Raw Checksum",
      "name": "CRC32",
    },
    {
      "code": "14100",
      "category": "Raw Cipher, Known-Plaintext attack",
      "name": "3DES (PT = $salt, key = $pass)",
    },
    {
      "code": "14000",
      "category": "Raw Cipher, Known-Plaintext attack",
      "name": "DES (PT = $salt, key = $pass)",
    },
    {
      "code": "15400",
      "category": "Raw Cipher, Known-Plaintext attack",
      "name": "ChaCha20",
    },
    {
      "code": "14900",
      "category": "Raw Cipher, Known-Plaintext attack",
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
      "code": "16900",
      "category": "Generic KDF",
      "name": "Ansible Vault",
    },
    {
      "code": "12001",
      "category": "Generic KDF",
      "name": "Atlassian (PBKDF2-HMAC-SHA1)",
    },
    {
      "code": "20200",
      "category": "Generic KDF",
      "name": "Python passlib pbkdf2-sha512",
    },
    {
      "code": "20300",
      "category": "Generic KDF",
      "name": "Python passlib pbkdf2-sha256",
    },
    {
      "code": "20400",
      "category": "Generic KDF",
      "name": "Python passlib pbkdf2-sha1",
    },
    {
      "code": "16100",
      "category": "Network Protocols",
      "name": "TACACS+",
    },
    {
      "code": "11400",
      "category": "Network Protocols",
      "name": "SIP digest authentication (MD5)",
    },
    {
      "code": "5300",
      "category": "Network Protocols",
      "name": "IKE-PSK MD5",
    },
    {
      "code": "5400",
      "category": "Network Protocols",
      "name": "IKE-PSK SHA1",
    },
    {
      "code": "2500",
      "category": "Network Protocols",
      "name": "WPA-EAPOL-PBKDF2",
    },
    {
      "code": "2501",
      "category": "Network Protocols",
      "name": "WPA-EAPOL-PMK",
    },
    {
      "code": "22000",
      "category": "Network Protocols",
      "name": "WPA-PBKDF2-PMKID+EAPOL",
    },
    {
      "code": "22001",
      "category": "Network Protocols",
      "name": "WPA-PMK-PMKID+EAPOL",
    },
    {
      "code": "16800",
      "category": "Network Protocols",
      "name": "WPA-PMKID-PBKDF2",
    },
    {
      "code": "16801",
      "category": "Network Protocols",
      "name": "WPA-PMKID-PMK",
    },
    {
      "code": "7300",
      "category": "Network Protocols",
      "name": "IPMI2 RAKP HMAC-SHA1",
    },
    {
      "code": "10200",
      "category": "Network Protocols",
      "name": "CRAM-MD5",
    },
    {
      "code": "4800",
      "category": "Network Protocols",
      "name": "iSCSI CHAP authentication, MD5(CHAP)",
    },
    {
      "code": "16500",
      "category": "Network Protocols",
      "name": "JWT (JSON Web Token)",
    },
    {
      "code": "22600",
      "category": "Network Protocols",
      "name": "Telegram Desktop App Passcode (PBKDF2-HMAC-SHA1)",
    },
    {
      "code": "22301",
      "category": "Network Protocols",
      "name": "Telegram Mobile App Passcode (SHA256)",
    },
    {
      "code": "7500",
      "category": "Network Protocols",
      "name": "Kerberos 5, etype 23, AS-REQ Pre-Auth",
    },
    {
      "code": "13100",
      "category": "Network Protocols",
      "name": "Kerberos 5, etype 23, TGS-REP",
    },
    {
      "code": "18200",
      "category": "Network Protocols",
      "name": "Kerberos 5, etype 23, AS-REP",
    },
    {
      "code": "19600",
      "category": "Network Protocols",
      "name": "Kerberos 5, etype 17, TGS-REP",
    },
    {
      "code": "19700",
      "category": "Network Protocols",
      "name": "Kerberos 5, etype 18, TGS-REP",
    },
    {
      "code": "19800",
      "category": "Network Protocols",
      "name": "Kerberos 5, etype 17, Pre-Auth",
    },
    {
      "code": "19900",
      "category": "Network Protocols",
      "name": "Kerberos 5, etype 18, Pre-Auth",
    },
    {
      "code": "5500",
      "category": "Network Protocols",
      "name": "NetNTLMv1 / NetNTLMv1+ESS",
    },
    {
      "code": "5600",
      "category": "Network Protocols",
      "name": "NetNTLMv2",
    },
    {
      "code": "23",
      "category": "Network Protocols",
      "name": "Skype",
    },
    {
      "code": "11100",
      "category": "Network Protocols",
      "name": "PostgreSQL CRAM (MD5)",
    },
    {
      "code": "11200",
      "category": "Network Protocols",
      "name": "MySQL CRAM (SHA1)",
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
      "code": "122",
      "category": "Operating System",
      "name": "macOS v10.4, macOS v10.5, MacOS v10.6",
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
      "code": "12",
      "category": "Database Server",
      "name": "PostgreSQL",
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
      "code": "1421",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "hMailServer",
    },
    {
      "code": "8300",
      "category": "FTP, HTTP, SMTP, LDAP Server",
      "name": "DNSSEC (NSEC3)",
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
      "code": "12200",
      "category": "Full-Disk Encryption (FDE)",
      "name": "eCryptfs",
    },
    {
      "code": "22400",
      "category": "Full-Disk Encryption (FDE)",
      "name": "AES Crypt (SHA256)",
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
      "code": "16700",
      "category": "Full-Disk Encryption (FDE)",
      "name": "FileVault 2",
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
      "code": "10400",
      "category": "Documents",
      "name": "PDF 1.1 - 1.3 (Acrobat 2 - 4)",
    },
    {
      "code": "10410",
      "category": "Documents",
      "name": "PDF 1.1 - 1.3 (Acrobat 2 - 4), collider #1",
    },
    {
      "code": "10420",
      "category": "Documents",
      "name": "PDF 1.1 - 1.3 (Acrobat 2 - 4), collider #2",
    },
    {
      "code": "10500",
      "category": "Documents",
      "name": "PDF 1.4 - 1.6 (Acrobat 5 - 8)",
    },
    {
      "code": "10600",
      "category": "Documents",
      "name": "PDF 1.7 Level 3 (Acrobat 9)",
    },
    {
      "code": "10700",
      "category": "Documents",
      "name": "PDF 1.7 Level 8 (Acrobat 10 - 11)",
    },
    {
      "code": "9400",
      "category": "Documents",
      "name": "MS Office 2007",
    },
    {
      "code": "9500",
      "category": "Documents",
      "name": "MS Office 2010",
    },
    {
      "code": "9600",
      "category": "Documents",
      "name": "MS Office 2013",
    },
    {
      "code": "9700",
      "category": "Documents",
      "name": "MS Office <= 2003 $0/$1, MD5 + RC4",
    },
    {
      "code": "9710",
      "category": "Documents",
      "name": "MS Office <= 2003 $0/$1, MD5 + RC4, collider #1",
    },
    {
      "code": "9720",
      "category": "Documents",
      "name": "MS Office <= 2003 $0/$1, MD5 + RC4, collider #2",
    },
    {
      "code": "9800",
      "category": "Documents",
      "name": "MS Office <= 2003 $3/$4, SHA1 + RC4",
    },
    {
      "code": "9810",
      "category": "Documents",
      "name": "MS Office <= 2003 $3, SHA1 + RC4, collider #1",
    },
    {
      "code": "9820",
      "category": "Documents",
      "name": "MS Office <= 2003 $3, SHA1 + RC4, collider #2",
    },
    {
      "code": "18400",
      "category": "Documents",
      "name": "Open Document Format (ODF) 1.2 (SHA-256, AES)",
    },
    {
      "code": "18600",
      "category": "Documents",
      "name": "Open Document Format (ODF) 1.1 (SHA-1, Blowfish)",
    },
    {
      "code": "16200",
      "category": "Documents",
      "name": "Apple Secure Notes",
    },
    {
      "code": "15500",
      "category": "Password Managers",
      "name": "JKS Java Key Store Private Keys (SHA1)",
    },
    {
      "code": "6600",
      "category": "Password Managers",
      "name": "1Password, agilekeychain",
    },
    {
      "code": "8200",
      "category": "Password Managers",
      "name": "1Password, cloudkeychain",
    },
    {
      "code": "9000",
      "category": "Password Managers",
      "name": "Password Safe v2",
    },
    {
      "code": "5200",
      "category": "Password Managers",
      "name": "Password Safe v3",
    },
    {
      "code": "6800",
      "category": "Password Managers",
      "name": "LastPass + LastPass sniffed",
    },
    {
      "code": "13400",
      "category": "Password Managers",
      "name": "KeePass 1 (AES/Twofish) and KeePass 2 (AES)",
    },
    {
      "code": "11300",
      "category": "Password Managers",
      "name": "Bitcoin/Litecoin wallet.dat",
    },
    {
      "code": "16600",
      "category": "Password Managers",
      "name": "Electrum Wallet (Salt-Type 1-3)",
    },
    {
      "code": "21700",
      "category": "Password Managers",
      "name": "Electrum Wallet (Salt-Type 4)",
    },
    {
      "code": "21800",
      "category": "Password Managers",
      "name": "Electrum Wallet (Salt-Type 5)",
    },
    {
      "code": "12700",
      "category": "Password Managers",
      "name": "Blockchain, My Wallet",
    },
    {
      "code": "15200",
      "category": "Password Managers",
      "name": "Blockchain, My Wallet, V2",
    },
    {
      "code": "18800",
      "category": "Password Managers",
      "name": "Blockchain, My Wallet, Second Password (SHA256)",
    },
    {
      "code": "16300",
      "category": "Password Managers",
      "name": "Ethereum Pre-Sale Wallet, PBKDF2-HMAC-SHA256",
    },
    {
      "code": "15600",
      "category": "Password Managers",
      "name": "Ethereum Wallet, PBKDF2-HMAC-SHA256",
    },
    {
      "code": "15700",
      "category": "Password Managers",
      "name": "Ethereum Wallet, SCRYPT",
    },
    {
      "code": "22500",
      "category": "Password Managers",
      "name": "MultiBit Classic .key (MD5)",
    },
    {
      "code": "22700",
      "category": "Password Managers",
      "name": "MultiBit HD (scrypt)",
    },
    {
      "code": "11600",
      "category": "Archives",
      "name": "7-Zip",
    },
    {
      "code": "12500",
      "category": "Archives",
      "name": "RAR3-hp",
    },
    {
      "code": "13000",
      "category": "Archives",
      "name": "RAR5",
    },
    {
      "code": "17200",
      "category": "Archives",
      "name": "PKZIP (Compressed)",
    },
    {
      "code": "17220",
      "category": "Archives",
      "name": "PKZIP (Compressed Multi-File)",
    },
    {
      "code": "17225",
      "category": "Archives",
      "name": "PKZIP (Mixed Multi-File)",
    },
    {
      "code": "17230",
      "category": "Archives",
      "name": "PKZIP (Mixed Multi-File Checksum-Only)",
    },
    {
      "code": "17210",
      "category": "Archives",
      "name": "PKZIP (Uncompressed)",
    },
    {
      "code": "20500",
      "category": "Archives",
      "name": "PKZIP Master Key",
    },
    {
      "code": "20510",
      "category": "Archives",
      "name": "PKZIP Master Key (6 byte optimization)",
    },
    {
      "code": "14700",
      "category": "Archives",
      "name": "iTunes backup < 10.0",
    },
    {
      "code": "14800",
      "category": "Archives",
      "name": "iTunes backup >= 10.0",
    },
    {
      "code": "23001",
      "category": "Archives",
      "name": "SecureZIP AES-128",
    },
    {
      "code": "23002",
      "category": "Archives",
      "name": "SecureZIP AES-192",
    },
    {
      "code": "23003",
      "category": "Archives",
      "name": "SecureZIP AES-256",
    },
    {
      "code": "13600",
      "category": "Archives",
      "name": "WinZip",
    },
    {
      "code": "18900",
      "category": "Archives",
      "name": "Android Backup",
    },
    {
      "code": "13200",
      "category": "Archives",
      "name": "AxCrypt",
    },
    {
      "code": "13300",
      "category": "Archives",
      "name": "AxCrypt in-memory SHA1",
    },
    {
      "code": "8400",
      "category": "Forums, CMS, E-Commerce",
      "name": "WBB3 (Woltlab Burning Board)",
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
      "code": "21",
      "category": "Forums, CMS, E-Commerce",
      "name": "osCommerce, xt:Commerce",
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
      "code": "18100",
      "category": "One-Time Passwords",
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
    }
  ]


def getHashById(code):
  if code.startswith("137"):
      code = "137XY"
  if code.startswith("62"):
      code = "62XY"
  for hash in hashtypes:
    if hash['code'] == code:
      return hash
      break
  return None
