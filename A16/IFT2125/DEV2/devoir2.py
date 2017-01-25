p1 = (16,20,19,11,9,13,42,19,6,46,27,11,31,14,30)

p2 = (134,177,404,158,144,22,46,13,259,426,41,42,110,259,417)

p3 = (159,200,164,44,191,178,66,101,97,22,74,142,119,176,82,101,189,187,40,130,8,121,70,166,53,180,13,94,52,91,178,161,196,15,6,88,124,55,120,129,147,138,48,18,54,199,152,74,54,181)

p4 = (1500,154,114,11,5,158,168,176,103,183,194,158,26,65,37,25,115,142,19,117,147,86,28,133,81,79,191,110,120,126,50,80,119,144,144,44,92,70,119,84,83,86,164,194,64,47,148,167,175,175)

p5 = (1800,24,59,24,4,83,8,35,45,95,99,39,49,63,56,19,63,53,90,6,6,23,60,72,11,68,1,13,30,13,30,45,39,86,28,74,32,54,20,81,96,79,16,52,96,94,67,11,61,47,83,88,19,3,87,59,80,92,32,35,99,45,81,41,86,95,76,15,68,64,65,31,89,62,45,83,71,88,58,59,38,49,55,99,94,81,67,72,53,47,29,65,13,29,28,52,14,82,55,42)

# Effectue le tri par échange d'un tuple et retourne un tuple
def swapsort(t):
    # Rend le tuple modifiable
    l = list(t)
    for x in range(0, len(l) - 1):
        for y in range(1, len(l) - x):
            if l[y - 1] > l[y]:
                l[y - 1], l[y] = l[y], l[y - 1]
    # Retourne sous la forme d'un tuple
    return tuple(l)

# Extrait le premier élément du tuple considéré comme le poids maximal
#    puis tente d'atteindre ce poids en ajoutant un nombre entier de
#    chacun des poids subséquents.
def Subsetsum(p):
    # Récupère le poids maximal et initialise la somme
    w0 = p[0]
    wi = 0;
    # Trie le tuple en ordre croissant puis inverse l'ordre
    p = swapsort(p[1:])
    p = p[::-1]
    # Tente d'ajouter la plus grosse valeur tant que possible
    for x in range(len(p)):
        while(w0 > wi + p[x]):
            wi += p[x]
    return wi

print("Max p1: " + str(Subsetsum(p1)) + " sur " + str(p1[0]))
print("Max p2: " + str(Subsetsum(p2)) + " sur " + str(p2[0]))
print("Max p3: " + str(Subsetsum(p3)) + " sur " + str(p3[0]))
print("Max p4: " + str(Subsetsum(p4)) + " sur " + str(p4[0]))
print("Max p5: " + str(Subsetsum(p5)) + " sur " + str(p5[0]))
