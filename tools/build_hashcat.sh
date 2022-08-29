git clone https://github.com/hashcat/hashcat
cd hashcat

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
make linux -j

# Needs win-iconv
# https://github.com/hashcat/hashcat/blob/master/BUILD_WSL.md
make win -j

chmod +x hashcat
else
patch -p1 < ../hashcat.patch
make -j
fi

chmod +x hashcat
./hashcat --hash-info --machine-readable --quiet > tmp.json
python3 -m json.tool tmp.json hash_info.json

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
zip -r hashcat_files_linux.zip OpenCL modules/*.so hashcat.hcstat2 hashcat.hctune hash_info.json
zip -r hashcat_files_windows.zip OpenCL modules/*.dll hashcat.hcstat2 hashcat.hctune hash_info.json
mv hashcat_files_linux.zip ../
mv hashcat_files_windows.zip ../
else
zip -r hashcat_files_mac.zip OpenCL modules hashcat.hcstat2 hashcat.hctune hash_info.json
mv hashcat_files_mac.zip ../
fi

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
mv hashcat.bin ../hashcat.bin
mv hashcat.exe ../hashcat.exe
else
mv hashcat ../hashcat.app
fi

cd ..
rm -rf hashcat

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
echo "Created: hashcat_files_linux.zip, hashcat_files_windows, hashcat_files_windows.zip, hashcat.bin, hashcat.exe"
else
echo "Created: hashcat_files_mac.zip, hashcat.app"
fi