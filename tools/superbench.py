import json, requests, argparse, time

HASH_INFO_PATH = '../webadmin/fitcrackAPI/hashcat/hash_info.json'

def create_and_start_job(session, api_url, job_name, attack_job_template):
    job_json_data = json.loads(attack_job_template)
    resp = session.post(api_url + '/job', json=job_json_data)
    assert resp.status_code == 200
    resp = session.get(api_url + '/job/' + str(resp.json()['job_id']) + '/action?operation=start')
    assert resp.json()['status'] == True
    print("Job", job_name, "created and running.")

def create_job_dict_attack(session, api_url, hashtype, hash, host):
    job_name = 'superbench-dict-%s' % (hashtype)
    dict_attack_template = '{"name":"%s","comment":"","hosts_ids":[%d],"seconds_per_job":3600,"time_start":"","time_end":"","attack_settings":{"attack_mode":0,"attack_name":"dict","rules":null,"left_dictionaries":[{"id":8,"name":"english.txt","keyspace":54308,"time":"2018-08-18T12:00:00"}],"right_dictionaries":[],"rule_left":"","rule_right":"","masks":[""],"attack_submode":2,"markov_treshold":null,"charset":[],"mask":"","keyspace_limit":0,"check_duplicates":true,"case_permute":false,"min_password_len":1,"max_password_len":8,"min_elem_in_chain":1,"max_elem_in_chain":8,"generate_random_rules":0},"hash_settings":{"hash_type":"%s","hash_list":[{"hash":"%s","result":"OK","isInCache":false}],"valid_only":true}}' % (job_name, host, hashtype, hash)
    create_and_start_job(session, api_url, job_name, dict_attack_template)

def create_job_comb_attack(session, api_url, hashtype, hash, host):
    job_name = 'superbench-comb-%s' % (hashtype)
    comb_attack_template = '{"name":"%s","comment":"","hosts_ids":[%d],"seconds_per_job":3600,"time_start":"","time_end":"","attack_settings":{"attack_mode":1,"attack_name":"combinator","rules":null,"left_dictionaries":[{"id":5,"name":"adobe100.txt","keyspace":100,"time":"2018-08-18T12:00:00"}],"right_dictionaries":[{"id":6,"name":"twitter-banned-small.txt","keyspace":397,"time":"2018-08-18T12:00:00"}],"rule_left":"","rule_right":"","masks":[""],"attack_submode":2,"markov_treshold":null,"charset":[],"mask":"","keyspace_limit":0,"check_duplicates":true,"case_permute":false,"min_password_len":1,"max_password_len":8,"min_elem_in_chain":1,"max_elem_in_chain":8,"generate_random_rules":0},"hash_settings":{"hash_type":"%s","hash_list":[{"hash":"%s","result":"OK","isInCache":false}],"valid_only":true}}' % (job_name, host, hashtype, hash)
    create_and_start_job(session, api_url, job_name, comb_attack_template)

def create_job_mask_attack(session, api_url, hashtype, hash, host):
    job_name = 'superbench-mask-%s' % (hashtype)
    mask_attack_template = '{"name":"%s","comment":"","hosts_ids":[%d],"seconds_per_job":3600,"time_start":"","time_end":"","attack_settings":{"attack_mode":3,"attack_name":"mask","rules":null,"left_dictionaries":[],"right_dictionaries":[],"rule_left":"","rule_right":"","masks":["?l?l?d?d"],"attack_submode":2,"markov_treshold":null,"markov":{"id":1,"name":"hashcat.hcstat2","time":"2018-08-18T12:00:00"},"charset":[],"mask":"","keyspace_limit":0,"check_duplicates":true,"case_permute":false,"min_password_len":1,"max_password_len":8,"min_elem_in_chain":1,"max_elem_in_chain":8,"generate_random_rules":0},"hash_settings":{"hash_type":"%s","hash_list":[{"hash":"%s","result":"OK","isInCache":false}],"valid_only":true}}' % (job_name, host, hashtype, hash)
    create_and_start_job(session, api_url, job_name, mask_attack_template)

