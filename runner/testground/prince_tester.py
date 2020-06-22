import hashlib
import subprocess
import os, sys

RUNNER_PATH = "../bin/runner.bin"
if not os.path.exists(RUNNER_PATH):
    print("No runner binary!", file=sys.stderr)
    exit(1)

def write_config(case_permute, min_password_len, max_password_len, min_elem_in_chain, max_elem_in_chain, limit=None, skip=None):
    with open("config", "w") as f:
        f.write("|||name|String|11|prince_test|||\n")
        f.write("|||attack_mode|UInt|1|8|||\n")
        f.write("|||attack_submode|UInt|1|0|||\n")
        f.write("|||hash_type|UInt|1|0||||\n")
        f.write("|||mode|String|1|n|||\n")
        f.write("|||case_permute|UInt|1|{}|||\n".format(case_permute))
        f.write("|||min_password_len|UInt|1|{}|||\n".format(min_password_len))
        f.write("|||max_password_len|UInt|1|{}|||\n".format(max_password_len))
        f.write("|||min_elem_in_chain|UInt|1|{}|||\n".format(min_elem_in_chain))
        f.write("|||max_elem_in_chain|UInt|1|{}|||\n".format(max_elem_in_chain))
        if limit:
            f.write("|||dict_hc_keyspace|BigUInt|1|{}|||\n".format(limit))
        if skip:
            f.write("|||skip_from_start|BigUInt|1|{}|||\n".format(skip))

def write_input_dict(words):
    with open("dict1", "w") as f:
        for p in words:
            f.write(p + "\n")

def write_data(passwords):
    with open("data", "w") as f:
        for p in passwords:
            h = hashlib.md5(p.encode())
            f.write(h.hexdigest() + "\n")

def call_runner():
    if os.path.exists("passwords.txt"):
        os.remove("passwords.txt")
    subprocess.call(RUNNER_PATH, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

def find_passwords():
    if not os.path.exists("passwords.txt"):
        return []
    
    with open("passwords.txt", "r") as f:
        lines = f.readlines()
        passwords = []
        for i in lines:
            pass_hex = i.split(":")[1].rstrip()
            passwords.append(bytearray.fromhex(pass_hex).decode())
        return passwords

def cleanup():
    test_files = ['config', 'data', 'passwords.txt', 'stderr.txt', 'out', 'dict']
    for f in test_files:
        if os.path.exists(f):
            os.remove(f)

def run_prince_tests(): 
    write_input_dict(["sand", "123", "blue", "deamon", "f"])

    write_data(["deamon"])
    write_config(0,1,4,1,1)
    call_runner()
    if find_passwords() != []:
        return False, "test max password len failed"

    write_data(["deamon"])
    write_config(0,7,8,1,1)
    call_runner()
    if find_passwords() != []:
        return False, "test min password len failed"

    write_data(["sand"])
    write_config(0,1,4,1,1)
    call_runner()
    if find_passwords() != ["sand"]:
        return False, "test simple password failed"

    write_input_dict(["sand", "123", "blue", "deamon"])
    write_data(["sand", "deamon"])
    write_config(0,1,4,1,1)
    call_runner()
    if find_passwords() != ["sand"]:
        return False, "test multiple password failed"

    write_input_dict(["sand", "123", "blue", "deamon"])
    write_data(["123123", "123sand"])
    write_config(0,1,7,2,2,2)
    call_runner()
    if find_passwords() != ["123123", "123sand"]:
        return False, "test multiple password with limit failed"

    write_input_dict(["sand", "123", "blue", "deamon"])
    write_data(["blue123", "123sand"])
    write_config(0,1,7,2,2,2,3)
    call_runner()
    if find_passwords() != ["blue123"]:
        return False, "test multiple password with limit and skip failed"
    
    write_input_dict(["sand", "123", "blue", "deamon"])
    write_data(["sandsand", "123sand"])
    write_config(0,1,8,2,3,None,3)
    call_runner()
    if find_passwords() != ["sandsand"]:
        return False, "test multiple password with skip failed"

    write_data(["bluesand", "blue123f"])
    write_config(0,1,8,2,2)
    call_runner()
    if find_passwords() != ["bluesand"]:
        return False, "test max 2 elements in chains failed"

    write_data(["sand"])
    write_config(0,1,8,2,2)
    call_runner()
    if find_passwords() != []:
        return False, "test min 2 elements in chains failed"

    write_data(["bluesand", "blue123f", "blueSand", "BlueSand"])
    write_config(1,1,8,2,2)
    call_runner()
    if find_passwords() != ["bluesand", "blueSand", "BlueSand"]:
        return False, "test password first case permutation failed"

    return True, "tests passed"


if __name__ == "__main__":
    res, msg = run_prince_tests()
    if res: 
        cleanup()
    assert res == True, msg + "\nSee stderr.txt or config for more info."
