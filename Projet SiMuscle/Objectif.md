# Objectif


## Premiers objectifs
- [ ] Compiler le code avec qt6
- [ ] Faire une allocation dynamique de vertice *(geometry.cpp:147)*
- [ ] Restructurer le projet : Dossier source/, autre ?
- [ ] Revoir les header pour éviter de charger plusieurs fois les même libraires
- [ ] Repenser un verbose coérant, peut-être plusieur mode de vervose
- [ ] Faire le Command Line Parsing
- [ ] Importer un modèle depuis Blender (geom+ squeleton+ weight+ muscle)
- [ ] Mettre en place un framework pour travailler facilement avec le Blender de mon ordi perso
Ensuite, on pourra commencer à faire des modèles de muscle, des collisions, et autres simu.

## Pour les muscles
- [ ] Système mass/ressort
- [ ] Element finie à 1 dimension (élément poutre section droite puis muscle)
- [ ] Element finie à 3 dimension avec une loi de comportement de composite anysotrope
Pour la collision et la peaux, il y a encore bcp de biblio à faire.
Pour les collisions :
- [ ] Modèle de Hertz
- [ ] Modèle plus complex … (collision = contact ?)

## Pour la peaux :
- [ ] Ressort de longeur variable aux arrête
- [ ] Element finie à 2 dimension (élément plaque) !!! matériaux hyperélasitque !!!
