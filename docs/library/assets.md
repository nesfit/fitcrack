Cracking Assets
===============

There are different kinds of assets used by the various attack modes available in Fitcrack, such as dictionaries, rulesets, and more. Many of these can be managed on their respective pages in Webadmin. You can edit or remove the ones shipping with Fitcrack, and you can, of course, upload new ones, extending the system's capabilities.


Dictionaries
------------

The Dictionaries page allows you to manage password dictionaries. A dictionary is simply a text file (with a .txt extension) containing different password candidates where each password is stored on a separate line. Dictionaries are used for dictionary attacks, a combination attacks, and hybrid attacks.

Each dictionary can be opened and searched by clicking its name. You can download or delete dictionaries. To add a new dictionary, you can directly upload the text file, or pick one you uploaded to the server's import directory.

Dictionaries can also be sorted on creation. This will sort the lines by length, which can lead to significant performance boosts when cracking certain hashes, for example a 7zip archive.


PCFGs
-----

PCFGs generate passwords using machine learning magic trained on password sets.

To define a new one, you can upload a zip archive directly or select a dictionary to create it from.


Rulesets
--------

Rules define various modifications of password candidates. Such alterations include replacing and swapping of characters and substrings, password truncation, etc. Fitcrack uses Hashcat, which uses its own rule engine. Supported rules can be found in the [Hashcat docs](//hashcat.net/wiki/doku.php?id=rule_based_attack).

While the combinator and hybrid attacks allow the use of only one rule definition for each part, the dictionary and PCFG attacks can use a ruleset file.

A ruleset file is a text file which can contain one or more password-mangling rules on each line. The rules are all applied to every candidate password in the dictionary, meaning the password count will be a product of the dictionary size and the rule count.


Character Sets
--------------

In password masks used within brute-force or hybrid attacks, you can use the substitute symbols `?1` to `?4` for custom characters. The custom user-defined character sets are stored within a charset file with a .txt, .hcchr or .charset extension, which may contain both ASCII and non-ASCII characters.

Charsets can be edited by clicking the _Edit_ button in their detail view. 


Masks
-----

Mask files contain a set of masks on separate lines. These can be loaded into a brute force attack when creating a job.


Markov Chains
-------------

Markov chains are used in brute borce attacks. They define what characters from a set go after another.

To define a new one, you can upload a hcstat2 file directly or select a dictionary to create it from.
