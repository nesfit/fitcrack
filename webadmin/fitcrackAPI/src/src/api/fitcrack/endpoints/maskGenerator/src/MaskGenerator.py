'''
About: Main MaskGenerator class handling parsing options,
       checking patterns, instantiating other classes,
       and saving generated masks and charsets in files.
Author: Samuel Hribik
'''

from src.api.fitcrack.endpoints.maskGenerator.src.functions import *
from src.api.fitcrack.endpoints.maskGenerator.src.PasswordAnalyzer import PasswordAnalyzer
from src.api.fitcrack.endpoints.maskGenerator.src.IterationGenerator import IterationGenerator
from src.api.fitcrack.endpoints.maskGenerator.src.MaskSorter import MaskSorter

class Options():
    '''Parse arguments from request.'''
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
        self.minlowerhex = int(options.get('minlowerhex'))
        self.maxlowerhex = int(options.get('maxlowerhex'))
        self.minupperhex = int(options.get('minupperhex'))
        self.maxupperhex = int(options.get('maxupperhex'))
        self.patinc = options.get('patinc')
        self.patexc = options.get('patexc')
        self.charset1 = options.get('charset1')
        self.charset2 = options.get('charset2')
        self.charset3 = options.get('charset3')
        self.charset4 = options.get('charset4')
        self.mincharset1 = int(options.get('mincharset1'))
        self.maxcharset1 = int(options.get('maxcharset1'))
        self.mincharset2 = int(options.get('mincharset2'))
        self.maxcharset2 = int(options.get('maxcharset2'))
        self.mincharset3 = int(options.get('mincharset3'))
        self.maxcharset3 = int(options.get('maxcharset3'))
        self.mincharset4 = int(options.get('mincharset4'))
        self.maxcharset4 = int(options.get('maxcharset4'))
        self.wordlists = options.get('wordlists')
        self.sorting = options.get('sortingMode')
        self.speed = int(options.get('speed'))
        self.time = int(options.get('time'))
        self.minocc = int(options.get('minocc'))
        self.filename = options.get('filename')
        self.charsetOrderList = sorted(list(options.get('charsetOrderList')), key=lambda x: x['order'])
        self.useHex = options.get('useHex')

class MaskGenerator():
    '''Main class handling mask generaion process logic.'''
    def __init__(self) -> None:
        self.message = "Success"
        self.charsets = []

    def getCustomCharsetList(self):
        '''Return list of used custom character sets.'''
        return self.charsets

    def generateMaskFile(self, arg_options, masks_path, wordlists_path, charsets_path):
        '''Check patterns and dictionaries, and either call analyzer or generator to get masks.'''
        options = Options(arg_options)
        
        if options.patinc:
            for pattern in options.patinc:
                if not check_charsets(pattern, options):
                    return "Criteria incompatible with pattern: " + str(pattern)
                if not check_custom_charsets(pattern, options):
                    return "Undefined charset used in pattern: " + str(pattern)
                if not options.minlength <= len(pattern.replace('?', '')) <= options.maxlength:
                    return "Pattern " + pattern + " incompatible with length criteria."

        if options.patexc:
            for pattern in options.patexc:
                if not check_charsets(pattern, options):
                    return "Criteria incompatible with pattern: " + str(pattern)
                if not check_custom_charsets(pattern, options):
                    return "Undefined charset used in pattern: " + str(pattern)
                if not options.minlength <= len(pattern.replace('?', '')) <= options.maxlength:
                    return "Pattern " + pattern + " incompatible with length criteria."
                
        if options.wordlists:
            analyzer = PasswordAnalyzer()
            masks = analyzer.analyze(options, wordlists_path)

        else:
            generator = IterationGenerator()
            masks = generator.generate(options)

        sorter = MaskSorter(options.sorting, masks)
        sorter.sort_masks(options)

        sorter.save_masks_to_file(options, masks_path)

        for i in range(1,5):
            charset = getattr(options, f"charset{i}")
            if charset:
                try:
                    self.charsets.append(f"charset{i}")
                    file = open(charsets_path + "/" + options.filename + "_" + f"charset{i}" + ".hcchr", "w", encoding="utf-8")
                    file.write(charset)
                    file.close()
                except OSError:
                    return charsets_path + "/" + options.filename + "_" + f"charset{i}" + ".hcchr"

        return self.message
    