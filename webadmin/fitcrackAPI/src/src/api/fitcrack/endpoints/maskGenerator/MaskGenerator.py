import string
import itertools

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
                            if arg_options.charset1 and letter in arg_options.charset1:
                                mask += "?1"
                            elif arg_options.charset2 and letter in arg_options.charset2:
                                mask += "?2"
                            elif arg_options.charset3 and letter in arg_options.charset3:
                                mask += "?3"
                            elif arg_options.charset4 and letter in arg_options.charset4:
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
            pass
            if occurrence >= arg_options.minocc:
                filtered_masks.update({mask:occurrence})

        return filtered_masks

class PasswordGenerator():
    '''Generates masks based on given password policies.'''
    def __init__(self):
        self.charset = ["?l", "?u", "?d", "?s"]
        self.masks = {}

    def generate(self, arg_options):
        '''Generate all compatible iterations of password masks.'''
        if arg_options.charset1:
            self.charset.append("?1")
        if arg_options.charset2:
            self.charset.append("?2")
        if arg_options.charset3:
            self.charset.append("?3")
        if arg_options.charset4:
            self.charset.append("?4")

        for iteration in range(arg_options.minlength, 10 if arg_options.maxlength > 10 else arg_options.maxlength + 1):
            for mask in itertools.product(self.charset, repeat=iteration):
                joined_mask = ''.join(mask)

                if joined_mask == '':
                    continue

                if not check_charsets(joined_mask, arg_options):
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

            self.mask_complexity.update({mask:{"Occurrence":occurrence,
                                        "Complexity":complexity, "Optimal":complexity//occurrence}})
    
    def sort_masks(self, input_options):
        '''Create sorted dictionary'''
        
        capacity = None
        if input_options.time != 0:
            capacity = input_options.time * input_options.speed

        self.add_complexity(input_options)
        
        if self.sorting_mode == "Occurrence":
            sorted_masks = dict(sorted(self.mask_complexity.items(),
                                 key=lambda x:x[1][self.sorting_mode], reverse=True))
        elif self.sorting_mode == "Complexity":
            sorted_masks = dict(sorted(self.mask_complexity.items(),
                                 key=lambda x:x[1][self.sorting_mode]))
        elif self.sorting_mode == "Optimal":
            sorted_masks = dict(sorted(self.mask_complexity.items(),
                                 key=lambda x:x[1][self.sorting_mode]))
            
        for mask in sorted_masks:
            if capacity is not None:
                if capacity - self.mask_complexity[mask]["Complexity"] < 0:
                    break
                else:
                    capacity -= self.mask_complexity[mask]["Complexity"]

            self.sorted_masks.append(mask)
    
    def save_masks_to_file(self, arg_options, masksPath):
        '''Save sorted masks to an output file'''
        if arg_options.filename == '':
            file = open(masksPath + "/test.hcmask", "w", encoding="utf-8")
        else:
            file = open(masksPath + "/" + arg_options.filename + ".hcmask", "w", encoding="utf-8")
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
        file.close()

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
        self.speed = int(options.get('speed'))
        self.time = int(options.get('time'))
        self.minocc = int(options.get('minocc'))
        self.filename = options.get('filename')

class MaskGenerator():

    def __init__(self) -> None:
        self.message = "all good"

    def generateMaskFile(self, arg_options, masksPath, wordlistsPath):
        
        options = Options(arg_options)
        
        if options.patinc:
            for pattern in options.patinc:
                if not (check_charsets(pattern, options) and check_custom_charsets(pattern, options) and
                        options.minlength <= len(pattern.replace('?', '')) <= options.maxlength):
                    return "Arguments incompatible with pattern: " + str(pattern)

        if options.patexc:
            for pattern in options.patexc:
                if not (check_charsets(pattern, options) and check_custom_charsets(pattern, options) and
                        options.minlength <= len(pattern.replace('?', '')) <= options.maxlength):
                    return "Arguments incompatible with pattern: " + str(pattern)
                
        if options.wordlists:
            analyzer = PasswordAnalyzer()
            masks = analyzer.analyze(options, wordlistsPath)

        else:
            generator = PasswordGenerator()
            masks = generator.generate(options)

        sorter = MaskSorter(options.sorting, masks)
        sorter.sort_masks(options)

        sorter.save_masks_to_file(options, masksPath)

        return self.message
    