def create_job_hybrid_wm_attack(session, api_url, hashtype, hash, host):
    job_name = 'superbench-hybrid-wm-%s' % (hashtype)
    hybrid_wm_attack_template = '{"name":"%s","comment":"","hosts_ids":[%d],"seconds_per_job":3600,"time_start":"","time_end":"","attack_settings":{"attack_mode":6,"attack_name":"Hybrid wordlist+mask","rules":null,"left_dictionaries":[{"id":6,"name":"twitter-banned-small.txt","keyspace":397,"time":"2018-08-18T12:00:00"}],"right_dictionaries":[],"rule_left":"","rule_right":"","masks":[""],"attack_submode":2,"markov_treshold":null,"charset":[],"mask":"?d?d","keyspace_limit":0,"check_duplicates":true,"case_permute":false,"min_password_len":1,"max_password_len":8,"min_elem_in_chain":1,"max_elem_in_chain":8,"generate_random_rules":0},"hash_settings":{"hash_type":"%s","hash_list":[{"hash":"%s","result":"OK","isInCache":false}],"valid_only":true}}' % (job_name, host, hashtype, hash)
    create_and_start_job(session, api_url, job_name, hybrid_wm_attack_template)

def create_job_hybrid_mw_attack(session, api_url, hashtype, hash, host):
    job_name = 'superbench-hybrid-mw-%s' % (hashtype)
    hybrid_mw_attack_template = '{"name":"%s","comment":"","hosts_ids":[%d],"seconds_per_job":3600,"time_start":"","time_end":"","attack_settings":{"attack_mode":7,"attack_name":"Hybrid mask+wordlist","rules":null,"left_dictionaries":[],"right_dictionaries":[{"id":6,"name":"twitter-banned-small.txt","keyspace":397,"time":"2018-08-18T12:00:00"}],"rule_left":"","rule_right":"","masks":[""],"attack_submode":2,"markov_treshold":null,"charset":[],"mask":"?d?d","keyspace_limit":0,"check_duplicates":true,"case_permute":false,"min_password_len":1,"max_password_len":8,"min_elem_in_chain":1,"max_elem_in_chain":8,"generate_random_rules":0},"hash_settings":{"hash_type":"%s","hash_list":[{"hash":"%s","result":"OK","isInCache":false}],"valid_only":true}}' % (job_name, host, hashtype, hash)
    create_and_start_job(session, api_url, job_name, hybrid_mw_attack_template)

def create_job_prince_attack(session, api_url, hashtype, hash, host):
    job_name = 'superbench-prince-%s' % (hashtype)
    prince_attack_template = '{"name":"%s","comment":"","hosts_ids":[%d],"seconds_per_job":3600,"time_start":"","time_end":"","attack_settings":{"attack_mode":8,"attack_name":"prince","rules":null,"left_dictionaries":[{"id":7,"name":"bible.txt","keyspace":12570,"time":"2018-08-18T12:00:00"}],"right_dictionaries":[],"rule_left":"","rule_right":"","masks":[""],"attack_submode":2,"markov_treshold":null,"charset":[],"mask":"","keyspace_limit":41039,"check_duplicates":true,"case_permute":false,"min_password_len":1,"max_password_len":5,"min_elem_in_chain":1,"max_elem_in_chain":2,"generate_random_rules":0},"hash_settings":{"hash_type":"%s","hash_list":[{"hash":"%s","result":"OK","isInCache":false}],"valid_only":true}}' % (job_name, host, hashtype, hash)
    create_and_start_job(session, api_url, job_name, prince_attack_template)

