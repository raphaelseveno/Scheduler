# Générateur d'Horaire de Cours

## Description

Ce programme génère un horaire de cours optimal en fonction de vos préférences. Il essaie de respecter vos choix d'horaires tout en évitant les conflits.

## Compilation

Pour compiler le programme, utilisez la commande suivante :

```bash
gcc scheduler.c -o scheduler
```

## Exécution

Pour exécuter le programme :

```bash
./scheduler classes.txt preferences.txt
```

### Arguments

- `classes.txt` : Fichier contenant tous les cours disponibles
- `preferences.txt` : Fichier contenant vos préférences de cours

## Format des Fichiers

### classes.txt

Format CSV : `nom_du_cours,jour,heure`

Exemple :
```
Mathématiques,Lundi,08:00-10:00
Mathématiques,Mardi,10:00-12:00
Physique,Lundi,10:00-12:00
Physique,Mercredi,08:00-10:00
Informatique,Jeudi,14:00-16:00
```

### preferences.txt

Format CSV : `nom_du_cours,premier_choix,deuxième_choix`

Exemple :
```
Mathématiques,08:00-10:00,10:00-12:00
Physique,08:00-10:00,10:00-12:00
Informatique,14:00-16:00,16:00-18:00
```

## Sortie

Le programme affiche l'horaire généré à l'écran et le sauvegarde dans le fichier `mon_horaire.txt`.

### Exemple de sortie

```
========== VOTRE HORAIRE GÉNÉRÉ ==========

Lundi:
  Mathématiques                  08:00-10:00 [1er choix]

Mercredi:
  Physique                       08:00-10:00 [1er choix]

Jeudi:
  Informatique                   14:00-16:00 [1er choix]

=============================================
```

## Notes

- Les lignes commençant par `#` dans les fichiers sont ignorées (commentaires)
- Le programme essaie d'abord votre premier choix, puis votre deuxième choix si un conflit est détecté
- Les conflits (même jour et même heure) sont automatiquement évités
