'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import os
import shutil
import zipfile
import pathlib

from pathlib import Path
from settings import HASHCAT_PATH, PCFG_DIR, HASHCAT_DIR
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
