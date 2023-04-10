import string
import itertools

from src.api.fitcrack.endpoints.maskGenerator.src.functions import *
from src.api.fitcrack.endpoints.maskGenerator.src.PasswordAnalyzer import PasswordAnalyzer
from src.api.fitcrack.endpoints.maskGenerator.src.IterationGenerator import IterationGenerator
from src.api.fitcrack.endpoints.maskGenerator.src.MaskSorter import MaskSorter

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
        self.wordlists = options.get('wordlists')
        self.sorting = options.get('sortingMode')
        self.speed = int(options.get('speed'))
        self.time = int(options.get('time'))
        self.minocc = int(options.get('minocc'))
        self.filename = options.get('filename')
        self.charsetOrderList = sorted(list(options.get('charsetOrderList')), key=lambda x: x['order'])
        self.useHex = options.get('useHex')

class MaskGenerator():

    def __init__(self) -> None:
        self.message = "Successfully created mask file."

    def generateMaskFile(self, arg_options, masksPath, wordlistsPath):
        
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
            masks = analyzer.analyze(options, wordlistsPath)

        else:
            generator = IterationGenerator()
            masks = generator.generate(options)

        sorter = MaskSorter(options.sorting, masks)
        sorter.sort_masks(options)

        sorter.save_masks_to_file(options, masksPath)

        return self.message
    