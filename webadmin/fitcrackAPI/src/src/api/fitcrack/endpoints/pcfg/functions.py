'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import os
import sys
import shutil
import zipfile
import pathlib
import subprocess

from pathlib import Path
from settings import HASHCAT_PATH, PCFG_DIR, HASHCAT_DIR, PCFG_MOWER_DIR, PCFG_MANAGER_DIR, PCFG_TRAINER_DIR, PCFG_TRAINER_RULE_DIR, DICTIONARY_DIR
from src.api.fitcrack.functions import shellExec


def readingFromFolderPostProcces(PcfgModel):
    PcfgModel.keyspace = calculateKeyspace(PcfgModel.name)
    return PcfgModel


def unzipGrammarToPcfgFolder(pcfgFilename):

    pathToZipFile = '/usr/share/collections/pcfg/%s' % (pcfgFilename,)

    if(pathlib.Path(pathToZipFile).exists()):

        zipObj = zipfile.ZipFile(pathToZipFile, 'r')
        path = "/usr/share/collections/pcfg/%s" % (Path(pathToZipFile).stem,)
        zipObj.extractall(path)

        if os.path.exists(pathToZipFile):
            os.remove(pathToZipFile)
    else:
        print("Does not exist.")


def deleteUnzipedFolderDirectory(pcfgZipFilePath):

    pcfgUnzipFolderPath = '/usr/share/collections/pcfg/%s' % (Path(pcfgZipFilePath).stem,)

    if(pathlib.Path(pcfgUnzipFolderPath).exists()):

        shutil.rmtree(pcfgUnzipFolderPath)

    else:
        print("Does not exist.")


def createPcfgGrammarBin(pcfgFileNameZip):
    test = PCFG_MANAGER_DIR + ' marshal -r ' \
                                 + os.path.join(PCFG_DIR, extractNameFromZipfile(pcfgFileNameZip)) \
                                 + ' -o ' + os.path.join(PCFG_DIR, extractNameFromZipfile(pcfgFileNameZip)) \
                                 + '/grammar.bin'
    pcfgKeyspace = shellExec(PCFG_MANAGER_DIR + ' marshal -r ' \
                                 + os.path.join(PCFG_DIR, extractNameFromZipfile(pcfgFileNameZip)) \
                                 + ' -o ' + os.path.join(PCFG_DIR, extractNameFromZipfile(pcfgFileNameZip)) \
                                 + '/grammar.bin')


def calculateKeyspace(pcfgFileNameZip):

    pcfgKeyspace = 0
    pcfgKeyspace = shellExec(PCFG_MOWER_DIR + ' -i ' + os.path.join(PCFG_DIR, extractNameFromZipfile(pcfgFileNameZip)))

    # Keyspace control
    INT_MAX = sys.maxsize - 1

    if int(pcfgKeyspace) >= INT_MAX:
        pcfgKeyspace = INT_MAX

    return pcfgKeyspace


def extractNameFromZipfile(pcfgFileNameZip):

    pcfgFileName = Path(pcfgFileNameZip).stem
    return pcfgFileName

def makePcfgFolder(nameWithExt):

    test = PCFG_TRAINER_DIR + ' --coverage 1.0 ' \
                                 + ' --rule ' + extractNameFromZipfile(nameWithExt) \
                                 + ' -t ' + os.path.join(DICTIONARY_DIR, nameWithExt)
    pcfgMakeGrammar = shellExec(test)

def moveGrammarToPcfgDir(nameWithExt):

    test = 'mv ' + PCFG_TRAINER_RULE_DIR + '/' + extractNameFromZipfile(nameWithExt) + ' ' + PCFG_DIR + '/' + extractNameFromZipfile(nameWithExt)
    pcfgMoveGrammar = shellExec(test)
