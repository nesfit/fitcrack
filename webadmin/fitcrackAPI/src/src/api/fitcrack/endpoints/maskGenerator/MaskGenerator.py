"""
Author: Samuel Hríbik
Login: xhribi00
About: This is an implementation of a password mask generator proposed in my bachelor thesis.
"""

import argparse
import string
import itertools
import sys


def check_compatibility(mask, mask_pattern):
    '''Compare mask with a given mask pattern.'''
    if len(mask) != len(mask_pattern):
        return False
    for i, letter in enumerate(mask):
        if mask_pattern[i] == 'a':
            if mask_pattern[i] == 'b':
                return False
        elif (mask_pattern[i] != 'b' and mask_pattern[i] != letter):
            return False
    return True

def check_charsets(mask, arg_options):
    '''Checks for required number of character sets within a mask.'''
    if (arg_options.minlower <= mask.count("l") <= arg_options.maxlower and
        arg_options.minupper <= mask.count("u") <= arg_options.maxupper and
        arg_options.mindigit <= mask.count("d") <= arg_options.maxdigit and
        arg_options.minspecial <= mask.count("s") <= arg_options.maxspecial):
        return True
    else:
        return False

def check_custom_charsets(mask, arg_options):
    '''Checks for undefined character sets within a mask.'''
    if (arg_options.charset1 is None and "1" in mask or
        arg_options.charset2 is None and "2" in mask or
        arg_options.charset3 is None and "3" in mask or
        arg_options.charset4 is None and "4" in mask):
        return False
    else:
        return True

class PasswordAnalyzer:
    '''Takes given arguments, filters and analyzes compatible passwords.'''
    def __init__(self):
        self.masks = {}

    def analyze(self, arg_options):
        '''Iterate through passwords in wordlists and analyze passwords compatible with policy.'''

        for filename in arg_options.wordlists:
            try:
                with open(filename, 'r', encoding="latin-1") as file:
                    for password in file:

                        password = password.rstrip('\r\n')

                        if password == "":
                            continue

                        mask = ""

                        for letter in password:
                            if arg_options.charset1 is not None and letter in arg_options.charset1:
                                mask += "?1"
                            elif arg_options.charset2 is not None and letter in arg_options.charset2:
                                mask += "?2"
                            elif arg_options.charset3 is not None and letter in arg_options.charset3:
                                mask += "?3"
                            elif arg_options.charset4 is not None and letter in arg_options.charset4:
                                mask += "?4"
                            elif letter in string.ascii_lowercase:
                                mask += "?l"
                            elif letter in string.ascii_uppercase:
                                mask += "?u"
                            elif letter in string.digits:
                                mask += "?d"
                            elif letter in string.printable:
                                mask += "?s"
                            else:
                                mask += "?b"

                        if not (check_charsets(mask, arg_options) and
                                arg_options.minlength <= len(password) <= arg_options.maxlength):
                            continue

                        if arg_options.patinc is not None:
                            comp_count = 0
                            for mask_pattern in arg_options.patinc:
                                if check_compatibility(mask, mask_pattern):
                                    comp_count += 1
                            if comp_count == 0:
                                continue

                        if arg_options.patexc is not None:
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

