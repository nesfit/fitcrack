import string

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
    if (not arg_options.charset1 and "1" in mask or
        not arg_options.charset2 and "2" in mask or
        not arg_options.charset3 and "3" in mask or
        not arg_options.charset4 and "4" in mask):
        return False
    else:
        return True

class PasswordAnalyzer:
    '''Takes given arguments, filters and analyzes compatible passwords.'''
    def __init__(self):
        self.masks = {}

    def analyze(self, arg_options):
        '''Iterate through passwords in wordlists and analyze passwords compatible with policy.'''
        pass

class PasswordGenerator():
    '''Generates masks based on given password policies.'''
    def __init__(self):
        self.charset = ["?l", "?u", "?d", "?s"]
        self.masks = {}

    def generate(self, arg_options):
        '''Generate all compatible iterations of password masks.'''
        pass

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
        pass
    
    def sort_masks(self, input_options):
        '''Create sorted dictionary'''
        pass
    
    def save_masks_to_file(self, arg_options):
        '''Save sorted masks to an output file'''
        pass

class Options():
    def __init__(self, options) -> None:
        self.minlower = int(options.get('minlower'))
        self.maxlower = int(options.get('maxlower'))
        self.minupper = int(options.get('minupper'))
        self.maxupper = int(options.get('maxupper'))
        self.mindigit = int(options.get('mindigit'))
        self.maxdigit = int(options.get('maxdigit'))
        self.minspecial = int(options.get('minspecial'))
        self.maxspecial = int(options.get('maxspecial'))
        self.minlength = int(options.get('minlength'))
        self.maxlength = int(options.get('maxlength'))
        self.patinc = options.get('patinc')
        self.patexc = options.get('patexc')
        self.charset1 = options.get('charset1')
        self.charset2 = options.get('charset2')
        self.charset3 = options.get('charset3')
        self.charset4 = options.get('charset4')
        self.wordlists = options.get('wordlists')
        self.sorting = options.get('sortingMode')

class MaskGenerator():

    def __init__(self) -> None:
        self.message = "all good"

    def generateMaskFile(self, arg_options, wordlistsPath):
        
        options = Options(arg_options)
        
        if options.patinc is not None:
            for pattern in options.patinc:
                if not (check_charsets(pattern, options) and check_custom_charsets(pattern, options) and
                        options.minlength <= len(pattern.replace('?', '')) <= options.maxlength):
                    return "Arguments incompatible with pattern: " + str(pattern)

        if options.patexc is not None:
            for pattern in options.patexc:
                if not (check_charsets(pattern, options) and check_custom_charsets(pattern, options) and
                        options.minlength <= len(pattern.replace('?', '')) <= options.maxlength):
                    return "Arguments incompatible with pattern: " + str(pattern)
                
        if options.wordlists:
            analyzer = PasswordAnalyzer()
            masks = analyzer.analyze(options)

        else:
            generator = PasswordGenerator()
            masks = generator.generate(options)

        masks = []
        sorter = MaskSorter(options.sorting, masks)
        sorter.sort_masks(options)

        sorter.save_masks_to_file(wordlistsPath)

        return self.message
    