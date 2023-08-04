
# I. Command Line Parsing
La premiere étape est de mettre en place des commandes lines afin de pouvoir lancer SiMuscle dans les 2 différents modes. L'idée est d'avoir :
- Un mode où l'on règle les paramètres de la simulation (j'aimerais mettre un préview au temps initial pour certains paramètres).
- Un mode où l'on lance le calcul. Une fois le calcul finit, une visualisation du résultats. (Eventuellement le calcul est fait en même temps que la visualisation si les modèles de calculs dont assez légers).
- Eventuellement un mode où l'on lit le résultat précédemment calculé.


## Mode « Settings »
**Idée** : le mode ***Setting*** consiste à régler les paramètres de la simulation que l'on veut faire. L'idée est de choisir ses paramètres sur l'interface Qt, avec un maximum de prévisualisation possible. 

On enumère les différentes options que l'on peut mettre à Setting :
##### --no-preview
Une option qui permet de ne pas afficher de prévisualisation (gain léger de performance)

## Mode « Computation » (ou « Run »)
**Idée** : Le mode ***Computation*** indique au code qu'il faut lancer le calcul avec les paramètres qui ont étés choisis précédemment. Les

On enumère les différentes options que l'on peut mettre à Computing :
##### --real-time
l'option **real time** permet d'afficher le résultat du calcul entre chaque image
##### Architecture option {--accel [Device]}
J'aiemerais avoir une option qui laisse facilement la possibilité à l'utilisateur de choisir avec quelle architecture il souhaite faire ses calculs.(**cpu/gpu**)
#####  --output [file] (ou directory)
Le fichier dans lequel le code écrit les résultats (la géométrie à chaque instant)
##### --log
Affiche le log du calcul précédant dans le bash ou l'interface, à voir…
##### --no-result
Avec cette option, le mode **Result** ne ce lance pas après la fin du calcul
##### --verbose

## Mode « Result »
**Idée** : Le mode ***Result*** affiche le résultat des calculs à l'utilisateur. Actuellement, c'est le seul mode implémenté.

Il sera composé de quelque mode de rendu, que l'on ca énumérer :
##### --wireframe
l'option permet de faire un rendu en wireframe ( voir autre type de rendu de la geometry )
##### --muscle-only / --skin-only
##### --no-rig / --no-muscle / --no-skin / no-floor
##### --matcap [name]
choisit un matcap parmis une selection de matcap déjà existant. Dans l'idée, elle pourras être compléter facilement par chaque utilisateur.
##### --render-mode {ray_tracing, mat_cap, basic, texture} ???
Selon les rendus que j'implémenterais, cette option permettra de choisir les différents types de rendu. Actuellement, seulement le basic est implémenté.
