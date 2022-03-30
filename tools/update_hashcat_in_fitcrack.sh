sh build_hashcat.sh

cp -f hashcat.bin ../server/client_bin/x86_64-pc-linux-gnu/hashcat1.bin
cp -f hashcat.exe ../server/client_bin/windows_x86_64/hashcat1.exe

cp -f hashcat_files.zip ../server/client_bin/x86_64-pc-linux-gnu/hashcat_files1.zip
cp -f hashcat_files.zip ../server/client_bin/windows_x86_64/hashcat_files1.zip

rm -rf ../webadmin/fitcrackAPI/hashcat/*
cp -f hashcat.bin ../webadmin/fitcrackAPI/hashcat/hashcat.bin
unzip hashcat_files.zip -d ../webadmin/fitcrackAPI/hashcat/

rm -f hashcat.bin hashcat.exe hashcat_files.zip

echo "Updated."