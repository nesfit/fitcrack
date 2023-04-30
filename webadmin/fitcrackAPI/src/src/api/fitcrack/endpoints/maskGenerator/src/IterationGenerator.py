'''
About: Class responsible for generating possible mask iterations.
Author: Samuel Hribik
'''

import itertools

from src.api.fitcrack.endpoints.maskGenerator.src.functions import *

class IterationGenerator():
    '''Generates masks based on given password policies.'''
    def __init__(self):
        self.charset = ["?l", "?u", "?d", "?s"]
        self.masks = {}

    def generate(self, arg_options):
        '''Generate all compatible iterations of password masks.'''
        for i in range(1, 5):
            charset_option = getattr(arg_options, f"charset{i}")
            if charset_option:
                self.charset.append(f"?{i}")

        if arg_options.useHex:
            self.charset.append("?h")
            self.charset.append("?H")

        for iteration in range(arg_options.minlength, 10 if arg_options.maxlength > 10 else arg_options.maxlength + 1):
            for mask in itertools.product(self.charset, repeat=iteration):
                joined_mask = ''.join(mask)

                if joined_mask == '':
                    continue

                if not check_charsets(joined_mask, arg_options) or not check_custom_charsets(joined_mask, arg_options):
                    continue

                if arg_options.patinc:
                    comp_count = 0
                    for mask_pattern in arg_options.patinc:
                        if check_compatibility(joined_mask, mask_pattern):
                            comp_count += 1
                    if comp_count == 0:
                        continue

                if arg_options.patexc:
                    comp_count = 0
                    for mask_pattern in arg_options.patexc:
                        if check_compatibility(joined_mask, mask_pattern):
                            comp_count += 1
                    if comp_count != 0:
                        continue

                if joined_mask in self.masks:
                    self.masks[joined_mask] += 1
                else:
                    self.masks[joined_mask] = 1

        return self.masks
