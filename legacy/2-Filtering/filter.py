NUMBER_OF_CYCLES_PER_GAME = 6000
MAXIMUM_SIZE_OF_PLAY = 150

import numpy as np
import pandas
import os

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

def readCSV(path):
    print('    Retirando Dados do CSV')
    df = pandas.read_csv(path, delimiter='\t', low_memory=False)
    print('    Filtrando Colunas Relevantes')
    df = pandas.concat([df['playmode'], df['cycle'], df['team_name'], df['player_num'], df['player_x'], df['player_y'], df['player_vx'], df['player_vy'] ,df['ball_x'], df['ball_y'], df['ball_vx'], df['ball_vy'], df['ballPossesion']], axis=1, sort=False)
    print('    Removendo Última Linha, Caso esteja Errada')
    if pandas.isna(df['cycle'][len(df['cycle'])-1]):
        df.drop(df.tail(1).index,inplace=True)
    print('    Reescrevendo Dados com Tipo Correto')
    df.rename(columns={'ballPossesion':'ballPossession'}, inplace=True)
    df['ballPossession'] = df[['ballPossession']].applymap(lambda x: 1 if x == 'True' else 0)
    df['playmode'] = df[['playmode']].applymap(lambda x: 'play_on' if x=='NCJNA' else x)
    return df

def writeCSV(path, df):
    path = path.replace('/input/', '/output/')
    dir = path.rsplit('/', 1)
    if not os.path.exists(dir[0]):
        os.makedirs(dir[0])   
    df.to_csv(path, sep=',', encoding='utf-8')

def filterPlayMode(df):
    dfFinalizationCycles = pandas.DataFrame(columns=['beginPlay', 'endPlay'])
    j = 0
    for i in range(0, len(df.index)):
        if( checkGoalieKickOff(df, i) or checkCornerKickOff(df, i) or checkGoalieCatch(df, i) or checkGoal(df, i)):
            if df['cycle'][i] - MAXIMUM_SIZE_OF_PLAY >= 1:
                dfFinalizationCycles.loc[j] = [df['cycle'][i] - MAXIMUM_SIZE_OF_PLAY] + [df['cycle'][i]]
            else:
                dfFinalizationCycles.loc[j] = [1] + [df['cycle'][i]]
            j += 1
    return dfFinalizationCycles

def checkGoalieKickOff(df, i):
    if((df['playmode'][i] == 'goal_kick_l' or df['playmode'][i] == 'goal_kick_r')):
        if(not(df['playmode'][i-1] == 'goal_kick_l' or df['playmode'][i-1] == 'goal_kick_r')):
            print('    de:', df['cycle'][i]-150, 'ate:', df['cycle'][i], '-> Tiro de Meta')
            return True
    return False

def checkCornerKickOff(df, i):
    if((df['playmode'][i] == 'corner_kick_l' or df['playmode'][i] == 'corner_kick_r')):
        if(not(df['playmode'][i-1] == 'corner_kick_l' or df['playmode'][i-1] == 'corner_kick_r')):
            print('    de:', df['cycle'][i]-150, 'ate:', df['cycle'][i], '-> Escanteio')
            return True
    return False

def checkGoalieCatch(df, i):
    if(df['playmode'][i] == 'free_kick_r' or df['playmode'][i] == 'free_kick_l'):
        if(df['ballPossession'][i]) and (df['player_num'][i] == 1.0):
            print('    de:', df['cycle'][i]-150, 'ate:', df['cycle'][i], '-> Goalie Catch')
            return True
    return False

def checkGoal(df, i):
    if(df['playmode'][i] == 'goal_r' or df['playmode'][i] == 'goal_l'):
        if(not(df['playmode'][i-1] == 'goal_r' or df['playmode'][i-1] == 'goal_l')):    
            print('    de:', df['cycle'][i]-150, 'ate:', df['cycle'][i], '-> Gol!')
            return True
    return False

def filterLastDeadBall(df, dfCycles):
    for j in range(0, len(dfCycles.index)):    
        for i in range(1, len(df.index)):
            if( not(df['cycle'][i] == df['cycle'][i-1]) ):
                if((df['cycle'][i]>=dfCycles['beginPlay'][j]) and (df['cycle'][i]<dfCycles['endPlay'][j])):
                    if(not(df['playmode'][i] == 'play_on')):
                        print('    Atualizado de:', dfCycles['beginPlay'][j], 'para:', df['cycle'][i])
                        dfCycles['beginPlay'][j] = df['cycle'][i]
    return dfCycles

def filterChangePossesion(df, dfCycles):
    gamePossession = pandas.DataFrame(columns=['change', 'team_name'])
    ultima_posse = None
    j=0
    print('    Reconhecendo momentos de troca de posse de bola entre times')
    for i in range(1, len(df.index)):
        if df['ballPossession'][i] == True:
            if not (df['team_name'][i] == ultima_posse):
                gamePossession.loc[j, 'change'] = df['cycle'][i]
                gamePossession.loc[j, 'team_name'] = df['team_name'][i]
                ultima_posse = df['team_name'][i]
                j=j+1

    print('    Calculando Filtro da troca de bola entre times')
    for i in range(0, len(dfCycles.index)):
        for j in range(0, len(gamePossession.index)):
            if gamePossession['change'][j] > dfCycles['beginPlay'][i] and gamePossession['change'][j] < dfCycles['endPlay'][i] - 3:
                print('       trocando', dfCycles['beginPlay'][i], 'por', gamePossession['change'][j])   
    return dfCycles

