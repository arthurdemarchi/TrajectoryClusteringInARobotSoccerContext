# _*_ coding: utf-8 _*_

"""
rcg2csv: Convert RoboCup Soccer Simulator rcg and rcl files to CSV.
Copyright (C) 2018 YangZheng
Data: 2018-05-04
Version： v1.0.1
Bugs or Suggestions please mail to cookie.yz@qq.com
"""

import sys
sys.path.append("../lib")
from world_model import World
import csv
import pandas as pd
import glob
import sys
import os

"""
TODO: 
    通过配置文件的形式，选择需要生成的字段
    添加体力信息等
"""



def getListOfFiles(dirName):
    # create a list of file and sub directories 
    # names in the given directory 
   
    listOfFile = os.listdir(dirName)
    allFiles = list()
    # Iterate over all the entries
    for entry in listOfFile:
        # Create full path
        fullPath = os.path.join(dirName, entry)
        
        # If entry is a directory then get the list of files in this directory 
        if os.path.isdir(fullPath):
            allFiles = allFiles + getListOfFiles(fullPath)
        else:
            allFiles.append(fullPath)
                
    return allFiles

class Rcg2csv(object):
    def __init__(self, wm):
        self.our_name = wm.teamName()
        self.opp_name = wm.opponentTeamName()
        self.rowlist = self.getRowList()

    def getRowList(self):
        rowlist = []
        pastWhistle = 0
        CurrentWhistle = 0
        while wm.time().kick_off() <= wm.time().cycle() and wm.time().cycle() <= 6000:    # test    
            #Trecho para ceifar a jogada anterior ao gol
            if wm.currentPlaymode(wm.time().cycle()) != 'NCJNA':
                pastWhistle = CurrentWhistle
                CurrentWhistle = wm.time().cycle()
            if (wm.currentPlaymode(wm.time().cycle()) == 'goal_r') | (wm.currentPlaymode(wm.time().cycle()) == 'goal_l'):
                wm.time().setTime(pastWhistle)
                while pastWhistle <= wm.time().cycle() and wm.time().cycle() <= CurrentWhistle:    # test
                    """add our player row"""
                    for unum in range(1, 12):
                        kick = wm.ourPlayer(unum, wm.time().cycle()).kick()
                        row = {
                            'cycle': wm.time().cycle(),
                            'player_num': unum,
                            'ball_x': wm.ball(wm.time().cycle()).x,
                            'ball_y': wm.ball(wm.time().cycle()).y,
                            'ball_vx': wm.ball(wm.time().cycle()).vx,
                            'ball_vy': wm.ball(wm.time().cycle()).vy,
                            'player_x': wm.ourPlayer(unum).x,
                            'player_y': wm.ourPlayer(unum).y,
                            'player_vx': wm.ourPlayer(unum, wm.time().cycle()).vx,
                            'player_vy': wm.ourPlayer(unum, wm.time().cycle()).vy,
                            'playmode' : wm.currentPlaymode(wm.time().cycle()),
                            'team_name': self.our_name
                          
                        }
                        rowlist.append(row)

                    """add their player row"""
                    for unum in range(1, 12):
                        kick = wm.theirPlayer(unum, wm.time().cycle()).kick()
                        row = {
                            'cycle': wm.time().cycle(),
                            'player_num': unum,
                            'ball_x': wm.ball(wm.time().cycle()).x,
                            'ball_y': wm.ball(wm.time().cycle()).y,
                            'ball_vx': wm.ball(wm.time().cycle()).vx,
                            'ball_vy': wm.ball(wm.time().cycle()).vy,
                            'player_x': wm.theirPlayer(unum).x,
                            'player_y': wm.theirPlayer(unum).y,
                            'player_vx': wm.theirPlayer(unum, wm.time().cycle()).vx,
                            'player_vy': wm.theirPlayer(unum, wm.time().cycle()).vy,
                            'playmode' : wm.currentPlaymode(wm.time().cycle()),
                            'team_name': self.opp_name
                        }
                        rowlist.append(row)
                        
                    wm.gameMode().UpdatePlayMode()
                    wm.time().addTime()

            wm.gameMode().UpdatePlayMode()
            wm.time().addTime()

        return rowlist

    def execute(self, newElem):
        newElem = newElem.replace(wm.file_name + '.rcg', '')
        print(newElem)
        if not os.path.exists(newElem):
            os.makedirs(newElem)
        
        print (newElem + '{}.csv'.format(wm.file_name))
        f = open(newElem + '{}.csv'.format(wm.file_name), 'a')
        
        writer = csv.writer(f, lineterminator='\n')
        writer.writerow(
            ['cycle', 'team_name', 'player_num', 'ball_x', 'ball_y', 'ball_vx', 'ball_vy', 'player_x', 'player_y', 'player_vx',
             'player_vy', 'kick', 'dash', 'turn', 'turn_neck', 'tackle', 'change_view', 'attentionto', 'pointto', 'say', 'playmode'])

        for i in self.rowlist:
            writer.writerow(
                [i['cycle'], i['team_name'], i['player_num'], i['ball_x'], i['ball_y'], i['ball_vx'], i['ball_vy'],
                 i['player_x'], i['player_y'], i['player_vx'], i['player_vy'], i['playmode']])
        f.close()



if __name__ == '__main__':

    
    dirName = '../log';
    
    # Get the list of all files in directory tree at given path
    listOfFiles = getListOfFiles(dirName)
    
    # Print the files
    for elem in listOfFiles:
        if 'rcg' in elem:
            wm = World(elem)
            newElem = elem.replace('/log/', '/csv/')
            Rcg2csv(wm).execute(newElem)   
    
    
