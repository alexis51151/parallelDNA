import random

letters = "ATCG"

#N = int(input("Nombre de patterns à générer: "))
N = 30
for i in range(N):
    taille = random.randrange(50)
    pattern = ""
    for j in range(taille):
        pattern += random.choice(letters)
    print(pattern, end=" ")
