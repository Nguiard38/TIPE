# -*- coding: utf-8 -*-
"""
Created on Thu Jun  2 18:42:55 2022

@author: Nathan
"""

import numpy as np
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import matplotlib.pyplot as plt


####  Variables  ####
# Les longeur sont en metre, les temps en secondes et les vitesses en metre par seconde

largeur = 2
longeur = 4.5
marge = 0

accelerationMax = 2.8
freinageMax = -3
vitesseMax = 14


yRoute1 = 0
xRoute2 = -12

d1 = -36
f1 = 12
vitesseInitial1 = 20
voiture1x = []
voiture1Vx = []
voiture1Ax = []


d2 = -24
f2 = 24
vitesseInitial2 = 20
voiture2y = []
voiture2Vy = []
voiture2Ay = []
######################


#### Calcul Optimal ####
def vitesse(t, vitesseInitial, acc):
    if (acc*t + vitesseInitial) > vitesseMax:
        return vitesseMax
    elif (acc*t + vitesseInitial) < 0:
        return 0
    else:
        return (acc*t + vitesseInitial)
    

def position(t, d, vitesseInitial, acc):
    if acc*t + vitesseInitial > vitesseMax:
        tMax = (vitesseMax - vitesseInitial) / acc
        return (1/2)* acc * tMax**2 + vitesseInitial*tMax + d + vitesseMax*(t - tMax)
    elif acc*t + vitesseInitial < 0:
        tZero = -vitesseInitial / acc 
        return (1/2)* acc * tZero**2 + vitesseInitial*tZero + d + vitesseMax*(t - tZero)
    else:
        return (1/2)* acc * t**2 + vitesseInitial*t + d


fig = plt.figure()
ax = plt.axes(projection='3d')

pas = 0.1
timeline = []
tInit=0
while position(tInit, d1, vitesseInitial1, accelerationMax) < f1 and position(tInit, d2, vitesseInitial2, accelerationMax) < f2:
    timeline.append(tInit)
    voiture1x.append(position(tInit, d1, vitesseInitial1, accelerationMax))
    voiture2y.append(position(tInit, d2, vitesseInitial2, accelerationMax))
    tInit = tInit+pas
    
margeTemps = 1.5 * tInit
while tInit <= margeTemps:
    timeline.append(tInit)
    voiture1x.append(position(tInit, d1, vitesseInitial1, accelerationMax))
    voiture2y.append(position(tInit, d2, vitesseInitial2, accelerationMax))
    tInit = tInit+pas
    
#######################


#### Pave collision ####
def cuboid_data2(o, size=(1,1,1)):
    X = [[[0, 1, 0], [0, 0, 0], [1, 0, 0], [1, 1, 0]],
         [[0, 0, 0], [0, 0, 1], [1, 0, 1], [1, 0, 0]],
         [[1, 0, 1], [1, 0, 0], [1, 1, 0], [1, 1, 1]],
         [[0, 0, 1], [0, 0, 0], [0, 1, 0], [0, 1, 1]],
         [[0, 1, 0], [0, 1, 1], [1, 1, 1], [1, 1, 0]],
         [[0, 1, 1], [0, 0, 1], [1, 0, 1], [1, 1, 1]]]
    X = np.array(X).astype(float)
    for i in range(3):
        X[:,:,i] *= size[i]
    X += np.array(o)
    return X

def plotCubeAt2(positions,sizes=None,colors=None, **kwargs):
    if not isinstance(colors,(list,np.ndarray)): colors=["C0"]*len(positions)
    if not isinstance(sizes,(list,np.ndarray)): sizes=[(1,1,1)]*len(positions)
    g = []
    for p,s,c in zip(positions,sizes,colors):
        g.append( cuboid_data2(p, size=s) )
    return Poly3DCollection(np.concatenate(g),  
                            facecolors=np.repeat(colors,6), **kwargs)

