import string

from src.api.fitcrack.endpoints.maskGenerator.src.functions import *

class PasswordAnalyzer:
    '''Takes given arguments, filters and analyzes compatible passwords.'''
    def __init__(self):
        self.masks = {}

    def analyze(self, arg_options, wordlistsPath):
        '''Iterate through passwords in wordlists and analyze passwords compatible with policy.'''
        for filename in arg_options.wordlists:
            try:
                with open(wordlistsPath + "/" + filename, 'r', encoding="latin-1") as file:
                    for password in file:

                        password = password.rstrip('\r\n')

                        if password == "":
                            continue

                        mask = ""

                        for letter in password:

                            maskLength = len(mask)

                            for charset in arg_options.charsetOrderList:
                                if charset['placeholder'] == '?1' and letter in arg_options.charset1:
                                    mask += "?1"
                                    break
                                elif charset['placeholder'] == '?2' and letter in arg_options.charset2:
                                    mask += "?2"
                                    break
                                elif charset['placeholder'] == '?3' and letter in arg_options.charset3:
                                    mask += "?3"
                                    break
                                elif charset['placeholder'] == '?4' and letter in arg_options.charset4:
                                    mask += "?4"
                                    break
                                elif charset['placeholder'] == '?l' and letter in string.ascii_lowercase:
                                    mask += "?l"
                                    break
                                elif charset['placeholder'] == '?u' and letter in string.ascii_uppercase:
                                    mask += "?u"
                                    break
                                elif charset['placeholder'] == '?d' and letter in string.digits:
                                    mask += "?d"
                                    break
                                elif charset['placeholder'] == '?s' and letter in string.printable:
                                    mask += "?s"
                                    break
                                elif charset['placeholder'] == '?h' and letter in 'abcdef0123456789':
                                    mask += "?h"
                                    break
                                elif charset['placeholder'] == '?H' and letter in 'ABCDEF0123456789':
                                    mask += "?H"
                                    break

                            if len(mask) == maskLength:    
                                mask += "?b"

                        if not (check_charsets(mask, arg_options) and check_custom_charsets(mask, arg_options) and
                                arg_options.minlength <= len(password) <= arg_options.maxlength):
                            continue

                        if arg_options.patinc:
                            comp_count = 0
                            for mask_pattern in arg_options.patinc:
                                if check_compatibility(mask, mask_pattern):
                                    comp_count += 1
                            if comp_count == 0:
                                continue

                        if arg_options.patexc:
                            comp_count = 0
                            for mask_pattern in arg_options.patexc:
                                if check_compatibility(mask, mask_pattern):
                                    comp_count += 1
                            if comp_count != 0:
                                continue

                        if mask in self.masks:
                            self.masks[mask] += 1
                        else:
                            self.masks[mask] = 1

            except OSError:
                pass

        filtered_masks = {}
        for mask, occurrence in self.masks.items():
            if occurrence >= arg_options.minocc:
                filtered_masks.update({mask:occurrence})

        return filtered_masks