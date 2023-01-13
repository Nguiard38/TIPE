import pygame as py
import sys

deltaTime = 2
prePath = "ResultatSimulation/"
nomFichier = "test 13|01 4"
path = prePath + nomFichier
fichier = open(path, "r")

tab = fichier.readlines()
res = {}
i = 0
print(tab)
for line in tab:
    if line[0] == 'V':
        i += 1
        print("new voiture")
    else:
        print("position voiture %d", i)
        lineSansRetour = line[:(len(line) - 2)].split()
        x = 10*int(lineSansRetour[0])
        y = 10*int(lineSansRetour[1])
        time = int ((round(float(lineSansRetour[2]),1)*10))
        try: 
            res[time].append((i,x,y))
            print("append")
        except:
            res[time] = [(i,x,y)]
print(res)
fichier.close()


py.init()
size = width, height = 1000,1000

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
                print("blit voiture : ")
                print(voiture[1:])
                screen.blit(voitureRouge, voiture[1:])
            py.display.update()
            print("Espace")
        if event.type == py.KEYDOWN and event.key == py.K_LEFT:
            actualTime -= deltaTime
            screen.blit(arrierePlan, (0,0))
            for voiture in res[actualTime]:
                print("blit voiture : ")
                print(voiture[1:])
                screen.blit(voitureRouge, voiture[1:])
            
            py.display.update()
            print("Espace")
    


pygame.quit()