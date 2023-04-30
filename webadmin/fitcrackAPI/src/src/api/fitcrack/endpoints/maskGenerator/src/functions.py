'''
About: Set of functions shared across MaskGenerator classes.
Author: Samuel Hribik
'''

def check_compatibility(mask, mask_pattern):
    '''Compare mask with a given mask pattern.'''
    if len(mask) != len(mask_pattern):
        return False
    for i, letter in enumerate(mask):
        if mask_pattern[i] == 'a':
            if letter not in 'luds':
                return False
        elif mask_pattern[i] != 'b' and mask_pattern[i] != letter:
            return False
    return True

def check_charsets(mask, arg_options):
    '''Checks for required number of character sets within a mask.'''
    for placeholder_char, charset_name in [('l', 'lower'), ('u', 'upper'), ('d', 'digit'), ('s', 'special'), ('h', 'lowerhex'), ('H', 'upperhex')]:
        min_count = getattr(arg_options, f'min{charset_name}')
        max_count = getattr(arg_options, f'max{charset_name}')
        charset_count = mask.count(placeholder_char)
        if not min_count <= charset_count + mask.count('a') and charset_count <= max_count:
            return False
        
    return True


def check_custom_charsets(mask, arg_options):
    '''Checks for undefined character sets within a mask.'''
    for i in range(1, 5):
        charset = getattr(arg_options, f"charset{i}")
        if charset:
            min_count = getattr(arg_options, f"mincharset{i}")
            max_count = getattr(arg_options, f"maxcharset{i}")
            if not min_count <= mask.count(str(i)) <= max_count:
                return False
        else:
            if str(i) in mask:
                return False

    return True
