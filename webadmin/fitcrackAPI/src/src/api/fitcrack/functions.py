'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import os
import subprocess
import time
from pathlib import Path

import sys
import os
import datetime
from flask_restplus import abort
from sqlalchemy import exc
from werkzeug.utils import secure_filename

from src.database import db

"""Functions which are used in multiple files"""


def shellExec(cmd, abortOnError=True, cwd=None, getOnlyReturnCode=False, getReturnCode=False):
    """
    Execute the external command
    """

    try:
        process = subprocess.Popen(cmd, shell=True,
                                   stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE,
                                   cwd=cwd)

        # wait for the process to terminate
        out, err = process.communicate()
        rtnCode = process.returncode
        print(out.decode('utf-8'), file= sys.stderr)
        print(err.decode('utf-8'), file=sys.stderr)
    except subprocess.CalledProcessError as err:
        print('SHELL ERROR:', err)
        if abortOnError:
            abort(400, 'Error with shell execution ' + err.decode('utf-8'))
        return rtnCode
    else:
        if getOnlyReturnCode:
            return rtnCode
        elif getReturnCode:
            return {
                'returnCode': rtnCode,
                'msg': out.decode('utf-8')
            }
        else:
            return out.decode('utf-8')


def getStringBetween(string, substring1, substring2):
    """
    Gets string between two strings
    """

    start = string.find(substring1)
    if start == -1:
        abort(500,'server error')
    start += len(substring1)
    end = string.find(substring2, start)
    return string[start:end]


def parserAsciiTable(table):
    table = table.splitlines()

    header = table[0].split('|')
    for i,columnName in enumerate(header):
        header[i] = columnName.strip()

    result = []
    for i,line in enumerate(table[2:]):
        tmpLine = line.split('|')
        row = {}
        for j,columnData in enumerate(tmpLine):
            row[header[j]] = columnData.strip()

        result.append(row)
    return result


def lenStr(o):
    """
    !!!STRING!!! that contains length of object
    """
    return str(len(o))


def allowed_file(filename, extensions):
    return '.' in filename and \
           filename.rsplit('.', 1)[1].lower() in extensions


def fileUpload(file, dir, extensions, content=None, suffix=None, withTimestamp=False):
    if file.filename == '':
        abort(500, 'No selected file')
    if file and allowed_file(file.filename, extensions):

        filename = secure_filename(file.filename)
        originalFilename = file.filename

        if withTimestamp:
            filename = Path(filename).stem + '_' + str(int(time.time())) + Path(filename).suffix

        if suffix:
            filename = Path(filename).stem + suffix

        path = os.path.join(dir, filename)

        if os.path.exists(path):
            abort(500, 'File with name ' + filename + ' already exists.' + path)

        file.save(path)
        file.close()

        if content:
            file = open(path, "w")
            file.write(content)
            file.close()

        return {
            'filename': secure_filename(originalFilename),
            'path': filename
        }
    else:
        abort(500,'Can not upload file ' + file.filename)
    return None


def getFilesFromFolder(folder, DBmodel, processFunction):
    if not os.path.exists(folder):
        abort(500, 'Directory ' + folder + ' does not exist.')

    result = []
    for file in os.listdir(folder):
        filePath = os.path.join(folder, file)
        if os.path.isfile(filePath):
            modificationTime = datetime.datetime.fromtimestamp(os.path.getmtime(filePath)).strftime('%Y-%m-%d %H:%M:%S')
            DBrecord = DBmodel.query.filter_by(modification_time=modificationTime, path=file).first()
            if not DBrecord:
                DBrecord = DBmodel(modification_time=modificationTime, path=file, name=file)
                DBrecord = processFunction(DBrecord)
                try:
                    db.session.add(DBrecord)
                    db.session.commit()
                except exc.IntegrityError as e:
                    db.session().rollback()
                    abort(500, 'Can\'t create record in DB with  ' + file + '.')
            if not DBrecord.deleted:
                result.append(DBrecord)
    return result
