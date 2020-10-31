import random

letters = "ATCG"

#N = int(input("Nombre de patterns à générer: "))
N = 20
for i in range(N):
    taille = random.randrange(10)
    pattern = ""
    for j in range(taille):
        pattern += random.choice(letters)
    print(pattern, end=" ")
