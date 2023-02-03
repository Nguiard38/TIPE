import pygame as py
import sys

grandissement = 10

tailleCroisement = 0
nbrDivision = 0
deltaTime = 0
tailleVoiture = 0
rapport = 0
prePath = "ResultatSimulation/"
nomFichier = "Test File 2"
path = prePath + nomFichier
fichier = open(path, "r")

tab = fichier.readlines()
res = {}
i = 0
nbrLigne = 0
print(tab)
for line in tab:
    if line[0] == 'V':
        i += 1
        print("new voiture")
    elif i != 0:
        lineSansRetour = line[:(len(line) - 2)].split()
        x = grandissement*int(lineSansRetour[0])//rapport
        y = grandissement*int(lineSansRetour[1])//rapport
        time = int ((round(float(lineSansRetour[2]),1)*10))
        try: 
            res[time].append((i,x,y))
        except:
            res[time] = [(i,x,y)]
    else:
        if nbrLigne == 0:
            tailleCroisement = int(line)
            print(tailleCroisement)
        elif nbrLigne == 1:
            nbrDivision = int(line)
            rapport = nbrDivision/tailleCroisement
            print(nbrDivision)
        elif nbrLigne == 2:
            deltaTime = int ((round(float(line),1)*10))
            print(deltaTime)
        else:
            tailleVoiture = float(line)
            print(tailleVoiture)

        nbrLigne += 1


print(res)
fichier.close()


py.init()
size = width, height = tailleCroisement*grandissement,tailleCroisement*grandissement



speed = [2, 2]

black = 0, 0, 0


screen = py.display.set_mode(size)


prePathImage = "AfficherSimulation/Images/"
imageV = "VoitureRouge.png"
imageArrierePlan = "ArrierePlan.png"
pathImage = prePathImage + imageV
pathArrierePlan = prePathImage + imageArrierePlan
voitureRouge = py.image.load(pathImage).convert_alpha()
voitureRouge = py.transform.scale(voitureRouge, (30,60))
arrierePlan = py.image.load(pathArrierePlan).convert_alpha()
arrierePlan = py.transform.scale(arrierePlan, (1000,1000))

screen.blit(arrierePlan, (0,0))

py.display.flip()
actualTime = -deltaTime
while True:
    for event in py.event.get():
        if event.type == py.QUIT: 
            sys.exit()
        if event.type == py.KEYDOWN and event.key == py.K_RIGHT:
            actualTime += deltaTime
            screen.blit(arrierePlan, (0,0))
            for voiture in res[actualTime]:
                screen.blit(voitureRouge, voiture[1:])
            py.display.update()
        if event.type == py.KEYDOWN and event.key == py.K_LEFT:
            actualTime -= deltaTime
            screen.blit(arrierePlan, (0,0))
            for voiture in res[actualTime]:
                screen.blit(voitureRouge, voiture[1:])
            
            py.display.update()


pygame.quit()