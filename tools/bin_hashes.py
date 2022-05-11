import os, json

HASH_INFO_PATH = '../webadmin/fitcrackAPI/hashcat/hash_info.json'

with open(HASH_INFO_PATH) as json_file:
    hash_info = json.load(json_file)

    if not hash_info:
        print("No hash info")
        exit(1)

    bin_hashes = []
    for hash_type in hash_info:
        if hash_info[hash_type]['example_hash_format'] != "plain":
            bin_hashes.append(hash_type)
    
    print(', '.join(bin_hashes))