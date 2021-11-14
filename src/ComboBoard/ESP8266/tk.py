import urequests as req
import time
import os

base = 'http://moorelife.nl/repo/'

def do_req(extra, dflt=''):
    txt = dflt
    try:
        print('get request', base+extra)
        rsp = req.get(url=base+extra, 
                      headers={'Content-Type': 'text/html;charset=utf-8'})
        txt = rsp.text
        rsp.close()
    except OSError as e:
        print('request error:', e)
    return txt

def rd_file(fn, dflt):
    txt = dflt
    try:
        with open(fn, 'r') as f:
            txt = f.read()
    except OSError as e:
        print('read error:', e)
    return txt

def del_file(fn):
    try:
        os.remove(fn)
    except OSError:
        pass

def wr_file(fn, txt):
    try:
        with open(fn, 'w') as f:
            f.write(str(txt))
    except OSError as e:
        print('write error:', e)
