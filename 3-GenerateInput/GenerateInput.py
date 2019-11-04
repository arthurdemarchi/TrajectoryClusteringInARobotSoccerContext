
import os
import pandas as pd

DIMENSION = 2

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
    df = pd.read_csv(path, delimiter=',', low_memory=False, header=None)
    df = df[1:]
    df = df.drop([1, 2, 3], axis=1)
    df.columns = range(df.shape[1])
    return df

def writeCSV(path, df):
    path = path.replace('/filteredLog/', '/filteredLogTrasClus/')
    dir = path.rsplit('/', 1)
    if not os.path.exists(dir[0]):
        os.makedirs(dir[0])   
    df.to_csv(path, sep=' ', encoding='utf-8', index=False, header=False)

if __name__ == '__main__':
    
    dirName = '../filteredLog'
    
    # Get the list of all files in directory tree at given path
    listOfFiles = getListOfFiles(dirName)
    
    # Print the files
    print('Lendo e Acoplando as jogadas de todos os jogos')
    df = pd.DataFrame()
    for path in listOfFiles:
        dfRead = readCSV(path)
        df = df.append(dfRead, ignore_index=True)

    print('Indexando dados')
    for i in range(len(df.index)):
        df.loc[i, 0] = int(i)
    
    print('Fazendo as transformações lineares de representação para o TrasClus')
    df = df.astype(float)
    df.loc[:,2::2] = df.loc[:,2::2]+55
    df.loc[:,3::2] = df.loc[:,3::2]+35
    # df.loc[:,3::2] = max(df.loc[:,3::2], 0)
    df.loc[:,2:] = df.loc[:,2:]*10
    df = df.round(3)
    df[0] = df[0].astype(int)
    df[1] = df[1].astype(int)

    
    print('Ecrevendo Dados')
    print(len(df.index))
    writeCSV(path, df)