def ConcatPlay(df, dfCycles):
    print('    Gerando colunas')
    columns = []
    for i in range(0, MAXIMUM_SIZE_OF_PLAY+1):
        for col in df.columns:
                if i == 0:
                    if col == 'cycle' or col == 'team_name' or col == 'player_num':
                        columns.append(col)
                    if col == 'player_x' or col == 'player_y':
                        columns.append('cycle_'+str(i)+'_'+col)
                else:
                    if col == 'player_x' or col == 'player_y':
                        columns.append('cycle_'+str(i)+'_'+col)

    print('    Determinando Transposição para Nova Estrutura')
    j=0
    dic = {'thisOne': None, 'goesHere': None}
    lines = []
    for i in range(len(df.index)):
        if df['cycle'][i] >= dfCycles['beginPlay'][j] and df['cycle'][i] <= dfCycles['endPlay'][j]:
            #saves a line for the ball wich will be taken always from first player
            if df['team_name'][i] == df['team_name'][0] and df['player_num'][i] == 1:
                dic['thisOne'] = i
                dic['goesHere'] = j*23
                lines.append(dic.copy())
            #not elif, cause above line is not exclusive since its saving a line for ball
            if df['team_name'][i] == df['team_name'][0]:
                dic['thisOne'] = i
                dic['goesHere'] = j*23+df['player_num'][i]
                lines.append(dic.copy())
            #saves a line for same player from other team
            elif df['team_name'][i] == df['team_name'][11]:
                dic['thisOne'] = i
                dic['goesHere'] = j*23+df['player_num'][i]+11
                lines.append(dic.copy())
            else:
                pass
        elif df['cycle'][i] > dfCycles['endPlay'][j]:
            j=j+1
        if j >= len(dfCycles.index):
            break
    
    print('    Gerando Novo Dataframe')
    filteredDf = pandas.DataFrame()
    auxiliar_df = pandas.DataFrame() 
    for i in range(len(dfCycles)*23+1):
        dfLine = None
        for j in range(len(lines)):
            if lines[j]['goesHere'] == i:
                if i % 23 == 0:
                    if dfLine is None:
                        auxiliar_df = df.loc[[int(lines[j]['thisOne'])]]
                        auxiliar_df = auxiliar_df.reset_index(drop=True)
                        auxiliar_df.loc[0, 'team_name'] = 'Ball'
                        auxiliar_df.loc[0, 'player_num'] = 0
                        auxiliar_df = pandas.concat([auxiliar_df['cycle'], auxiliar_df['team_name'], auxiliar_df['player_num'], auxiliar_df['ball_x'], auxiliar_df['ball_y']], axis=1, sort=False)
                        dfLine = pandas.concat([dfLine, auxiliar_df], axis=1, ignore_index=True)
                    else:
                        auxiliar_df = df.loc[[int(lines[j]['thisOne'])]]
                        auxiliar_df = pandas.concat([auxiliar_df['player_x'], auxiliar_df['player_y']], axis=1, sort=False)
                        auxiliar_df = auxiliar_df.reset_index(drop=True)
                        dfLine = pandas.concat([dfLine, auxiliar_df], axis=1, ignore_index=True)
                else:
                    if dfLine is None:
                        auxiliar_df = df.loc[[int(lines[j]['thisOne'])]]
                        auxiliar_df = pandas.concat([auxiliar_df['cycle'], auxiliar_df['team_name'], auxiliar_df['player_num'], auxiliar_df['player_x'], auxiliar_df['player_y']], axis=1, sort=False)
                        auxiliar_df = auxiliar_df.reset_index(drop=True)
                        dfLine = pandas.concat([dfLine, auxiliar_df], axis=1, ignore_index=True)
                    else:
                        auxiliar_df = df.loc[[int(lines[j]['thisOne'])]]
                        auxiliar_df = pandas.concat([auxiliar_df['player_x'], auxiliar_df['player_y']], axis=1, sort=False)
                        auxiliar_df = auxiliar_df.reset_index(drop=True)
                        dfLine = pandas.concat([dfLine, auxiliar_df], axis=1, ignore_index=True)
        if dfLine is not None:
            filteredDf = pandas.concat([filteredDf, dfLine], ignore_index=True, sort=False)

    filteredDf.columns = columns
    return filteredDf

def playLength(df, dfCycles):
    df.insert(3, 'playLength','151')
    df['playLength'] = (df.notnull().sum(axis=1) - 4)/2

    return df
    
def applyFilter(df, dfCycles):
    filteredDf = pandas.DataFrame(columns=df.dtypes.index) 
    j = 0
    k = 0
    for i in range(0, len(df.index)):
        if(df['cycle'][i] >= dfCycles['beginPlay'][j] and df['cycle'][i] <= dfCycles['endPlay'][j]):
            filteredDf.loc[k] = df.loc[i]
            k=k+1
        if(df['cycle'][i] > dfCycles['endPlay'][j]):
            print('    Filtrada Jogada :', j)
            j=j+1
            if j >= len(dfCycles.index):
                break

    return filteredDf

if __name__ == '__main__':
    
    dirName = './input'
    
    # Get the list of all files in directory tree at given path
    listOfFiles = getListOfFiles(dirName)
    
    # Print the files
    for path in listOfFiles:
        print('################### ARQUIVO: ', path, ' ###################')
        print('Lendo CSV')
        df = readCSV(path)
        print('Calculando Filtro de Finzalizacoes')
        dfCycles = filterPlayMode(df)
        print('Calculando o Filtro de Ultima Bola Parada')
        dfCycles = filterLastDeadBall(df, dfCycles)
        print('Calculando Fitro de Troca de Posse')
        dfCycles = filterChangePossesion(df, dfCycles)
        print('Horizontalizando dados por jogador de mesma jogada')
        df = ConcatPlay(df, dfCycles)
        print('Adicionando Coluna playLength')
        df = playLength(df, dfCycles)
        print('Ecrevendo Dados')
        writeCSV(path, df)