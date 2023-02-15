import pygame as py
import sys
import math

grandissement = 10

tailleCroisement = 0
nbrDivision = 0
deltaTime = 0
tailleVoiture = 0
rapport = 0
init = []
prePath = "ResultatSimulation/"
nomFichier = sys.argv[1]
path = prePath + nomFichier
fichier = open(path, "r")

tailleCroisement = int(fichier.readline())
print(tailleCroisement)

nbrDivision = int(fichier.readline())
print(nbrDivision)

rapport = nbrDivision/tailleCroisement

deltaTime = (round(float(fichier.readline()),1))
print(deltaTime)

tailleVoiture = float(fichier.readline())
print(tailleVoiture)

for j in range(nbrDivision):
    init.append([])
    line = fichier.readline()
    line = line.split()
    for k in range(nbrDivision):
        init[j].append(int(line[k]))

print(init)

tab = fichier.readlines()
resMode1 = {}
i = 0
minTime = math.inf
maxTime = 0
print(tab)
for line in tab:
    if line[0] == 'V':
        i += 1
        print("new voiture")
    else:
        lineSansRetour = line[:(len(line) - 2)].split()
        x = grandissement*int(lineSansRetour[0])//rapport
        y = grandissement*int(lineSansRetour[1])//rapport
        time = round(float(lineSansRetour[2]),1)
        if time < minTime:
            minTime = time
        if time > maxTime:
            maxTime = time
        try: 
            resMode1[time].append((i,x,y))
        except:
            resMode1[time] = [(i,x,y)]



print(resMode1)
fichier.close()


py.init()
size = width, height = tailleCroisement*grandissement,tailleCroisement*grandissement

sizeVoiture = (tailleVoiture / 2)*grandissement, tailleVoiture*grandissement

speed = [2, 2]

black = 0, 0, 0


screen = py.display.set_mode(size)


prePathImage = "AfficherSimulation/Images/"

imageV = "VoitureRouge.png"
pathVoiture = prePathImage + imageV
voitureRouge = py.image.load(pathVoiture).convert_alpha()
voitureRouge = py.transform.scale(voitureRouge, sizeVoiture)

imageCarreRoute = "Carré gris.png"
pathCR = prePathImage + imageCarreRoute
carreRoute = py.image.load(pathCR).convert_alpha()

imageCarreTrot = "carré noir.png"
pathCT = prePathImage + imageCarreTrot
carreTrot = py.image.load(pathCT).convert_alpha()

tailleCarre = (tailleCroisement/nbrDivision)*grandissement

carreRoute = py.transform.scale(carreRoute, (tailleCarre,tailleCarre))
carreTrot = py.transform.scale(carreTrot, (tailleCarre,tailleCarre))

def initCroisement():
    for i in range(nbrDivision):
        for j in range(nbrDivision):
            if init[i][j] == 0:
                screen.blit(carreRoute, (i*tailleCarre, j*tailleCarre))
            else:
                screen.blit(carreTrot, (i*tailleCarre, j*tailleCarre))

initCroisement()
py.display.flip()
actualTime = -deltaTime
"""
while True:
    py.key.set_repeat(100, 100)
    tkey = py.key.get_pressed()
    for event in py.event.get():
        if event.type == py.QUIT: 
            sys.exit()
        if tkey[py.K_RIGHT]:
            actualTime += deltaTime
            initCroisement()
            for voiture in resMode1[actualTime]:
                screen.blit(voitureRouge, voiture[1:])
            py.display.update()
        if tkey[py.K_LEFT]:
            actualTime -= deltaTime
            initCroisement()
            for voiture in resMode1[actualTime]:
                screen.blit(voitureRouge, voiture[1:])
            
            py.display.update()
"""

def chercherPosSuivant(actualPoint, i):
    if round(actualPoint + deltaTime, 1) > maxTime:
        return (-1,-1)
    for voiture in resMode1[round(actualPoint + deltaTime, 1)]:
        if voiture[0] == i:
            return voiture[1:]
    return (-1,-1)

play = 0
actualTime = 0
lastPoint = 0
ticks = py.time.get_ticks()
while True:
    py.key.set_repeat(100, 100)
    tkey = py.key.get_pressed()
    actualTime += play*(py.time.get_ticks() - ticks)/1000
    ticks = py.time.get_ticks()
    if actualTime >= lastPoint + deltaTime:
        lastPoint += deltaTime
        lastPoint = round(lastPoint, 1)
    elif actualTime < lastPoint:
        lastPoint -= deltaTime
        lastPoint = round(lastPoint, 1)
    if lastPoint < minTime:
        lastPoint = minTime
    if lastPoint > maxTime:
        lastPoint = maxTime
    initCroisement()
    for voiture in resMode1[lastPoint]:
        posSuiv = chercherPosSuivant(lastPoint, voiture[0])
        if posSuiv == (-1,-1):
            posX = voiture[1:][0]
            posY = voiture[1:][1]
        else:
            posX = voiture[1:][0] + (posSuiv[0] - voiture[1:][0])*(actualTime - lastPoint)
            posY = voiture[1:][1] + (posSuiv[1] - voiture[1:][1])*(actualTime - lastPoint)
        screen.blit(voitureRouge, (posX-(tailleVoiture/4)*grandissement, posY-(tailleVoiture/2*grandissement)))
    for event in py.event.get():
        if event.type == py.QUIT: 
            sys.exit()
        if tkey[py.K_SPACE]:
            play = 0
        elif tkey[py.K_RIGHT]:
            play = 1
        elif tkey[py.K_LEFT]:
            play = -1              
    py.display.update()
pygame.quit()