#!/usr/bin/python3

# This is XtoHashcat script in Python 3
# Written as part of TARZAN project for identifying digital threads.
# It extracts hash from various files in format appropriate for hashcat
# using several open-source scripts.
# For more info see --help and README

# Author: Lukas Zobal (izobal@fit.vutbr.cz)

# Modifications:
# @date 30.5.2017 Tool created
# @date 17.7.2017 Added StaticHelper class, tool now prints hashType after newline
# @date 28.7.2017 Format numbers in -f are same as hashcat's --hash-type
# @date 06.7.2018 Added check for pkzip and special return code, new RAR/ZIP john2hashcat parsing

# RETURN CODES:
#  * 0 = Everything OK
#  * 1 = General error (file not found/format not recognized/hash not extracted)
#  * 2 = Extracted hash, which is not usable with hashcat (pkzip)


import argparse
import os.path
import binascii
import subprocess

from sys import exit, stderr, argv


class StaticHelper:
    'Class that provides support functions and variables'

    # ---------- Supported formats ----------
    SupportedFormats = {
        "MS_OFFICE" : [9400, 9500, 9600, 9700, 9800],
        "PDF" : [10400, 10500, 10600, 10700],
        "RAR" : [12500, 13000],
        "ZIP" : [13600],
        "7-ZIP" : [11600]
    }

    @staticmethod
    def getHashType(hashStr, formatId):
        # MS Office
        if (formatId == 0):
            if (hashStr[1:10].lower() == "oldoffice"):
                if (hashStr[11:12] == "0" or hashStr[11:12] == "1"):
                    return "9700"       # Office 97-2000 (MD5)
                else:
                    return "9800"       # Office 2003 (SHA1)
            else:
                if (hashStr[9:13] == "2007"):
                    return "9400"       # Office 2007 (AES + SHA1)
                elif (hashStr[9:13] == "2010"):
                    return "9500"       # Office 2010 (SHA512)
                else:
                    return "9600"       # Office 2013/16 (100k rounds)
                
        # PDF
        elif (formatId == 1):
            if (hashStr[5:6] == "1"):
                return "10400"          # PDF 1.1 - 1.3 (Acrobat 2 - 4)
            elif (hashStr[5:6] == "2" or
                  hashStr[5:6] == "3" or
                  hashStr[5:6] == "4"):
                return "10500"          # PDF 1.4 - 1.6 (Acrobat 5 - 8)
            elif (hashStr[5:8] == "5*5"):
                return "10600"          # PDF 1.7 Level 3 (Acrobat 9)
            else:
                return "10700"          # PDF 1.7 Level 8 (Acrobat 10 - 11)
        
        # RAR
        elif (formatId == 2):
            if (hashStr[1:5].lower() == "rar3"):
                return "12500"          # RAR3-hp
            else:
                return "13000"          # RAR5
        
        # ZIP
        elif (formatId == 3):
            return "13600"              # Win-Zip
        
        # 7-Zip
        elif (formatId == 4):
            return "11600"              # 7-Zip
        
        # Unsupported format
        else:
            return "-1"


# ------------ Format class ------------
class Format:
    'Class that represents a single extraction format'
    
    def __init__(self, id, extensions, signatures, scriptPath, compiler):
        self.id = id
        self.extensions = extensions
        self.signatures = signatures
        self.scriptPath = scriptPath
        self.compiler = compiler
        
    def checkSignature(self, signature):
        for sig in self.signatures:
            if (signature.startswith(sig)):
                return True
        return False
    
    def checkExtension(self, extension):
        for ext in self.extensions:
            if (extension.lower() == ext):
                return True            
        return False
    
    def checkHash(self, path):
        # Call extraction script
        process = subprocess.Popen([self.scriptPath, path], stdout=subprocess.PIPE)
        out, err = process.communicate()
        
        # Check output
        if (len(out) == 0):
            if (err is None or len(err) == 0):
                print("Empty output. Is the file encrypted?", file=stderr)
            else:
                print(err.decode('utf-8'), file=stderr)
            exit(1)            
        else:
            hashStr = out.decode('utf-8')

            # ZIP/RAR john2hashcat
            if (self.extensions[0] == ".rar" or self.extensions[0] == ".zip"):
                hashStr = hashStr[(hashStr.find(':') + 1):]
                colonIndex = hashStr.find(':')
                
                if (colonIndex != -1):
                    hashStr = hashStr[:colonIndex]

            # ZIP often contains path inside hash
            zfileIndex = hashStr.find("ZFILE")
            if (zfileIndex != -1):
                suffixIndex = hashStr.find('*', zfileIndex + 6)
                prefixIndex = (hashStr[:(zfileIndex - 2)]).rfind('*')
                hashStr = hashStr[:prefixIndex] + hashStr[suffixIndex:]
            
            # Print hash '\n' hash-type
            print(hashStr.strip(' \t\n\r') + "\n" + StaticHelper.getHashType(hashStr, self.id))

            # Check for pkzip
            if (hashStr[1:6] == "pkzip"):
                return 2
            else:
                return 0
    

