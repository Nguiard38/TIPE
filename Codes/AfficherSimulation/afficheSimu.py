import pygame as py
import sys

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

deltaTime = int(round(float(fichier.readline()),1)*10)
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
res = {}
i = 0

print(tab)
for line in tab:
    if line[0] == 'V':
        i += 1
        print("new voiture")
    else:
        lineSansRetour = line[:(len(line) - 2)].split()
        x = grandissement*int(lineSansRetour[0])//rapport
        y = grandissement*int(lineSansRetour[1])//rapport
        time = int ((round(float(lineSansRetour[2]),1)*10))
        try: 
            res[time].append((i,x,y))
        except:
            res[time] = [(i,x,y)]


print(res)
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
while True:
    py.key.set_repeat(100, 100)
    tkey = py.key.get_pressed()
    for event in py.event.get():
        if event.type == py.QUIT: 
            sys.exit()
        if tkey[py.K_RIGHT]:
            actualTime += deltaTime
            initCroisement()
            for voiture in res[actualTime]:
                screen.blit(voitureRouge, voiture[1:])
            py.display.update()
        if tkey[py.K_LEFT]:
            actualTime -= deltaTime
            initCroisement()
            for voiture in res[actualTime]:
                screen.blit(voitureRouge, voiture[1:])
            
            py.display.update()


pygame.quit()