def create_job_pcfg_attack(session, api_url, hashtype, hash, host):
    job_name = 'superbench-pcfg-%s' % (hashtype)
    pcfg_attack_template = '{"name":"%s","comment":"","hosts_ids":[%d],"seconds_per_job":3600,"time_start":"","time_end":"","attack_settings":{"attack_mode":9,"attack_name":"pcfg","rules":null,"left_dictionaries":[],"right_dictionaries":[],"rule_left":"","rule_right":"","masks":[""],"attack_submode":2,"markov_treshold":null,"charset":[],"mask":"","pcfg_grammar":{"id":1,"name":"john","keyspace":1321431161,"time_added":"2019-08-30T12:14:53"},"keyspace_limit":60000,"check_duplicates":true,"case_permute":false,"min_password_len":1,"max_password_len":8,"min_elem_in_chain":1,"max_elem_in_chain":8,"generate_random_rules":0},"hash_settings":{"hash_type":"%s","hash_list":[{"hash":"%s","result":"OK","isInCache":false}],"valid_only":true}}' % (job_name, host, hashtype, hash)
    create_and_start_job(session, api_url, job_name, pcfg_attack_template)

def main():
    hash_info = None
    with open(HASH_INFO_PATH) as json_file:
        hash_info = json.load(json_file)

    if not hash_info:
        print("No hash info")
        exit(1)

    parser = argparse.ArgumentParser(description='Sample app')

    parser.add_argument("--api-url", action="store", default='http://localhost:5000')
    parser.add_argument("--user", action="store", default='fitcrack')
    parser.add_argument("--password", action="store", default='FITCRACK')
    parser.add_argument("--host-id", action="store", default=1)
    parser.add_argument("--host-name", action="store")

    arguments = parser.parse_args()

    s = requests.Session()
    auth = s.post(arguments.api_url + '/user/login', json={'username': arguments.user, 'password': arguments.password})

    if auth.status_code == 400:
        print("Unable to connect to fitcrack API")
        exit(1)

    
    host_id = None
    if arguments.host_name:
        resp = s.get(arguments.api_url + '/hosts?name=' + arguments.host_name)
        for item in resp.json()['items']:
            if item['domain_name'] == arguments.host_name:
                host_id = item['id']
                break
    else:
        host_id = arguments.host_id

    assert host_id != None

    print("=========== Benchmarking (dictionary attack) ===========")
    for hashcode in hash_info:
        hash_to_crack = hash_info[hashcode]['example_hash']
        #create_job_dict_attack(s, arguments.api_url, hashcode, hash_to_crack, host_id)
        #time.sleep(60)

    print("=========== Benchmarking (combinatory attack) ===========")   
    for hashcode in hash_info:
        hash_to_crack = hash_info[hashcode]['example_hash']
        #create_job_comb_attack(s, arguments.api_url, hashcode, hash_to_crack, host_id)
        #time.sleep(60)

    print("=========== Benchmarking (mask attack) ===========")
    for hashcode in hash_info:
        hash_to_crack = hash_info[hashcode]['example_hash']
        #create_job_mask_attack(s, arguments.api_url, hashcode, hash_to_crack, host_id)
        #time.sleep(60)

    print("=========== Benchmarking (hybrid wordlist + mask attack) ===========")
    for hashcode in hash_info:
        hash_to_crack = hash_info[hashcode]['example_hash']
        #create_job_hybrid_wm_attack(s, arguments.api_url, hashcode, hash_to_crack, host_id)
        #time.sleep(60)

    print("=========== Benchmarking (hybrid mask + wordlist attack) ===========")
    for hashcode in hash_info:
        hash_to_crack = hash_info[hashcode]['example_hash']
        #create_job_hybrid_mw_attack(s, arguments.api_url, hashcode, hash_to_crack, host_id)
        #time.sleep(60)

    print("=========== Benchmarking (prince attack) ===========")
    for hashcode in hash_info:
        hash_to_crack = hash_info[hashcode]['example_hash']
        #create_job_prince_attack(s, arguments.api_url, hashcode, hash_to_crack, host_id)
        #time.sleep(60)

    print("=========== Benchmarking (pcfg attack) ===========")
    for hashcode in hash_info:
        hash_to_crack = hash_info[hashcode]['example_hash']
        create_job_pcfg_attack(s, arguments.api_url, hashcode, hash_to_crack, host_id)
        time.sleep(60)

if __name__ == "__main__":
    main()
