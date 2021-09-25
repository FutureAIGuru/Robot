import machine
import time
import os
import tk

lst  = 'load_files.txt'
vrs  = 'version.txt'

def upd(src):
    tk.del_file(src)
    txt = tk.do_req(src)
    tk.wr_file(src, txt)

def check_for_updates():
    old_vrs = tk.rd_file(vrs, '0.0.0')
    new_vrs = tk.do_req(vrs, '0.0.0')
    if str(old_vrs) == str(new_vrs):
        print('versions equal, no update')
        return
    tk.wr_file(vrs, new_vrs)
    txt = tk.do_req(lst)
    lines = txt.split('\n')
    for line in lines:
        if line.endswith('\r'):
            line = line[0:-1]
        if len(line) > 0:
            upd(line)
    machine.reset()

check_for_updates()
print('version:', tk.rd_file(vrs))

# Main program after this...