positions = [(0, xRoute2 - (largeur + longeur)/2, yRoute1 - (largeur + longeur)/2)]
sizes = [(tInit,largeur + longeur, largeur + longeur)]
colors = ["blue"]

pc = plotCubeAt2(positions,sizes,colors=colors, edgecolor="k")
#############################


#### Ajustement Optimal ####

def chevauchementIntervalle(debut1, fin1, debut2, fin2):
    return (debut1 < debut2 < fin1) or (debut1 < fin2 < fin1) or (debut2 < debut1 < fin2) or (debut2 < fin1 < fin2)

def collision(x,y):
    return chevauchementIntervalle(yRoute1 - largeur/2, yRoute1 + largeur/2, y - longeur/2, y + longeur/2) and chevauchementIntervalle(xRoute2 - largeur/2, xRoute2 + largeur/2, x - longeur/2, x + longeur/2)

def propageContrainte(indexFin, numVoiture):
    print("Correction trajectoire")
    if numVoiture == 1:
        acc = (2 * (voiture1x[indexFin] - d1 - vitesseInitial1*timeline[indexFin])) / timeline[indexFin]**2
        if acc > accelerationMax:
            acc = accelerationMax
        if acc < freinageMax:
            acc = freinageMax
        for i in range(indexFin):
            voiture1x[i] = position(timeline[i], d1, vitesseInitial1, acc)
        
        for i in range(indexFin, len(timeline)):
            voiture1x[i] = position(timeline[i] - timeline[indexFin], voiture1x[indexFin], (voiture1x[indexFin] - voiture1x[indexFin - 1]) / pas, accelerationMax)
        
    else:
        acc = (2 * (voiture2y[indexFin] - d1 - vitesseInitial2*timeline[indexFin])) / timeline[indexFin]**2
        print(acc)
        if acc > accelerationMax:
            acc = accelerationMax
        if acc < freinageMax:
            acc = freinageMax
        print(acc)
        for i in range(indexFin):
            voiture2y[i] = position(timeline[i], d2, vitesseInitial2, acc)
        
        for i in range(indexFin, len(timeline)):
            voiture2y[i] = position(timeline[i] - timeline[indexFin], voiture2y[indexFin], (voiture2y[indexFin] - voiture2y[indexFin - 1]) / pas, accelerationMax)
        

for i in range(len(timeline)):
    x = voiture1x[i]
    y = voiture2y[i]
    if collision(x, y) and not(collision(voiture1x[i + 1], voiture2y[i + 1])):
        positionMurGauche = xRoute2 - largeur/2 - longeur/2
        distanceGauche = x - positionMurGauche
        
        positionMurBas = yRoute1 - largeur/2 - longeur/2
        distanceBas = y - positionMurBas
        
        if distanceGauche <= distanceBas:
            voiture1x[i] = positionMurGauche - marge
            propageContrainte(i, 1)
        else:
            voiture2y[i] = positionMurBas - marge
            propageContrainte(i, 2)
        
        
        


############################


#### Affichage ####
ax.add_collection3d(pc) 

ax.plot3D(timeline, voiture1x, voiture2y, 'gray')
ax.set_xlabel('Time (s)')
ax.set_ylabel('x (m)')
ax.set_zlabel('y (m)')

afficheTout = False

voiture1 = (90,0)
voiture2 = (0,-90)
sansTemps = (0,0)
classique = (45,45)
angle1 = (10,10)
angle2 = (20,20)


if afficheTout:
    for i in range(180):
        for j in range(180):
            ax.view_init(i,j) 
            
else:
    choixAngle = sansTemps
    ax.view_init(choixAngle[0],choixAngle[1])
    

###################

#### Temps de passage ####

for i in range(len(timeline)):
    if voiture1x[i] >= f1 and voiture2y[i] >= f2:
        print("Le temps de passage est : " + str(timeline[i]))
        break
        


##########################

plt.show()
