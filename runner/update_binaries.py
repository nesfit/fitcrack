import os
import shutil
import subprocess

FITCRACK_BASE_DIR = "/home/boincadm/projects/fitcrack/"
FITCRACK_APPS_DIR = "/home/boincadm/projects/fitcrack/apps/fitcrack/"


def change_runner_ver(filename, old, new):
    with open(filename, 'r') as file:
        filedata = file.read()

    filedata = filedata.replace("runner" + str(old), "runner" + str(new))

    with open(filename, 'w') as file:
        file.write(filedata)


if os.geteuid() != 0:
    print("Run as root.")
    exit(1)

files = os.listdir(FITCRACK_APPS_DIR)
if len(files) == 0:
    print("No base binaries")
    exit(1)

last_ver = 1
try:
    last_ver = int(sorted(files)[-1])
except:
    print("Bad last version")
    exit(1)

print("Old version: %d, new version: %d" % (last_ver, last_ver + 1))

shutil.copytree(FITCRACK_APPS_DIR + str(last_ver),
                FITCRACK_APPS_DIR + str(last_ver + 1))

print("Created dir:", FITCRACK_APPS_DIR + str(last_ver + 1))

change_runner_ver(FITCRACK_APPS_DIR + str(last_ver + 1) +
                  "/windows_x86_64/version.xml", last_ver, last_ver + 1)
change_runner_ver(FITCRACK_APPS_DIR + str(last_ver + 1) +
                  "/x86_64-pc-linux-gnu/version.xml", last_ver, last_ver + 1)

subprocess.run(["make", "clean"], stdout=subprocess.DEVNULL)
print("Building new linux binary")
subprocess.run(["make", "linux", "-j4"], stdout=subprocess.DEVNULL)
shutil.copy("./bin/runner.bin", FITCRACK_APPS_DIR + str(last_ver + 1) +
            "/x86_64-pc-linux-gnu/runner" + str(last_ver + 1) + ".bin")
os.remove(FITCRACK_APPS_DIR + str(last_ver + 1) +
          "/x86_64-pc-linux-gnu/runner" + str(last_ver) + ".bin")

subprocess.run(["make", "clean"], stdout=subprocess.DEVNULL)
print("Building new windows binary")
subprocess.run(["make", "windows", "-j4"], stdout=subprocess.DEVNULL)
shutil.copy("./bin/runner.exe", FITCRACK_APPS_DIR + str(last_ver + 1) +
            "/windows_x86_64/runner" + str(last_ver + 1) + ".exe")
os.remove(FITCRACK_APPS_DIR + str(last_ver + 1) +
          "/windows_x86_64/runner" + str(last_ver) + ".exe")

os.chdir(FITCRACK_BASE_DIR)
subprocess.run(["perl", "./bin/update_versions"])

print("Binaries were successfully updated!")
