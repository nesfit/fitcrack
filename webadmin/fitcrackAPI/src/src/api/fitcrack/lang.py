'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

package_status_text_to_code_dict = {
    0: 'ready',
    1: 'finished',
    2: 'exhausted',
    3: 'malformed',
    4: 'timeout',
    10: 'running',
    11: 'validating',
    12: 'finishing'
}

package_status_text_info_to_code_dict = {
    0: 'The job has been created and waits for the start.',
    10: 'The job is running and new workunits will assigned to hosts.',
    12: 'The job is still running, but no more workunits will be assigned to hosts.',
    1: 'The job is not running anymore, and at least one hash has been cracked.',
    2: 'The job is not running anymore, and no hash has been cracked.',
    3: 'The job has been malformed by a computation error.'
}

host_status_text_to_code_dict = {
    0: 'benchmark',
    1: 'normal',
    2: 'validation',
    3: 'done',
    4: 'error.'
}
statuses = {
    'ready': 0,
    'finished': 1,
    'exhausted': 2,
    'malformed': 3,
    'timeout': 4,
    'running': 10,
    'validating': 11,
    'finishing': 12
}
