git clone https://github.com/hashcat/hashcat
cd hashcat
make linux -j

# Needs win-iconv
# https://github.com/hashcat/hashcat/blob/master/BUILD_WSL.md
make win -j

chmod +x hashcat.bin
./hashcat.bin --hash-info --machine-readable --quiet > tmp.json
python3 -m json.tool tmp.json hash_info.json

zip -r hashcat_files.zip OpenCL modules hashcat.hcstat2 hashcat.hctune hash_info.json

mv hashcat_files.zip ../
mv hashcat.bin ../hashcat.bin
mv hashcat.exe ../hashcat.exe

cd ..
rm -rf hashcat

echo "Created: hashcat_files.zip, hashcat.bin, hashcat.exe"