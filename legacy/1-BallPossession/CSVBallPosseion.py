PS = 0.3
PB = 0.085
KM = 0.7
PMV = 1.2*1.05
KA = (PS + PB + KM)*1.05
DD = 1.2*2*1.05



import pandas, os

def ballPossesionRotine(myPath):
    df = pandas.read_csv(myPath)
    print(myPath)
    distanceFromBall = ((df['player_x'] - df['ball_x'])**2 + (df['player_y'] - df['ball_y'])**2)**0.5
    velocityDifference = ((df['player_vx'] - df['ball_vx'])**2 + (df['player_vy'] - df['ball_vy'])**2)**0.5
    ballVelocity = ((df['ball_vx']**2 + df['ball_vy'])**2)**0.5

    beginPossession = [False]
    endPossession = [False]
    ballPossession = [False]
    
    for x in range(0, len(df)):
        beginPossession.append(x)
        endPossession.append(x)
        #In Condition
        if (distanceFromBall[x] <= PS+PB):
            beginPossession[x] = True
        elif distanceFromBall[x] <= KA:
            if velocityDifference[x] > (2*KA)/1.5:
                if ((abs(ballVelocity[x]-ballVelocity[x-22])) > 0.4):
                    beginPossession[x] = True
                else:
                    beginPossession[x] = False
            else:
                beginPossession[x] = True
        else:
            beginPossession[x] = False
            
        #Out Condition
        if distanceFromBall[x] <= KA:
            endPossession[x] = False
        elif distanceFromBall[x] <= DD:
            if ballVelocity[x] > PMV:
                endPossession[x] = True
            else:
                endPossession[x] = False
        else:
            endPossession[x] = True            


    for j in range(0, len(df)):
        ballPossession.append(j)
        ballPossession[j] = False

    ballPossessionFlag = False
    for j in range(0, len(df)):
        ballPossession[j] = ballPossession[j-22]
        if endPossession[j] == True:
            if ballPossession[j] == True:
                ballPossessionFlag = False
            ballPossession[j] = False
        if beginPossession[j] == True and not(ballPossessionFlag):
            ballPossession[j] = True
            ballPossessionFlag = True
        
    # Creating a dataframe object from listoftuples
    ballPossessionDf = pandas.DataFrame(ballPossession, columns = ['ballPossesion'])
    beginPossessionDF = pandas.DataFrame(beginPossession, columns = ['BeginPossession']) 
    endPossessionDf = pandas.DataFrame(endPossession, columns = ['EndPossession']) 
    distanceFromBallDf = pandas.DataFrame(distanceFromBall, columns = ['distanceFromBall']) 
    velocityDifferenceDF = pandas.DataFrame(velocityDifference, columns = ['velocityDifference']) 

    newdf = pandas.concat([df, ballPossessionDf, beginPossessionDF, endPossessionDf ,distanceFromBallDf, velocityDifferenceDF], axis=1, sort=False)
    newmyPath = myPath.replace('/input/', '/output/')
    dir = newmyPath.rsplit('/', 1)
    if not os.path.exists(dir[0]):
        os.makedirs(dir[0])   
    newdf.to_csv(newmyPath, sep='\t', encoding='utf-8')





def getListOfFiles(dirName):
    # create a list of file and sub directories 
    # names in the given directory 
   
    listOfFile = os.listdir(dirName)
    allFiles = list()
    # Iterate over all the entries
    for entry in listOfFile:
        # Create full myPath
        fullmyPath = os.path.join(dirName, entry)
        
        # If entry is a directory then get the list of files in this directory 
        if os.path.isdir(fullmyPath):
            allFiles = allFiles + getListOfFiles(fullmyPath)
        else:
            allFiles.append(fullmyPath)
                
    return allFiles

if __name__ == '__main__':

    
    dirName = './input'
    
    # Get the list of all files in directory tree at given myPath
    listOfFiles = getListOfFiles(dirName)
    
    # Print the files
    for myPath in listOfFiles:
        ballPossesionRotine(myPath)
    