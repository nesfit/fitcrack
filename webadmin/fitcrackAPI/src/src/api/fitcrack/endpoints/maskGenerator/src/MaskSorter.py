import string

from src.api.fitcrack.endpoints.maskGenerator.src.functions import *

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
                elif charset == 'h' or charset == 'H':
                    complexity *= 16
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