# ---------- Extractor class -----------
class Extractor:
    'Class for extracting a hash from the file'
    
    def __init__(self, path):
        self.path = path
        self.activeFormat = -1
        
        # Create Formats dynamically
        self.extractorFormats = []
        self.extractorFormats.append(Format(0, [".doc", ".docx", ".xls", ".xlsx", ".ppt", ".pptx"], [b"d0cf11e0a1b11ae1"], "scripts/office2hashcat.py", "python"))
        self.extractorFormats.append(Format(1, [".pdf"], [b"25504446"], "scripts/pdf2hashcat.py", "python"))
        self.extractorFormats.append(Format(2, [".rar"], [b"526172211a07"], "scripts/rar2john", ""))
        self.extractorFormats.append(Format(3, [".zip"], [b"504b0304"], "scripts/zip2john", ""))
        self.extractorFormats.append(Format(4, [".7z"], [b"377abcaf271c"], "scripts/7z2hashcat.pl", "perl"))
        
    def checkFormat(self):
        try:
            # Open the file, extract signature and extension
            f = open(self.path, "rb")
            sig = f.read(20)
            sig = binascii.hexlify(sig)
            ext = os.path.splitext(self.path)[1]
        except:
            print("Failed to open input file.", file=stderr)
            return False
        
        # Look for a match
        for format in self.extractorFormats:
            if (format.checkSignature(sig) and format.checkExtension(ext)):
                self.activeFormat = format.id
                # print("Identified format: " + StaticHelper.SupportedFormats[self.activeFormat])
                return True

        print("Format could not be identified, please specify it manually using -f flag.\nAlso, the document might not be encrypted.", file=stderr)
        return False
    
    def setFormat(self, format):
        if (format >= 9400 and format <= 9800 and format % 100 == 0):
            self.activeFormat = 0
        elif (format >= 10400 and format <= 10700 and format % 100 == 0):
            self.activeFormat = 1
        elif (format == 12500 or format == 13000):
            self.activeFormat = 2
        elif (format == 13600):
            self.activeFormat = 3
        elif (format == 11600):
            self.activeFormat = 4
        else:
            print("The --hash-type " + str(format) + " is not supported by XtoHashcat.", file=stderr)
            return False
        
        return True
    
    def extract(self):
        return self.extractorFormats[self.activeFormat].checkHash(self.path)
        

# ---------- Argument parsing ----------
class ArgumentParser(object):
    'Class for argument parsing, using argparse library'
    
    def __init__(self):
        pass
    
    # Help strings
    help_program = 'This program extracts hash from number of formats + prints hashcat --hash-type on the last line'
    help_help = 'Output this message'
    help_file = 'Path to file from which hash is extracted'
    help_format = 'If this argument is set, program will NOT try to determine format automatically but will use supplied format instead.\
Supported set is a sub-set of hashcat supported --hash-types:\n\
\tMicrosoft Office Documents (-f 9400-9800)\n\
\tPDF documents (-f 10400-10700)\n\
\tRAR archive (-f 12500/13000)\n\
\tZIP archive (-f 13600)\n\
\t7-Zip archive (-f 11600)\n\n\
Note: Setting different types of the same document (e.g. 9400 or 9800) makes no difference.'    
    
    parser = argparse.ArgumentParser(description=help_program, formatter_class=argparse.RawTextHelpFormatter)
    
    # Load arguments
    def loadArguments(self):
        self.parser.add_argument("filePath", type=str, help=self.help_file)
        self.parser.add_argument("-f", type=int, help=self.help_format)
                
        try:
            args = self.parser.parse_args()
        except:
            print("See --help for more info.", file=stderr)
            exit(1)

        # Check for duplicates
        if (len(argv) > len(set(argv))):
            print("Do not use duplicate arguments.", file=stderr)
            exit(1)
        
        return args


# ---------- MAIN ----------

# Argument parsing
argParser = ArgumentParser()
args = argParser.loadArguments()

# Create extractor
extr = Extractor(args.filePath)

if (args.f or args.f == 0):
    if (not extr.setFormat(args.f)):    # set format manually
        exit(1)
else:
    if (not extr.checkFormat()):  # choose format automatically
        exit(1)

# Try to extract the hash
exit(extr.extract())
