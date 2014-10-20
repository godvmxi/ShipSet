#!/usr/bin/python
# -*- coding: utf-8 -*- 
import sqlite3
import sys
import os
sql_db_init_cmds =['''CREATE TABLE shipInfo (date text, trans text, symbol text, qty real, price real)''',
'''CREATE TABLE tankInfo (date text, trans text, symbol text, qty real, price real)'''
]



help ='''
app shipId tankId  dat_file db_file
can add more dat file to db file 
'''
def init_db_file(fileName):

    db_conn = sqlite3.connect(db_file)
    #init db tables
    db_cursor = db_conn.cursor()
    db_cursor.execute('''SELECT  * FROM sqlite_master WHERE type='table' ''')
    result =  db_cursor.fetchall()
    table_number =  len(result)
    db_verify_state = True
    if table_number == 0 or table_number != 2:
        db_verify_state = False
        #create new tables
        pass
    else :
        for temp in result :
            if temp[1] not in ["shipInfo","tankInfo"]:
                db_verify_state = False
                break
    if not db_verify_state :
        if table_number != 0:
            #clear all tables
            for temp in result :
                sql =  "DROP TABLE %s"%temp[1]
                db_cursor.execute(sql)
        for sql in sql_db_init_cmds :
                db_cursor.execute(sql)
            
    db_cursor.close()
    return db_conn
def get_ship_info(fd):
    
    pass
def store_ship_info(db,info):
    pass
def store_tank_info(db,info):
    pass
    
    
if __name__ == "__main__" :
    print("sys argv -> ",len(sys.argv) )
    if len(sys.argv) != 5 :
        print(help)
        exit(0)
    shipId = int(sys.argv[1])
    tankId = int(sys.argv[2])
    dat_file =  sys.argv[3]
    db_file  = sys.argv[4]
    
    db_conn = init_db_file(db_file)
    
    
    
    
    