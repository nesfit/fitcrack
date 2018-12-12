'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

hashtypes = [
    {
      "code": "900",
      "category": "Raw Hash",
      "name": "MD4"
    },
    {
      "code": "0",
      "category": "Raw Hash",
      "name": "MD5"
    },
    {
      "code": "5100",
      "category": "Raw Hash",
      "name": "Half MD5"
    },
    {
      "code": "100",
      "category": "Raw Hash",
      "name": "SHA1"
    },
    {
      "code": "1300",
      "category": "Raw Hash",
      "name": "SHA-224"
    },
    {
      "code": "1400",
      "category": "Raw Hash",
      "name": "SHA-256"
    },
    {
      "code": "10800",
      "category": "Raw Hash",
      "name": "SHA-384"
    },
    {
      "code": "1700",
      "category": "Raw Hash",
      "name": "SHA-512"
    },
    {
      "code": "5000",
      "category": "Raw Hash",
      "name": "SHA-3 (Keccak)"
    },
    {
      "code": "600",
      "category": "Raw Hash",
      "name": "BLAKE2b-512"
    },
    {
      "code": "10100",
      "category": "Raw Hash",
      "name": "SipHash"
    },
    {
      "code": "6000",
      "category": "Raw Hash",
      "name": "RIPEMD-160"
    },
    {
      "code": "6100",
      "category": "Raw Hash",
      "name": "Whirlpool"
    },
    {
      "code": "6900",
      "category": "Raw Hash",
      "name": "GOST R 34.11-94"
    },
    {
      "code": "11700",
      "category": "Raw Hash",
      "name": "GOST R 34.11-2012 (Streebog) 256-bit"
    },
    {
      "code": "11800",
      "category": "Raw Hash",
      "name": "GOST R 34.11-2012 (Streebog) 512-bit"
    },
    {
      "code": "10",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5($pass.$salt)"
    },
    {
      "code": "20",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5($salt.$pass)"
    },
    {
      "code": "30",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5(utf16le($pass).$salt)"
    },
    {
      "code": "40",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5($salt.utf16le($pass))"
    },
    {
      "code": "3800",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5($salt.$pass.$salt)"
    },
    {
      "code": "3710",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5($salt.md5($pass))"
    },
    {
      "code": "4010",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5($salt.md5($salt.$pass))"
    },
    {
      "code": "4110",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5($salt.md5($pass.$salt))"
    },
    {
      "code": "2600",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5(md5($pass))"
    },
    {
      "code": "3910",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5(md5($pass).md5($salt))"
    },
    {
      "code": "4300",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5(strtoupper(md5($pass)))"
    },
    {
      "code": "4400",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "md5(sha1($pass))"
    },
    {
      "code": "110",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1($pass.$salt)"
    },
    {
      "code": "120",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1($salt.$pass)"
    },
    {
      "code": "130",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1(utf16le($pass).$salt)"
    },
    {
      "code": "140",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1($salt.utf16le($pass))"
    },
    {
      "code": "4500",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1(sha1($pass))"
    },
    {
      "code": "4520",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1($salt.sha1($pass))"
    },
    {
      "code": "4700",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1(md5($pass))"
    },
    {
      "code": "4900",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1($salt.$pass.$salt)"
    },
    {
      "code": "14400",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha1(CX)"
    },
    {
      "code": "1410",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha256($pass.$salt)"
    },
    {
      "code": "1420",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha256($salt.$pass)"
    },
    {
      "code": "1430",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha256(utf16le($pass).$salt)"
    },
    {
      "code": "1440",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha256($salt.utf16le($pass))"
    },
    {
      "code": "1710",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha512($pass.$salt)"
    },
    {
      "code": "1720",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha512($salt.$pass)"
    },
    {
      "code": "1730",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha512(utf16le($pass).$salt)"
    },
    {
      "code": "1740",
      "category": "Raw Hash, Salted and/or Iterated",
      "name": "sha512($salt.utf16le($pass))"
    },
    {
      "code": "50",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-MD5 (key = $pass)"
    },
    {
      "code": "60",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-MD5 (key = $salt)"
    },
    {
      "code": "150",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-SHA1 (key = $pass)"
    },
    {
      "code": "160",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-SHA1 (key = $salt)"
    },
    {
      "code": "1450",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-SHA256 (key = $pass)"
    },
    {
      "code": "1460",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-SHA256 (key = $salt)"
    },
    {
      "code": "1750",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-SHA512 (key = $pass)"
    },
    {
      "code": "1760",
      "category": "Raw Hash, Authenticated",
      "name": "HMAC-SHA512 (key = $salt)"
    },
    {
      "code": "14000",
      "category": "Raw Cipher, Known-Plaintext attack",
      "name": "DES (PT = $salt, key = $pass)"
    },
    {
      "code": "14100",
      "category": "Raw Cipher, Known-Plaintext attack",
      "name": "3DES (PT = $salt, key = $pass)"
    },
    {
      "code": "14900",
      "category": "Raw Cipher, Known-Plaintext attack",
      "name": "Skip32 (PT = $salt, key = $pass)"
    },
    {
      "code": "15400",
      "category": "Raw Cipher, Known-Plaintext attack",
      "name": "ChaCha20"
    },
    {
      "code": "400",
      "category": "Generic KDF",
      "name": "phpass"
    },
    {
      "code": "8900",
      "category": "Generic KDF",
      "name": "scrypt"
    },
    {
      "code": "11900",
      "category": "Generic KDF",
      "name": "PBKDF2-HMAC-MD5"
    },
    {
      "code": "12000",
      "category": "Generic KDF",
      "name": "PBKDF2-HMAC-SHA1"
    },
    {
      "code": "10900",
      "category": "Generic KDF",
      "name": "PBKDF2-HMAC-SHA256"
    },
    {
      "code": "12100",
      "category": "Generic KDF",
      "name": "PBKDF2-HMAC-SHA512"
    },
    {
      "code": "23",
      "category": "Network Protocols",
      "name": "Skype"
    },
    {
      "code": "2500",
      "category": "Network Protocols",
      "name": "WPA/WPA2"
    },
    {
      "code": "4800",
      "category": "Network Protocols",
      "name": "iSCSI CHAP authentication, MD5(CHAP)"
    },
    {
      "code": "5300",
      "category": "Network Protocols",
      "name": "IKE-PSK MD5"
    },
    {
      "code": "5400",
      "category": "Network Protocols",
      "name": "IKE-PSK SHA1"
    },
    {
      "code": "5500",
      "category": "Network Protocols",
      "name": "NetNTLMv1"
    },
    {
      "code": "5500",
      "category": "Network Protocols",
      "name": "NetNTLMv1+ESS"
    },
    {
      "code": "5600",
      "category": "Network Protocols",
      "name": "NetNTLMv2"
    },
    {
      "code": "7300",
      "category": "Network Protocols",
      "name": "IPMI2 RAKP HMAC-SHA1"
    },
    {
      "code": "7500",
      "category": "Network Protocols",
      "name": "Kerberos 5 AS-REQ Pre-Auth etype 23"
    },
    {
      "code": "8300",
      "category": "Network Protocols",
      "name": "DNSSEC (NSEC3)"
    },
    {
      "code": "10200",
      "category": "Network Protocols",
      "name": "CRAM-MD5"
    },
    {
      "code": "11100",
      "category": "Network Protocols",
      "name": "PostgreSQL CRAM (MD5)"
    },
    {
      "code": "11200",
      "category": "Network Protocols",
      "name": "MySQL CRAM (SHA1)"
    },
    {
      "code": "11400",
      "category": "Network Protocols",
      "name": "SIP digest authentication (MD5)"
    },
    {
      "code": "13100",
      "category": "Network Protocols",
      "name": "Kerberos 5 TGS-REP etype 23"
    },
    {
      "code": "121",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "SMF (Simple Machines Forum) > v1.1"
    },
    {
      "code": "400",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "phpBB3 (MD5)"
    },
    {
      "code": "2611",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "vBulletin < v3.8.5"
    },
    {
      "code": "2711",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "vBulletin >= v3.8.5"
    },
    {
      "code": "2811",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "MyBB 1.2+"
    },
    {
      "code": "2811",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "IPB2+ (Invision Power Board)"
    },
    {
      "code": "8400",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "WBB3 (Woltlab Burning Board)"
    },
    {
      "code": "11",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "Joomla < 2.5.18"
    },
    {
      "code": "400",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "Joomla >= 2.5.18 (MD5)"
    },
    {
      "code": "400",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "WordPress (MD5)"
    },
    {
      "code": "2612",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "PHPS"
    },
    {
      "code": "7900",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "Drupal7"
    },
    {
      "code": "21",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "osCommerce"
    },
    {
      "code": "21",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "xt:Commerce"
    },
    {
      "code": "11000",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "PrestaShop"
    },
    {
      "code": "124",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "Django (SHA-1)"
    },
    {
      "code": "10000",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "Django (PBKDF2-SHA256)"
    },
    {
      "code": "3711",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "MediaWiki B type"
    },
    {
      "code": "13900",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "OpenCart"
    },
    {
      "code": "4521",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "Redmine"
    },
    {
      "code": "4522",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "PunBB"
    },
    {
      "code": "12001",
      "category": "Forums, CMS, E-Commerce, Frameworks",
      "name": "Atlassian (PBKDF2-HMAC-SHA1)"
    },
    {
      "code": "12",
      "category": "Database Server",
      "name": "PostgreSQL"
    },
    {
      "code": "131",
      "category": "Database Server",
      "name": "MSSQL (2000)"
    },
    {
      "code": "132",
      "category": "Database Server",
      "name": "MSSQL (2005)"
    },
    {
      "code": "1731",
      "category": "Database Server",
      "name": "MSSQL (2012, 2014)"
    },
    {
      "code": "200",
      "category": "Database Server",
      "name": "MySQL323"
    },
    {
      "code": "300",
      "category": "Database Server",
      "name": "MySQL4.1/MySQL5"
    },
    {
      "code": "3100",
      "category": "Database Server",
      "name": "Oracle H: Type (Oracle 7+)"
    },
    {
      "code": "112",
      "category": "Database Server",
      "name": "Oracle S: Type (Oracle 11+)"
    },
    {
      "code": "12300",
      "category": "Database Server",
      "name": "Oracle T: Type (Oracle 12+)"
    },
    {
      "code": "8000",
      "category": "Database Server",
      "name": "Sybase ASE"
    },
    {
      "code": "141",
      "category": "HTTP, SMTP, LDAP Server",
      "name": "Episerver 6.x < .NET 4"
    },
    {
      "code": "1441",
      "category": "HTTP, SMTP, LDAP Server",
      "name": "Episerver 6.x >= .NET 4"
    },
    {
      "code": "1600",
      "category": "HTTP, SMTP, LDAP Server",
      "name": "Apache $apr1$ MD5, md5apr1, MD5 (APR)"
    },
    {
      "code": "12600",
      "category": "HTTP, SMTP, LDAP Server",
      "name": "ColdFusion 10+"
    },
    {
      "code": "1421",
      "category": "HTTP, SMTP, LDAP Server",
      "name": "hMailServer"
    },
    {
      "code": "101",
      "category": "HTTP, SMTP, LDAP Server",
      "name": "nsldap, SHA-1(Base64), Netscape LDAP SHA"
    },
    {
      "code": "111",
      "category": "HTTP, SMTP, LDAP Server",
      "name": "nsldaps, SSHA-1(Base64), Netscape LDAP SSHA"
    },
    {
      "code": "1411",
      "category": "HTTP, SMTP, LDAP Server",
      "name": "SSHA-256(Base64), LDAP {SSHA256}"
    },
    {
      "code": "1711",
      "category": "HTTP, SMTP, LDAP Server",
      "name": "SSHA-512(Base64), LDAP {SSHA512}"
    },
    {
      "code": "15000",
      "category": "FTP Server",
      "name": "FileZilla Server >= 0.9.55"
    },
    {
      "code": "11500",
      "category": "Checksums",
      "name": "CRC32"
    },
    {
      "code": "3000",
      "category": "Operating Systems",
      "name": "LM"
    },
    {
      "code": "1000",
      "category": "Operating Systems",
      "name": "NTLM"
    },
    {
      "code": "1100",
      "category": "Operating Systems",
      "name": "Domain Cached Credentials (DCC), MS Cache"
    },
    {
      "code": "2100",
      "category": "Operating Systems",
      "name": "Domain Cached Credentials 2 (DCC2), MS Cache 2"
    },
    {
      "code": "15300",
      "category": "Operating Systems",
      "name": "DPAPI masterkey file v1 and v2"
    },
    {
      "code": "12800",
      "category": "Operating Systems",
      "name": "MS-AzureSync  PBKDF2-HMAC-SHA256"
    },
    {
      "code": "1500",
      "category": "Operating Systems",
      "name": "descrypt, DES (Unix), Traditional DES"
    },
    {
      "code": "12400",
      "category": "Operating Systems",
      "name": "BSDi Crypt, Extended DES"
    },
    {
      "code": "500",
      "category": "Operating Systems",
      "name": "md5crypt, MD5 (Unix), Cisco-IOS $1$ (MD5)"
    },
    {
      "code": "3200",
      "category": "Operating Systems",
      "name": "bcrypt $2*$, Blowfish (Unix)"
    },
    {
      "code": "7400",
      "category": "Operating Systems",
      "name": "sha256crypt $5$, SHA256 (Unix)"
    },
    {
      "code": "1800",
      "category": "Operating Systems",
      "name": "sha512crypt $6$, SHA512 (Unix)"
    },
    {
      "code": "122",
      "category": "Operating Systems",
      "name": "OSX v10.4, OSX v10.5, OSX v10.6"
    },
    {
      "code": "1722",
      "category": "Operating Systems",
      "name": "OSX v10.7"
    },
    {
      "code": "7100",
      "category": "Operating Systems",
      "name": "OSX v10.8+ (PBKDF2-SHA512)"
    },
    {
      "code": "6300",
      "category": "Operating Systems",
      "name": "AIX {smd5}"
    },
    {
      "code": "6700",
      "category": "Operating Systems",
      "name": "AIX {ssha1}"
    },
    {
      "code": "6400",
      "category": "Operating Systems",
      "name": "AIX {ssha256}"
    },
    {
      "code": "6500",
      "category": "Operating Systems",
      "name": "AIX {ssha512}"
    },
    {
      "code": "2400",
      "category": "Operating Systems",
      "name": "Cisco-PIX MD5"
    },
    {
      "code": "2410",
      "category": "Operating Systems",
      "name": "Cisco-ASA MD5"
    },
    {
      "code": "500",
      "category": "Operating Systems",
      "name": "Cisco-IOS $1$ (MD5)"
    },
    {
      "code": "5700",
      "category": "Operating Systems",
      "name": "Cisco-IOS type 4 (SHA256)"
    },
    {
      "code": "9200",
      "category": "Operating Systems",
      "name": "Cisco-IOS $8$ (PBKDF2-SHA256)"
    },
    {
      "code": "9300",
      "category": "Operating Systems",
      "name": "Cisco-IOS $9$ (scrypt)"
    },
    {
      "code": "22",
      "category": "Operating Systems",
      "name": "Juniper NetScreen/SSG (ScreenOS)"
    },
    {
      "code": "501",
      "category": "Operating Systems",
      "name": "Juniper IVE"
    },
    {
      "code": "15100",
      "category": "Operating Systems",
      "name": "Juniper/NetBSD sha1crypt"
    },
    {
      "code": "7000",
      "category": "Operating Systems",
      "name": "FortiGate (FortiOS)"
    },
    {
      "code": "5800",
      "category": "Operating Systems",
      "name": "Samsung Android Password/PIN"
    },
    {
      "code": "13800",
      "category": "Operating Systems",
      "name": "Windows Phone 8+ PIN/password"
    },
    {
      "code": "8100",
      "category": "Operating Systems",
      "name": "Citrix NetScaler"
    },
    {
      "code": "8500",
      "category": "Operating Systems",
      "name": "RACF"
    },
    {
      "code": "7200",
      "category": "Operating Systems",
      "name": "GRUB 2"
    },
    {
      "code": "9900",
      "category": "Operating Systems",
      "name": "Radmin2"
    },
    {
      "code": "125",
      "category": "Operating Systems",
      "name": "ArubaOS"
    },
    {
      "code": "7700",
      "category": "Enterprise Application Software (EAS)",
      "name": "SAP CODVN B (BCODE)"
    },
    {
      "code": "7800",
      "category": "Enterprise Application Software (EAS)",
      "name": "SAP CODVN F/G (PASSCODE)"
    },
    {
      "code": "10300",
      "category": "Enterprise Application Software (EAS)",
      "name": "SAP CODVN H (PWDSALTEDHASH) iSSHA-1"
    },
    {
      "code": "8600",
      "category": "Enterprise Application Software (EAS)",
      "name": "Lotus Notes/Domino 5"
    },
    {
      "code": "8700",
      "category": "Enterprise Application Software (EAS)",
      "name": "Lotus Notes/Domino 6"
    },
    {
      "code": "9100",
      "category": "Enterprise Application Software (EAS)",
      "name": "Lotus Notes/Domino 8"
    },
    {
      "code": "133",
      "category": "Enterprise Application Software (EAS)",
      "name": "PeopleSoft"
    },
    {
      "code": "13500",
      "category": "Enterprise Application Software (EAS)",
      "name": "PeopleSoft PS_TOKEN"
    },
    {
      "code": "11600",
      "category": "Archives",
      "name": "7-Zip"
    },
    {
      "code": "12500",
      "category": "Archives",
      "name": "RAR3-hp"
    },
    {
      "code": "13000",
      "category": "Archives",
      "name": "RAR5"
    },
    {
      "code": "13200",
      "category": "Archives",
      "name": "AxCrypt"
    },
    {
      "code": "13300",
      "category": "Archives",
      "name": "AxCrypt in-memory SHA1"
    },
    {
      "code": "13600",
      "category": "Archives",
      "name": "WinZip"
    },
    {
      "code": "14700",
      "category": "Backup",
      "name": "iTunes backup < 10.0"
    },
    {
      "code": "14800",
      "category": "Backup",
      "name": "iTunes backup >= 10.0"
    },
    {
      "code": "62XY",
      "category": "Full-Disk Encryption (FDE)",
      "name": "TrueCrypt",
      "subcategories": {
          "X": [
            {
                'description' : "PBKDF2-HMAC-RIPEMD160",
                "code": '1'
            },
            {
                "description": "PBKDF2-HMAC-SHA512",
                "code": "2"
            },
            {
                "description": "PBKDF2-HMAC-Whirlpool",
                "code": "3"
            },
            {
                "description": "PBKDF2-HMAC-RIPEMD160 + boot-mode",
                "code": "4"
            }
          ],
          "Y": [
            {
                "description": "XTS  512 bit pure AES",
                "code": "1"
            },
            {
                "description": "XTS  512 bit pure Serpent",
                "code": "1"
            },
            {
                "description": "XTS  512 bit pure Twofish",
                "code": "1"
            },
            {
                "description": "XTS 1024 bit pure AES",
                "code": "2"
            },
            {
                "description": "XTS 1024 bit pure Serpent",
                "code": "2"
            },
            {
                "description": "XTS 1024 bit pure Twofish",
                "code": "2"
            },
            {
                "description": "XTS 1024 bit cascaded AES-Twofish",
                "code": "2"
            },
            {
                "description": "XTS 1024 bit cascaded Serpent-AES",
                "code": "2"
            },
            {
                "description": "XTS 1024 bit cascaded Twofish-Serpent",
                "code": "2"
            },
            {
                "description": "XTS 1536 bit all",
                "code": "3"
            }
          ],
      }
    },
    {
      "code": "8800",
      "category": "Full-Disk Encryption (FDE)",
      "name": "Android FDE <= 4.3"
    },
    {
      "code": "12900",
      "category": "Full-Disk Encryption (FDE)",
      "name": "Android FDE (Samsung DEK)"
    },
    {
      "code": "12200",
      "category": "Full-Disk Encryption (FDE)",
      "name": "eCryptfs"
    },
    {
      "code": "137XY",
      "category": "Full-Disk Encryption (FDE)",
      "name": "VeraCrypt",
      "subcategories": {
          "X": [
              {
                  "description": "PBKDF2-HMAC-RIPEMD160",
                  "code": "1"
              },
              {
                  "description": "PBKDF2-HMAC-SHA512",
                  "code": "2"
              },
              {
                  "description": "PBKDF2-HMAC-Whirlpool",
                  "code": "3"
              },
              {
                  "description": "PBKDF2-HMAC-RIPEMD160 + boot-mode",
                  "code": "4"
              },
              {
                  "description": "PBKDF2-HMAC-SHA256",
                  "code": "5"
              },
              {
                  "description": "PBKDF2-HMAC-SHA256 + boot-mode",
                  "code": "6"
              },
          ],
          "Y": [
              {
                  "description": "XTS  512 bit pure AES",
                  "code":"1"
              },
              {
                  "description": "XTS  512 bit pure Serpent",
                  "code":"1"
              },
              {
                  "description": "XTS  512 bit pure Twofish",
                  "code":"1"
              },
              {
                  "description": "XTS 1024 bit pure AES",
                  "code":"2"
              },
              {
                  "description": "XTS 1024 bit pure Serpent",
                  "code":"2"
              },
              {
                  "description": "XTS 1024 bit pure Twofish",
                  "code":"2"
              },
              {
                  "description": "XTS 1024 bit cascaded AES-Twofish",
                  "code":"2"
              },
              {
                  "description": "XTS 1024 bit cascaded Serpent-AES",
                  "code":"2"
              },
              {
                  "description": "XTS 1024 bit cascaded Twofish-Serpent",
                  "code":"2"
              },
              {
                  "description": "XTS 1536 bit all",
                  "code":"3"
              },
          ],
      }
    },
    {
      "code": "14600",
      "category": "Full-Disk Encryption (FDE)",
      "name": "LUKS"
    },
    {
      "code": "9700",
      "category": "Documents",
      "name": "MS Office <= 2003 $0/$1, MD5 + RC4"
    },
    {
      "code": "9710",
      "category": "Documents",
      "name": "MS Office <= 2003 $0/$1, MD5 + RC4, collider #1"
    },
    {
      "code": "9720",
      "category": "Documents",
      "name": "MS Office <= 2003 $0/$1, MD5 + RC4, collider #2"
    },
    {
      "code": "9800",
      "category": "Documents",
      "name": "MS Office <= 2003 $3/$4, SHA1 + RC4"
    },
    {
      "code": "9810",
      "category": "Documents",
      "name": "MS Office <= 2003 $3, SHA1 + RC4, collider #1"
    },
    {
      "code": "9820",
      "category": "Documents",
      "name": "MS Office <= 2003 $3, SHA1 + RC4, collider #2"
    },
    {
      "code": "9400",
      "category": "Documents",
      "name": "MS Office 2007"
    },
    {
      "code": "9500",
      "category": "Documents",
      "name": "MS Office 2010"
    },
    {
      "code": "9600",
      "category": "Documents",
      "name": "MS Office 2013"
    },
    {
      "code": "10400",
      "category": "Documents",
      "name": "PDF 1.1 - 1.3 (Acrobat 2 - 4)"
    },
    {
      "code": "10410",
      "category": "Documents",
      "name": "PDF 1.1 - 1.3 (Acrobat 2 - 4), collider #1"
    },
    {
      "code": "10420",
      "category": "Documents",
      "name": "PDF 1.1 - 1.3 (Acrobat 2 - 4), collider #2"
    },
    {
      "code": "10500",
      "category": "Documents",
      "name": "PDF 1.4 - 1.6 (Acrobat 5 - 8)"
    },
    {
      "code": "10600",
      "category": "Documents",
      "name": "PDF 1.7 Level 3 (Acrobat 9)"
    },
    {
      "code": "10700",
      "category": "Documents",
      "name": "PDF 1.7 Level 8 (Acrobat 10 - 11)"
    },
    {
      "code": "9000",
      "category": "Password Managers",
      "name": "Password Safe v2"
    },
    {
      "code": "5200",
      "category": "Password Managers",
      "name": "Password Safe v3"
    },
    {
      "code": "6800",
      "category": "Password Managers",
      "name": "LastPass + LastPass sniffed"
    },
    {
      "code": "6600",
      "category": "Password Managers",
      "name": "1Password, agilekeychain"
    },
    {
      "code": "8200",
      "category": "Password Managers",
      "name": "1Password, cloudkeychain"
    },
    {
      "code": "11300",
      "category": "Password Managers",
      "name": "Bitcoin/Litecoin wallet.dat"
    },
    {
      "code": "12700",
      "category": "Password Managers",
      "name": "Blockchain, My Wallet"
    },
    {
      "code": "15200",
      "category": "Password Managers",
      "name": "Blockchain, My Wallet, V2"
    },
    {
      "code": "13400",
      "category": "Password Managers",
      "name": "KeePass 1 (AES/Twofish) and KeePass 2 (AES)"
    },
    {
      "code": "15500",
      "category": "Password Managers",
      "name": "JKS Java Key Store Private Keys (SHA1)"
    },
    {
      "code": "15600",
      "category": "Password Managers",
      "name": "Ethereum Wallet, PBKDF2-HMAC-SHA256"
    },
    {
      "code": "15700",
      "category": "Password Managers",
      "name": "Ethereum Wallet, SCRYPT"
    },
    {
      "code": "99999",
      "category": "Plaintext",
      "name": "Plaintext"
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
