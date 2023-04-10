import string
import itertools

def hello() :
    return "Hello"

def check_compatibility(mask, mask_pattern):
    '''Compare mask with a given mask pattern.'''
    if len(mask) != len(mask_pattern):
        return False
    for i, letter in enumerate(mask):
        if mask_pattern[i] == 'a':
            if mask_pattern[i] == 'b' or mask_pattern[i] == 'h' or mask_pattern[i] == 'H':
                return False
        elif mask_pattern[i] != 'b' and mask_pattern[i] != letter:
            return False
    return True

def check_charsets(mask, arg_options):
    '''Checks for required number of character sets within a mask.'''
    if (arg_options.minlower <= mask.count("l") <= arg_options.maxlower and
        arg_options.minupper <= mask.count("u") <= arg_options.maxupper and
        arg_options.mindigit <= mask.count("d") <= arg_options.maxdigit and
        arg_options.minspecial <= mask.count("s") <= arg_options.maxspecial and
        arg_options.minlowerhex <= mask.count("h") <= arg_options.maxlowerhex and
        arg_options.minupperhex <= mask.count("H") <= arg_options.maxupperhex):
        return True
    else:
        return False

def check_custom_charsets(mask, arg_options):
    '''Checks for undefined character sets within a mask.'''
    if (not arg_options.charset1 and "1" in mask or
        not arg_options.charset2 and "2" in mask or
        not arg_options.charset3 and "3" in mask or
        not arg_options.charset4 and "4" in mask):
        return False
    else:
        return True