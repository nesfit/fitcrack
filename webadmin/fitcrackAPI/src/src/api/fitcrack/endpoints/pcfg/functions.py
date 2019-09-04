'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import os
import shutil
import zipfile
import pathlib
import subprocess

from pathlib import Path
from settings import HASHCAT_PATH, PCFG_DIR, HASHCAT_DIR, PCFG_MOWER_DIR, PCFG_MANAGER_DIR, PCFG_TRAINER_DIR, PCFG_TRAINER_RULE_DIR, DICTIONARY_DIR
from src.api.fitcrack.functions import shellExec


def readingFromFolderPostProcces(PcfgModel):
    PcfgModel.keyspace = int(shellExec(HASHCAT_PATH + ' --keyspace -a 0 "' + os.path.join(PCFG_DIR, PcfgModel.path + '"'),
                                             cwd=HASHCAT_DIR))
    return PcfgModel


def unzipGrammarToPcfgFolder(pcfgFilename):

    pathToZipFile = '/usr/share/collections/pcfg/%s' % (pcfgFilename,)

    if(pathlib.Path(pathToZipFile).exists()):

        zipObj = zipfile.ZipFile(pathToZipFile, 'r')
        path = "/usr/share/collections/pcfg/%s" % (Path(pathToZipFile).stem,)
        zipObj.extractall(path)

    else:
        print("Does not exist.")


def deleteUnzipedFolderDirectory(pcfgZipFilePath):

    pcfgUnzipFolderPath = '/usr/share/collections/pcfg/%s' % (Path(pcfgZipFilePath).stem,)

    if(pathlib.Path(pcfgUnzipFolderPath).exists()):

        shutil.rmtree(pcfgUnzipFolderPath)

    else:
        print("Does not exist.")


def createPcfgGrammarBin(pcfgFileNameZip):
    #./pcfg-manager marshal -r /usr/share/collections/pcfg/atom

    test = PCFG_MANAGER_DIR + ' marshal -r ' \
                                 + os.path.join(PCFG_DIR, extractNameFromZipfile(pcfgFileNameZip)) \
                                 + ' -o ' + os.path.join(PCFG_DIR, extractNameFromZipfile(pcfgFileNameZip)) \
                                 + '/grammar.bin'
    print(test)
    pcfgKeyspace = shellExec(PCFG_MANAGER_DIR + ' marshal -r ' \
                                 + os.path.join(PCFG_DIR, extractNameFromZipfile(pcfgFileNameZip)) \
                                 + ' -o ' + os.path.join(PCFG_DIR, extractNameFromZipfile(pcfgFileNameZip)) \
                                 + '/grammar.bin')


def calculateKeyspace(pcfgFileNameZip):

    pcfgKeyspace = 0
    pcfgKeyspace = int(shellExec(PCFG_MOWER_DIR + ' -i ' + os.path.join(PCFG_DIR, extractNameFromZipfile(pcfgFileNameZip))))
    return pcfgKeyspace


def extractNameFromZipfile(pcfgFileNameZip):

    pcfgFileName = Path(pcfgFileNameZip).stem
    return pcfgFileName

def makePcfgFolder(nameWithExt):

    # ./pcfg_trainer.py --coverage 1.0 --rule rty -t test
    test = PCFG_TRAINER_DIR + ' --coverage 1.0 ' \
                                 + ' --rule ' + extractNameFromZipfile(nameWithExt) \
                                 + ' -t ' + os.path.join(DICTIONARY_DIR, nameWithExt)
    print(test)

    pcfgMakeGrammar = shellExec(test)

def moveGrammarToPcfgDir(nameWithExt):

    test = 'mv ' + PCFG_TRAINER_RULE_DIR + '/' + extractNameFromZipfile(nameWithExt) + ' ' + PCFG_DIR + '/' + extractNameFromZipfile(nameWithExt)
    print(test)

    pcfgMoveGrammar = shellExec(test)