class MaskSorter:
    '''
    Depending on the given sorting mode, sorts masks by their complexity, occurrence,
    or their respective ratio in an optimal sorting
    '''
    def __init__(self, sorting_mode, input_masks):
        self.sorting_mode = sorting_mode
        self.input_masks = input_masks
        self.mask_complexity = {}
        self.sorted_masks = []

    def add_complexity(self, arg_options):
        '''Iterate through input masks and evaluate its complexity'''
        for mask, occurrence in self.input_masks.items():
            complexity = 1
            for charset in mask.split('?'):
                if charset == '1':
                    complexity *= len(arg_options.charset1)
                elif charset == '2':
                    complexity *= len(arg_options.charset2)
                elif charset == '3':
                    complexity *= len(arg_options.charset3)
                elif charset == '4':
                    complexity *= len(arg_options.charset4)
                elif charset == 'd':
                    complexity *= len(string.digits)
                elif charset == 'l':
                    complexity *= len(string.ascii_lowercase)
                elif charset == 'u':
                    complexity *= len(string.ascii_uppercase)
                elif charset == 's':
                    complexity *= 33
                elif charset == 'b':
                    complexity *= 256

            self.mask_complexity.update({mask:{"occurrence":occurrence,
                                        "complexity":complexity, "optimal":complexity//occurrence}})

    def sort_masks(self, input_options):
        '''Create sorted dictionary'''

        capacity = None
        if input_options.time != 0:
            capacity = input_options.time * input_options.speed

        self.add_complexity(options)
        if self.sorting_mode == "occurrence":
            sorted_masks = dict(sorted(self.mask_complexity.items(),
                                 key=lambda x:x[1][self.sorting_mode], reverse=True))
        elif self.sorting_mode == "complexity":
            sorted_masks = dict(sorted(self.mask_complexity.items(),
                                 key=lambda x:x[1][self.sorting_mode]))
        elif self.sorting_mode == "optimal":
            sorted_masks = dict(sorted(self.mask_complexity.items(),
                                 key=lambda x:x[1][self.sorting_mode]))

        for mask in sorted_masks:
            if capacity is not None:
                if capacity - self.mask_complexity[mask]["complexity"] < 0:
                    break
                else:
                    capacity -= self.mask_complexity[mask]["complexity"]

            if not input_options.quiet:
                print(mask, self.mask_complexity[mask])
            else:
                print(mask)
            self.sorted_masks.append(mask)

    def save_masks_to_file(self, arg_options):
        '''Save sorted masks to an output file'''
        file = open(arg_options.output, "w", encoding="utf-8")
        for mask in self.sorted_masks:
            if "1" in mask:
                file.write(arg_options.charset1+",")
            if "2" in mask:
                file.write(arg_options.charset2+",")
            if "3" in mask:
                file.write(arg_options.charset3+",")
            if "4" in mask:
                file.write(arg_options.charset4+",")
            file.write(mask+"\n")

class PasswordGenerator():
    '''Generates masks based on given password policies.'''
    def __init__(self):
        self.charset = ["?l", "?u", "?d", "?s"]
        self.masks = {}

    def generate(self, arg_options):
        '''Generate all compatible iterations of password masks.'''
        if arg_options.charset1 is not None:
            self.charset.append("?1")
        if arg_options.charset2 is not None:
            self.charset.append("?2")
        if arg_options.charset3 is not None:
            self.charset.append("?3")
        if arg_options.charset4 is not None:
            self.charset.append("?4")

        for iteration in range(arg_options.minlength, 9 if arg_options.maxlength > 9 else arg_options.maxlength + 1):
            for mask in itertools.product(self.charset, repeat=iteration):
                joined_mask = ''.join(mask)

                if not check_charsets(joined_mask, arg_options):
                    continue

                if arg_options.patinc is not None:
                    comp_count = 0
                    for mask_pattern in arg_options.patinc:
                        if check_compatibility(joined_mask, mask_pattern):
                            comp_count += 1
                    if comp_count == 0:
                        continue

                if arg_options.patexc is not None:
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

class MaskGenerator():

    def generateMaskFile(arg_values, path):

        return "oznuk je cesta"

        parser = argparse.ArgumentParser()

        parser.add_argument("--minlength", dest="minlength", type=int,
                            default=1, help="Minimum password length")
        parser.add_argument("--maxlength", dest="maxlength", type=int,
                            default=9999, help="Maximum password length")
        parser.add_argument("--minlower", dest="minlower", type=int,
                            default=0, help="Minimum number of lowercase characters")
        parser.add_argument("--maxlower", dest="maxlower", type=int,
                            default=9999, help="Maximum number of lowercase characters")
        parser.add_argument("--minupper", dest="minupper", type=int,
                            default=0, help="Minimum number of uppercase characters")
        parser.add_argument("--maxupper", dest="maxupper", type=int,
                            default=9999, help="Maximum number of uppercase characters")
        parser.add_argument("--mindigit", dest="mindigit", type=int,
                            default=0, help="Minimum number of digits")
        parser.add_argument("--maxdigit", dest="maxdigit", type=int,
                            default=9999, help="Maximum number of digits")
        parser.add_argument("--minspecial", dest="minspecial", type=int,
                            default=0, help="Minimum number of special characters")
        parser.add_argument("--maxspecial", dest="maxspecial", type=int,
                            default=9999, help="Maximum number of special characters")
        parser.add_argument("--minocc", dest="minocc", type=int,
                            default=0, help="Minimum number of occurences of a mask")
        parser.add_argument("--time", dest="time", type=int,
                            default=0, help="Time limit for cracking")
        parser.add_argument("--speed", dest="speed", type=int,
                            default=100000, help="Passwords generated per second")
        parser.add_argument("--sorting", dest="sorting",
                            default="occurrence", help="Mask sorting mode")
        parser.add_argument("--output", dest="output",
                            default="default", help="Output file")
        parser.add_argument("--wordlists", dest="wordlists", nargs='*',
                            help="Wordlists for analysis")
        parser.add_argument("--patinc", dest="patinc", nargs='*',
                            help="Desired password mask patterns")
        parser.add_argument("--patexc", dest="patexc", nargs='*',
                            help="Password mask patterns to be excluded")
        parser.add_argument("--charset1", dest="charset1",
                            help="Custom character set number 1")
        parser.add_argument("--charset2", dest="charset2",
                            help="Custom character set number 2")
        parser.add_argument("--charset3", dest="charset3",
                            help="Custom character set number 3")
        parser.add_argument("--charset4", dest="charset4",
                            help="Custom character set number 4")
        parser.add_argument("-q", "--quiet", dest="quiet", action="store_true",
                            help="Print only masks without their attributes")

        options = parser.parse_args()
        masks = {}

        if options.patinc is not None:
            for pattern in options.patinc:
                if not (check_charsets(pattern, options) and check_custom_charsets(pattern, options) and
                        options.minlength <= len(pattern.replace('?', '')) <= options.maxlength):
                    print("Arguments incompatible with pattern: " + str(pattern))
                    sys.exit(1)

        if options.patexc is not None:
            for pattern in options.patexc:
                if not (check_charsets(pattern, options) and check_custom_charsets(pattern, options) and
                        options.minlength <= len(pattern.replace('?', '')) <= options.maxlength):
                    print("Arguments incompatible with pattern: " + str(pattern))
                    sys.exit(1)

        if options.wordlists is not None:
            analyzer = PasswordAnalyzer()
            masks = analyzer.analyze(options)

        else:
            generator = PasswordGenerator()
            masks = generator.generate(options)

        sorter = MaskSorter(options.sorting, masks)
        sorter.sort_masks(options)

        if options.output != "default":
            sorter.save_masks_to_file(options)
