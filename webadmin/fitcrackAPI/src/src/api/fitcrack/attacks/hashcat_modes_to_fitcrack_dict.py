import os
import sys
import subprocess

# Extract hashlist from hashcat --help and save to hash file.
f = open("hash", "r")
lines = f.readlines()
f.close()
print("hashtypes = [")
for l in lines:
    hashinfo = l.split("|")
    hashcode = hashinfo[0].strip()
    hashname = hashinfo[1].strip()
    hashcategory = hashinfo[2].strip()
    print("    {")
    print("      \"code\": \"{}\",".format(hashcode))
    print("      \"category\": \"{}\",".format(hashcategory))
    print("      \"name\": \"{}\",".format(hashname))
    print("    },")
print("]")

# Now take output and update file webadmin/fitcrackAPI/src/src/api/fitcrack/attacks/hashtypes.py