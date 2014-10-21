#!/usr/bin/python
# -*- coding: utf-8 -*- 
import codecs
import sqlite3
import sys
import os
import pprint
result_db_file = '''ships.db'''
sql_db_init_cmds =['''CREATE TABLE "shipInfo" (
    "shipId" INTEGER NOT NULL DEFAULT (0),
    "crt" TEXT NOT NULL,
    "shipName" TEXT,
    "tankNumber" INTEGER NOT NULL DEFAULT (8),
    "capacityNumber" INTEGER NOT NULL DEFAULT (7),
    "shipTrimMin" REAL NOT NULL,
    "shipTrimStep" REAL,
    "finalDate" INTEGER
);''',
'''CREATE TABLE "tankInfo" (
    "shipId" INTEGER NOT NULL,
    "tankId" INTEGER NOT NULL,
    "sounding" INTEGER NOT NULL,
    "capacity" TEXT
);'''
]

test ='''INSERT INTO shipInfo VALUES(1,"DSFASD0","HELLO",6,7,-0.4,0.4,"2014") ; '''

help ='''
app --add/del   ship/tank   [file]/shipId [tankId]
    result db file : ships.db
    you can del or move it and the app will generate a new one
    for example :
            app --add  ship   ship.txt
            app --add  tank   tank.txt
            app --del  ship   shipId
            app --del  tank   shipId    tankId
            
            
    please format you file like following:
    ship file format :
        ship id
        tank crt
        ship name
        tank number
        capacity number
        ship trim min
        ship trim step
        final date

    tank file format :
        ship id
        tank id
        cacacity data
'''
def init_db_file(fileName):

    db_conn = sqlite3.connect(fileName)
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


def add_ship_info(fileName):
    fd = open(fileName,'r')
    bufs = fd.readlines()
    print bufs
    lines = []
    for line in bufs :
        if line[0] == "#":
            continue
        line = line.replace("\n","").replace("\r",'').replace(" ","")
        if len(line) > 0:
            lines.append(line)
    pprint.pprint( lines)
    if len(lines) !=  8 :
        print "ship file format error ,please check it "
        sys.exit(0)
    shipId = int(lines[0] )
    crt = lines[1]
    shipName = lines[2]
    tankNumber = int(lines[3])
    
    capacityNumber =  int(lines[4])
    trimMin = float(lines[5])
    trimStep = float(lines[6])
    finalDate = lines[7]
    db_conn = init_db_file(result_db_file)
    sql = "SELECT * FROM shipInfo WHERE shipId = %s"%shipId
    db_cursor = db_conn.cursor()
    db_cursor.execute(sql)
    result = db_cursor.fetchall()
    print result
    if len(result) != 0:
        while True :
            print("current ship info is exits ,will you update it")
            print (lines)
            input = raw_input("will you update it ? y/n    ")
            if input == 'y' :
                print "will update the ship info"
                break
            elif input == 'n':
                print "update abort ,do nothing"
                
                
                sys.exit(0)
            else :
                print "woring input ,try again "
        #sql = "SELECT * FROM shipInfo WHERE shipId = %s"%shipId
        #not use update sql,jsut del and add
        sql = "DELETE FROM shipInfo where shipId = %s"%shipId
        db_cursor.execute(sql)

    print("update ship info")
    sql = '''INSERT INTO shipInfo VALUES(%s,"%s","%s",%s,%s,%s,%s,"%s") ;'''%(shipId,crt,shipName,tankNumber,capacityNumber,trimMin,trimStep,finalDate)
    print sql
    db_cursor.execute(sql)
    db_conn.commit()
    db_cursor.close()
        
        

def add_tank_info(fileName):
    pass
def del_ship_info(shipId):
    pass
def del_tank_info(shipId,tankId):
    pass
    fd =  open(fileName,'r')
    for i in range(0,6) :
        buf = fd.readline()
        print buf
    pass
if __name__ == "__main__" :

    argc = len(sys.argv) 
    if argc < 2 :
        print(help)
        exit(0)
    if sys.argv[1] == "--add" :
        if sys.argv[2] == 'ship' and argc == 4 :
            add_ship_info(sys.argv[3])
            pass
        elif sys.argv[2] == 'tank' and argc == 4 :
            add_tank_info(sys.argv[3])
            pass
        else :
            print( help)

    elif sys.argv[1] == "--del" :
        if sys.argv[2] == 'ship' and argc == 4 :
            del_ship_info(int(sys.argv[3]) )
            pass
        elif sys.argv[2] == 'tank' and argc == 5 :
            del_tank_info(int(sys.argv[3]),int(sys.argv[4]))
            pass
        pass
    elif sys.argv[1] == "--help" :
        print help
    sys.exit(0)
    shipId = int(sys.argv[1])
    tankId = int(sys.argv[2])
    dat_file =  sys.argv[3]
    db_file  = sys.argv[4]
    parse_data_file(dat_file)
   # db_conn = init_db_file(db_file)

    
    
    
    