prePath = "ResultatSimulation/"
nomFichier = "test n2"
path = prePath + nomFichier
fichier = open(path, "r")

print(fichier.readlines())

fichier.close()